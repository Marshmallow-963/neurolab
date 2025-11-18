/**
 * @file izhikevich_config.c
 * @brief Defines the constant parameter values for the Izhikevich model.
 */
#include "model/neural/izhikevich/izhikevich_config.h"

/**
 * @brief The peak voltage (in mV) that triggers a spike and reset.
 */
const float IZHIKEVICH_SPIKE_PEAK = 30.0f;

/**
 * @brief Global array of parameters for different neuron types.
 *
 * Sourced from Izhikevich, E. M. (2003). "Simple model of spiking neurons".
 */
const IzhikevichConfig IZHIKEVICH_PARAMETERS[] = {
    {.type = CHATTERING,             .a = 0.02f, .b = 0.20f, .c = -50.0f, .d =  2.0f},
    {.type = FAST_SPIKING,           .a = 0.10f, .b = 0.20f, .c = -65.0f, .d =  2.0f},
    {.type = INTRINSICALLY_BURSTING, .a = 0.02f, .b = 0.20f, .c = -55.0f, .d =  4.0f},
    {.type = LOW_THRESHOLD_SPIKING,  .a = 0.02f, .b = 0.25f, .c = -65.0f, .d =  2.0f},
    {.type = REGULAR_SPIKING,        .a = 0.02f, .b = 0.20f, .c = -65.0f, .d =  8.0f},
    {.type = RESONATOR,              .a = 0.10f, .b = 0.26f, .c = -60.0f, .d = -1.0f},
    {.type = THALAMO_CORTICAL,       .a = 0.02f, .b = 0.25f, .c = -65.0f, .d =  0.05f}
    // Note: Add more types here if needed
};
