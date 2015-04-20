#include "spiking_neuron.h"
#include "stimuli/fixedratespikestim.h"
#include "constants.h"

SpikingNeuron::SpikingNeuron() {
    reset(this);
}

void SpikingNeuron::update(float Ge_syn, float Gi_syn, bool doSpike, float t) {
    didSpike = doSpike;
    if (doSpike) {
        lastSpike = t;
    }
}

void reset(SpikingNeuron *neuron) {
    neuron->didSpike = false;
    neuron->lastSpike = -Inf;
}
