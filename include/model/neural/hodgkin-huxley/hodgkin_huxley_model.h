/**
 * @file hodgkin_huxley_model.h
 * @brief Public interface (API) for the Hodgkin-Huxley model simulation.
 *
 * Defines the functions to initialize, update, control, and
 * destroy a Hodgkin-Huxley neuron model.
 */
#ifndef HODGKIN_HUXLEY_MODEL_H
#define HODGKIN_HUXLEY_MODEL_H

#include <stdio.h>
#include <stdbool.h>
#include "hodgkin_huxley_struct.h"

/**
 * @brief Allocates and initializes a new Hodgkin-Huxley model.
 *
 * Creates all data structures, allocates memory, and sets the
 * initial states (V, m, h, n) to their resting values.
 *
 * @param dt The simulation time step (delta t) (in ms).
 * @return A pointer to the allocated HodgkinHuxleyModel, or NULL if
 * memory allocation fails.
 */
HodgkinHuxleyModel* HodgkinHuxleyInitModel(const float dt);

/**
 * @brief Sets the external current injected into the neuron.
 *
 * @param model Pointer to the HH model.
 * @param iExt The value of the external current (in uA/cm^2).
 * @return true if the current was set successfully, false if the
 * model pointer is NULL.
 */
bool HodgkinHuxleySetExternalCurent(HodgkinHuxleyModel *model, float iExt);

/**
 * @brief Advances the model simulation by one time step (dt).
 *
 * Uses the RK4 integrator to calculate the next state (V, m, h, n).
 *
 * @param model Pointer to the HH model.
 * @return The new membrane potential (V) after the update.
 * Returns 0.0f if the model is NULL.
 */
float HodgkinHuxleyUpdateModel(HodgkinHuxleyModel *model);

/**
 * @brief Gets the current value of the Sodium current (I_Na).
 *
 * @param model Pointer to the HH model.
 * @return The value of I_Na, or 0.0f if the model is NULL.
 */
float HodgkinHuxleyGetINa(HodgkinHuxleyModel *model);

/**
 * @brief Gets the current value of the Potassium current (I_K).
 *
 * @param model Pointer to the HH model.
 * @return The value of I_K, or 0.0f if the model is NULL.
 */
float HodgkinHuxleyGetIK(HodgkinHuxleyModel *model);

/**
 * @brief Gets the current value of the Leak current (I_Leak).
 *
 * @param model Pointer to the HH model.
 * @return The value of I_Leak, or 0.0f if the model is NULL.
 */
float HodgkinHuxleyGetILeak(HodgkinHuxleyModel *model);

/**
 * @brief Gets the current value of the 'm' activation gate.
 *
 * @param model Pointer to the HH model.
 * @return The value of 'm', or 0.0f if the model is NULL.
 */
float HodgkinHuxleyGetMGate(HodgkinHuxleyModel *model);

/**
 * @brief Gets the current value of the 'h' inactivation gate.
 *
 * @param model Pointer to the HH model.
 * @return The value of 'h', or 0.0f if the model is NULL.
 */
float HodgkinHuxleyGetHGate(HodgkinHuxleyModel *model);

/**
 * @brief Gets the current value of the 'n' activation gate.
 *
 * @param model Pointer to the HH model.
 * @return The value of 'n', or 0.0f if the model is NULL.
 */
float HodgkinHuxleyGetNGate(HodgkinHuxleyModel *model);

/**
 * @brief Frees all memory associated with the HH model.
 *
 * @param model Pointer to the HH model to be freed.
 * @return true if memory was freed, false if the pointer was already NULL.
 */
bool HodgkinHuxleyFreeModel(HodgkinHuxleyModel *model);

#endif // HODGKIN_HUXLEY_MODEL_H
