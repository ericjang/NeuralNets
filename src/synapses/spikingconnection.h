#ifndef SPIKINGCONNECTION_H
#define SPIKINGCONNECTION_H

#include <vector>
#include "synapses/spikingsynapse.h"

using std::vector;
struct SpikingSynapse;
struct SpikingConnection;
typedef vector<SpikingConnection*> conn_vec;

struct SpikingConnection
{
    SpikingConnection(int S, int T, synapse_vec synapses, bool stdp_enabled);
    ~SpikingConnection();

    int s;
    int t;
    synapse_vec synapses;
    bool stdp_enabled;
    float target_w_avg; // target avg
};

void add_synapse(SpikingConnection *conn, SpikingSynapse *syn);
void reset(SpikingConnection *conn);
void normalizeWeights(SpikingConnection *conn);

#endif // SPIKINGCONNECTION_H
