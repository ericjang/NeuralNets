#ifndef RUNSETTINGS
#define RUNSETTINGS

#include <vector>

using std::vector;

class Stim;

typedef vector<Stim*> stim_vec;

struct RunSettings {
    float T;
    float dt;
    bool progressbar;
    vector<Stim*> stimuli; // per-layer stimuli
    RunSettings(float T, float dt, stim_vec stimuli, bool progressbar);
    ~RunSettings(); // destructor
};


#endif // RUNSETTINGS

