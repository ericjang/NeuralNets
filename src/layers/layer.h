#ifndef LAYER_H
#define LAYER_H

// solution to the templating business:
// network stores layer_vec, must be vector of pointers for polymorphism to work
// SpikingLayer inherits from Layer, and is templated to different neuron types
// since templated are subclass of Layer, they will still fit together in the layer vector

#include <vector>
#include <string>

using std::vector;
using std::string;

class Layer;
typedef vector<Layer*> layer_vec;

class Layer
{
public:
    Layer(string name, int n);
    ~Layer();
    int n; // number of units
    string name;
};

#endif // LAYER_H
