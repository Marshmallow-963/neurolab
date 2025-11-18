/**
 * @file simulation_logic.h
 * @brief Public interface for controlling the main simulation loop.
 *
 * Defines the main entry points for updating and resetting the
 * neuron simulation state based on the application context.
 */
#ifndef SIMULATION_LOGIC_H
#define SIMULATION_LOGIC_H

#include "app_state.h"

/**
 * @brief Advances the simulation by one time step (dt).
 *
 * This function is the main "tick" of the simulation. It checks if the
 * simulation is running, calls the appropriate model's update function
 * (Izhikevich or Hodgkin-Huxley), updates plot data, and handles
 * auto-scaling.
 *
 * @param ctx Pointer to the global AppContext, containing simulation state,
 * models, and GUI state.
 */
void SimulationUpdate(AppContext *ctx);

/**
 * @brief Resets the simulation state to its default values.
 *
 * This stops the simulation, resets the internal time and plot data count,
 * and frees/re-initializes the active neuron models to their starting
 * conditions.
 *
 * @param ctx Pointer to the global AppContext.
 */
void SimulationReset(AppContext *ctx);

#endif // SIMULATION_LOGIC_H
