/**
 * @file hodgkin_huxley_rates.c
 * @brief Implementation of the rate functions (Alpha and Beta) for the
 * HH model's ionic gates.
 */

#include <math.h>
#include "model/neural/hodgkin-huxley/hodgkin_huxley_rates.h"

float AlphaM(float voltage) {
    // Avoids division by zero at v = 25.0
    // This is the limit of the function as v -> 25 (L'Hôpital's Rule)
    if (voltage == 25.0) return 1.0;

    return (25.0 - voltage) / (10 * (exp((25.0 - voltage) / 10.0) - 1));
}

float BetaM(float voltage) {
    return 4.0 * exp(-voltage / 18.0);
}

float AlphaH(float voltage) {
    return 0.07 * exp(-voltage / 20.0);
}

float BetaH(float voltage) {
    return 1.0 / (exp((30.0 - voltage)/ 10.0) + 1);
}

float AlphaN(float voltage) {
    // Avoids division by zero at v = 10.0
    // This is the limit of the function as v -> 10 (L'Hôpital's Rule)
    if (voltage == 10.0) return 0.1;

    return (10.0 - voltage) / (100.0 * (exp((10.0 - voltage) / 10.0) - 1.0));
}

float BetaN(float voltage) {
    return 0.125 * exp(-voltage / 80.0);
}
