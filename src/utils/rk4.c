#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils/rk4.h"

bool RK4Init(RK4 *integrator, DerivativeFunc func, void *params, int n, float dt) {
    integrator->derivFunc = func;
    integrator->params = params;
    integrator->n = n;
    integrator->dt = dt;

    integrator->buffer = (float*)calloc(5 * n, sizeof(float));
    if (integrator->buffer == NULL) return false;

    integrator->slopes.k1 = integrator->buffer;
    integrator->slopes.k2 = integrator->buffer + n;
    integrator->slopes.k3 = integrator->buffer + 2 * n;
    integrator->slopes.k4 = integrator->buffer + 3 * n;
    integrator->slopes.tempState = integrator->buffer + 4 * n;

    return true;
}

void RK4Calculate(RK4 *integrator, float *state) {
    const int n = integrator->n;
    const float dt = integrator->dt;
    const float dtHalf = dt * 0.5;

    void *params = integrator->params;
    DerivativeFunc f = integrator->derivFunc;

    float *k1 = integrator->slopes.k1;
    float *k2 = integrator->slopes.k2;
    float *k3 = integrator->slopes.k3;
    float *k4 = integrator->slopes.k4;
    float *temp_state = integrator->slopes.tempState;

    // k1 = f(y_n)
    f(state, k1, params);

    // k2 = f(y_n + 0.5 * dt * k1)
    for (int i = 0; i < n; i++) {
        temp_state[i] = state[i] + dtHalf * k1[i];
    }
    f(temp_state, k2, params);

    // k3 = f(y_n + 0.5 * dt * k2)
    for (int i = 0; i < n; i++) {
        temp_state[i] = state[i] + dtHalf * k2[i];
    }
    f(temp_state, k3, params);

    // k4 = f(y_n + dt * k3)
    for (int i = 0; i < n; i++) {
        temp_state[i] = state[i] + dt * k3[i];
    }
    f(temp_state, k4, params);

    // Update state: y_{n+1} = y_n + (dt/6) * (k1 + 2k2 + 2k3 + k4)
    const float dtSixth = dt / 6.0;
    for (int i = 0; i < n; i++) {
        state[i] += (k1[i] + 2.0 * (k2[i] + k3[i]) + k4[i]) * dtSixth;
    }
}

void RK4Free(RK4 *integrator) {
    if (integrator && integrator->buffer) {
        free(integrator->buffer);
        integrator->buffer = NULL;
        integrator->slopes.k1 = NULL;
        integrator->slopes.k2 = NULL;
        integrator->slopes.k3 = NULL;
        integrator->slopes.k4 = NULL;
        integrator->slopes.tempState = NULL;
    }
}
