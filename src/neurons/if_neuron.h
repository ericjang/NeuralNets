#ifndef IF_NEURON_H
#define IF_NEURON_H

#include "spiking_neuron.h"

// Leaky integrate-and-fire model

struct IFNeuron : SpikingNeuron
{
    IFNeuron();
    float v;/// membrane potential
    // TODO - add params here
    // overriding SpikingNeuron::update
    virtual void update(float Ge_syn, float Gi_syn, bool doSpike, float t);
};

void reset(IFNeuron *neuron);

#endif // IF_NEURON_H
