#ifndef TRAIN_H
#define TRAIN_H


// implements methods specific to training the neural net
#include <QString>
#include "sim/runsettings.h"
#include "proto/neural_nets.pb.h"

struct SpikingNet;
struct RunSettings;
using std::string;

// single iter
neuralnets::SimulationResults train_iter(SpikingNet *net, QString img_name);

// runs the full training sequence
void full_train_default();
void full_train_default(SpikingNet *net, int startIter);
bool resume_training(std::string fname);

#endif // TRAIN_H
