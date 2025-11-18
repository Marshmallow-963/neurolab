/**
 * @file plot_state.c
 * @brief Implements the plot state logic, including the reset function.
 */
#include "gui/plotting/plot_state.h"

/**
 * @brief Global instance of the plot axis state.
 */
PlotState G_PLOT_STATE;

/**
 * @brief Resets all plot axis boundaries to sensible default values.
 *
 * This is called during simulation reset to ensure the plots start
 * at a consistent zoom level.
 */
void PlotStateReset() {
    // Default for Main graph (V vs t)
    G_PLOT_STATE.plotXMin  = 0.00f;
    G_PLOT_STATE.plotXMax  = 200.0f;
    G_PLOT_STATE.plotYMin  = -80.0f;
    G_PLOT_STATE.plotYMax  = 40.0f;

    // Default for Phase plot (u vs v)
    G_PLOT_STATE.phaseYMin = -80.0f;
    G_PLOT_STATE.phaseYMax = 40.0f;
    G_PLOT_STATE.phaseXMin = -12.0f;
    G_PLOT_STATE.phaseXMax = -10.0f;

    // Default for Gates plot (m,h,n vs t) - Y-axis is 0 to 1
    G_PLOT_STATE.probYMin  = -0.00f;
    G_PLOT_STATE.probYMax  = 1.00f;

    // Default for Currents plot (I vs t)
    G_PLOT_STATE.currentYMin = -20.0f;
    G_PLOT_STATE.currentYMax = 20.0f;
}
