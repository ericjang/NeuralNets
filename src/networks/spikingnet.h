#ifndef SPIKINGNET_H
#define SPIKINGNET_H

#include <vector>
#include <string>
#include "synapses/spikingconnection.h"
#include "layers/spikinglayer.h"
#include "common.h"
#include "neurons/hodgkin_huxley.h"
#include "neurons/if_neuron.h"
#include "neurons/if_neuron.h"
#include "proto/neural_nets.pb.h"
#include <QString>

using std::vector;
using std::string;

struct SpikingLayer;
struct SpikingConnection;
struct RunSettings;
struct RunResults;

struct SpikingNet
{
    SpikingNet();

    ~SpikingNet();
    vector<SpikingLayer*> layers;
    vector<vector<SpikingConnection*>> pre;
    vector<vector<SpikingConnection*>> post;
    bool stdp_enabled; // turn this off for testing
};

// construction
void add_layer(SpikingNet* net, SpikingLayer* layer);

// template method to create different kinds of laeyrs
// template functions that are not specialized need to be accessible
// to the compiler (in order to generate code for all specializations)
// therefore, they go in the header
// T - neuron class type
template <typename T>
SpikingLayer *create_layer(SpikingNet* net, string name, int N) {
    SpikingLayer *layer = new SpikingLayer(name, N);

    layer->units.resize(N);
    for (int i=0; i<N; ++i) {
        SpikingNeuron *unit = new T();
        layer->units[i] = unit;
    }

    if (typeid(T) == typeid(HodgkinHuxley)) {
        layer->type = neuralnets::SpikingLayer_LayerType_HodgkinHuxley;
    } else if (typeid(T) == typeid(IFNeuron)){
        layer->type = neuralnets::SpikingLayer_LayerType_IntegrateFire;
    } else {
        layer->type = neuralnets::SpikingLayer_LayerType_Simple;
    }

    add_layer(net, layer);
    return layer;
}

void connect(SpikingNet *net, SpikingConnection *conn);
void connect(SpikingNet *net, SpikingLayer *source, SpikingLayer *target, synapse_vec synapses, bool stdp_enabled);
// syanpses->layers
void updateLayer(SpikingLayer *layer, conn_vec pre_arr, boolvec doSpike, float t);

// TODO - specialize the update function.

// layers->synapses
void updateTransmission(conn_vec conn_arr, SpikingLayer *source); // per-layer version (for parallel)
void updateTransmission(SpikingConnection *conn, SpikingLayer *source);
void updateSTDP(SpikingConnection *conn, SpikingLayer *source, SpikingLayer *target);

void recordSpikes(neuralnets::Raster* raster, SpikingLayer *layer, int t);

// simulation
neuralnets::SimulationResults run(SpikingNet* net, RunSettings *rs, bool save_weights);


void reset(SpikingNet *net);
void normalizeWeights(SpikingNet* net);

// note: pre and post refer to synapses
// source and target refer to layers

// IO

void saveResults(neuralnets::SimulationResults results, QString fname);
void recordState(neuralnets::NetworkState *state, SpikingNet *net, bool save_weights);

#endif // SPIKINGNET_H
