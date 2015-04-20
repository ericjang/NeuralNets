#ifndef SPIKINGNEURON_H
#define SPIKINGNEURON_H

#include "common.h"

class FixedRateSpikeStim;

struct SpikingNeuron
{
    bool didSpike; // whether neuron is currently spiking or not
    float lastSpike; // time of last spike
    SpikingNeuron(); // constructor (implemented in cpp)

    // note: "virtual" is crucial here if pointers derived classes are to do custom
    // behavior
    virtual void update(float Ge_syn, float Gi_syn, bool doSpike, float t);
};


void reset(SpikingNeuron *neuron);

#endif // SPIKINGNEURON_H
