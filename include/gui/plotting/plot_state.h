/**
 * @file plot_state.h
 * @brief Defines the state structure for plot axis boundaries.
 *
 * This tracks the min/max values for the X and Y axes of all
 * graphs, which is essential for both manual and auto-scaling.
 */
#ifndef PLOT_STATE_H
#define PLOT_STATE_H

/**
 * @struct PlotState
 * @brief Holds the min/max boundaries for all plot views.
 */
typedef struct {
    // Main graph (V vs t)
    float plotXMin;
    float plotXMax;
    float plotYMin;
    float plotYMax;

    // Phase graph (Izhikevich: u vs v)
    float phaseXMin;
    float phaseXMax;
    float phaseYMin;
    float phaseYMax;

    // Gates graph (Hodgkin-Huxley: m,h,n vs t)
    float probYMax; // Typically 1.0
    float probYMin; // Typically 0.0

    // Currents graph (Hodgkin-Huxley: I vs t)
    float currentYMin;
    float currentYMax;
} PlotState;

/**
 * @brief Global extern instance of the plot state.
 *
 * Defined in plot_state.c and modified by simulation_logic.c.
 */
extern PlotState G_PLOT_STATE;

/**
 * @brief Resets the plot axes to their default (initial) values.
 */
void PlotStateReset(void);

#endif // PLOT_STATE_H
