/**
 * @file neuron_models.h
 * @brief Defines an enumeration to identify different neuron model types
 * available in the simulation.
 */
#ifndef NEURON_MODELS_H
#define NEURON_MODELS_H

/**
 * @enum NeuronModelType
 * @brief Enumerates the supported neuron model implementations.
 *
 * This is used to create and manage different types of neuron models
 * through a unified interface.
 */
typedef enum {
    IZHIKEVICH_MODEL = 0,
    HODGKIN_HUXLEY_MODEL
} NeuronModel;

#endif // NEURON_MODELS_H
