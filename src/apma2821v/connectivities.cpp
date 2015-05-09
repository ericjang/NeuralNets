#include "connectivities.h"
#include "common.h"
#include <tuple>
#include <cmath>
#include <stdlib.h>
#include "synapses/spikingconnection.h"
#include "synapses/spikingsynapse.h"
#include <algorithm>
#include <iostream>
#include "utilities/indexing.h"
#include <cassert>
#include <utility>

using std::tuple;
using std::min;
using std::max;
using std::pair;
using std::make_pair;


// assumes mapping from N source to N target units
// kernel = postsynaptic mapping
template <typename T>
tuple<ivec, ivec, vector<T>> topographicWeights2D(int N, vector<T> kernel)
{
    ivec Sources;
    ivec Targets;
    vector<T> Values;


    int sW = (int)sqrt(N);
    int tW = sW; // for syntactic clarity
    int kW = (int)sqrt(kernel.size());
    assert(kW % 2 == 1); // assume odd kernel shapes so topographic mapping is well-defined
    int ks = (kW-1)/2; // kernel stride. if kw = 4, ks = 1 (int rounding)

    int s,t,k,sr,sc,tr,tc,kr,kc,r_offset,c_offset;
    T val;
    for (s=0; s<N; ++s) {
        ind2sub(sW,sW,s,sr,sc);
        // find matching tiles on target
        for (kr=0, r_offset=-ks; kr<kW; ++kr, ++r_offset) {
            for (kc=0, c_offset=-ks; kc<kW; ++kc, ++c_offset) {
                sub2ind(kW,kW,kr,kc,k); // kernel index
                tr = sr+r_offset;
                tc = sc+c_offset;
                val = kernel[k];
                // make sure tr, tc are within bounds of target map
                if (0 <= tr && tr < tW && 0 <= tc && tc < tW && val != 0) {
                    sub2ind(tW,tW,tr,tc,t);
                    Sources.push_back(s);
                    Targets.push_back(t);
                    Values.push_back(val);
                }
            }
        }
    }
    return make_tuple(Sources, Targets, Values);
}

// mapping from a smaller map to a larger map (fan-in)
template <typename T>
tuple<ivec, ivec, vector<T>> topographicWeights2D(int sN, int tN, vector<T> kernel)
{
    ivec Sources;
    ivec Targets;
    vector<T> Values;

    // simple algorithm - compute weights as if sN == tN,
    // then aggregate sources and targets together
    std::tie(Sources, Targets, Values) = topographicWeights2D(tN,kernel);
    int sW = (int)sqrt(sN);
    int tW = (int)sqrt(tN); // for syntactic clarity
    int s,t,v,sr,sc,sr2,sc2,kr,kc;

    int w_ratio = (int)(float(tW)/sW);
    assert(w_ratio == floor(float(tW)/sW));
    assert(w_ratio == (int)sqrt(kernel.size()));

    // adjacency list
    vector<vector< pair<int,T> > > Adj_tmp; // (target,val)
    for (int i=0; i<sN; ++i) {
        Adj_tmp.push_back(vector< pair<int,T> >());
    }

    // for each created edge, loop through and re-assign to original
    // small tile
    int s2;
    for (int i=0; i<Sources.size(); ++i) {
        s = Sources[i]; t = Targets[i]; v = Values[i];
        ind2sub(tW,tW,s,sr2,sc2);
        sr = sr2/w_ratio;
        sc = sc2/w_ratio;
        sub2ind(sW,sW,sr,sc,s2);
        assert(s2 < sN);
        Adj_tmp[s2].push_back(make_pair(t,v)); // pair = target, val
    }

    // bug - visited are double counted when we encouter them again
    // they end up collecting gthemselves and all following.

    ivec Sources2, Targets2;
    vector<T> Values2;
    for (s=0; s<sN; ++s) { // for each smaller tile
        vector< pair<int,T> > targets = Adj_tmp[s];
        for (t=0; t<tN; ++t) { // for each possible target (more efficient would be to just loop through set of all
            // .first in targets
            bool found = false;
            T totalV = 0;
            // loop through vector and collect same targets
            for (int i=0; i<targets.size(); ++i) {
                if (targets[i].first == t) {
                    found = true;
                    totalV += targets[i].second;
                }
            }
            if (found) { // at least one synapse
                Sources2.push_back(s);
                Targets2.push_back(t);
                Values2.push_back(totalV);
            }
        }
    }

    // TODO:\ flip sources & targets if tN < sN
    // the kernel is essenntially treated as a fan-in kernel
    // instead of a fanout.

    return make_tuple(Sources2, Targets2, Values2);
}

// <N -> N
synapse_vec p_2_lgn(int sN, int tN, syn_t stype, float base_weight) {

    // hacky override
    if (sN == tN) {
        return forward(sN,tN,stype,base_weight);
    }

    int small_N = min(sN,tN);
    int big_N = max(sN,tN);
    float kernel_width = sqrt((float)big_N/(float)small_N);
    assert(floor(kernel_width) == kernel_width);

    int kw = (int)kernel_width;
    ivec S, T, W;

    ivec kernel(kw*kw, 1);
    std::tie(S, T, W) = topographicWeights2D(sN,tN,kernel); // weights are aggregated
    synapse_vec vec;
    for (int i=0; i<S.size(); ++i) {
        float delay = 1.0 + (uniform01()-0.5);
        float w = 0;
        for (int i=0; i<W[i]; ++i) {
            w += base_weight;
        }
        w *= uniform01();
        vec.push_back(new SpikingSynapse(stype, S[i], T[i], w, delay));
    }
    return vec;
}

// N->N
synapse_vec forward(int sN, int tN, syn_t stype, float base_weight) {
    assert(sN == tN);
    ivec kernel =
    {
        0,1,0,
        1,1,1,
        0,1,0
    };
    ivec S,T, Tmp;

    std::tie(S, T, Tmp) = topographicWeights2D(sN, kernel);
    synapse_vec vec;
    for (int i=0; i<S.size(); ++i) {
        float delay = 1.0 + (uniform01()-0.5);
        float w = base_weight * (0.5+2.0*uniform01());
        vec.push_back(new SpikingSynapse(stype, S[i], T[i], w, delay));
    }
    return vec;
}

// N->N
synapse_vec lateral(int N, float w_pot, float w_dep) {
    int width= sqrt(N);
    const int e = 1;
    const int i = 2;

    ivec type_kernel =
    {
        0,0,i,i,i,0,0,
        0,i,e,e,e,i,0,
        i,e,e,e,e,e,i,
        i,e,e,0,e,e,i,
        i,e,e,e,e,e,i,
        0,i,e,e,e,i,0,
        0,0,i,i,i,0,0
    };
    
    ivec S, T, Types;
    std::tie(S, T, Types) = topographicWeights2D(N, type_kernel);

    float A = 1, B = 2, C = 3, D = 4;
    fvec delay_kernel= {
        0,0,D,D,D,0,0,
        0,D,C,C,C,D,0,
        D,C,B,A,B,C,D,
        D,C,A,0,A,C,D,
        D,C,B,A,B,C,D,
        0,D,C,C,C,D,0,
        0,0,D,D,D,0,0
    };
    fvec delays;
    std::tie(S,T,delays) = topographicWeights2D(N, delay_kernel);

    synapse_vec vec;
    for (int i=0; i<S.size(); ++i) {
        syn_t stype = (Types[i] == 1) ? EXC : INH;
        float base_weight = (stype == EXC) ? w_pot : w_dep;
        float w = base_weight *  (0.5+2.0*uniform01());
        vec.push_back(new SpikingSynapse(stype, S[i], T[i], w, delays[i]));
    }
    return vec;
}
