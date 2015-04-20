#NeuralNets

Code for APMA2821V Final Project, Spring 2015.

This is (yet another) spiking neural network simulator, implemented the way I like it.

Runs about twice as fast as the equivalent model implemented in Neuron.

## Design Decisions

Artificial Neural Networks and very simple spiking neural network models are ammendable to SIMD vectorization (i.e. parallel updating
of many neurons by taking advantage of BLAS matrix multiplication routines), but adding additional complexitiies to the neuron or synaptic
transmission model results in diminishing returns.

This code is implemented to be as readable and extendible as possible.

Instead of using object-oriented programming, this code takes a more functional approach and treats classes like bags of data, with
exterior functions that operate on those bags.

## Building

Dependencies: [Google Protobufs](https://developers.google.com/protocol-buffers/docs/downloads.html), Qt (only qmake, no Qt libraries).

In your terminal:

```bash
git clone https://ericjang/neuralnets
build # runs protoc, qmake, make
./neuralnets
```

## Julia Implementation

This was initially implemented in Julia, see [https://ericjang/NeuralNets.jl](https://ericjang/NeuralNets.jl).
However, Julia lacks shared a shared memory threading model (crucial for speedup) so I had to drop down to C++ for adequate performance.


when installing google protobuf, will also need to install the bundled python scripts. see their README.md for instructions


## Profiling

uncomment these 2 flags in the .pro file:
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
then compile with:
qmake -recursive -spec linux-clang -config debug
./nets # generates a gmon.out
gprof nets > outfile # outfile contains profiled information
