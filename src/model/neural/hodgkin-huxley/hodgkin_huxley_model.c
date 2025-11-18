/**
 * @file hodgkin_huxley_model.c
 * @brief Implementation of the Hodgkin-Huxley simulation logic.
 *
 * Manages memory allocation, the update loop (via RK4), and
 * the calculation of the model's differential equations.
 */

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model/neural/hodgkin-huxley/hodgkin_huxley_config.h"
#include "model/neural/hodgkin-huxley/hodgkin_huxley_rates.h"
#include "model/neural/hodgkin-huxley/hodgkin_huxley_model.h"

// --- Internal Module Constants ---

/** @brief Dimension of the system of ODEs (Ordinary Differential Equations).
 * (V, m, h, n) -> 4 variables
 */
#define SYS_DIM 4

/** @brief Number of currents stored in the internal buffer.
 * (iNa, iK, iL, iSyn, iExt) -> 5 currents
 */
#define NUM_CURRENTS 5

/** @brief Shortcut for the resting potential from the configuration. */
#define RP HH_CONFIG.restingPotential

/** @brief Resting (steady-state) value for the m-gate. */
#define GATE_REST_M (AlphaM(RP) / (AlphaM(RP) + BetaM(RP)))
/** @brief Resting (steady-state) value for the h-gate. */
#define GATE_REST_H (AlphaH(RP) / (AlphaH(RP) + BetaH(RP)))
/** @brief Resting (steady-state) value for the n-gate. */
#define GATE_REST_N (AlphaN(RP) / (AlphaN(RP) + BetaN(RP)))

// --- Static Forward Declarations ---
// (Forward declarations for private functions in this file)

/**
 * @brief Allocates the internal current buffer and maps pointers.
 * @param model The HH model.
 * @return true if allocation is successful, false otherwise.
 */
static bool AllocCurrents(HodgkinHuxleyModel *model);

/**
 * @brief The derivatives function (dy/dt) for the RK4 integrator.
 *
 * Calculates the derivatives of V, m, h, and n, which are the HH model's ODEs.
 * dV/dt, dV/dt, dV/dt, dV/dt
 *
 * @param state The current state vector [V, m, h, n].
 * @param deriv The output vector where the derivatives [V', m', h', n']
 * will be written.
 * @param params A (void*) pointer to the HodgkinHuxleyNeuron struct,
 * used to access parameters and currents.
 */
static void HodgkinHuxleyDerivatives(const float *state, float *deriv, void *params);


// --- Private (static) Function Implementations ---

static bool AllocCurrents(HodgkinHuxleyModel *model) {
    model->internalBuffer = (float*)calloc(NUM_CURRENTS, sizeof(float));
    if (!model->internalBuffer) return false;

    // Map the 'currents' struct pointers to the allocated buffer
    float *p = model->internalBuffer;
    model->neuron.currents.iNa  = p++;
    model->neuron.currents.iK   = p++;
    model->neuron.currents.iL   = p++;
    model->neuron.currents.iSyn = p++;
    model->neuron.currents.iExt = p;

    return true;
}

static void HodgkinHuxleyDerivatives(const float *state, float *deriv, void *params) {
    HodgkinHuxleyNeuron *neuron = (HodgkinHuxleyNeuron*)params;

    // Unpack state vector for readability
    const float v = state[0];
    const float m = state[1];
    const float h = state[2];
    const float n = state[3];

    // Calculate ionic currents (based on current state values)
    const float iL  = neuron->params.gL * (neuron->params.eL - v);
    const float iK  = neuron->params.gK * n * n * n * n * (neuron->params.eK - v);
    const float iNa = neuron->params.gNa * m * m * m * h * (neuron->params.eNa - v);

    // Total injected current (external + synaptic)
    const float I = *(neuron->currents.iExt) + *(neuron->currents.iSyn);

    // Store calculated currents so they can be read (GetINa, etc.)
    *(neuron->currents.iL)  = iL;
    *(neuron->currents.iK)  = iK;
    *(neuron->currents.iNa) = iNa;

    // The 4 Differential Equations
    // dV/dt
    deriv[0] = ((iNa + iK + iL) + I) / neuron->params.C;
    // dm/dt
    deriv[1] = AlphaM(v) * (1.0 - m) - BetaM(v) * m;
    // dh/dt
    deriv[2] = AlphaH(v) * (1.0 - h) - BetaH(v) * h;
    // dn/dt
    deriv[3] = AlphaN(v) * (1.0 - n) - BetaN(v) * n;
}


// --- Public (API) Function Implementations ---

/**
 * @brief Implementation of the model initialization.
 */
HodgkinHuxleyModel* HodgkinHuxleyInitModel(const float dt) {
    // 1. Allocate the main model struct
    HodgkinHuxleyModel *model = (HodgkinHuxleyModel*)calloc(1, sizeof(HodgkinHuxleyModel));
    if (!model) return NULL;

    model->stateVector    = NULL;
    model->internalBuffer = NULL;

    // 2. Allocate the state vector (V, m, h, n)
    model->stateVector = (float*)calloc(SYS_DIM, sizeof(float));
    if (!model->stateVector) {
        HodgkinHuxleyFreeModel(model); // allocation failed, free the model
        return NULL;
    }

    // 3. Map the state pointers to the state vector
    model->neuron.state.v = &model->stateVector[0];
    model->neuron.state.m = &model->stateVector[1];
    model->neuron.state.h = &model->stateVector[2];
    model->neuron.state.n = &model->stateVector[3];

    // 4. Copy parameters from the global configuration
    model->neuron.params.C   = HH_CONFIG.membraneCapacitancy;
    model->neuron.params.eL  = HH_CONFIG.leakReversal;
    model->neuron.params.eK  = HH_CONFIG.potassiumReversal;
    model->neuron.params.eNa = HH_CONFIG.sodiumReversal;
    model->neuron.params.gL  = HH_CONFIG.leakConductance;
    model->neuron.params.gK  = HH_CONFIG.potassiumConductance;
    model->neuron.params.gNa = HH_CONFIG.sodiumConductance;

    // 5. Set the initial state (resting potential)
    *(model->neuron.state.v) = HH_CONFIG.restingPotential;
    *(model->neuron.state.m) = GATE_REST_M;
    *(model->neuron.state.h) = GATE_REST_H;
    *(model->neuron.state.n) = GATE_REST_N;

    // 6. Allocate and map the current buffer
    if (AllocCurrents(model) != true) {
        HodgkinHuxleyFreeModel(model); // allocation failed
        return NULL;
    }
    // Set initial (syn and ext) currents to zero
    *(model->neuron.currents.iExt) = 0.0f;
    *(model->neuron.currents.iSyn) = 0.0f;


    // 7. Initialize the RK4 integrator
    if (RK4Init(&model->integrator, HodgkinHuxleyDerivatives, &model->neuron, SYS_DIM, dt) != true) {
        HodgkinHuxleyFreeModel(model); // RK4 init failed
        return NULL;
    }

    return model;
}

/**
 * @brief Implementation of the external current setter.
 */
bool HodgkinHuxleySetExternalCurent(HodgkinHuxleyModel *model, float iExt) {
    if (!model) return false;

    // Set the external current in the buffer
    *(model->neuron.currents.iExt) = iExt;
    return true;
}

/**
 * @brief Implementation of the model update.
 */
float HodgkinHuxleyUpdateModel(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;

    // Calculate the next step for V, m, h, n
    RK4Calculate(&model->integrator, model->stateVector);

    // Return the new voltage
    return *(model->neuron.state.v);
}

/**
 * @brief Implementation of the Potassium current getter.
 */
float HodgkinHuxleyGetIK(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;
    // Return the value that was calculated in HodgkinHuxleyDerivatives
    return *(model->neuron.currents.iK);
}

/**
 * @brief Implementation of the Sodium current getter.
 */
float HodgkinHuxleyGetINa(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;
    // Return the value that was calculated in HodgkinHuxleyDerivatives
    return *(model->neuron.currents.iNa);
}

/**
 * @brief Implementation of the Leak current getter.
 */
float HodgkinHuxleyGetILeak(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;
    // Return the value that was calculated in HodgkinHuxleyDerivatives
    return *(model->neuron.currents.iL);
}

/**
 * @brief Implementation of the m-gate getter.
 */
float HodgkinHuxleyGetMGate(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;
    return *(model->neuron.state.m);
}

/**
 * @brief Implementation of the h-gate getter.
 */
float HodgkinHuxleyGetHGate(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;
    return *(model->neuron.state.h);
}

/**
 * @brief Implementation of the n-gate getter.
 */
float HodgkinHuxleyGetNGate(HodgkinHuxleyModel *model) {
    if (!model) return 0.0f;
    return *(model->neuron.state.n);
}

/**
 * @brief Implementation of the memory-freeing function.
 */
bool HodgkinHuxleyFreeModel(HodgkinHuxleyModel *model) {
    if (!model) return false;

    // Free the RK4's internal buffer (if it exists)
    if (model->integrator.buffer) {
        RK4Free(&model->integrator);
    }

    // Free the buffers allocated by InitModel
    free(model->internalBuffer);
    free(model->stateVector);

    // Free the main struct
    free(model);

    return true;
}
