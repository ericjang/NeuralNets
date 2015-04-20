#include "hodgkin_huxley.h"
#include "constants.h"
#include "common.h"
#include <cmath>

const float Cm = 10; // Capacitative density [μF/cm^2]
const float gNa = 120; // maximum Na channel conductance
const float gK = 36;
const float gL = 1;
const float ENa = 115;
const float EK = -12;
const float ELeak = 0;
const float v0 = -65; // reset potential


// Synaptic transmission parameters - these are specific to HH neurons
// so they go here.
const float e_rev = 0; // reversal potential for AMPA synapses (mV)
const float i_rev = -80; // reversal potential for GABA synapses (mV)
const float abs_refr=5; // ms
const float v_th=-20;

inline float inf(float alpha, float beta) { return alpha/(alpha+beta); }
float vtrap(float x, float y) { return (fabs(x/y)<1e-6) ? y*(1 - x/y/2) : x/(exp(x/y) - 1); }

// Na channel activation
inline float alphaM(float v) { return .1*vtrap(-(v+40),10); }
inline float betaM(float v) { return 4*exp(-(v+65)/18); }
float Minf(float v) { return inf(alphaM(v),betaM(v));}

// Na channel inactivation
inline float alphaH(float v) { return .07*exp(-(v+65)/20); }
inline float betaH(float v) { return 1/(exp(-(v+35)/10)+1); }
float Hinf(float v) { return inf(alphaH(v),betaH(v));}

// K channel activation
inline float alphaN(float v) { return .01*vtrap(-(v+55),10); }
inline float betaN(float v) { return 0.125*exp(-(v+65)/80); }
float Ninf(float v) { return inf(alphaN(v),betaN(v));}


// default constructor
HodgkinHuxley::HodgkinHuxley() :
    SpikingNeuron() // call parent struct constructor
{
    reset(this);
}

void HodgkinHuxley::update(float Ge_syn, float Gi_syn, bool doSpike, float t) {
    float m = this->m, h = this->h, n = this->n, v = this->v;
    float Isyn = Ge_syn*(e_rev-v)+Gi_syn*(i_rev-v); // TODO - make sure inhibitory is working properly

    float vdot = (gNa*m*m*m*h*(ENa-(v+65)) +
                  gK*n*n*n*n*(EK-(v+65)) +
                  gL*(ELeak-(v+65)) +
                  Isyn)/Cm;

    this->m += dt*(alphaM(v)*(1-m) - betaM(v)*m);
    this->h += dt*(alphaH(v)*(1-h) - betaH(v)*h);
    this->n += dt*(alphaN(v)*(1-n) - betaN(v)*n);
    this->v += dt*vdot;

    // check spiking
    this->didSpike = ((this->v > v_th) && (t > this->lastSpike+abs_refr)) || doSpike; // doSpike = force spiking
    if (this->didSpike) {
        this->lastSpike = t;
    }
}

void reset(HodgkinHuxley *neuron) {
    float v = v0;
    neuron->v = v;
    neuron->m = Minf(v);
    neuron->h = Hinf(v);
    neuron->n = Ninf(v);
    neuron->didSpike = false;
    neuron->lastSpike = -Inf;
}
