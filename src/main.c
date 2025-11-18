#include "raylib.h"
#include "app_state.h"
#include "gui/input/keys_logic.h"
#include "gui/themes/gui_styles.h"
#include "gui/plotting/plot_state.h"
#include "simulation/simulation_logic.h"
#include "gui/screens/doc_menu_screen.h"
#include "gui/screens/main_menu_screen.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define NEUROLAB_VERSION_MAJOR 1
#define NEUROLAB_VERSION_MINOR 0
#define NEUROLAB_VERSION_PATCH 0
#define NEUROLAB_VERSION "1.0.0"

static AppContext gAppContext;

static void AppInit(void);
static void AppSetInitValues(void);

int main (void) {

    AppInit();

    PlotStateReset();

    AppSetInitValues();

    while (!WindowShouldClose()) {

        InputHanndleKeys(&gAppContext);
        SimulationUpdate(&gAppContext);

        BeginDrawing();
            ClearBackground(G_UI_STYLES.colors.backgroundColor);

            switch (gAppContext.app.currentScreen) {
                case MAIN_MENU: ScreenMainMenuDraw(&gAppContext); break;

                case DOCUMENTATION: ScreenDocMenuDraw(&gAppContext); break;

                default: break;
            }
        EndDrawing();
    }

    SimulationReset(&gAppContext);
    CloseWindow();
    return 0;
}

static void AppInit(void) {
    gAppContext.app.currentScreen        = MAIN_MENU;
    gAppContext.focus.focusTab           = FOCUS_NONE;
    gAppContext.focus.activeControlFocus = CONTROL_FOCUS_NONE;
    gAppContext.focus.activeAuxFocus     = AUX_FOCUS_NONE;

    gAppContext.simState = (SimulationState) {
        .models.izModel          = NULL,
        .models.hhModel          = NULL,
        .runtime.isRunning       = false,
        .inputs.externCurrent    = 0.00f,
        .inputs.ampaConductancy  = 0.00f,
        .inputs.gabaaConductancy = 0.00f,
    };

    gAppContext.tabs = (Tabs) {
        .activeTab             = 0,
        .activeNeuronModel     = 0,
        .activeIzhikevichModel = 0
    };
}

static void AppSetInitValues() {
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(100, 100, "NEUROLAB - Neural Simulator");

    const int w = GetMonitorWidth(0);
    const int h = GetMonitorHeight(0);

    const int x = (w - (w * G_UI_STYLES.global.perCentMonitorWidth)) / 2;
    const int y = (h - (h * G_UI_STYLES.global.perCentMonitorHeight)) / 2;

    SetWindowSize(w * G_UI_STYLES.global.perCentMonitorWidth, h * G_UI_STYLES.global.perCentMonitorHeight);
    SetWindowPosition(x, y);

    GuiLoadStyle("assets/styles/style_dark.rgs");
    ClearWindowState(FLAG_WINDOW_HIDDEN);
    SetTargetFPS(60);
}
