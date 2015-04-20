#ifndef EMPTYSTIM_H
#define EMPTYSTIM_H

#include "stim.h"
#include "common.h"

class EmptyStim : public Stim
{
public:
    EmptyStim(int N);
    ~EmptyStim();
    boolvec yield();
};

#endif // EMPTYSTIM_H
