/**
 * @file hodgkin_huxley_struct.h
 * @brief Defines the core data structures for the HH model simulation.
 */
#ifndef HODGKIN_HUXLEY_STRUCT_H
#define HODGKIN_HUXLEY_STRUCT_H

#include "utils/rk4.h"

/**
 * @struct HodgkinHuxleyParams
 * @brief Neuron parameters (conductances and reversal potentials).
 *
 * These values are copied from the global configuration (HH_CONFIG)
 * during model initialization.
 */
typedef struct {
    float C;
    float gL;
    float eL;
    float eK;
    float gK;
    float eNa;
    float gNa;
} HodgkinHuxleyParams;

/**
 * @struct HodgkinHuxleyState
 * @brief State variables of the model (voltage and gates).
 *
 * Note: These are pointers that point to sections of the main
 * 'stateVector' in the HodgkinHuxleyModel.
 */
typedef struct {
    float *v; ///< Pointer to membrane voltage (V)
    float *m; ///< Pointer to Sodium activation variable (m)
    float *h; ///< Pointer to Sodium inactivation variable (h)
    float *n; ///< Pointer to Potassium activation variable (n)
} HodgkinHuxleyState;

/**
 * @struct HodgkinHuxleyCurrents
 * @brief Calculated currents of the model.
 *
 * Note: These are pointers that point to sections of the 'internalBuffer'
 * in the HodgkinHuxleyModel.
 */
typedef struct {
    float *iL;   ///< Pointer to Leak current (I_L)
    float *iK;   ///< Pointer to Potassium current (I_K)
    float *iNa;  ///< Pointer to Sodium current (I_Na)
    float *iSyn; ///< Pointer to total synaptic current (I_syn)
    float *iExt; ///< Pointer to applied external current (I_ext)
} HodgkinHuxleyCurrents;

/**
 * @struct HodgkinHuxleyNeuron
 * @brief Groups all components of a single HH neuron.
 */
typedef struct {
    HodgkinHuxleyParams params;
    HodgkinHuxleyState state;
    HodgkinHuxleyCurrents currents;
} HodgkinHuxleyNeuron;

/**
 * @struct HodgkinHuxleyModel
 * @brief The main structure that encapsulates the entire HH model.
 *
 * Contains the neuron's state, the numerical integrator (RK4),
 * and the memory buffers required for simulation.
 */
typedef struct {
    RK4 integrator;             ///< Runge-Kutta 4 integrator instance
    float *stateVector;         ///< Contiguous buffer for the 4 state variables (V, m, h, n)
    float *internalBuffer;      ///< Contiguous buffer for the currents (I_Na, I_K, etc.)
    HodgkinHuxleyNeuron neuron; ///< The neuron instance
} HodgkinHuxleyModel;

#endif // HODGKIN_HUXLEY_STRUCT_H
