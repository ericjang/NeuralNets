#ifndef HODGKIN_HUXLEY_H
#define HODGKIN_HUXLEY_H

#include "spiking_neuron.h"

class SquareWaveStim;

struct HodgkinHuxley : SpikingNeuron
{
    HodgkinHuxley();
    float m;/// Na channel inactivation
    float h;/// Na channel inactivation
    float n;/// K channel activation
    float v;/// membrane potential

    // overriding SpikingNeuron::update
    virtual void update(float Ge_syn, float Gi_syn, bool doSpike, float t);
};


void reset(HodgkinHuxley* neuron);


#endif // HODGKIN_HUXLEY_H
