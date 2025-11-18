/**
 * @file gui_plot.h
 * @brief Public interface for the custom plotting widget.
 *
 * Defines the configuration structure (PlotCfg) and the main drawing
 * functions required to render a 2D data plot.
 */
#ifndef GUI_PLOT_H
#define GUI_PLOT_H

#include "raylib.h"

/**
 * @struct PlotCfg
 * @brief Configuration structure holding all parameters needed to draw a plot.
 */
typedef struct {
    Color dataColor;        ///< Color of the data line.

    const char *xLabel;     ///< Text label for the X-axis.
    const char *yLabel;     ///< Text label for the Y-axis.

    float axisMargin;       ///< Padding around the plot area for labels/axes.
    float xMin, xMax;       ///< Boundaries of the X-axis.
    float yMin, yMax;       ///< Boundaries of the Y-axis.

    int dataCount;          ///< Number of points in the data buffer.
    int fontSize;           ///< Font size for axis labels and ticks.

    Rectangle bounds;       ///< The outer rectangle defining the widget's total area.

    Vector2 *data;          ///< Pointer to the array of Vector2 data points.
} PlotCfg;

/**
 * @brief Draws the complete plot, including axes and data.
 *
 * This is the main entry point, which calls GuiPlotDrawAxes
 * and GuiPlotDrawData.
 *
 * @param cfg Pointer to the PlotCfg configuration structure.
 */
void GuiPlotDraw(const PlotCfg *cfg);

/**
 * @brief Draws the axes, labels, and tick marks for the plot.
 * @param cfg Pointer to the PlotCfg configuration structure.
 */
void GuiPlotDrawAxes(const PlotCfg *cfg);

/**
 * @brief Draws the data lines onto the plot.
 * @param cfg Pointer to the PlotCfg configuration structure.
 */
void GuiPlotDrawData(const PlotCfg *cfg);

#endif // GUI_PLOT_H
