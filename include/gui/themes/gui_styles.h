/**
 * @file gui_styles.h
 * @brief Defines data structures for the application's UI theme (styles).
 *
 * This file centralizes all colors, layout metrics (padding, spacing),
 * and component sizes used by the raygui interface.
 */
#ifndef UI_STYLES_H
#define UI_STYLES_H

#include <raylib.h>

/**
 * @struct UiColors
 * @brief Defines the color palette for the UI.
 */
typedef struct {
    Color plotColor1;
    Color plotColor2;
    Color plotColor3;

    Color backgroundColor;
    Color focusColor;
    Color plotAxisColor;
    Color textColor;
    Color textSpecial;
} UiColors;

/**
 * @struct UiLayoutStyles
 * @brief Defines layout metrics like padding and spacing.
 */
typedef struct {
    float padding;
    float spacingBetweenLines;
    float verticalGroupSpacing;
} UiLayoutStyles;

/**
 * @struct UiGlobalStyles
 * @brief Defines global styling parameters.
 */
typedef struct {
    float focusThickness;       ///< Thickness of the focus rectangle
    float perCentMonitorWidth;  ///< Percentage of monitor width to use
    float perCentMonitorHeight; ///< Percentage of monitor height to use
} UiGlobalStyles;

/** @brief Defines styles for button widgets. */
typedef struct {
    float width;
    float height;
} UiButtonStyles;

/** @brief Defines styles for label widgets. */
typedef struct {
    float height;
} UiLabelStyles;

/** @brief Defines styles for tab widgets. */
typedef struct {
    float height;
} UiTabStyles;

/** @brief Defines styles for plotting (axis, font). */
typedef struct {
    float axisMargin;
    float fontSize;
} UiPlotStyles;

/** @brief Defines variables for controls */
typedef struct {
    float step;
    float currentMinValue;
    float currentMaxvalue;
} UiSliderStyles;

/**
 * @struct UiStyles
 * @brief The main aggregate struct for all UI styles.
 *
 * This combines all other style structs into a single object
 * that can be accessed globally.
 */
typedef struct {
    // Aggregates
    UiColors colors;
    UiLayoutStyles layout;
    UiGlobalStyles global;
    UiSliderStyles slider;

    // Individual component styles
    UiButtonStyles button;
    UiLabelStyles label;
    UiPlotStyles plot;
    UiTabStyles tab;
} UiStyles;

/**
 * @brief Global extern instance of the UI styles.
 *
 * This is defined in gui_styles.c and used throughout the application.
 */
extern const UiStyles G_UI_STYLES;

#endif // UI_STYLES_H
