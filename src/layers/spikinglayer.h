#ifndef SPIKINGLAYER_H
#define SPIKINGLAYER_H

#include <vector>
#include <string>
#include "common.h"
#include "synapses/spikingconnection.h"
#include "layer.h"

using std::vector;
using std::string;
struct SpikingNeuron;

struct SpikingLayer : Layer
{
    ~SpikingLayer();
    vector<SpikingNeuron*> units;

    // constructors cannot be templated functions
    SpikingLayer(std::string name, int N) : Layer(name,N) {}


};

void reset(SpikingLayer *layer);

#endif
