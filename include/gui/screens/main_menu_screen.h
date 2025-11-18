/**
 * @file main_menu_screen.h
 * @brief Public interface for drawing the main menu screen.
 *
 * This file declares the main entry point function responsible for
 * rendering the entire main simulation UI.
 */
#ifndef MAIN_MENU_SCREEN_H
#define MAIN_MENU_SCREEN_H

#include "app_state.h"

/**
 * @brief Draws the entire main menu screen, including all panels and widgets.
 *
 * This function orchestrates the layout and drawing of the three main panels:
 * - Top-Left (Controls)
 * - Top-Right (Main Display)
 * - Bottom-Right (Auxiliary Displays)
 *
 * @param ctx Pointer to the global AppContext, containing all simulation
 * and GUI state.
 */
void ScreenMainMenuDraw(AppContext *ctx);

#endif // MAIN_MENU_SCREEN_H
