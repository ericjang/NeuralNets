#include "fixedratespikestim.h"
#include "constants.h"
#include <cmath>
#include "common.h"

// to conserve memory -
// each unit has ISI.
// when counter > last spike count + ISI, spike again

FixedRateSpikeStim::FixedRateSpikeStim(fvec hz, fvec noise, int nstart, int nstop) :
    Stim(hz.size()), m_nstart(nstart), m_nstop(nstop), m_i(0)
{
    m_lastSpike = ivec(m_N,0);
    m_ISI = ivec(m_N,iInf);
    m_noise = ivec(m_N,0);

    // compute ISI of each neuron
    int nduration = nstop-nstart;
    for (int n=0; n<m_N; ++n) {// each neuron
        float rate = hz[n];
        int nspikes = rate*(nduration*dt)/1000; // nspikes/sec * num secs
        if (nspikes > 0) {
            m_ISI[n] = nduration/nspikes;
            m_noise[n] = int(noise[n]*m_ISI[n]); // i.e. ISI fluctuates between 0.9 and 1.1 * ISI
        }
    }
}

FixedRateSpikeStim::FixedRateSpikeStim(fvec hz, float noise, int nstart, int nstop) :
    FixedRateSpikeStim(hz,fvec(hz.size(),noise),nstart,nstop) {}

boolvec FixedRateSpikeStim::yield() {
    boolvec vec(m_N,false);
    if (m_nstart <= m_i && m_i < m_nstop)
    for (int n=0;n<m_N;++n) {
        int noise = m_noise[n];
        int ISI = m_ISI[n] + uniformInt(-noise, noise);
        int lastSpike = m_lastSpike[n];
        if (m_i >= lastSpike+ISI) {
            vec[n]=true;
            m_lastSpike[n]=m_i;
        }
    }
    m_i++;
    return vec;
}
