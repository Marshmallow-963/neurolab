/**
 * @file izhikevich_struct.h
 * @brief Defines the core data structures for the Izhikevich model simulation.
 */
#ifndef IZHIKEVICH_STRUCT_H
#define IZHIKEVICH_STRUCT_H

#include "utils/rk4.h"

/**
 * @struct IzhikevichParams
 * @brief Pointers to the model parameters (a, b, c, d).
 *
 * These point to locations within the 'internalBuffer' of the main model.
 */
typedef struct {
    float *a;
    float *b;
    float *c;
    float *d;
} IzhikevichParams;

/**
 * @struct IzhikevichState
 * @brief Pointers to the model's state variables (v, u).
 *
 * These point to locations within the 'stateVector' of the main model.
 */
typedef struct {
    float *v; ///< Pointer to membrane potential 'v'
    float *u; ///< Pointer to recovery variable 'u'
} IzhikevichState;

/**
 * @struct IzhikevichCurrents
 * @brief Pointers to the input currents.
 *
 * These point to locations within the 'internalBuffer' of the main model.
 */
typedef struct {
    float *Iext; ///< Pointer to the external applied current
    float *Isyn; ///< Pointer to the total synaptic current
} IzhikevichCurrents;

/**
 * @struct IzhikevichNeuron
 * @brief Groups all components of a single Izhikevich neuron.
 */
typedef struct {
    IzhikevichParams params;
    IzhikevichState state;
    IzhikevichCurrents currents;
} IzhikevichNeuron;

/**
 * @struct IzhikevichModel
 * @brief The main structure that encapsulates the entire Izhikevich model.
 */
typedef struct {
    RK4 integrator;             ///< Runge-Kutta 4 integrator instance
    float *stateVector;         ///< Contiguous buffer for state variables (v, u)
    float *internalBuffer;      ///< Contiguous buffer for params (a,b,c,d) and currents
    IzhikevichNeuron neuron;    ///< The neuron instance
} IzhikevichModel;

#endif // IZHIKEVICH_STRUCT_H
