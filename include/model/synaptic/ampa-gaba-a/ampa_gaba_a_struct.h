/**
 * @file ampa_gaba_a_struct.h
 * @brief Defines the core data structures for the AMPA/GABA-A synapse simulation.
 */
#ifndef AMPA_GABA_A_STRUCT_H
#define AMPA_GABA_A_STRUCT_H

#include "utils/rk4.h"

/**
 * @struct ReceptorParams
 * @brief Pointers to the parameters defining the post-synaptic receptor kinetics.
 * These point to locations in the 'internalBuffer'.
 */
typedef struct {
    float *alphaRate; ///< Pointer to connection rate (alpha_r)
    float *betaRate;  ///< Pointer to disconnection rate (beta_r)
    float *gMax;      ///< Pointer to maximum conductance (g_max)
    float *eRev;      ///< Pointer to reversal potential (E_rev)
} ReceptorParams;

/**
 * @struct NeurotransmitterParams
 * @brief Pointers to parameters and external states for neurotransmitter (NT) release.
 */
typedef struct {
    float *vP;        ///< Pointer to midpoint voltage for NT release
    float *kP;        ///< Pointer to steepness factor for NT release
    float *tMax;      ///< Pointer to maximum NT concentration
    float *vPre;      ///< Pointer to the PRE-synaptic neuron's voltage
    float *vPost;     ///< Pointer to the POST-synaptic neuron's voltage
    float *iSynPost;  ///< Pointer to the POST-synaptic neuron's synaptic current variable (Isyn)
} NeurotransmitterParams;

/**
 * @struct SynapseState
 * @brief Pointers to the dynamic state variables of the synapse.
 */
typedef struct {
    float *synCurrent;      ///< Pointer to the calculated synaptic current (I_syn)
    float *openChannels;    ///< Pointer to the fraction of open channels (r) - This is the main state variable
    float *ntConcentration; ///< Pointer to the calculated neurotransmitter concentration (T)
} SynapseState;

/**
 * @struct AmpaGabaaSynapse
 * @brief Groups all components of a single synapse.
 */
typedef struct {
    SynapseState state;
    ReceptorParams receptor;
    NeurotransmitterParams ntParams;
} AmpaGabaaSynapse;

/**
 * @struct AmpaGabaaModel
 * @brief The main structure that encapsulates the entire synapse model.
 */
typedef struct {
    RK4 integrator;             ///< Runge-Kutta 4 integrator instance
    float *stateVector;         ///< Contiguous buffer for the state variable (r)
    float *internalBuffer;      ///< Contiguous buffer for all parameters and other states
    AmpaGabaaSynapse synapse;   ///< The synapse instance
} AmpaGabaaModel;

#endif // AMPA_GABA_A_STRUCT_H
