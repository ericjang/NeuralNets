#NeuralNets

Code for APMA2821V Final Project, Spring 2015.

This is (yet another) spiking neural network simulator, implemented the way I like it.

Runs about twice as fast as the equivalent model implemented in Neuron.

## Design Decisions

Artificial Neural Networks and very simple spiking neural network models are ammendable to SIMD vectorization (i.e. parallel updating
of many neurons by taking advantage of BLAS matrix multiplication). However, code becomes a lot harder to write when trying to account for more complex models like synaptic transmission and per-unit conditional branching behavior.

Instead of using object-oriented programming, this code (mostly) uses a C/Julia-like programming approach, by treating object instances like bags of data, with external functions operating on those bags, rather than reats classes like "bags of data" instead of objects that "own" functions.


## Dependencies

* [Google Protobufs](https://developers.google.com/protocol-buffers/docs/downloads.html)
* Qt
* ncurses
* Clang, Clang++


## Building

In your terminal:

```bash
git clone https://ericjang/NeuralNets
./compile.sh
./neuralnets
```

## Profiling

uncomment these 2 flags in the .pro file:
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
then compile with:
qmake -recursive -spec linux-clang -config debug
./nets # generates a gmon.out
gprof nets > outfile # outfile contains profiled information
