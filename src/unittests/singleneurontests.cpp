#include "singleneurontests.h"
#include "neurons/hodgkin_huxley.h"
#include "stimuli/fixedratespikestim.h"
#include "proto/neural_nets.pb.h"
#include "constants.h"
#include "apma2821v/connectivities.h"
#include "stimuli/emptystim.h"
#include "sim/runsettings.h"
#include "networks/spikingnet.h"
#include <cmath>

void SingleNeuronTests::test()
{
    // assemble a tiny 2-cell network
    // verify that spike delays are arriving at the appropriate time.
    // also ensure that spike is disabled after using it!
    SpikingNet *net = new SpikingNet();
    SpikingLayer* P_L = create_layer<SpikingNeuron>(net, "Pre",1);
    SpikingLayer* LGN_L = create_layer<HodgkinHuxley>(net,"Post",1);

    float w = 1.0; // huge weight -> this causes spiking in HH neuron

    synapse_vec synapses = p_2_lgn(P_L->n, LGN_L->n, EXC, w);
    assert(synapses.size() == 1);
    connect(net, P_L,LGN_L,synapses,false);

    stim_vec stims;
    stims.resize(2);
    fvec hz(1,40); // single neuron, 40 hz stimulus
    fvec noise(1,0);
    float T = 300;
    int nstart = int(50/dt);
    int nstop = int(250/dt);
    stims[0] = new FixedRateSpikeStim(hz,noise,nstart,nstop);
    stims[1] = new EmptyStim(1);

    RunSettings* rs = new RunSettings(T,dt,stims,true); // true = show progress
    neuralnets::SimulationResults results = run(net, rs);

    delete rs;
    delete net;
}
