#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

#include <raylib.h>
#include "model/neural/neuron_models.h"
#include "model/neural/izhikevich/izhikevich_model.h"
#include "model/neural/hodgkin-huxley/hodgkin_huxley_model.h"

// Defines the total simulation duration (Duration = (K_MAX_PLOT_POINTS - 1) * K_DT).
#define K_MAX_PLOT_POINTS 50001

// Defines the time step (delta time) for each simulation iteration, in milliseconds.
#define K_DT 0.01f

// --- Plot Data Buffers ---

/**
 * @struct HodgkinHuxleyGatePlots
 * @brief Stores plot data for HH model gating variables (m, h, n).
 */
typedef struct {
    Vector2 MGate[K_MAX_PLOT_POINTS];
    Vector2 HGate[K_MAX_PLOT_POINTS];
    Vector2 NGate[K_MAX_PLOT_POINTS];
} HodgkinHuxleyGatePlots;

/**
 * @struct HodgkinHuxleyCurrentPlots
 * @brief Stores plot data for HH model ionic currents (IK, INa, ILeak).
 */
typedef struct {
    Vector2 kCurrent[K_MAX_PLOT_POINTS];
    Vector2 naCurrent[K_MAX_PLOT_POINTS];
    Vector2 leakCurrent[K_MAX_PLOT_POINTS];
} HodgkinHuxleyCurrentPlots;

/**
 * @struct SimulationPlotData
 * @brief Aggregates all plot data buffers for the simulation.
 */
typedef struct {
    int dataCount; ///< Current number of data points collected.

    // Common plots
    Vector2 membranePotential[K_MAX_PLOT_POINTS];
    Vector2 phase[K_MAX_PLOT_POINTS]; ///< (IZ: v vs u)

    // HH-specific plots
    HodgkinHuxleyGatePlots hhGatePlots;
    HodgkinHuxleyCurrentPlots hhCurrentPlots;
} SimulationPlotData;

// --- Simulation State ---

/**
 * @struct SimulationInputs
 * @brief Stores parameters controlled by the GUI.
 */
typedef struct {
    float externCurrent;
    float ampaConductancy;
    float gabaaConductancy;
} SimulationInputs;

/**
 * @struct SimulationRuntime
 * @brief Stores the real-time state of the simulation.
 */
typedef struct {
    bool isRunning;
    float currentTime;
} SimulationRuntime;

/**
 * @struct SimulationModels
 * @brief Holds pointers to the instantiated neuron models.
 */
typedef struct {
    IzhikevichModel *izModel;
    HodgkinHuxleyModel *hhModel;
} SimulationModels;

/**
 * @struct SimulationState
 * @brief Encapsulates the entire simulation's data and state.
 */
typedef struct {
    SimulationModels models;
    SimulationInputs inputs;
    SimulationRuntime runtime;
    SimulationPlotData plotData;
} SimulationState;

#endif // SIMULATION_STATE_H
