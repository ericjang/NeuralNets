#ifndef SPIKINGSYNAPSE_H
#define SPIKINGSYNAPSE_H

// conductance-based synaptic transmission model
// with delayed spike transmission + STDP

#include <vector>

using std::vector;
struct SpikingNeuron;
struct SpikingSynapse;

typedef vector<SpikingSynapse*> synapse_vec;

enum syn_t {
    EXC, // excitatory
    INH // inhibitory
};

struct SpikingSynapse
{
    syn_t stype; // synapse type <- param not used, but handy to keep around
    int s; // source neuron index
    int t; // target neuron index
    float w; // synaptic weight
    float d; // synaptic delay (ms)
    float g; // transmitted conductance
    float tau; // time constant

    // TODO - this is a circular queue!
    int _head;//
    vector<bool> incoming; // maintains a continuous spike train that has yet to reach target neuron

    SpikingSynapse(syn_t st, int s, int t, float w, float d);
};

void reset(SpikingSynapse *syn);

void updateTransmission(SpikingSynapse *syn, SpikingNeuron *source);
void updateSTDP(SpikingSynapse *syn, SpikingNeuron *source, SpikingNeuron *target);

#endif // SPIKINGSYNAPSE_H
