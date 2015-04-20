#include "runsettings.h"
#include "stimuli/stim.h"

RunSettings::RunSettings(float T, float dt, stim_vec stimuli, bool progressbar)  :
    T(T), dt(dt), progressbar(progressbar), stimuli(stimuli) {}

RunSettings::~RunSettings()
{
    for (Stim* stim : stimuli)
        delete stim;
}
