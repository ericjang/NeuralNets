#Tensor Standard Library (TSL)



Replacement to standard library that allows writing C++ code that looks like Julia / Matlab / Octave (but 0-indexed and statically-compiled).

Modeled after Julia
https://github.com/JuliaLang/julia

1) C++ string manipulation is really annoying - I have to look up documentation every time I want to do anything.
2) C++ datatypes don't build off each other. For example, Julia's Gadfly uses DataFrames package. 
3) Dict<ASCIIString, int> is much more concise than std::unordered_map<std::string, int>. 
4) generating a vector of random numbers should be as easy as Vector<float> = rand(10);

Ammendable to high-level, high-performance technical computing.

TODO - study Julia's libraries and what they include.
Maybe check out Boost too.