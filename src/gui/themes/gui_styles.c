/**
 * @file gui_styles.c
 * @brief Defines the concrete values for the global UI styles.
 */
#include "raylib.h"
#include "gui/themes/gui_styles.h"

/**
 * @brief Global instance of the application's UI theme.
 */
const UiStyles G_UI_STYLES = {
    .button.width               = 200.0f,
    .button.height              = 30.0f,

    // --- Colors ---
    .colors.plotColor1          = DARKBLUE,
    .colors.plotColor2          = DARKGREEN,
    .colors.plotColor3          = DARKPURPLE,
    .colors.backgroundColor     = DARKGRAY,
    .colors.plotAxisColor       = LIGHTGRAY,
    .colors.focusColor          = DARKBLUE,
    .colors.textColor           = WHITE,
    .colors.textSpecial         = DARKGREEN,

    // --- Global ---
    .global.focusThickness       = 2.0f,
    .global.perCentMonitorWidth  = 0.98,
    .global.perCentMonitorHeight = 0.88,

    // --- Components ---
    .label.height                = 20.f,
    .tab.height                  = 30.0f,

    // --- Layout ---
    .layout.padding              = 10.f,
    .layout.spacingBetweenLines  = 25.0f,
    .layout.verticalGroupSpacing = 20.0f,

    // --- Plot ---
    .plot.axisMargin             = 40.0f,
    .plot.fontSize               = 10.0f,

    .slider.step                 = 0.01,
    .slider.currentMinValue      = 0.00f,
    .slider.currentMaxvalue      = 500.00f
};
