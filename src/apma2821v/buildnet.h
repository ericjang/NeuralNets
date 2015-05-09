#ifndef BUILDNET_H
#define BUILDNET_H

struct SpikingNet;
#include "proto/neural_nets.pb.h"
#include "networks/spikingnet.h"
#include "synapses/spikingconnection.h"
#include "connectivities.h"
#include "neurons/spiking_neuron.h"
#include "neurons/hodgkin_huxley.h"
#include "neurons/if_neuron.h"
#include "layers/spikinglayer.h"
#include "constants.h"
#include <iostream>



template <typename T>
SpikingNet * buildnet(int input_w) {
    SpikingNet *net = new SpikingNet();

    int D = input_w * input_w;

    // create layers
    SpikingLayer* P_L = create_layer<SpikingNeuron>(net, "P_L",D);
    SpikingLayer* P_M = create_layer<SpikingNeuron>(net, "P_M",D);
    SpikingLayer* P_S =  create_layer<SpikingNeuron>(net,"P_S",D);

    SpikingLayer* LGN_L = create_layer<T>(net,"LGN_L",900);
    SpikingLayer* LGN_C1 = create_layer<T>(net,"LGN_C1",900);
    SpikingLayer* LGN_C2 = create_layer<T>(net,"LGN_C2",900);

    SpikingLayer* V1_4 = create_layer<T>(net,"V1_4",900);
    SpikingLayer* V1_23 = create_layer<T>(net,"V1_23",900);
    SpikingLayer* V1_5 = create_layer<T>(net,"V1_5",900);

    // wire it up!
    synapse_vec synapses;

    float w = BASE_WEIGHT; // test
    if (input_w == 30)
        w/=2; // hack - scale down base weights for larger receptive fields.

    // P_L -> LGN
    synapses = p_2_lgn(P_L->n, LGN_L->n, EXC, w);
    connect(net, P_L,LGN_L,synapses,false);
    std::cout << synapses.size() << std::endl;

    synapses = p_2_lgn(P_L->n, LGN_C1->n, EXC, w);
    connect(net, P_L,LGN_C1,synapses,false);
    std::cout << synapses.size() << std::endl;

    synapses = p_2_lgn(P_L->n, LGN_C2->n, EXC, w);
    connect(net, P_L,LGN_C2,synapses,false);
    std::cout << synapses.size() << std::endl;

    // P_M -> LGN
    synapses = p_2_lgn(P_M->n,LGN_L->n,EXC,w);
    connect(net, P_M,LGN_L,synapses,false);
    synapses = p_2_lgn(P_M->n,LGN_C2->n,EXC,w);
    connect(net, P_M,LGN_C2,synapses,false);
    synapses = p_2_lgn(P_M->n, LGN_C1->n, INH,w);
    connect(net, P_M,LGN_C1,synapses,false);
    // P_S -> LGN
    synapses = p_2_lgn(P_S->n, LGN_C2->n, INH,w);
    connect(net, P_S,LGN_C1,synapses,false);

    // LGN->V1 Cortex
    synapses = forward(LGN_L->n, V1_4->n, EXC, w);
    connect(net,LGN_L, V1_4,synapses,true);
    synapses = forward(LGN_C1->n, V1_4->n, EXC, w);
    connect(net,LGN_C1, V1_4, synapses,true);
    synapses = forward(LGN_C2->n, V1_23->n, EXC, w*1.5);
    connect(net,LGN_C2, V1_23, synapses,true);

    // V1->V1
    synapses = forward(V1_4->n, V1_23->n, EXC, w);
    connect(net,V1_4, V1_23, synapses,true);
    synapses = forward(V1_23->n, V1_5->n, EXC, w);
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


SpikingNet* reconstruct_net(neuralnets::NetworkState state);



// for examining properties of lateral dynamics
SpikingNet * lateral_net();

#endif // BUILDNET_H
