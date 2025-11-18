/**
 * @file ampa_gaba_a_model.c
 * @brief Implementation of the AMPA/GABA-A synapse simulation logic.
 *
 * This model calculates:
 * 1. Neurotransmitter (NT) concentration [T] based on pre-synaptic voltage.
 * 2. The fraction of open channels [r] using an ODE (solved by RK4).
 * 3. The post-synaptic current (I_syn) based on [r] and post-synaptic voltage.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model/synaptic/ampa-gaba-a/ampa_gaba_a_model.h"

// --- Internal Module Constants ---

/** @brief Dimension of the system of ODEs (only 'r') */
#define SYS_DIM 1
/** @brief Size of the internal buffer (all params and internal states) */
#define INTERNAL_VARS 12
/** @brief Default value for initialization (0.0) */
#define DEFAULT_VALUE 0.0
/** @brief Default pre-synaptic voltage if not connected */
#define DEFAULT_V_PRE -70.0
/** @brief Default post-synaptic voltage if not connected (to avoid NaN in I_syn calc) */
#define DEFAULT_V_POST -70.0


// --- Static Forward Declarations ---

/**
 * @brief Allocates the internal buffer and maps all synapse pointers.
 *
 * @param model Pointer to the AmpaGabaaModel.
 * @return true on success, false if memory allocation fails.
 */
static bool SynapseAllocMemory(AmpaGabaaModel *model);

/**
 * @brief The derivatives function (dy/dt) for the RK4 integrator.
 *
 * Calculates the derivative for 'r' (the fraction of open channels).
 * dr/dt = alpha * [T] * (1 - r) - beta * r
 *
 * @param state The current state vector [r].
 * @param deriv The output vector where the derivative [r'] will be written.
 * @param params A (void*) pointer to the AmpaGabaaSynapse struct.
 */
static void AmpaGabaaDerivatives(const float *state, float *deriv, void *params);


// --- Private (static) Function Implementations ---

static bool SynapseAllocMemory(AmpaGabaaModel *model) {
    model->internalBuffer = (float*)calloc(INTERNAL_VARS, sizeof(float));
    if (!model->internalBuffer) return false;

    // Map all pointers (state, receptor, ntParams) to the buffer
    float *p = model->internalBuffer;
    model->synapse.state.synCurrent      = p++;
    model->synapse.state.openChannels    = p++;
    model->synapse.state.ntConcentration = p++;
    model->synapse.receptor.alphaRate    = p++;
    model->synapse.receptor.betaRate     = p++;
    model->synapse.receptor.eRev         = p++;
    model->synapse.receptor.gMax         = p++;
    model->synapse.ntParams.vP           = p++;
    model->synapse.ntParams.kP           = p++;
    model->synapse.ntParams.tMax         = p++;
    model->synapse.ntParams.vPre         = p++; // Pointer to external V_pre
    model->synapse.ntParams.vPost        = p++; // Pointer to external V_post
    // Note: iSynPost is handled by AmpaGabaaConnectSynapse, not in this buffer.

    return true;
}

static void AmpaGabaaDerivatives(const float *state, float *deriv, void *params) {
    AmpaGabaaSynapse *synapse = (AmpaGabaaSynapse*)params;

    // Unpack state
    const float r = state[0]; // Fraction of open channels

    // Calculate Neurotransmitter concentration (Sigmoid function)
    const float vPre = *(synapse->ntParams.vPre) ? *(synapse->ntParams.vPre) : DEFAULT_V_PRE;
    const float tMax = *(synapse->ntParams.tMax);
    const float kP   = *(synapse->ntParams.kP);
    const float vP   = *(synapse->ntParams.vP);

    const float t = tMax / (1.0 + exp(-(vPre - vP) / kP));

    // Store T for external access/debugging
    *(synapse->state.ntConcentration) = t;

    // Unpack rates
    const float alpha = *(synapse->receptor.alphaRate);
    const float beta  = *(synapse->receptor.betaRate);

    // The ODE: dr/dt = alpha * T * (1 - r) - beta * r
    deriv[0] = alpha * t * (1.0 - r) - (beta * r);
}


// --- Public (API) Function Implementations ---

AmpaGabaaModel *AmpaGabaaInitModel(AmpaGabaaSynapseType synType, NeuronModel nrnType, float dt) {
    // 1. Allocate main struct
    AmpaGabaaModel *model = (AmpaGabaaModel*)calloc(1, sizeof(AmpaGabaaModel));
    if (!model) return NULL;

    model->stateVector    = NULL;
    model->internalBuffer = NULL;

    // 2. Allocate state vector (only 'r')
    model->stateVector = (float*)calloc(SYS_DIM, sizeof(float));
    if (!model->stateVector) {
        AmpaGabaaFreeModel(model);
        return NULL;
    }
    // Map the openChannels pointer to the state vector
    model->synapse.state.openChannels = &model->stateVector[0];
    model->stateVector[0] = DEFAULT_VALUE; // Start with 0 open channels

    // 3. Allocate internal buffer for all other params
    if (!SynapseAllocMemory(model)) {
        AmpaGabaaFreeModel(model);
        return NULL;
    }

    // 4. Set parameters based on NeuronModel and SynapseType
    *(model->synapse.ntParams.tMax) = T_MAX;

    if (nrnType == IZHIKEVICH_MODEL) {
        *(model->synapse.ntParams.kP) = IZ_SYN_CFG.KP;
        *(model->synapse.ntParams.vP) = IZ_SYN_CFG.VP;
        if (synType == AMPA) {
            *(model->synapse.receptor.alphaRate) = IZ_SYN_CFG.ampaConnectionRate;
            *(model->synapse.receptor.betaRate)  = IZ_SYN_CFG.ampaDisconnectionRate;
            *(model->synapse.receptor.eRev)      = IZ_SYN_CFG.ampaReversalPotential;
            *(model->synapse.receptor.gMax)      = IZ_SYN_CFG.ampaMaximumConductancy;
        } else { // GABA_A
            *(model->synapse.receptor.alphaRate) = IZ_SYN_CFG.gaba_aConnectionRate;
            *(model->synapse.receptor.betaRate)  = IZ_SYN_CFG.gaba_aDisconnectionRate;
            *(model->synapse.receptor.eRev)      = IZ_SYN_CFG.gaba_aReversalPotential;
            *(model->synapse.receptor.gMax)      = IZ_SYN_CFG.gaba_aMaximumConductancy;
        }
    } else if (nrnType == HODGKIN_HUXLEY_MODEL) {
        *(model->synapse.ntParams.kP) = HH_SYN_CFG.KP;
        *(model->synapse.ntParams.vP) = HH_SYN_CFG.VP;
        if (synType == AMPA) {
            *(model->synapse.receptor.alphaRate) = HH_SYN_CFG.ampaConnectionRate;
            *(model->synapse.receptor.betaRate)  = HH_SYN_CFG.ampaDisconnectionRate;
            *(model->synapse.receptor.eRev)      = HH_SYN_CFG.ampaReversalPotential;
            *(model->synapse.receptor.gMax)      = HH_SYN_CFG.ampaMaximumConductancy;
        } else { // GABA_A
            *(model->synapse.receptor.alphaRate) = HH_SYN_CFG.gaba_aConnectionRate;
            *(model->synapse.receptor.betaRate)  = HH_SYN_CFG.gaba_aDisconnectionRate;
            *(model->synapse.receptor.eRev)      = HH_SYN_CFG.gaba_aReversalPotential;
            *(model->synapse.receptor.gMax)      = HH_SYN_CFG.gaba_aMaximumConductancy;
        }
    }

    // 5. Initialize internal states
    *(model->synapse.state.ntConcentration) = DEFAULT_VALUE;
    *(model->synapse.state.synCurrent)      = DEFAULT_VALUE;

    // 6. Initialize RK4 integrator
    if (!RK4Init(&model->integrator, AmpaGabaaDerivatives, &model->synapse, SYS_DIM, dt)) {
        AmpaGabaaFreeModel(model);
        return NULL;
    }

    return model;
}

bool AmpaGabaaUpdateModel(AmpaGabaaModel *model) {
    if (!model) {
        fprintf(stderr, "Error: AmpaGabaaUpdateModel called with NULL model.\n");
        return false;
    }

    // 1. Solve the ODE for 'r' (fraction of open channels)
    // AmpaGabaaDerivatives (called by RK4) will also re-calculate [T]
    RK4Calculate(&model->integrator, model->stateVector);

    // 2. Unpack values needed to calculate I_syn = g_max * r * (E_rev - V_post)
    const float r     = model->stateVector[0]; // r
    const float gMax  = *(model->synapse.receptor.gMax);
    const float eRev  = *(model->synapse.receptor.eRev);
    // Use default V_post if not connected, otherwise read from pointer
    const float vPost = *(model->synapse.ntParams.vPost) ? *(model->synapse.ntParams.vPost) : DEFAULT_V_POST;

    // 3. Calculate I_syn
    float iSyn = gMax * r * (eRev - vPost);
    *(model->synapse.state.synCurrent) = iSyn;

    // 4. Inject the current into the post-synaptic neuron (if connected)
    if (model->synapse.ntParams.iSynPost) {
        *(model->synapse.ntParams.iSynPost) += iSyn;
    }

    return true;
}

bool AmpaGabaaConnectSynapse(AmpaGabaaModel *model, float *preVolt, float *postVolt, float *postIsyn) {
    if (!model || !preVolt || !postVolt || !postIsyn) return false;

    // Store pointers to the external neuron variables
    model->synapse.ntParams.vPre     = preVolt;
    model->synapse.ntParams.vPost    = postVolt;
    model->synapse.ntParams.iSynPost = postIsyn;

    return true;
}

float AmpaGabaaGetSynapticCurrent(const AmpaGabaaModel *model) {
    if (!model) return 0.0f;
    return *(model->synapse.state.synCurrent);
}

bool AmpaGabaaSetMaximumConductancy(AmpaGabaaModel *model, float g) {
    if (!model) return false;
    *(model->synapse.receptor.gMax) = g;
    return true;
}

bool AmpaGabaaFreeModel(AmpaGabaaModel *model) {
    if (!model) return false;

    // Free RK4 buffer
    if (model->integrator.buffer) {
        RK4Free(&model->integrator);
    }

    // Free main model buffers
    free(model->internalBuffer);
    free(model->stateVector);

    // Free the struct itself
    free(model);

    return true;
}
