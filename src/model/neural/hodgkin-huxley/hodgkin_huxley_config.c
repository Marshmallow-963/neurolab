/**
 * @file hodgkin_huxley_config.c
 * @brief Implementation and definition of default configuration values
 * for the Hodgkin-Huxley model.
 */
#include "model/neural/hodgkin-huxley/hodgkin_huxley_config.h"

/**
 * @brief Pi constant, private to this file.
 *
 * Used to calculate capacitance and conductance values based on membrane area.
 * These specific values are sourced from Tabela 1 (Table 1) of the
 * thesis "Dinâmica de circuitos neuronais", UFAL 2023 by J.M.G.L. Silva.
 */
#ifndef PI
    #define PI 3.14159265358979323846f
#endif

/**
 * @brief Definition of the default Hodgkin-Huxley model parameters.
 *
 * These values are based on the original paper, adjusted for
 * the units used in the simulation.
 */
const HodgkinHuxleyConfig HH_CONFIG = {
    .restingPotential     = -65.0f,
    .membraneCapacitancy  = 9.0f * PI,
    .leakReversal         = 10.6f,
    .sodiumReversal       = 115.0f,
    .potassiumReversal    = -12.0f,
    .leakConductance      = 2.7f * PI,
    .sodiumConductance    = 1080.0f * PI,
    .potassiumConductance = 324.0f * PI
};
