/**
 * @file hodgkin_huxley_rates.h
 * @brief Defines the rate functions (Alpha and Beta) for the
 * HH model's ionic gates (m, h, n).
 *
 * These functions describe the voltage-dependent kinetics of the gates.
 */
#ifndef HODGKIN_HUXLEY_RATES_H
#define HODGKIN_HUXLEY_RATES_H

/**
 * @brief Opening rate (alpha) for the 'm' gate (Sodium activation).
 * @param voltage The current membrane voltage (in mV).
 * @return The transition rate.
 */
float AlphaM(float voltage);

/**
 * @brief Closing rate (beta) for the 'm' gate.
 * @param voltage The current membrane voltage (in mV).
 * @return The transition rate.
 */
float BetaM(float voltage);

/**
 * @brief Opening rate (alpha) for the 'h' gate (Sodium inactivation).
 * @param voltage The current membrane voltage (in mV).
 * @return The transition rate.
 */
float AlphaH(float voltage);

/**
 * @brief Closing rate (beta) for the 'h' gate.
 * @param voltage The current membrane voltage (in mV).
 * @return The transition rate.
 */
float BetaH(float voltage);

/**
 * @brief Opening rate (alpha) for the 'n' gate (Potassium activation).
 * @param voltage The current membrane voltage (in mV).
 * @return The transition rate.
 */
float AlphaN(float voltage);

/**
 * @brief Closing rate (beta) for the 'n' gate.
 * @param voltage The current membrane voltage (in mV).
 * @return The transition rate.
 */
float BetaN(float voltage);

#endif // HODGKIN_HUXLEY_RATES_H
