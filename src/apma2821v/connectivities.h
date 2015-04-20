#ifndef CONNECTIVITIES_H
#define CONNECTIVITIES_H

#include "synapses/spikingsynapse.h"

struct SpikingConnection;

// generates connectivities described in the paper

// No STDP
synapse_vec p_2_lgn(int sN, int tN, syn_t stype, float base_weight);

// STDP enabled
synapse_vec forward(int sN, int tN, syn_t stype, float base_weight);

// No STDP
synapse_vec lateral(int N, float w_pot, float w_dep);

#endif // CONNECTIVITIES_H
