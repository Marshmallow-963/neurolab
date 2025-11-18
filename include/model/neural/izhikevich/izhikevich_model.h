/**
 * @file izhikevich_model.h
 * @brief Public interface (API) for the Izhikevich neuron model simulation.
 *
 * Defines functions to initialize, update, control, and destroy the model.
 */
#ifndef IZHIKEVICH_MODEL_H
#define IZHIKEVICH_MODEL_H

#include <stdio.h>
#include <stdbool.h>
#include "izhikevich_struct.h"
#include "model/neural/izhikevich/izhikevich_config.h"

/**
 * @brief Allocates and initializes a new Izhikevich model.
 *
 * @param type The desired neuron firing pattern (e.g., REGULAR_SPIKING).
 * @param dt The simulation time step (delta t) (in ms).
 * @return A pointer to the allocated IzhikevichModel, or NULL if
 * memory allocation fails.
 */
IzhikevichModel* IzhikevichInitModel(const IzNeuronType type, const float dt);

/**
 * @brief Sets the external current injected into the neuron.
 *
 * @param model Pointer to the Izhikevich model.
 * @param iExt The value of the external current.
 * @return true if the current was set successfully, false if the
 * model pointer is NULL.
 */
bool IzhikevichSetExternalCurrent(IzhikevichModel *model, float iExt);

/**
 * @brief Advances the model simulation by one time step (dt).
 *
 * This function uses an RK4 integrator for the sub-threshold dynamics and
 * applies the spike-reset mechanism when 'v' reaches the peak.
 *
 * @param model Pointer to the Izhikevich model.
 * @return The new membrane potential (v). If a spike occurred, returns
 * the IZHIKEVICH_SPIKE_PEAK value for detection, while the internal state
 * is reset to 'c'.
 */
float IzhikevichUpdateModel(IzhikevichModel *model);

/**
 * @brief Gets the current value of the recovery variable 'u'.
 *
 * @param model Pointer to the Izhikevich model.
 * @return The value of 'u', or 0.0f if the model is NULL.
 */
float IzhikevichGetRecovery(IzhikevichModel *model);

/**
 * @brief Frees all memory associated with the Izhikevich model.
 *
 * @param model Pointer to the model to be freed.
 * @return true if memory was freed, false if the pointer was already NULL.
 */
bool IzhikevichFreeModel(IzhikevichModel *model);

#endif // IZHIKEVICH_MODEL_H
