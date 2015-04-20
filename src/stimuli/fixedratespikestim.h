#ifndef FIXEDRATESPIKESTIM_H
#define FIXEDRATESPIKESTIM_H

#include "common.h"
#include <vector>
#include "stim.h"

using std::vector;

class FixedRateSpikeStim : public Stim
{
private:
    int m_nstart;
    int m_nstop;
    int m_i;
    ivec m_ISI; // in step units
    ivec m_noise; // ISI perturbation, in step units
    ivec m_lastSpike;
public:
    // constructors
    FixedRateSpikeStim(fvec hz, fvec noise, int nstart, int nstop);
    FixedRateSpikeStim(fvec hz, float noise, int nstart, int nstop); // single noise rate

    boolvec yield();
};

#endif // FIXEDRATESPIKESTIM_H
