/**
 * @file izhikevich_model.c
 * @brief Implementation of the Izhikevich neuron simulation logic.
 *
 * This model uses a hybrid simulation approach:
 * 1. RK4 integration for sub-threshold dynamics.
 * 2. A discrete reset mechanism when voltage hits the spike peak.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "model/neural/izhikevich/izhikevich_model.h"

// --- Internal Module Constants ---

/** @brief Default time step (not used by init, but as a reference) */
#define DT 0.01
/** @brief Dimension of the system of ODEs (v, u) */
#define SYS_DIM 2
/** @brief Size of the internal buffer (a, b, c, d, Iext, Isyn) */
#define NUM_CURRENTS_AND_PARAMS 6

// Coefficients for the 'v' derivative: (0.04*v^2 + 5*v + 140)
/** @brief Quadratic coefficient for the v-derivative */
#define QUAD_COEFF   0.04
/** @brief Linear coefficient for the v-derivative */
#define LINEAR_COEFF 5.0
/** @brief Constant term for the v-derivative */
#define CONST_TERM   140.0

// --- Static Forward Declarations ---

/**
 * @brief Allocates the internal buffer and maps all neuron pointers.
 *
 * The internal buffer holds parameters (a,b,c,d) and currents (Iext, Isyn).
 *
 * @param model Pointer to the Izhikevich model.
 * @return true on success, false if memory allocation fails.
 */
static bool AllocateMemory(IzhikevichModel *model);

/**
 * @brief The derivatives function (dy/dt) for the RK4 integrator.
 *
 * Calculates the derivatives for v (dv/dt) and u (du/dt).
 *
 * @param state The current state vector [v, u].
 * @param deriv The output vector where derivatives [v', u'] will be written.
 * @param params A (void*) pointer to the IzhikevichNeuron struct.
 */
static void IzhikevichDerivatives(const float *state, float *deriv, void *params);


// --- Private (static) Function Implementations ---

static bool AllocateMemory(IzhikevichModel *model) {
    // Allocate one contiguous block for params and currents
    model->internalBuffer = (float*)calloc(NUM_CURRENTS_AND_PARAMS, sizeof(float));
    if (!model->internalBuffer) return false;

    // Map pointers to this buffer
    float *p = model->internalBuffer;
    model->neuron.params.a      = p++;
    model->neuron.params.b      = p++;
    model->neuron.params.c      = p++;
    model->neuron.params.d      = p++;
    model->neuron.currents.Iext = p++;
    model->neuron.currents.Isyn = p;

    return true;
}

static void IzhikevichDerivatives(const float *state, float *deriv, void *params) {
    IzhikevichNeuron *neuron = (IzhikevichNeuron*)params;

    // Unpack state
    const float v = state[0];
    const float u = state[1];

    // Unpack params
    const float a = *(neuron->params.a);
    const float b = *(neuron->params.b);
    const float I = *(neuron->currents.Iext) + *(neuron->currents.Isyn);

    // Izhikevich's ODEs
    // dv/dt
    deriv[0] = (QUAD_COEFF * v * v) + (LINEAR_COEFF * v) + CONST_TERM - u + I;
    // du/dt
    deriv[1] = a * (b * v - u);
}


// --- Public (API) Function Implementations ---

IzhikevichModel* IzhikevichInitModel(const IzNeuronType type, const float dt) {
    // 1. Allocate main struct
    IzhikevichModel *model = (IzhikevichModel*)calloc(1, sizeof(IzhikevichModel));
    if (!model) return NULL;

    model->stateVector    = NULL;
    model->internalBuffer = NULL;

    // 2. Allocate state vector (v, u)
    model->stateVector = (float*)calloc(SYS_DIM, sizeof(float));
    if (!model->stateVector) {
        IzhikevichFreeModel(model);
        return NULL;
    }

    // Map state pointers
    model->neuron.state.v = &model->stateVector[0];
    model->neuron.state.u = &model->stateVector[1];

    // 3. Allocate internal buffer (for params and currents)
    if (!AllocateMemory(model)) {
        IzhikevichFreeModel(model);
        return NULL;
    }

    // 4. Set parameters and initial currents from the global config
    *(model->neuron.params.a) = IZHIKEVICH_PARAMETERS[type].a;
    *(model->neuron.params.b) = IZHIKEVICH_PARAMETERS[type].b;
    *(model->neuron.params.c) = IZHIKEVICH_PARAMETERS[type].c;
    *(model->neuron.params.d) = IZHIKEVICH_PARAMETERS[type].d;

    *(model->neuron.currents.Iext) = 0.0f;
    *(model->neuron.currents.Isyn) = 0.0f;

    // 5. Set initial state (resting)
    // Start 'v' near its reset potential 'c'
    *(model->neuron.state.v) = (*(model->neuron.params.c) - 10.0);
    // Initialize 'u' to be in equilibrium (u = b*v)
    *(model->neuron.state.u) = (*(model->neuron.params.b)) * (*(model->neuron.state.v));

    // 6. Initialize RK4 integrator
    if (!RK4Init(&model->integrator, IzhikevichDerivatives, &model->neuron, SYS_DIM, dt)) {
        IzhikevichFreeModel(model);
        return NULL;
    }

    return model;
}

bool IzhikevichSetExternalCurrent(IzhikevichModel *model, float iExt) {
    if (!model) return false;

    *(model->neuron.currents.Iext) = iExt;
    return true;
}

float IzhikevichUpdateModel(IzhikevichModel *model) {
    if (!model) return 0.00f;

    // 1. Calculate sub-threshold dynamics
    RK4Calculate(&model->integrator, model->stateVector);

    // 2. Check for spike (discrete reset)
    if (*(model->neuron.state.v) >= IZHIKEVICH_SPIKE_PEAK) {
        // Reset v
        *(model->neuron.state.v) = *(model->neuron.params.c);
        // Reset u
        *(model->neuron.state.u) += *(model->neuron.params.d);

        // Return the peak value to signal a spike occurred
        return IZHIKEVICH_SPIKE_PEAK;
    }

    // Return the new membrane potential
    return *(model->neuron.state.v);
}

float IzhikevichGetRecovery(IzhikevichModel *model) {
    if (!model) return 0.0f;
    return *(model->neuron.state.u);
}

bool IzhikevichFreeModel(IzhikevichModel *model) {
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
