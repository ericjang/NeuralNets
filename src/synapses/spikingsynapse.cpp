#include "spikingsynapse.h"
#include "neurons/spiking_neuron.h"
#include "stdp.h"
#include "constants.h"
#include <algorithm>

using std::max;
using std::min;

const float tau_e = 5; //ms
const float tau_i = 10;
const float g_spike = 1.0;

SpikingSynapse::SpikingSynapse(syn_t st, int s, int t, float w, float d) :
    stype(st), s(s), t(t), w(w), d(d)
{
    reset(this);
}


void reset(SpikingSynapse *syn)
{
    syn->tau = (syn->stype == syn_t::EXC) ? tau_e : tau_i;
    syn->incoming.resize(int(syn->d/dt)+1);
    syn->g = 0;
    syn->_head = 0;
}


void updateTransmission(SpikingSynapse *syn,
            SpikingNeuron *source) {

    // transmission decay
    syn->g += dt*(-syn->g/syn->tau);
    if (syn->incoming[syn->_head]) {
        syn->g += g_spike;
    }
    // compute index of element just before head

    int L = syn->incoming.size(); // length of circular buffer
    int tail = (syn->_head + L-1) % L;

    // if spiked, add to end of incoming queue
    syn->incoming[tail] = source->didSpike;
    syn->_head = (syn->_head + 1) % L; // cycle queue 1 step
}

void updateSTDP(SpikingSynapse *syn, SpikingNeuron *source, SpikingNeuron *target) {
    if (source->didSpike || target->didSpike) {
        syn->w += stdp(target->lastSpike - source->lastSpike);
        syn->w = max(min(syn->w, MAX_WEIGHT), MIN_WEIGHT); // all weights should be positive
    }
}

