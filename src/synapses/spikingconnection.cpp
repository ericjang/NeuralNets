#include "spikingconnection.h"
#include <algorithm>

using std::for_each;


SpikingConnection::SpikingConnection(int S, int T, synapse_vec synapses, bool stdp_enabled)
    : s(S), t(T), synapses(synapses), stdp_enabled(stdp_enabled)
{
    // compute average
    float w_sum = 0;
    for_each(synapses.begin(), synapses.end(), [&](SpikingSynapse *syn) { w_sum += syn->w; });
    target_w_avg = w_sum/synapses.size(); // target average calculated from initial conditions
}

SpikingConnection::~SpikingConnection()
{
    for (auto syn : synapses)
        delete syn;
}

void add_synapse(SpikingConnection *conn, SpikingSynapse *syn)
{
    conn->synapses.push_back(syn);
}


void normalizeWeights(SpikingConnection *conn) {
    if (conn->stdp_enabled) {
        float w_sum = 0;
        for_each(conn->synapses.begin(), conn->synapses.end(), [&](SpikingSynapse *syn) { w_sum += syn->w; });
        float w_avg = w_sum/conn->synapses.size(); // empirical average
        for (SpikingSynapse* syn : conn->synapses) {
            syn->w *= conn->target_w_avg / w_avg;
        }
    }
}

void reset(SpikingConnection *conn) {
    for (SpikingSynapse *syn : conn->synapses)
        reset(syn);
}
