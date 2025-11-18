/**
 * @file keys_logic.c
 * @brief Implementation of the keyboard input handling logic.
 *
 * This file contains the state machine for navigating the UI using
 * the keyboard, updating the application's focus state. It uses
 * static helper functions to break down the logic by component.
 */

#include "raygui.h"
#include "raylib.h"
#include "gui/input/keys_logic.h"
#include "gui/themes/gui_styles.h"
#include "simulation/simulation_logic.h"

//================================================================================
// Static Forward Declarations
//================================================================================

/**
 * @brief Handles key presses related to global screen switching.
 * (e.g., F2 for Documentation, Backspace to return)
 * @param ctx Pointer to the global AppContext.
 */
static void ScreenHandleKeys(AppContext *ctx);

/**
 * @brief Handles navigation *between* the main UI panels.
 * (e.g., FOCUS_CONTROLS <-> FOCUS_MAIN_DISPLAY)
 * @param ctx Pointer to the global AppContext.
 */
static void PanelHandleNavigation(AppContext *ctx);

/**
 * @brief Handles navigation *within* the FOCUS_CONTROLS panel.
 * (e.g., START_BUTTON -> PAUSE_BUTTON)
 * @param ctx Pointer to the global AppContext.
 */
static void ControlsHandleNavigation(AppContext *ctx);

/**
 * @brief Handles actions (ENTER, slider changes) *within* the
 * FOCUS_CONTROLS panel.
 * @param ctx Pointer to the global AppContext.
 */
static void ControlsHandleFocusActions(AppContext *ctx);

//================================================================================
// Public Function Implementation
//================================================================================

/**
 * @brief Handles all global keyboard input for the application.
 * (This is the main entry point, which calls static helpers).
 * @param ctx Pointer to the global AppContext.
 */
void InputHanndleKeys(AppContext *ctx) {
    ScreenHandleKeys(ctx);

    PanelHandleNavigation(ctx);

    ControlsHandleNavigation(ctx);

    ControlsHandleFocusActions(ctx);
}


//================================================================================
// Static Function Implementations
//================================================================================

/**
 * @brief (Componente: Screen, Verbo: HandleKeys)
 * Handles key presses related to global screen switching.
 */
static void ScreenHandleKeys(AppContext *ctx) {
    if (ctx->app.currentScreen == DOCUMENTATION && IsKeyPressed(KEY_BACKSPACE)) ctx->app.currentScreen = MAIN_MENU;

    if (ctx->app.currentScreen == MAIN_MENU && IsKeyPressed(KEY_F2)) ctx->app.currentScreen = DOCUMENTATION;
}

/**
 * @brief (Componente: Panel, Verbo: HandleNavigation)
 * Handles navigation *between* the main UI panels.
 */
static void PanelHandleNavigation(AppContext *ctx) {
    switch (ctx->focus.focusTab) {
        case FOCUS_NONE: if (IsKeyPressed(KEY_DOWN)) ctx->focus.focusTab = FOCUS_CONTROLS; break;

        case FOCUS_CONTROLS: {
            if (ctx->focus.activeControlFocus == CONTROL_FOCUS_NONE) {
                if (IsKeyPressed(KEY_UP))    ctx->focus.focusTab = FOCUS_NONE;
                if (IsKeyPressed(KEY_RIGHT)) ctx->focus.focusTab = FOCUS_MAIN_DISPLAY;
            }
        } break;

        case FOCUS_MAIN_DISPLAY: {
            if (IsKeyPressed(KEY_UP))    ctx->focus.focusTab = FOCUS_NONE;
            if (IsKeyPressed(KEY_LEFT))  ctx->focus.focusTab = FOCUS_CONTROLS;
            if (IsKeyPressed(KEY_DOWN))  ctx->focus.focusTab = FOCUS_AUXILIARY_DISPLAY;
        } break;

        case FOCUS_AUXILIARY_DISPLAY: {
            if (IsKeyPressed(KEY_UP))    ctx->focus.focusTab = FOCUS_MAIN_DISPLAY;
            if (IsKeyPressed(KEY_LEFT))  ctx->focus.focusTab = FOCUS_CONTROLS;
            if (IsKeyPressed(KEY_DOWN))  ctx->focus.focusTab = FOCUS_NONE;
        } break;
    }
}

/**
 * @brief (Componente: Controls, Verbo: HandleNavigation)
 * Handles navigation *within* the FOCUS_CONTROLS panel.
 */
static void ControlsHandleNavigation(AppContext *ctx) {
    if (ctx->focus.focusTab != FOCUS_CONTROLS) return;

    switch (ctx->focus.activeControlFocus) {
        case CONTROL_FOCUS_NONE: if (IsKeyPressed(KEY_DOWN)) ctx->focus.activeControlFocus = CONTROL_FOCUS_START_BUTTON; break;

        case CONTROL_FOCUS_START_BUTTON: {
            if (IsKeyPressed(KEY_UP))    ctx->focus.activeControlFocus = CONTROL_FOCUS_NONE;
            if (IsKeyPressed(KEY_DOWN))  ctx->focus.activeControlFocus = CONTROL_FOCUS_MODEL_SELECTOR;
            if (IsKeyPressed(KEY_LEFT))  ctx->focus.activeControlFocus = CONTROL_FOCUS_RESET_BUTTON;
            if (IsKeyPressed(KEY_RIGHT)) ctx->focus.activeControlFocus = CONTROL_FOCUS_PAUSE_BUTTON;
        } break;

        case CONTROL_FOCUS_PAUSE_BUTTON: {
            if (IsKeyPressed(KEY_UP))    ctx->focus.activeControlFocus = CONTROL_FOCUS_NONE;
            if (IsKeyPressed(KEY_DOWN))  ctx->focus.activeControlFocus = CONTROL_FOCUS_MODEL_SELECTOR;
            if (IsKeyPressed(KEY_LEFT))  ctx->focus.activeControlFocus = CONTROL_FOCUS_START_BUTTON;
            if (IsKeyPressed(KEY_RIGHT)) ctx->focus.activeControlFocus = CONTROL_FOCUS_RESET_BUTTON;
        } break;

        case CONTROL_FOCUS_RESET_BUTTON: {
            if (IsKeyPressed(KEY_UP))    ctx->focus.activeControlFocus = CONTROL_FOCUS_NONE;
            if (IsKeyPressed(KEY_DOWN))  ctx->focus.activeControlFocus = CONTROL_FOCUS_MODEL_SELECTOR;
            if (IsKeyPressed(KEY_LEFT))  ctx->focus.activeControlFocus = CONTROL_FOCUS_PAUSE_BUTTON;
            if (IsKeyPressed(KEY_RIGHT)) ctx->focus.activeControlFocus = CONTROL_FOCUS_START_BUTTON;
        } break;

        case CONTROL_FOCUS_MODEL_SELECTOR: {
            if (IsKeyPressed(KEY_UP)) ctx->focus.activeControlFocus = CONTROL_FOCUS_START_BUTTON;

            if (ctx->tabs.activeNeuronModel == IZHIKEVICH_MODEL) {
                if (IsKeyPressed(KEY_DOWN)) ctx->focus.activeControlFocus = CONTROL_FOCUS_IZ_MODEL_SELECTOR;
            } else if (ctx->tabs.activeNeuronModel == HODGKIN_HUXLEY_MODEL) {
                if (IsKeyPressed(KEY_DOWN)) ctx->focus.activeControlFocus = CONTROL_FOCUS_CURRENT_SLIDER;
            }
        } break;

        case CONTROL_FOCUS_IZ_MODEL_SELECTOR: {
            if (IsKeyPressed(KEY_UP))   ctx->focus.activeControlFocus = CONTROL_FOCUS_MODEL_SELECTOR;
            if (IsKeyPressed(KEY_DOWN)) ctx->focus.activeControlFocus = CONTROL_FOCUS_CURRENT_SLIDER;
        } break;

        case CONTROL_FOCUS_CURRENT_SLIDER: {
            if (ctx->tabs.activeNeuronModel == IZHIKEVICH_MODEL) {
                if (IsKeyPressed(KEY_UP)) ctx->focus.activeControlFocus = CONTROL_FOCUS_IZ_MODEL_SELECTOR;
            } else if (ctx->tabs.activeNeuronModel == HODGKIN_HUXLEY_MODEL) {
                if (IsKeyPressed(KEY_UP)) ctx->focus.activeControlFocus = CONTROL_FOCUS_MODEL_SELECTOR;
            }

            if (IsKeyPressed(KEY_DOWN))  ctx->focus.activeControlFocus = CONTROL_FOCUS_NONE;
        } break;
    }
}

/**
 * @brief (Componente: Controls, Verbo: HandleFocusNavigation)
 * Handles navigation *within* the FOCUS_CONTROLS panel (changing focus).
 */
static void ControlsHandleFocusActions(AppContext *ctx) {
    bool simulationStarted = (ctx->simState.models.izModel != NULL || ctx->simState.models.hhModel != NULL);
    if (ctx->focus.focusTab != FOCUS_CONTROLS) return;

    if (ctx->focus.activeControlFocus == CONTROL_FOCUS_CURRENT_SLIDER) {
        float *current = &ctx->simState.inputs.externCurrent;
        float min = G_UI_STYLES.slider.currentMinValue;
        float max = G_UI_STYLES.slider.currentMaxvalue;

        bool continuoMode = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        if (continuoMode) {
            float continuousSpeed =  G_UI_STYLES.slider.step * 10.0f;
            if (IsKeyDown(KEY_LEFT))  *current -= continuousSpeed * GetFrameTime();
            if (IsKeyDown(KEY_RIGHT)) *current += continuousSpeed * GetFrameTime();
        } else {
            if (IsKeyPressed(KEY_LEFT))  *current -= G_UI_STYLES.slider.step;
            if (IsKeyPressed(KEY_RIGHT)) *current += G_UI_STYLES.slider.step;
        }

        if (*current < min) *current = min;
        if (*current > max) *current = max;
    }

    switch (ctx->focus.activeControlFocus) {
        case CONTROL_FOCUS_START_BUTTON: {
            if (IsKeyPressed(KEY_ENTER)) {
                SimulationReset(ctx);

                if (!ctx->simState.runtime.isRunning) {
                    ctx->simState.runtime.isRunning = true;

                    if (ctx->tabs.activeNeuronModel == IZHIKEVICH_MODEL) ctx->simState.models.izModel = IzhikevichInitModel(ctx->tabs.activeIzhikevichModel, K_DT);
                    if (ctx->tabs.activeNeuronModel == HODGKIN_HUXLEY_MODEL) ctx->simState.models.hhModel = HodgkinHuxleyInitModel(K_DT);
                }
            }
        } break;

        case CONTROL_FOCUS_PAUSE_BUTTON: {
            if (IsKeyPressed(KEY_ENTER)) {
                if (simulationStarted) ctx->simState.runtime.isRunning = !ctx->simState.runtime.isRunning;
            }
        } break;

        case CONTROL_FOCUS_RESET_BUTTON: {
            if (IsKeyPressed(KEY_ENTER)) {
                SimulationReset(ctx);
                ctx->simState.runtime.isRunning = false;
            }
        } break;

        case CONTROL_FOCUS_MODEL_SELECTOR: {
            if (simulationStarted) break;

            int *model = (int*)&ctx->tabs.activeNeuronModel;
            const int numModels = 2;

            if (IsKeyPressed(KEY_LEFT)) *model = (*model - 1 + numModels) % numModels;
            if (IsKeyPressed(KEY_RIGHT)) *model = (*model + 1) % numModels;
        } break;

        case CONTROL_FOCUS_IZ_MODEL_SELECTOR: {
            if (simulationStarted) break;

            int *model = (int*)&ctx->tabs.activeIzhikevichModel;
            const int numModels = 7;

            if (IsKeyPressed(KEY_LEFT)) *model = (*model - 1 + numModels) % numModels;
            if (IsKeyPressed(KEY_RIGHT)) *model = (*model + 1) % numModels;
        } break;

        default: break;
    }
}
