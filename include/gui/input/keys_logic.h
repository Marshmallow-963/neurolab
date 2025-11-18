/**
 * @file keys_logic.h
 * @brief Public interface for handling global keyboard input.
 *
 * This file declares the main function responsible for processing
 * keyboard events for application navigation and control.
 */
#ifndef KEYS_LOGIC_H
#define KEYS_LOGIC_H

#include "app_state.h"

/**
 * @brief Handles all global keyboard input for the application.
 *
 * This function processes key presses for:
 * - Switching between application screens (Main Menu <-> Documentation).
 * - Navigating between main UI panels (Controls, Main Display, Auxiliary).
 * - Navigating between widgets within the focused panel.
 *
 * @param ctx Pointer to the global AppContext, which will be modified
 * based on user input.
 */
void InputHanndleKeys(AppContext *ctx);

#endif // KEYS_LOGIC_H
