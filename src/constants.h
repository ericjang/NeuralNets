#ifndef CONSTANTS
#define CONSTANTS

#include <limits>
#include <QString>

// TODO - change this to your own local directory
const QString ROOT = "/home/eric/NeuralNets";

// forward-Euler integration timestep
const float dt = 0.025; // ms


const float Inf = std::numeric_limits<float>::infinity();
const float iInf = std::numeric_limits<int>::max();


const float BASE_WEIGHT = 0.1;

// learning rate for STDP
//const float LR = 0.0001;
const float LR = 0.0001; // temporary, rather large learning rate

// if one weight gets too large, then
// weight normalization will drag other weights to become really weak
const float MAX_WEIGHT = 1;
const float MIN_WEIGHT = 0;

//const int TRAIN_ITER = 2001;
const int TRAIN_ITER = 501;

const int SAVE_INTERVAL = 100; // set this to 100 for the full_train run

#endif // CONSTANTS

