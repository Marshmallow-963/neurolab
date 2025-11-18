/**
 * @file main_menu_screen.c
 * @brief Implementation for drawing the main menu screen of the application.
 *
 * This file handles the layout, logic, and drawing of all UI elements
 * for the main simulation interface, including controls, plots, and info tabs.
 */

#include <stdbool.h>
#include "raylib.h"
#include "raygui.h"
#include "gui/themes/gui_styles.h"
#include "gui/plotting/plot_state.h"
#include "gui/components/gui_plot.h"
#include "simulation/simulation_logic.h"
#include "gui/screens/main_menu_screen.h"

//================================================================================
// Static File Constants
//================================================================================
static const float layoutSplitX = 0.25f;    /**< Horizontal split point (percentage) for the layout. */
static const float layoutSplitY = 0.5f;     /**< Vertical split point (percentage) for the layout. */

static const char *kNrnModelStr = "Izhikevich;Hodgkin-Huxley"; /**< String for the neuron model ComboBox. */
static const char *KIzModelStr  = "Chaterring;Fast Spiking;Intrinsically Bursting;Low-Threshold Spiking;Regular Spiking;Resonator;Thalamo Cortical"; /**< String for the Izhikevich model ComboBox. */

//================================================================================
// Private Function Declarations (Forward Declarations)
//================================================================================

// --- Main Panel Drawing Functions ---

/**
 * @brief Draws the top-left panel (Controls).
 * @param ctx Pointer to the global AppContext.
 * @param topLeftPanel The rectangle defining the bounds of this panel.
 */
static void MainMenuDrawTopLeftPanel(AppContext *ctx, Rectangle topLeftPanel);

/**
 * @brief Draws the top-right panel (Main Display).
 * @param ctx Pointer to the global AppContext.
 * @param topRightPanel The rectangle defining the bounds of this panel.
 */
static void MainMenuDrawTopRightPanel(AppContext *ctx, Rectangle topRightPanel);

/**
 * @brief Draws the bottom-right panel (Auxiliary Displays).
 * @param ctx Pointer to the global AppContext.
 * @param bottomRightPanel The rectangle defining the bounds of this panel.
 */
static void MainMenuDrawbottomRightPanel(AppContext *ctx, Rectangle bottomRightPanel);

// --- Top-Left Panel (Controls) Helpers ---

/**
 * @brief Helper for drawing Start/Pause/Reset buttons.
 * @param ctx Pointer to the global AppContext.
 * @param layout The rectangle defining the layout area for these buttons.
 */
static void MainMenuDrawControlButtons(AppContext *ctx, Rectangle layout);

/**
 * @brief Helper for drawing neuron model selection ComboBoxes.
 * @param ctx Pointer to the global AppContext.
 * @param layout The rectangle for the first widget.
 * @param posY A pointer to the current Y-position, which will be updated.
 */
static void MainMenuDrawModelSelectors (AppContext *ctx, Rectangle layout, float *posY);

/**
 * @brief Helper for drawing input sliders (e.g., external current).
 * @param ctx Pointer to the global AppContext.
 * @param layout The rectangle for the first widget.
 * @param posY A pointer to the current Y-position, which will be updated.
 */
static void MainMenuDrawSliders(AppContext *ctx, Rectangle layout, float *posY);

// --- Top-Right Panel (Main Display) Helpers ---

/**
 * @brief Draws the specific Phase Plot for the Izhikevich model.
 * @param ctx Pointer to the global AppContext.
 * @param graphRect The rectangle defining the area for the plot.
 */
static void MainMenuDrawIzPhasePlot(AppContext *ctx, Rectangle graphRect);

/**
 * @brief Draws the specific plots (Currents, Gates) for the Hodgkin-Huxley model.
 * @param ctx Pointer to the global AppContext.
 * @param tabContentRect The rectangle defining the area for this tab's content.
 */
static void MainMenuDrawHHPhasePlots(AppContext *ctx, Rectangle tabContentRect);

// --- Bottom-Right Panel (Auxiliary Display) Helpers ---

/**
 * @brief Draws the content of the "Actual state" tab.
 * @param ctx Pointer to the global AppContext.
 * @param labelRect The rectangle defining the area for the text labels.
 */
static void MainMenuDrawStateTab(AppContext *ctx, Rectangle labelRect);

/**
 * @brief Draws the content of the "Event Log" tab.
 * @param ctx Pointer to the global AppContext.
 * @param labelRect The rectangle defining the area for the text labels.
 */
static void MainMenuDrawEventsTab(AppContext *ctx, Rectangle labelRect);


//================================================================================
// Public Function Implementations
//================================================================================

/**
 * @brief Main drawing function for this screen.
 * @param ctx Pointer to the global AppContext.
 */
void ScreenMainMenuDraw(AppContext *ctx) {
    float screenwidth  = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    float splitX = screenwidth * layoutSplitX;
    float splitY = screenHeight * layoutSplitY;

    // --- Panel Layout Definitions ---
    Rectangle topLeftPanel = {
        G_UI_STYLES.layout.padding,
        G_UI_STYLES.layout.padding,
        splitX - G_UI_STYLES.layout.padding - G_UI_STYLES.layout.padding / 2.0f,
        screenHeight - G_UI_STYLES.layout.padding * 2
    };

    Rectangle topRightPanel = {
        splitX + (G_UI_STYLES.layout.padding / 2.0f),
        G_UI_STYLES.layout.padding,
        screenwidth - (splitX + (G_UI_STYLES.layout.padding / 2.0f)) - G_UI_STYLES.layout.padding,
        splitY - (G_UI_STYLES.layout.padding / 2.0f) - G_UI_STYLES.layout.padding
    };

    Rectangle bottomRightPanel = {
        splitX + (G_UI_STYLES.layout.padding / 2.0f),
        splitY + (G_UI_STYLES.layout.padding / 2.0f),
        screenwidth - (splitX + G_UI_STYLES.layout.padding / 2.0f) - G_UI_STYLES.layout.padding,
        screenHeight - (splitY + G_UI_STYLES.layout.padding / 2.0f) - G_UI_STYLES.layout.padding
    };

    // --- Focus Highlighting ---
    switch (ctx->focus.focusTab) {
        case FOCUS_NONE: break;
        case FOCUS_CONTROLS: DrawRectangleLinesEx(topLeftPanel, G_UI_STYLES.global.focusThickness, G_UI_STYLES.colors.focusColor); break;
        case FOCUS_MAIN_DISPLAY: DrawRectangleLinesEx(topRightPanel,G_UI_STYLES.global.focusThickness, G_UI_STYLES.colors.focusColor); break;
        case FOCUS_AUXILIARY_DISPLAY: DrawRectangleLinesEx(bottomRightPanel, G_UI_STYLES.global.focusThickness, G_UI_STYLES.colors.focusColor); break;
        default: break;
    }

    // --- Panel Group Drawing ---
    GuiGroupBox(topLeftPanel, "CONTROLS");
    GuiGroupBox(topRightPanel, "MAIN DISPLAY");
    GuiGroupBox(bottomRightPanel, "AUXILIARY DISPLAYS");

    // --- Call Drawing Functions for each Panel ---
    MainMenuDrawTopLeftPanel(ctx, topLeftPanel);
    MainMenuDrawTopRightPanel(ctx, topRightPanel);
    MainMenuDrawbottomRightPanel(ctx, bottomRightPanel);
}

//================================================================================
// Private Function Implementations
//================================================================================

//--------------------------------------------------------------------------------
// Top-Left Panel (Controls)
//--------------------------------------------------------------------------------

/**
 * @brief Draws the top-left panel, which contains all simulation controls.
 * @param ctx Pointer to the global AppContext.
 * @param topLeftPanel The rectangle defining the bounds of this panel.
 */
static void MainMenuDrawTopLeftPanel(AppContext *ctx, Rectangle topLeftPanel) {
    Rectangle leftContent = {
        topLeftPanel.x + G_UI_STYLES.layout.padding,
        topLeftPanel.y + G_UI_STYLES.layout.padding,
        topLeftPanel.width - G_UI_STYLES.layout.padding * 2,
        topLeftPanel.height - G_UI_STYLES.layout.padding * 2
    };

    float posX   = leftContent.x;
    float posY   = leftContent.y + G_UI_STYLES.layout.padding;
    float width  = leftContent.width;
    float height = G_UI_STYLES.button.height;

    MainMenuDrawControlButtons(ctx, (Rectangle){ posX, posY, width, height });
    posY += G_UI_STYLES.layout.spacingBetweenLines + G_UI_STYLES.layout.padding * 2;

    MainMenuDrawModelSelectors(ctx, (Rectangle){ posX, posY, width, height }, &posY);

    MainMenuDrawSliders(ctx, (Rectangle){ posX, posY, width, height }, &posY);
}

/**
 * @brief Draws the Start, Pause, and Reset buttons and handles their logic.
 * @param ctx Pointer to the global AppContext.
 * @param layout The rectangle defining the layout area for these buttons.
 */
static void MainMenuDrawControlButtons(AppContext *ctx, Rectangle layout) {
    float buttonwidth  = (layout.width - (G_UI_STYLES.layout.padding * 2)) / 3;

    Rectangle btnStart = { layout.x, layout.y, buttonwidth, layout.height };
    Rectangle btnPause = { layout.x + buttonwidth + G_UI_STYLES.layout.padding, layout.y, buttonwidth, layout.height };
    Rectangle btnReset = { layout.x + (buttonwidth + G_UI_STYLES.layout.padding) * 2, layout.y, buttonwidth, layout.height };

    if (GuiButton(btnStart, "START")) {
        SimulationReset(ctx);

        if (!ctx->simState.runtime.isRunning) {
            ctx->simState.runtime.isRunning = true;

            if (ctx->tabs.activeNeuronModel == IZHIKEVICH_MODEL) {
                int IzhikevichType = ctx->tabs.activeIzhikevichModel;
                ctx->simState.models.izModel = IzhikevichInitModel(IzhikevichType, K_DT);
            }

            if (ctx->tabs.activeNeuronModel == HODGKIN_HUXLEY_MODEL) {
                ctx->simState.models.hhModel = HodgkinHuxleyInitModel(K_DT);
            }
        }
    }

    bool simulationStarted      = (ctx->simState.models.izModel != NULL || ctx->simState.models.hhModel != NULL);
    const char *pauseButtonText = ctx->simState.runtime.isRunning ? "PAUSE" : "CONTINUE";

    if (!simulationStarted) GuiSetState(STATE_DISABLED);
    if (GuiButton(btnPause, pauseButtonText)) ctx->simState.runtime.isRunning = !ctx->simState.runtime.isRunning;
    GuiSetState(STATE_NORMAL);

    if (GuiButton(btnReset, "RESET")) {
        SimulationReset(ctx);
        ctx->simState.runtime.isRunning = false;
    }

    // --- Button Focus Highlighting ---
    Color color = G_UI_STYLES.colors.focusColor;
    float thickness = G_UI_STYLES.global.focusThickness;
    if (ctx->focus.focusTab == FOCUS_CONTROLS) {
        switch (ctx->focus.activeControlFocus) {
            case CONTROL_FOCUS_NONE: break;
            case CONTROL_FOCUS_START_BUTTON: DrawRectangleLinesEx(btnStart, thickness, color); break;
            case CONTROL_FOCUS_PAUSE_BUTTON: DrawRectangleLinesEx(btnPause, thickness, color); break;
            case CONTROL_FOCUS_RESET_BUTTON: DrawRectangleLinesEx(btnReset, thickness, color); break;
            default: break;
        }
    }
}

/**
 * @brief Draws the ComboBoxes for selecting the main neuron model and the Izhikevich type.
 * @param ctx Pointer to the global AppContext.
 * @param layout The rectangle for the first widget.
 * @param posY A pointer to the current Y-position, which will be updated by this function.
 */
static void MainMenuDrawModelSelectors (AppContext *ctx, Rectangle layout, float *posY) {
    bool simulationStarted = (ctx->simState.models.izModel != NULL || ctx->simState.models.hhModel != NULL);

    GuiLabel((Rectangle){ layout.x, *posY, layout.width, layout.height }, "Neuron");
    *posY += G_UI_STYLES.layout.spacingBetweenLines;

    if (simulationStarted) GuiSetState(STATE_DISABLED);
    Rectangle sltNrnModel = { layout.x, *posY, layout.width, layout.height };
    GuiComboBox(sltNrnModel, kNrnModelStr, (int*)&ctx->tabs.activeNeuronModel);
    GuiSetState(STATE_NORMAL);

    *posY += G_UI_STYLES.layout.spacingBetweenLines;

    Rectangle sltIzModel = { layout.x, *posY + G_UI_STYLES.layout.spacingBetweenLines, layout.width, layout.height };
    if (ctx->tabs.activeNeuronModel == IZHIKEVICH_MODEL) {

        if (simulationStarted) GuiSetState(STATE_DISABLED);
        GuiComboBox(sltIzModel, KIzModelStr, (int*)&ctx->tabs.activeIzhikevichModel);
        GuiSetState(STATE_NORMAL);

        *posY += G_UI_STYLES.layout.spacingBetweenLines + G_UI_STYLES.layout.padding;
    }

    *posY += G_UI_STYLES.layout.spacingBetweenLines + G_UI_STYLES.layout.padding;

    Color color = G_UI_STYLES.colors.focusColor;
    float thickness = G_UI_STYLES.global.focusThickness;
    switch (ctx->focus.activeControlFocus) {
        case CONTROL_FOCUS_MODEL_SELECTOR: DrawRectangleLinesEx(sltNrnModel, thickness, color); break;
        case CONTROL_FOCUS_IZ_MODEL_SELECTOR: DrawRectangleLinesEx(sltIzModel, thickness, color); break;
        default: break;
    }
}

/**
 * @brief Draws the sliders for user input, such as external current.
 * @param ctx Pointer to the global AppContext.
 * @param layout The rectangle for the first widget.
 * @param posY A pointer to the current Y-position, which will be updated by this function.
 */
static void MainMenuDrawSliders(AppContext *ctx, Rectangle layout, float *posY) {
    const char *externCurrentText = TextFormat("Extern current: %.2f pA", ctx->simState.inputs.externCurrent);

    Rectangle barRect = { layout.x, *posY, 300, 20 };

    GuiLabel(barRect, externCurrentText);
    barRect.y += G_UI_STYLES.layout.padding * 2;
    GuiSliderBar(barRect, NULL, NULL, &ctx->simState.inputs.externCurrent, G_UI_STYLES.slider.currentMinValue, G_UI_STYLES.slider.currentMaxvalue);

    *posY = barRect.y + G_UI_STYLES.layout.padding * 3;
    *posY += G_UI_STYLES.layout.spacingBetweenLines + G_UI_STYLES.layout.verticalGroupSpacing;

    Color color = G_UI_STYLES.colors.focusColor;
    float thickness = G_UI_STYLES.global.focusThickness;
    switch (ctx->focus.activeControlFocus) {
        case CONTROL_FOCUS_CURRENT_SLIDER: DrawRectangleLinesEx(barRect, thickness, color); break;
        default: break;
    }
}

//--------------------------------------------------------------------------------
// Top-Right Panel (Main Display)
//--------------------------------------------------------------------------------

/**
 * @brief Draws the top-right panel, which contains the main plot tabs.
 * @param ctx Pointer to the global AppContext.
 * @param topRightPanel The rectangle defining the bounds of this panel.
 */
static void MainMenuDrawTopRightPanel(AppContext *ctx, Rectangle topRightPanel) {
    Rectangle topRightContent = {
        topRightPanel.x + G_UI_STYLES.layout.padding * 2.0f,
        topRightPanel.y + G_UI_STYLES.layout.padding * 2.0f,
        topRightPanel.width - G_UI_STYLES.layout.padding * 4.0f,
        topRightPanel.height - G_UI_STYLES.layout.padding * 4.0f
    };

    Rectangle tabRect = {
        topRightContent.x,
        topRightContent.y + G_UI_STYLES.layout.padding,
        topRightContent.width,
        G_UI_STYLES.tab.height
    };

    Rectangle tabContentRect = {
        topRightContent.x + G_UI_STYLES.layout.padding * 2,
        topRightContent.y + G_UI_STYLES.layout.padding * 5,
        topRightContent.width - G_UI_STYLES.layout.padding * 4.0f,
        topRightContent.height - G_UI_STYLES.layout.padding * 4.0f
    };

    // --- Main Display Tabs ---
    const char *tabsName[] = {"Main Graph", "Auxiliary Graph" };
    int result             = GuiTabBar(tabRect, tabsName, 2, (int*)&ctx->tabs.activeMainFocus);
    if (result >= 0 && result < 2) ctx->tabs.activeMainFocus = (MainTabType)result;

    switch (ctx->tabs.activeMainFocus) {
        case TAB_MAIN_GRAPH: {
            PlotCfg mainPlotCfg = {
                .dataColor  = G_UI_STYLES.colors.plotColor1,
                .xLabel     = "Time (ms)",
                .yLabel     = "Potential (mV)",
                .axisMargin = G_UI_STYLES.plot.axisMargin,
                .xMin       = G_UI_STYLES.slider.currentMinValue,
                .xMax       = G_PLOT_STATE.plotXMax,
                .yMin       = G_PLOT_STATE.plotYMin,
                .yMax       = G_PLOT_STATE.plotYMax,
                .dataCount  = ctx->simState.plotData.dataCount,
                .fontSize   = G_UI_STYLES.plot.fontSize,
                .bounds     = tabContentRect,
                .data       = ctx->simState.plotData.membranePotential
            };

            GuiPlotDraw(&mainPlotCfg);
        } break;

        case TAB_AUXILIARY_GRAPH: {
            switch (ctx->tabs.activeNeuronModel) {
                case IZHIKEVICH_MODEL: MainMenuDrawIzPhasePlot(ctx, tabContentRect); break;
                case HODGKIN_HUXLEY_MODEL: MainMenuDrawHHPhasePlots(ctx, tabContentRect); break;
                default: break;
            }
        } break;

        default: break;
    }
}

//--------------------------------------------------------------------------------
// Bottom-Right Panel (Auxiliary Display)
//--------------------------------------------------------------------------------

/**
 * @brief Draws the bottom-right panel, which contains info tabs.
 * @param ctx Pointer to the global AppContext.
 * @param bottomRightPanel The rectangle defining the bounds of this panel.
 */
static void MainMenuDrawbottomRightPanel(AppContext *ctx, Rectangle bottomRightPanel) {
    Rectangle bottomRightContent = {
        bottomRightPanel.x + G_UI_STYLES.layout.padding * 2.0f,
        bottomRightPanel.y + G_UI_STYLES.layout.padding * 2.0f,
        bottomRightPanel.width - G_UI_STYLES.layout.padding * 4.0f,
        bottomRightPanel.height - G_UI_STYLES.layout.padding * 4.0f
    };

    Rectangle tabRect = {
        bottomRightContent.x,
        bottomRightContent.y + G_UI_STYLES.layout.padding,
        bottomRightContent.width,
        G_UI_STYLES.tab.height
    };

    Rectangle tabContentRect = {
        bottomRightContent.x + G_UI_STYLES.layout.padding * 2,
        bottomRightContent.y + G_UI_STYLES.layout.padding * 5,
        bottomRightContent.width - G_UI_STYLES.layout.padding * 4.0f,
        bottomRightContent.height - G_UI_STYLES.layout.padding * 4.0f
    };

    // --- Auxiliary Display Tabs ---
    const char *tabsName[] = { "Actual state", "Event Log" };
    int result             = GuiTabBar(tabRect, tabsName, 2, (int*)&ctx->tabs.activeTab);
    if (result >= 0 && result < 2) ctx->tabs.activeTab = (AuxiliaryTabType)result;

    switch (ctx->tabs.activeTab) {
        case TAB_STATE: MainMenuDrawStateTab(ctx, tabContentRect); break;
        case TAB_EVENTS: MainMenuDrawEventsTab(ctx, tabContentRect); break;
        default: break;
    }
}

/**
 * @brief Draws the content of the "Actual state" tab (e.g., current time, potential).
 * @param ctx Pointer to the global AppContext.
 * @param labelRect The rectangle defining the area for the text labels.
 */
static void MainMenuDrawStateTab(AppContext *ctx, Rectangle labelRect) {
    bool simulationStarted = (ctx->simState.models.izModel != NULL || ctx->simState.models.hhModel != NULL);
    if (!simulationStarted) DrawText("Simulation not running", labelRect.x, labelRect.y, G_UI_STYLES.plot.fontSize, G_UI_STYLES.colors.textColor);

    Color color    = G_UI_STYLES.colors.textColor;
    int fontSize   = G_UI_STYLES.plot.fontSize;
    int padding    = G_UI_STYLES.layout.padding;
    int lineHeight = fontSize + 5;

    int posX = labelRect.x;
    int posY = labelRect.y;

    int index = ctx->simState.plotData.dataCount - 1;
    if (ctx->simState.plotData.dataCount > 0) {
        switch (ctx->tabs.activeNeuronModel) {
            case IZHIKEVICH_MODEL: {
                Vector2 lastData = ctx->simState.plotData.phase[index];

                const char *timeText = TextFormat("Time: %.2f ms", ctx->simState.runtime.currentTime);
                DrawText(timeText, posX, posY, fontSize, color);
                posX += MeasureText(timeText, fontSize) + padding;

                const char *recoveryText = TextFormat("| Recovery: %.2f", lastData.x);
                DrawText(recoveryText, posX, posY, fontSize, color);
                posX += MeasureText(recoveryText, fontSize) + padding;

                const char *potentialText = TextFormat("| Potential: %.2f", lastData.y);
                DrawText(potentialText, posX, posY, fontSize, color);
            } break;

            case HODGKIN_HUXLEY_MODEL: {
                Vector2 iK           = ctx->simState.plotData.hhCurrentPlots.kCurrent[index];
                Vector2 iNa          = ctx->simState.plotData.hhCurrentPlots.naCurrent[index];
                Vector2 iLeak        = ctx->simState.plotData.hhCurrentPlots.leakCurrent[index];
                Vector2 mGateProb    = ctx->simState.plotData.hhGatePlots.MGate[index];
                Vector2 hGateProb    = ctx->simState.plotData.hhGatePlots.HGate[index];
                Vector2 nGateProb    = ctx->simState.plotData.hhGatePlots.NGate[index];
                Vector2 memPotential = ctx->simState.plotData.membranePotential[index];

                const char *timeText = TextFormat("Time: %.2f ms", ctx->simState.runtime.currentTime);
                DrawText(timeText, posX, posY, fontSize, color);
                posX += MeasureText(timeText, fontSize) + (padding);

                const char *potText = TextFormat("| Potential: %.2f mV", memPotential.y);
                DrawText(potText, posX, posY, fontSize, color);

                posY += lineHeight;
                posX = labelRect.x;

                const char *mText = TextFormat("M Gate: %.2f", mGateProb.y);
                DrawText(mText, posX, posY, fontSize, color);
                posX += MeasureText(mText, fontSize) + padding;

                const char *hText = TextFormat("| H Gate: %.2f", hGateProb.y);
                DrawText(hText, posX, posY, fontSize, color);
                posX += MeasureText(hText, fontSize) + padding;

                const char *nText = TextFormat("| N Gate: %.2f", nGateProb.y);
                DrawText(nText, posX, posY, fontSize, color);

                posY += lineHeight;
                posX = labelRect.x;

                const char *naText = TextFormat("Na Current: %.2f pA", iNa.y);
                DrawText(naText, posX, posY, fontSize, color);
                posX += MeasureText(naText, fontSize) + padding;

                const char *kText = TextFormat("| K Current: %.2f pA", iK.y);
                DrawText(kText, posX, posY, fontSize, color);
                posX += MeasureText(kText, fontSize) + padding;

                const char *leakText = TextFormat("| Leak Current: %.2f pA", iLeak.y);
                DrawText(leakText, posX, posY, fontSize, color);
            }
        }
    }
}

/**
 * @brief Draws the content of the "Event Log" tab.
 * @param ctx Pointer to the global AppContext.
 * @param labelRect The rectangle defining the area for the text labels.
 */
static void MainMenuDrawEventsTab(AppContext *ctx, Rectangle labelRect) {
    DrawText("Soon...", labelRect.x + 10, labelRect.y + 10, 40, G_UI_STYLES.colors.textSpecial);
}


//--------------------------------------------------------------------------------
// Plot Drawing Functions (Panel Helpers)
//--------------------------------------------------------------------------------

/**
 * @brief Draws the Izhikevich phase plot (Recovery vs. Potential).
 * @param ctx Pointer to the global AppContext.
 * @param graphRect The rectangle defining the area for the plot.
 */
static void MainMenuDrawIzPhasePlot(AppContext *ctx, Rectangle graphRect) {
    PlotCfg izPhasePlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Recovery (mV)",
        .yLabel     = "Potential (mV)",
        .dataColor  = G_UI_STYLES.colors.plotColor2,
        .xMin       = G_PLOT_STATE.phaseXMin,
        .xMax       = G_PLOT_STATE.phaseXMax,
        .yMin       = G_PLOT_STATE.phaseYMin,
        .yMax       = G_PLOT_STATE.phaseYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphRect,
        .data       = ctx->simState.plotData.phase
    };

    GuiPlotDraw(&izPhasePlotCfg);
}

/**
 * @brief Draws the Hodgkin-Huxley scrolling plots (Currents and Gating variables).
 * @param ctx Pointer to the global AppContext.
 * @param tabContentRect The rectangle defining the total area for this tab.
 */
static void MainMenuDrawHHPhasePlots(AppContext *ctx, Rectangle tabContentRect) {
    float singlePlotHeight = tabContentRect.height - G_UI_STYLES.layout.padding * 4.0f;

    // Virtual area for the scroll (2 plots high)
    Rectangle contentSize = {
        0, 0,
        tabContentRect.width - G_UI_STYLES.layout.padding * 2,
        (singlePlotHeight * 2) + (G_UI_STYLES.layout.padding * 4.)
    };

    Rectangle visibleContent = { 0 };
    GuiScrollPanel(tabContentRect, NULL, contentSize, &ctx->tabs.phasePlotScroll, &visibleContent);

    BeginScissorMode(visibleContent.x, visibleContent.y, visibleContent.width, visibleContent.height);

    float plotwidth = visibleContent.width - G_UI_STYLES.layout.padding * 4.0f;

    // Plot positions inside the scroll panel
    Rectangle graphArea1 = {
        visibleContent.x + G_UI_STYLES.layout.padding * 2.0f,
        visibleContent.y + G_UI_STYLES.layout.padding * 2.0f + ctx->tabs.phasePlotScroll.y,
        plotwidth,
        singlePlotHeight
    };

    Rectangle graphArea2 = {
        visibleContent.x + G_UI_STYLES.layout.padding * 2.0f,
        graphArea1.y + singlePlotHeight + G_UI_STYLES.layout.padding * 2.0f,
        plotwidth,
        singlePlotHeight
    };

    // --- HH Plot Configurations ---

    // Plot 1: Currents
    PlotCfg hhINaPlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Time (ms)",
        .yLabel     = "Current pA",
        .dataColor  = G_UI_STYLES.colors.plotColor1,
        .xMin       = G_PLOT_STATE.plotXMin,
        .xMax       = G_PLOT_STATE.plotXMax,
        .yMin       = G_PLOT_STATE.currentYMin,
        .yMax       = G_PLOT_STATE.currentYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphArea1,
        .data       = ctx->simState.plotData.hhCurrentPlots.naCurrent
    };

    PlotCfg hhIKPlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Time (ms)",
        .yLabel     = "Current pA",
        .dataColor  = G_UI_STYLES.colors.plotColor2,
        .xMin       = G_PLOT_STATE.plotXMin,
        .xMax       = G_PLOT_STATE.plotXMax,
        .yMin       = G_PLOT_STATE.currentYMin,
        .yMax       = G_PLOT_STATE.currentYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphArea1,
        .data       = ctx->simState.plotData.hhCurrentPlots.kCurrent
    };

    PlotCfg hhILeakPlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Time (ms)",
        .yLabel     = "Current pA",
        .dataColor  = G_UI_STYLES.colors.plotColor3,
        .xMin       = G_PLOT_STATE.plotXMin,
        .xMax       = G_PLOT_STATE.plotXMax,
        .yMin       = G_PLOT_STATE.currentYMin,
        .yMax       = G_PLOT_STATE.currentYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphArea1,
        .data       = ctx->simState.plotData.hhCurrentPlots.leakCurrent
    };

    // Plot 2: Probabilities
    PlotCfg hhMProbPlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Time (ms)",
        .yLabel     = "Gate probability",
        .dataColor  = G_UI_STYLES.colors.plotColor1,
        .xMin       = G_PLOT_STATE.plotXMin,
        .xMax       = G_PLOT_STATE.plotXMax,
        .yMin       = G_PLOT_STATE.probYMin,
        .yMax       = G_PLOT_STATE.probYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphArea2,
        .data       = ctx->simState.plotData.hhGatePlots.MGate
    };

    PlotCfg hhNProbPlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Time (ms)",
        .yLabel     = "Gate probability",
        .dataColor  = G_UI_STYLES.colors.plotColor2,
        .xMin       = G_PLOT_STATE.plotXMin,
        .xMax       = G_PLOT_STATE.plotXMax,
        .yMin       = G_PLOT_STATE.probYMin,
        .yMax       = G_PLOT_STATE.probYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphArea2,
        .data       = ctx->simState.plotData.hhGatePlots.NGate
    };

    PlotCfg hhHProbPlotCfg = {
        .axisMargin = G_UI_STYLES.plot.axisMargin,
        .xLabel     = "Time (ms)",
        .yLabel     = "Gate probability",
        .dataColor  = G_UI_STYLES.colors.plotColor3,
        .xMin       = G_PLOT_STATE.plotXMin,
        .xMax       = G_PLOT_STATE.plotXMax,
        .yMin       = G_PLOT_STATE.probYMin,
        .yMax       = G_PLOT_STATE.probYMax,
        .dataCount  = ctx->simState.plotData.dataCount,
        .fontSize   = G_UI_STYLES.plot.fontSize,
        .bounds     = graphArea2,
        .data       = ctx->simState.plotData.hhGatePlots.HGate
    };

    // --- Plot Drawing ---

    // Plot 1: Currents (draw axes + 3 data lines)
    GuiPlotDrawAxes(&hhINaPlotCfg);
    GuiPlotDrawData(&hhIKPlotCfg);
    GuiPlotDrawData(&hhILeakPlotCfg);
    GuiPlotDrawData(&hhINaPlotCfg); // Draw Na last to be "on top"

    // Plot 2: Probabilities (draw axes + 3 data lines)
    GuiPlotDrawAxes(&hhMProbPlotCfg);
    GuiPlotDrawData(&hhNProbPlotCfg);
    GuiPlotDrawData(&hhHProbPlotCfg);
    GuiPlotDrawData(&hhMProbPlotCfg); // Draw M last

    EndScissorMode();
}
