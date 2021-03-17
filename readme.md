# RV IO
RV IO stands for Random Variables Input/Output. It contains MATLAB and C++ files that write and read realization of random variables as well as their covariance.

# What's in this repo?
- Mathematial data generator that simulates data from mathematical models.
- The measurements are corrupted with noise and bias.
- C++ read/write functions.

# Library and namespace
The library is stored in the `RV.h` header file. It contains two (nested) namespaces:
1. `RV` for all functions converning random variables.
   1. `IO` for input/output operations (such as writing, reading, importing, etc.).