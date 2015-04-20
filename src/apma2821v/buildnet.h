#ifndef BUILDNET_H
#define BUILDNET_H

struct SpikingNet;
#include "proto/neural_nets.pb.h"

SpikingNet* buildnet();

SpikingNet* reconstruct_net(neuralnets::NetworkState state);


// build net with larger receptive fields
SpikingNet* buildnet_large();

#endif // BUILDNET_H
