#!/bin/bash
# optionally, add -config debug for debug mode.
qmake -recursive -spec linux-clang src/NeuralNets.pro

make
