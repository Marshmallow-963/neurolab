/**
 * @file app_state.h
 * @brief Defines the core data structures for the entire application,
 * including simulation state, model pointers, and GUI state.
 *
 * This file acts as the global context for the application, aggregating
 * all distinct state modules (simulation, GUI, focus, etc.).
 */
#ifndef APP_STATE_H
#define APP_STATE_H

#include "raygui.h"
#include <stdbool.h>
#include "gui/plotting/plot_state.h"
#include "simulation/simulation_state.h"

// --- Application-Level State ---

/** @brief Defines the main application screens. */
typedef enum {
    MAIN_MENU = 0,
    DOCUMENTATION
} ScreenMenus;

/**
 * @struct AppState
 * @brief Groups high-level application state (e.g., current screen).
 */
typedef struct {
    ScreenMenus currentScreen;
    Vector2 docScroll;
} AppState;

// --- Keyboard Focus/Navigation State ---

/** @brief Defines the main focused UI panel. */
typedef enum {
    FOCUS_NONE = 0,
    FOCUS_CONTROLS,
    FOCUS_MAIN_DISPLAY,
    FOCUS_AUXILIARY_DISPLAY
} FocusedTab;

/** @brief Defines the focused widget within the control panel. */
typedef enum {
    CONTROL_FOCUS_NONE = 0,
    CONTROL_FOCUS_START_BUTTON,
    CONTROL_FOCUS_PAUSE_BUTTON,
    CONTROL_FOCUS_RESET_BUTTON,
    CONTROL_FOCUS_MODEL_SELECTOR,
    CONTROL_FOCUS_IZ_MODEL_SELECTOR,
    CONTROL_FOCUS_CURRENT_SLIDER
} ControlFocus;

/** @brief Defines the focused widget within the auxiliary panel. */
typedef enum {
    AUX_FOCUS_NONE = 0,
    AUX_FOCUS_TAB_BAR,
    AUX_FOCUS_HH_PLOT_SCROLL
} AuxiliaryFocus;

/**
 * @struct FocusState
 * @brief Aggregates all keyboard focus-related state.
 */
typedef struct {
    FocusedTab focusTab;
    ControlFocus activeControlFocus;
    AuxiliaryFocus activeAuxFocus;
} FocusState;

// --- GUI Tab/Panel State ---

/** @brief Defines the tabs in the auxiliary (right-hand) panel. */
typedef enum {
    TAB_STATE = 0,
    TAB_EVENTS
} AuxiliaryTabType;

/** @brief Defines the tabs in the main (center) panel. */
typedef enum {
    TAB_MAIN_GRAPH = 0,      ///< e.g., Membrane Potential vs. Time
    TAB_AUXILIARY_GRAPH      ///< e.g., Phase Plot or Gate Variables
} MainTabType;

/**
 * @struct Tabs
 * @brief Encapsulates the state of the GUI panels and tabs.
 */
typedef struct {
    AuxiliaryTabType activeTab;
    MainTabType activeMainFocus;
    NeuronModel activeNeuronModel;
    IzNeuronType activeIzhikevichModel;
    Vector2 phasePlotScroll;
    Vector2 gatePlotScroll;
    Vector2 currentPlotScroll;
} Tabs;

// --- Global Application Context ---

/**
 * @struct AppContext
 * @brief The main application context struct.
 *
 * This struct aggregates all sub-states of the application into a
 * single context object to be passed to update and drawing functions.
 */
typedef struct {
    SimulationState simState; ///< All simulation-related data and runtime state.
    Tabs tabs;                ///< State of GUI tabs and panels.
    FocusState focus;         ///< State of the keyboard navigation focus.
    AppState app;             ///< High-level application state (e.g., screen).
} AppContext;

#endif // APP_STATE_H
