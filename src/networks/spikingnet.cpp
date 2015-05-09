#include "spikingnet.h"
#include "synapses/spikingconnection.h"
#include "layers/layer.h"
#include "sim/runsettings.h"
#include <stdexcept>
#include <cmath>
#include "constants.h"
#include <algorithm>
#include "stimuli/stim.h"
#include <cassert>
#include "neurons/spiking_neuron.h"
#include <iostream>
#include <fstream>

extern "C" {
#include "progressbar.h"
}

using namespace std;


SpikingNet::SpikingNet()
{

}


SpikingNet::~SpikingNet()
{
    // clean up all data structures
    for (auto layer : layers)
        delete layer;
    for (auto conn_vec : pre)
        for (auto conn : conn_vec)
            delete conn;
}

void add_layer(SpikingNet *net, SpikingLayer *layer) {
    net->layers.push_back(layer);
    net->pre.push_back(conn_vec());
    net->post.push_back(conn_vec());
}

void connect(SpikingNet *net, SpikingConnection *conn) {
    int s = conn->s;
    int t = conn->t;
    net->pre[t].push_back(conn);
    net->post[s].push_back(conn);
}

void connect(SpikingNet *net, SpikingLayer *source, SpikingLayer *target, synapse_vec synapses, bool stdp_enabled) {
    auto layers = net->layers;
    auto it = std::find(layers.begin(), layers.end(), source);
    assert(it != layers.end());
    int s = it - layers.begin();

    it = std::find(layers.begin(), layers.end(), target);
    assert(it != layers.end());
    int t = it - layers.begin();

    // construct conn object
    SpikingConnection *conn = new SpikingConnection(s,t,synapses,stdp_enabled);

    //std::cout << conn->synapses.size() << std::endl;

    net->pre[t].push_back(conn);
    net->post[s].push_back(conn);
}

// SIMULATION

void reset(SpikingNet *net) {
    for (auto layer : net->layers)
        reset(layer);
    for (conn_vec conn_arr : net->pre)
        for (SpikingConnection *conn : conn_arr)
            reset(conn);
}

void normalizeWeights(SpikingNet* net) {
    for (conn_vec arr : net->pre) {
        for (SpikingConnection *conn : arr) {
            normalizeWeights(conn);
        }
    }
}

// note - net state is NOT reset between calls to run()
// that is up to the user. There are use cases where one might wish
// to have continuous simulation with short RunSetting and "pauses"

neuralnets::SimulationResults run(SpikingNet *net, RunSettings *rs, bool save_weights) {
    int nsteps = ceil(rs->T/rs->dt);
    neuralnets::SimulationResults results;
    // set up rasters
    for (size_t li=0; li<net->layers.size(); ++li) {
        results.add_spikes();
    }

    results.set_dt(rs->dt);
    results.set_t(rs->T);

    // free data structures
    progressbar *progress;
    char msg [] = "Progress (Single)...";
    if (rs->progressbar) progress = progressbar_new(msg,nsteps);
    size_t nL = net->layers.size();
    for (int i = 0; i<nsteps; ++i)
    {
        float t = dt*i;

        // update layers
        for (size_t li=0; li < nL; ++li) {
            SpikingLayer *layer = net->layers[li];
            Stim *stim = rs->stimuli[li];
            boolvec doSpike = stim->yield();
            conn_vec pre_arr = net->pre[li];
            updateLayer(layer, pre_arr, doSpike, t);
            recordSpikes(results.mutable_spikes(li), layer, i);
        }

        // update transmission & stdp

        for (size_t li=0; li < nL; ++li) {
            SpikingLayer *layer = net->layers[li];

            // transmission
            for (SpikingConnection *conn : net->post[li]) {
                #pragma omp parallel for
                for (SpikingSynapse* syn : conn->synapses) {
                    updateTransmission(syn, layer->units[syn->s]);
                }
            }

            // STDP
            for (SpikingConnection *conn : net->pre[li]) {
                SpikingLayer *source = net->layers[conn->s];
                SpikingLayer *target = net->layers[conn->t];
                if (conn->stdp_enabled) {
                    #pragma omp parallel for
                    for (SpikingSynapse* syn : conn->synapses) {
                        updateSTDP(syn, source->units[syn->s], target->units[syn->t]);
                    }
                }
            } // end STDP

        } // end for

        if (rs->progressbar) progressbar_inc(progress);
    }
    if (rs->progressbar) progressbar_finish(progress);

    neuralnets::NetworkState * state = results.mutable_nstate();
    recordState(state, net, save_weights);

    return results;
}

void recordState(neuralnets::NetworkState *state, SpikingNet *net, bool save_weights) {
    for (SpikingLayer *layer : net->layers) {
        neuralnets::SpikingLayer *proto_layer = state->add_layers();
        proto_layer->set_n(layer->n);
        proto_layer->set_name(layer->name);
        proto_layer->set_layer_type(layer->type);
    }
    if (save_weights) {
        for (conn_vec conn_arr : net->pre) {
            for (SpikingConnection *conn : conn_arr) {
                neuralnets::SpikingConnection * proto_conn = state->add_connections();
                proto_conn->set_source(conn->s);
                proto_conn->set_target(conn->t);
                proto_conn->set_stdp_enabled(conn->stdp_enabled);
                proto_conn->set_avg_w(conn->target_w_avg);
                for (SpikingSynapse *syn : conn->synapses) {
                    neuralnets::Synapse *proto_syn = proto_conn->add_synapses();
                    neuralnets::Synapse_SynapseType stype =
                            (syn->stype == EXC) ? neuralnets::Synapse_SynapseType_EXC : neuralnets::Synapse_SynapseType_INH;
                    proto_syn->set_stype(stype);
                    proto_syn->set_s(syn->s);
                    proto_syn->set_t(syn->t);
                    proto_syn->set_w(syn->w);
                    proto_syn->set_d(syn->d);
                }
            }
        }
    }
}

void recordSpikes(neuralnets::Raster *raster, SpikingLayer *layer, int t) {
    for (int i=0; i<layer->n; ++i) {
        SpikingNeuron *neuron = layer->units[i];
        if (neuron->didSpike) {
            neuralnets::Spike * spike = raster->add_spikes();
            spike->set_i(i);
            spike->set_t(t);
        }
    }
}


void updateLayer(SpikingLayer *layer, conn_vec pre_arr, boolvec doSpike, float t) {
    fvec Ge_syn(layer->n, 0); // zeros
    fvec Gi_syn(layer->n, 0);
    for (SpikingConnection *conn : pre_arr) {
        for (SpikingSynapse * syn : conn->synapses) {
            if (syn->stype == EXC) {
                Ge_syn[syn->t] += syn->w * syn->g;
            } else {
                Gi_syn[syn->t] += syn->w * syn->g;
            }
        }
    }
    #pragma omp parallel for
    for (int i=0; i<layer->n; ++i) {
        layer->units[i]->update(Ge_syn[i], Gi_syn[i], doSpike[i], t);
    }
}



void saveResults(neuralnets::SimulationResults results, QString fname) {
    fstream output(fname.toStdString(), ios::out | ios::trunc | ios::binary);
    if (!results.SerializeToOstream(&output)) {
        cerr << "Failed to write " << fname.toStdString() << endl;
    } else {
        cout << "Saved to " << fname.toStdString() << endl;
    }
}

