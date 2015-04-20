#include "emptystim.h"

#include "common.h"


EmptyStim::EmptyStim(int N)
    : Stim(N)
{
}

EmptyStim::~EmptyStim()
{

}

boolvec EmptyStim::yield()
{
    // all falses
    return boolvec(m_N,false);
}
