/**
 * @file ampa_gaba_a_model.h
 * @brief Public interface (API) for the AMPA/GABA-A synapse model simulation.
 *
 * Defines functions to initialize, connect, update, and destroy the synapse model.
 */
#ifndef AMPA_GABA_A_MODEL_H
#define AMPA_GABA_A_MODEL_H

#include "model/neural/neuron_models.h"
#include "model/synaptic/ampa-gaba-a/ampa_gaba_a_config.h"
#include "model/synaptic/ampa-gaba-a/ampa_gaba_a_struct.h"

/**
 * @brief Allocates and initializes a new synapse model.
 *
 * @param synType The type of synapse (AMPA or GABA_A).
 * @param nrnType The type of neuron model (HH or IZHI) it will connect to.
 * @param dt The simulation time step (delta t) (in ms).
 * @return A pointer to the allocated AmpaGabaaModel, or NULL if
 * memory allocation fails.
 */
AmpaGabaaModel *AmpaGabaaInitModel(AmpaGabaaSynapseType synType, NeuronModel nrnType, float dt);

/**
 * @brief Connects the synapse model to the relevant neuron state variables.
 *
 * This function "links" the synapse to the outside world by storing pointers
 * to the pre-synaptic voltage, post-synaptic voltage, and the post-synaptic
 * current injection variable.
 *
 * @param model Pointer to the synapse model.
 * @param preVolt Pointer to the PRE-synaptic neuron's membrane voltage.
 * @param postVolt Pointer to the POST-synaptic neuron's membrane voltage.
 * @param postIsyn Pointer to the variable where the calculated synaptic
 * current (I_syn) should be *added* (e.g., model->neuron.currents.iSyn).
 * @return true on success, false if any pointer is NULL.
 */
bool AmpaGabaaConnectSynapse(AmpaGabaaModel *model, float *preVolt, float *postVolt, float *postIsyn);

/**
 * @brief Gets the last calculated synaptic current (I_syn).
 *
 * @param model Pointer to the synapse model.
 * @return The value of I_syn.
 */
float AmpaGabaaGetSynapticCurrent(const AmpaGabaaModel *model);

/**
 * @brief Sets the maximum conductance (g_max) of the synapse.
 *
 * This is the main way to define the "strength" or weight of a synapse.
 *
 * @param model Pointer to the synapse model.
 * @param g The desired maximum conductance.
 * @return true on success, false if model is NULL.
 */
bool AmpaGabaaSetMaximumConductancy(AmpaGabaaModel *model, float g);

/**
 * @brief Advances the synapse simulation by one time step (dt).
 *
 * Calculates neurotransmitter release, solves the ODE for open channels (r)
 * using RK4, and calculates the resulting I_syn.
 *
 * @param model Pointer to the synapse model.
 * @return true on success, false if model is NULL.
 */
bool AmpaGabaaUpdateModel(AmpaGabaaModel *model);

/**
 * @brief Frees all memory associated with the synapse model.
 *
 * @param model Pointer to the model to be freed.
 * @return true if memory was freed, false if the pointer was already NULL.
 */
bool AmpaGabaaFreeModel(AmpaGabaaModel *model);

#endif // AMPA_GABA_A_MODEL_H
