#ifndef RK4_H
#define RK4_H

#include <stddef.h>
#include <stdbool.h>

typedef void (*DerivativeFunc)(const float *state, float *deriv, void *params);

typedef struct {
    float *k1;
    float *k2;
    float *k3;
    float *k4;
    float *tempState;
} Slopes;

typedef struct {
    int n;
    float dt;
    void *params;
    float *buffer;
    Slopes slopes;
    DerivativeFunc derivFunc;
} RK4;

bool RK4Init(RK4 *integrator, DerivativeFunc func, void *params, int n, float dt);

void RK4Calculate(RK4 *integrator, float *state);

void RK4Free(RK4 *integrator);

#endif // RK4_H
