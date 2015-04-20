#include "stdp.h"

#include <cmath>
#include "constants.h"

const float dtau = 34; // ms
const float ptau = 17; // ms

float stdp(float deltaT)
{
    if (deltaT > 0)
        return LR*exp(-deltaT/ptau);
    else if (deltaT < 0)
        return -LR*exp(deltaT/dtau);
    else // deltaT = 0, pre/post are simultaneous => no modification
        return 0;
}
