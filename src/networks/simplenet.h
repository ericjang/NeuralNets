#ifndef SIMPLENET_H
#define SIMPLENET_H

// TODO (Low priority) - implement this!

#include "neurons/spiking_neuron.h"
#include "synapses/spikingsynapse.h"

struct SimpleNet
{
    SimpleNet();
    ~SimpleNet();
    vector<SpikingNeuron*> neurons;
    synapse_vec synapses;
    bool stdp_enabled; // turn this off for testing network
};

#endif // SIMPLENET_H
