/**
 * @file ampa_gaba_a_config.h
 * @brief Defines configuration constants, enums, and structs for
 * AMPA/GABA-A synapse models.
 */
#ifndef AMPA_GABA_A_CONFIG_H
#define AMPA_GABA_A_CONFIG_H

/**
 * @brief Maximum neurotransmitter concentration (T_max).
 * Used in the sigmoidal function for neurotransmitter release.
 */
extern const float T_MAX;

/**
 * @enum AmpaGabaaSynapseType
 * @brief Enumerates the type of synapse (excitatory or inhibitory).
 */
typedef enum {
    AMPA = 0, ///< Excitatory synapse
    GABA_A    ///< Inhibitory synapse
} AmpaGabaaSynapseType;

/**
 * @struct IzhikevichSynapsConfig
 * @brief Synaptic parameters tailored for connection to Izhikevich neurons.
 */
typedef struct {
    float KP;                       ///< Steepness of the NT release sigmoid
    float VP;                       ///< Midpoint voltage of the NT release sigmoid
    float ampaConnectionRate;       ///< Alpha rate for AMPA
    float ampaDisconnectionRate;    ///< Beta rate for AMPA
    float ampaReversalPotential;    ///< Reversal potential (E_rev) for AMPA
    float ampaMaximumConductancy;   ///< Maximum conductance (g_max) for AMPA
    float gaba_aConnectionRate;     ///< Alpha rate for GABA-A
    float gaba_aDisconnectionRate;  ///< Beta rate for GABA-A
    float gaba_aReversalPotential;  ///< Reversal potential (E_rev) for GABA-A
    float gaba_aMaximumConductancy; ///< Maximum conductance (g_max) for GABA-A
} IzhikevichSynapsConfig;

/**
 * @struct HodgkinHuxleySynapseConfig
 * @brief Synaptic parameters tailored for connection to Hodgkin-Huxley neurons.
 */
typedef struct {
    float KP;                       ///< Steepness of the NT release sigmoid
    float VP;                       ///< Midpoint voltage of the NT release sigmoid
    float ampaConnectionRate;       ///< Alpha rate for AMPA
    float ampaDisconnectionRate;    ///< Beta rate for AMPA
    float ampaReversalPotential;    ///< Reversal potential (E_rev) for AMPA
    float ampaMaximumConductancy;   ///< Maximum conductance (g_max) for AMPA
    float gaba_aConnectionRate;     ///< Alpha rate for GABA-A
    float gaba_aDisconnectionRate;  ///< Beta rate for GABA-A
    float gaba_aReversalPotential;  ///< Reversal potential (E_rev) for GABA-A
    float gaba_aMaximumConductancy; ///< Maximum conductance (g_max) for GABA-A
} HodgkinHuxleySynapseConfig;

/** @brief Global instance of synapse config for Izhikevich models. */
extern const IzhikevichSynapsConfig IZ_SYN_CFG;
/** @brief Global instance of synapse config for Hodgkin-Huxley models. */
extern const HodgkinHuxleySynapseConfig HH_SYN_CFG;

#endif // AMPA_GABA_A_CONFIG_H
