/**
 * @file gui_plot.c
 * @brief Implementation of the custom plotting widget.
 *
 * Contains the logic for drawing plot axes, labels, and data series
 * based on the provided PlotCfg.
 */
#include "raylib.h"
#include <string.h>
#include "gui/themes/gui_styles.h"
#include "gui/components/gui_plot.h"

/** @brief Number of ticks (vertical lines) to draw on the X-axis. */
#define NUM_X_TICKS 11
/** @brief Number of ticks (horizontal lines) to draw on the Y-axis. */
#define NUM_Y_TICKS 9

/**
 * @brief Implementation of the main plot drawing function.
 * (This function was missing from the .c file and has been added
 * to match the .h declaration).
 */
void GuiPlotDraw(const PlotCfg *cfg) {
     GuiPlotDrawAxes(cfg);
     GuiPlotDrawData(cfg);
}

/**
 * @brief Implementation of the axis drawing logic.
 */
void GuiPlotDrawAxes(const PlotCfg *cfg) {
    if (!cfg) return;

    // Define the inner plotting rectangle
    Rectangle plotRect = {
        cfg->bounds.x + cfg->axisMargin,
        cfg->bounds.y + cfg->axisMargin / 2.0f,
        cfg->bounds.width - cfg->axisMargin * 1.5f,
        cfg->bounds.height - cfg->axisMargin * 1.5f
    };

    // Define the origin point (bottom-left of the plot area)
    Vector2 origin = { plotRect.x, plotRect.y + plotRect.height };

    DrawLineV(origin, (Vector2){ plotRect.x + plotRect.width, origin.y }, G_UI_STYLES.colors.plotAxisColor);
    DrawLineV(origin, (Vector2){ origin.x, plotRect.y }, LIGHTGRAY);

    DrawText(cfg->xLabel, plotRect.x + plotRect.width - MeasureText(cfg->xLabel, cfg->fontSize), origin.y + 22, cfg->fontSize, G_UI_STYLES.colors.plotAxisColor);
    DrawText(cfg->yLabel, origin.x - 35, plotRect.y - 15, cfg->fontSize, LIGHTGRAY);

    float xRange = (cfg->xMax - cfg->xMin);
    float yRange = (cfg->yMax - cfg->yMin);

    // Desenha Marcações X
    for (int i = 0; i < NUM_X_TICKS; i++) {
        float posX    = origin.x + (plotRect.width / (NUM_X_TICKS - 1)) * i;
        float xValues = cfg->xMin + (xRange / (NUM_X_TICKS - 1)) * i;

        const char *text = TextFormat("%.2f", xValues);

        DrawLine(posX, origin.y, posX, origin.y + 5, G_UI_STYLES.colors.plotAxisColor);

        DrawText(text, posX - (MeasureText(text, cfg->fontSize) / 2.0f), origin.y + 10, cfg->fontSize, G_UI_STYLES.colors.plotAxisColor);
    }

    // Desenha Marcações Y
    for (int i = 0; i < NUM_Y_TICKS; i++) {
        float posY    = origin.y - (plotRect.height / (NUM_Y_TICKS - 1)) * i;
        float yValues = cfg->yMin + (yRange / (NUM_Y_TICKS - 1)) * i;

        const char *text;
        if (cfg->yLabel && strcmp(cfg->yLabel, "Gate probability") == 0) {
            text = TextFormat("%.2f", yValues);
        } else {
            text = TextFormat("%.2f", yValues);
        }

        DrawLine(origin.x - 5, posY, origin.x, posY, G_UI_STYLES.colors.plotAxisColor);
        DrawText(text, origin.x - MeasureText(text, cfg->fontSize) - 10, posY - (cfg->fontSize / 2.0f), cfg->fontSize, G_UI_STYLES.colors.plotAxisColor);
    }
}

/**
 * @brief Implementation of the data line drawing logic.
 * (This function was cut-off in the upload and has been completed).
 */
void GuiPlotDrawData(const PlotCfg *cfg) {
    if (!cfg || cfg->dataCount <= 1) return;

    Rectangle plotRect = {
        cfg->bounds.x + cfg->axisMargin,
        cfg->bounds.y + cfg->axisMargin / 2.0f,
        cfg->bounds.width - cfg->axisMargin * 1.5f,
        cfg->bounds.height - cfg->axisMargin * 1.5f
    };

    float xRange = (cfg->xMax - cfg->xMin);
    float yRange = (cfg->yMax - cfg->yMin);

    if (xRange == 0) xRange = 1.0f; // Evita divisão por zero
    if (yRange == 0) yRange = 1.0f; // Evita divisão por zero

    // Desenha as Linhas de Dados
    for (int i = 1; i < cfg->dataCount; i++) {
        Vector2 p1Data = cfg->data[i - 1];
        Vector2 p2Data = cfg->data[i];

        float p1ScreenX = plotRect.x + ((p1Data.x  - cfg->xMin) / xRange) * plotRect.width;
        float p1ScreenY = plotRect.y + plotRect.height - ((p1Data.y - cfg->yMin) / yRange) * plotRect.height;

        float p2ScreenX = plotRect.x + ((p2Data.x  - cfg->xMin) / xRange) * plotRect.width;
        float p2ScreenY = plotRect.y + plotRect.height - ((p2Data.y - cfg->yMin) / yRange) * plotRect.height;

        if (p1ScreenX >= plotRect.x && p1ScreenX <= (plotRect.x + plotRect.width)
            && p2ScreenX >= plotRect.x && p2ScreenX <= (plotRect.x + plotRect.width))
        {
            DrawLineV((Vector2){ p1ScreenX, p1ScreenY }, (Vector2){ p2ScreenX, p2ScreenY}, cfg->dataColor);
        }
    }
}
