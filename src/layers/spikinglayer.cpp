#include "spikinglayer.h"
#include "layer.h"
#include "neurons/spiking_neuron.h"

SpikingLayer::~SpikingLayer()
{
    // free units
    for (auto unit : units)
    {
        delete unit;
    }
}

void reset(SpikingLayer *layer) {
    for (SpikingNeuron *unit : layer->units)
        reset(unit);
}


