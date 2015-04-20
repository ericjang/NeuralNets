#ifndef STIM_H
#define STIM_H

#include <vector>
using std::vector;

class Stim {
protected:
    int m_N; // number of units
public:
    Stim(int N) : m_N(N) {}
    virtual vector<bool> yield() = 0;
};

#endif
