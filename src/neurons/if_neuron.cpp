#include "if_neuron.h"
#include "constants.h"

const float Cm = 10; // Capacitative density [μF/cm^2]
const float v0 = -65; // reset potential
const float v_rest = -65;
const float e_rev = 0; // reversal potential for AMPA synapses (mV)
const float i_rev = -80; // reversal potential for GABA synapses (mV)
const float abs_refr=20; // ms
const float v_th=-40;
const float tau_m = 20;

// default constructor
IFNeuron::IFNeuron() :
    SpikingNeuron() // call parent struct constructor
{
    reset(this);
}


void reset(IFNeuron *neuron) {
    neuron->v = v0;
    neuron->didSpike = false;
    neuron->lastSpike = -Inf;
}

void IFNeuron::update(float Ge_syn, float Gi_syn, bool doSpike, float t) {
    float v = this->v;
    float Isyn = Ge_syn*(e_rev-v)+Gi_syn*(i_rev-v); // TODO - make sure inhibitory is working properly
    float vdot = (v_rest-v)/tau_m + Isyn/Cm;
    this->v += dt*vdot;

    // check spiking
    this->didSpike = ((this->v > v_th) && (t > this->lastSpike+abs_refr)) || doSpike; // doSpike = force spiking
    if (this->didSpike) {
        this->lastSpike = t;
        this->v = v_rest;
    }
}
