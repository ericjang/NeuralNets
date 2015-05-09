#include "buildnet.h"



SpikingNet* reconstruct_net(neuralnets::NetworkState state) {
    int nL = state.layers_size();

    SpikingNet *net = new SpikingNet();
    SpikingLayer* layer;
    for (int li=0; li<nL; ++li) {
        neuralnets::SpikingLayer proto_layer = state.layers(li);
        string name = proto_layer.name();
        int N = proto_layer.n();

        // hack - make sure to disable this
        if (li > 2) {
            layer = create_layer<HodgkinHuxley>(net, name, N);
        } else {
            layer = create_layer<SpikingNeuron>(net, name, N);
        }

#if 0
        if (proto_layer.layer_type() == neuralnets::SpikingLayer_LayerType_Simple) {
            layer = create_layer<SpikingNeuron>(net, name, N);
        } else if (proto_layer.layer_type() == neuralnets::SpikingLayer_LayerType_HodgkinHuxley) {
            layer = create_layer<HodgkinHuxley>(net, name, N);
        } else if (proto_layer.layer_type() == neuralnets::SpikingLayer_LayerType_IntegrateFire) {
            layer = create_layer<IFNeuron>(net,name,N);
        } else {
            std::cerr << "Unknown Layer Type Defined in Proto" << std::endl;
            delete net;
            std::exit(-1);
        }
#endif
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
        //std::cout << synapses.size() << std::endl;
        SpikingConnection *conn = new SpikingConnection(s,t,synapses,stdp_enabled);
        connect(net,conn);
    }
    return net;
}

SpikingNet * buildnet_large() {

}

SpikingNet * lateral_net() {

}
