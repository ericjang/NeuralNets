#include "stim_tests.h"
#include "stimuli/fixedratespikestim.h"
#include "common.h"
#include <iostream>
#include "constants.h"
#include <cmath>
#include <cassert>

void test_stim() {
    float noise = .1;
    fvec hz(1,100); //1 neuron, 100 Hz
    int nstart = int(50/dt);
    int nstop = int(250/dt);
    FixedRateSpikeStim *stim = new FixedRateSpikeStim(hz, noise, nstart, nstop);
    int nsteps = ceil(300/dt);
    int spikecount = 0;
    for (int i=0; i<nsteps; ++i) {
        boolvec doSpike = stim->yield();
        if (doSpike[0]) {
            spikecount++;
            std::cout << i*dt << std::endl;// 100 hz for 200 ms = 20 spikes
        }
    }
    std::cout << "total spikes: " << spikecount << std::endl;
    assert(spikecount == 20);
}
