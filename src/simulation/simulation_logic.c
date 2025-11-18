/**
 * @file simulation_logic.c
 * @brief Implements the core simulation engine.
 *
 * This file contains the logic for stepping the active neuron model
 * (Izhikevich or Hodgkin-Huxley), collecting the resulting data for
 * plotting, and managing the simulation's runtime state (running/stopped).
 */
#include "gui/plotting/plot_state.h"
#include "simulation/simulation_logic.h"
#include "model/neural/izhikevich/izhikevich_model.h"
#include "model/neural/hodgkin-huxley/hodgkin_huxley_model.h"

// --- Static Forward Declarations ---

/**
 * @brief Updates the plot auto-scaling boundaries based on new data.
 * @param ctx Pointer to the global AppContext.
 */
static void SimulationUpdateAutoScale(AppContext *ctx);

/**
 * @brief Runs a single simulation step for the Izhikevich model.
 *
 * Fetches the external current from the GUI, updates the model,
 * and stores the resulting voltage (v) and recovery (u) in the
 * plot data buffer.
 *
 * @param ctx Pointer to the global AppContext.
 */
static void SimulationRunStepIzhikevich(AppContext *ctx);

/**
 * @brief Runs a single simulation step for the Hodgkin-Huxley model.
 *
 * Fetches the external current from the GUI, updates the model,
 * and stores the resulting voltage (V) and gating variables
 * (m, h, n) and currents (IK, INa, ILeak) in their respective
 * plot data buffers.
 *
 * @param ctx Pointer to the global AppContext.
 */
static void SimulationRunStepHodgkinHuxley(AppContext *ctx);

// --- Public Function Implementations ---

void SimulationUpdate(AppContext *ctx) {
    if (!ctx->simState.runtime.isRunning) return;

    if (ctx->simState.plotData.dataCount >= K_MAX_PLOT_POINTS) {
        ctx->simState.runtime.isRunning = false;
        return;
    }

    switch ((NeuronModel)ctx->tabs.activeNeuronModel) {
        case IZHIKEVICH_MODEL: SimulationRunStepIzhikevich(ctx); break;

        case HODGKIN_HUXLEY_MODEL: SimulationRunStepHodgkinHuxley(ctx); break;

        default: break;
    }

    SimulationUpdateAutoScale(ctx);

    ctx->simState.runtime.currentTime += K_DT;
    ctx->simState.plotData.dataCount++;
}

void SimulationReset(AppContext *ctx) {
    ctx->simState.runtime.isRunning   = false;
    ctx->simState.runtime.currentTime = 0.0f;
    ctx->simState.plotData.dataCount  = 0;

    if (ctx->simState.models.izModel) IzhikevichFreeModel(ctx->simState.models.izModel);
    if (ctx->simState.models.hhModel) HodgkinHuxleyFreeModel(ctx->simState.models.hhModel);

    ctx->simState.models.izModel = NULL;
    ctx->simState.models.hhModel = NULL;

    ctx->tabs.phasePlotScroll = (Vector2){ 0, 0 };

    PlotStateReset();
}

// --- Static Function Implementations ---

/**
 * @brief Implementation for the Izhikevich model step.
 */
static void SimulationRunStepIzhikevich(AppContext *ctx) {
    IzhikevichModel *model = ctx->simState.models.izModel;
    if (!model) return;

    IzhikevichSetExternalCurrent(ctx->simState.models.izModel, ctx->simState.inputs.externCurrent);
    float potential = IzhikevichUpdateModel(ctx->simState.models.izModel);
    float recovery  = IzhikevichGetRecovery(ctx->simState.models.izModel);

    int index  = ctx->simState.plotData.dataCount;
    float time = ctx->simState.runtime.currentTime;

    ctx->simState.plotData.membranePotential[index] = (Vector2){ time, potential };
    ctx->simState.plotData.phase[index]             = (Vector2){ recovery, potential };
}

/**
 * @brief Implementation for the Hodgkin-Huxley model step.
 */
static void SimulationRunStepHodgkinHuxley(AppContext *ctx) {
    HodgkinHuxleyModel *model = ctx->simState.models.hhModel;
    if (!model) return;

    HodgkinHuxleySetExternalCurent(ctx->simState.models.hhModel, ctx->simState.inputs.externCurrent);
    float potential = HodgkinHuxleyUpdateModel(ctx->simState.models.hhModel);
    float iK        = HodgkinHuxleyGetIK(ctx->simState.models.hhModel);
    float iNa       = HodgkinHuxleyGetINa(ctx->simState.models.hhModel);
    float iLeak     = HodgkinHuxleyGetILeak(ctx->simState.models.hhModel);
    float mGateProb = HodgkinHuxleyGetMGate(ctx->simState.models.hhModel);
    float nGateProb = HodgkinHuxleyGetNGate(ctx->simState.models.hhModel);
    float hGateProb = HodgkinHuxleyGetHGate(ctx->simState.models.hhModel);

    int index  = ctx->simState.plotData.dataCount;
    float time = ctx->simState.runtime.currentTime;

    ctx->simState.plotData.membranePotential[index] = (Vector2){ time, potential };

    ctx->simState.plotData.hhGatePlots.MGate[index] = (Vector2){ time, mGateProb };
    ctx->simState.plotData.hhGatePlots.NGate[index] = (Vector2){ time, nGateProb };
    ctx->simState.plotData.hhGatePlots.HGate[index] = (Vector2){ time, hGateProb };

    ctx->simState.plotData.hhCurrentPlots.kCurrent[index]    = (Vector2){ time, iK };
    ctx->simState.plotData.hhCurrentPlots.naCurrent[index]   = (Vector2){ time, iNa };
    ctx->simState.plotData.hhCurrentPlots.leakCurrent[index] = (Vector2){ time, iLeak };
}

static void SimulationUpdateAutoScale(AppContext *ctx) {
    int index  = ctx->simState.plotData.dataCount;
    float time = ctx->simState.runtime.currentTime;

    Vector2 memPotential = ctx->simState.plotData.membranePotential[index];
    G_PLOT_STATE.plotXMax = time; // O eixo X sempre avanca
    if (memPotential.y > G_PLOT_STATE.plotYMax) G_PLOT_STATE.plotYMax = memPotential.y;
    if (memPotential.y < G_PLOT_STATE.plotYMin) G_PLOT_STATE.plotYMin = memPotential.y - 2.00f;

    if (ctx->tabs.activeNeuronModel == IZHIKEVICH_MODEL) {
        Vector2 phase = ctx->simState.plotData.phase[index];
        if (phase.x > G_PLOT_STATE.phaseXMax) G_PLOT_STATE.phaseXMax = phase.x; // recovery
        if (phase.x < G_PLOT_STATE.phaseXMin) G_PLOT_STATE.phaseXMin = phase.x;
        if (phase.y > G_PLOT_STATE.phaseYMax) G_PLOT_STATE.phaseYMax = phase.y; // potential
        if (phase.y < G_PLOT_STATE.phaseYMin) G_PLOT_STATE.phaseYMin = phase.y - 2.00f;
    }

    if (ctx->tabs.activeNeuronModel == HODGKIN_HUXLEY_MODEL) {
        G_PLOT_STATE.plotXMax = time;

        Vector2 iK    = ctx->simState.plotData.hhCurrentPlots.kCurrent[index];
        Vector2 iNa   = ctx->simState.plotData.hhCurrentPlots.naCurrent[index];
        Vector2 iLeak = ctx->simState.plotData.hhCurrentPlots.leakCurrent[index];

        if (iK.y > G_PLOT_STATE.currentYMax)    G_PLOT_STATE.currentYMax = iK.y;
        if (iNa.y > G_PLOT_STATE.currentYMax)   G_PLOT_STATE.currentYMax = iNa.y;
        if (iLeak.y > G_PLOT_STATE.currentYMax) G_PLOT_STATE.currentYMax = iLeak.y;

        if (iK.y < G_PLOT_STATE.currentYMin)    G_PLOT_STATE.currentYMin = iK.y - 10000;
        if (iNa.y < G_PLOT_STATE.currentYMin)   G_PLOT_STATE.currentYMin = iNa.y - 10000;
        if (iLeak.y < G_PLOT_STATE.currentYMin) G_PLOT_STATE.currentYMin = iLeak.y - 10000;
    }
}
