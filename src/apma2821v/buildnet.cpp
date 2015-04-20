#include "buildnet.h"
#include "networks/spikingnet.h"
#include "synapses/spikingconnection.h"
#include "connectivities.h"
#include "neurons/spiking_neuron.h"
#include "neurons/hodgkin_huxley.h"
#include "layers/spikinglayer.h"
#include "constants.h"


SpikingNet * buildnet() {
    SpikingNet *net = new SpikingNet();

    // create layers
    SpikingLayer* P_L = create_layer<SpikingNeuron>(net, "P_L",100);
    SpikingLayer* P_M = create_layer<SpikingNeuron>(net, "P_M",100);
    SpikingLayer* P_S =  create_layer<SpikingNeuron>(net,"P_S",100);

    SpikingLayer* LGN_L = create_layer<HodgkinHuxley>(net,"LGN_L",900);
    SpikingLayer* LGN_C1 = create_layer<HodgkinHuxley>(net,"LGN_C1",900);
    SpikingLayer* LGN_C2 = create_layer<HodgkinHuxley>(net,"LGN_C2",900);

    SpikingLayer* V1_4 = create_layer<HodgkinHuxley>(net,"V1_4",900);
    SpikingLayer* V1_23 = create_layer<HodgkinHuxley>(net,"V1_23",900);
    SpikingLayer* V1_5 = create_layer<HodgkinHuxley>(net,"V1_5",900);

    // wire it up!
    synapse_vec synapses;
    float w = 0.005;

    w = BASE_WEIGHT; // test

    // P_L -> LGN
    synapses = p_2_lgn(P_L->n, LGN_L->n, EXC, w);
    connect(net, P_L,LGN_L,synapses,false);

    synapses = p_2_lgn(P_L->n, LGN_C1->n, EXC, w);
    connect(net, P_L,LGN_C1,synapses,false);

    synapses = p_2_lgn(P_L->n, LGN_C2->n, EXC, w);
    connect(net, P_L,LGN_C2,synapses,false);

    // P_M -> LGN
    synapses = p_2_lgn(P_M->n,LGN_L->n,EXC,w);
    connect(net, P_M,LGN_L,synapses,false);

    synapses = p_2_lgn(P_M->n,LGN_C2->n,EXC,w);
    connect(net, P_M,LGN_C2,synapses,false);

    synapses = p_2_lgn(P_M->n, LGN_C1->n, INH,w);
    connect(net, P_M,LGN_C1,synapses,false);

    // P_S -> LGN
    synapses = p_2_lgn(P_S->n, LGN_C1->n, INH,w);
    connect(net, P_S,LGN_C1,synapses,false);

    // LGN->V1 Cortex
    synapses = forward(LGN_L->n, V1_4->n, EXC, w);
    connect(net,LGN_L, V1_4,synapses,true);

    synapses = forward(LGN_C1->n, V1_4->n, EXC, w);
    connect(net,LGN_C1, V1_4, synapses,true);

    synapses = forward(LGN_C2->n, V1_23->n, EXC, w*1.5);
    connect(net,LGN_C2, V1_23, synapses,true);

    // V1->V1
    synapses = forward(V1_4->n, V1_23->n, EXC, w*1.5);
    connect(net,V1_4, V1_23, synapses,true);
    synapses = forward(V1_23->n, V1_5->n, EXC, w*3);
    connect(net,V1_23, V1_5, synapses,true);

    // Lateral connectivities
    float w_pot, w_dep;
    w_pot = w*.9; w_dep = w*1.5*1.7;
    synapses = lateral(V1_4->n, w_pot, w_dep);
    connect(net,V1_4,V1_4,synapses,false);

    w_pot = w * 0.5; w_dep = w * 1.5 * 1.5;
    synapses = lateral(V1_23->n, w_pot, w_dep);
    connect(net,V1_23,V1_23,synapses,false);

    w_pot = w * 3; w_dep = w * 1.5 * 1;
    synapses = lateral(V1_5->n, w_pot, w_dep);
    connect(net,V1_5,V1_5,synapses,false);

    return net;
}

SpikingNet* reconstruct_net(neuralnets::NetworkState state) {
    int nL = state.layers_size();

    SpikingNet *net = new SpikingNet();
    SpikingLayer* layer;
    for (int li=0; li<nL; ++li) {
        neuralnets::SpikingLayer proto_layer = state.layers(li);
        string name = proto_layer.name();
        int N = proto_layer.n();
        if (proto_layer.layer_type() == neuralnets::SpikingLayer_LayerType_Simple) {
            layer = create_layer<SpikingNeuron>(net, name, N);
        } else if (proto_layer.layer_type() == neuralnets::SpikingLayer_LayerType_HodgkinHuxley) {
            layer = create_layer<HodgkinHuxley>(net, name, N);
        }
    }

    // now, restore spiking connections
    int nC = state.connections_size();
    for (int i=0; i<nC; ++i) {
        neuralnets::SpikingConnection proto_conn = state.connections(i);
        int s = proto_conn.source();
        int t = proto_conn.target();
        bool stdp_enabled = proto_conn.stdp_enabled();
        synapse_vec synapses;
        for (neuralnets::Synapse proto_syn : proto_conn.synapses()) {
            syn_t stype = (proto_syn.stype() == neuralnets::Synapse_SynapseType_EXC) ? EXC : INH;
            SpikingSynapse* syn = new SpikingSynapse(stype, proto_syn.s(), proto_syn.t(), proto_syn.w(), proto_syn.d());
            synapses.push_back(syn);
        }
        SpikingConnection *conn = new SpikingConnection(s,t,synapses,stdp_enabled);
        connect(net,conn);
    }
    return net;
}
