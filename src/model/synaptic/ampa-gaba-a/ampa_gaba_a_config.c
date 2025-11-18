/**
 * @file ampa_gaba_a_config.c
 * @brief Defines the constant parameter values for the synapse models.
 */
#include "model/synaptic/ampa-gaba-a/ampa_gaba_a_config.h"

/**
 * @brief Maximum neurotransmitter concentration (T_max).
 */
const float T_MAX = 1.0f;

/**
 * @brief Default synapse parameters for Izhikevich neurons.
 */
const IzhikevichSynapsConfig IZ_SYN_CFG = {
    .KP                       = 5.0f,
    .VP                       = 2.0f,
    .ampaConnectionRate       = 1.1f,
    .ampaDisconnectionRate    = 0.30f,  // Faster decay for IZHI
    .ampaReversalPotential    = 0.0f,
    .ampaMaximumConductancy   = 0.0f,  // Default to 0, set externally
    .gaba_aConnectionRate     = 5.0f,
    .gaba_aDisconnectionRate  = 0.18f,
    .gaba_aReversalPotential  = -80.0f,
    .gaba_aMaximumConductancy = 0.0f   // Default to 0, set externally
};

/**
 * @brief Default synapse parameters for Hodgkin-Huxley neurons.
 * Note the different V_p and reversal potentials, scaled for the HH voltage range.
 */
const HodgkinHuxleySynapseConfig HH_SYN_CFG = {
    .KP                       = 5.0f,
    .VP                       = 62.0f, // V_p adjusted for HH spiking
    .ampaConnectionRate       = 1.1f,
    .ampaDisconnectionRate    = 0.19f, // Slower decay for HH
    .ampaReversalPotential    = 60.0f, // E_rev adjusted for HH
    .ampaMaximumConductancy   = 0.0f,  // Default to 0, set externally
    .gaba_aConnectionRate     = 5.0f,
    .gaba_aDisconnectionRate  = 0.18f,
    .gaba_aReversalPotential  = -80.0f,
    .gaba_aMaximumConductancy = 0.0f   // Default to 0, set externally
};
