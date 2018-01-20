# libcem

C EMbedded Helper Library. This contains modules that commonly seem to be missing from embedded projects, specificially queues and pools that can be used safely in an interrupt context.

More modules will be added as I have need of them (or they are proposed / PR'd!)

## Status

[![Build Status](https://travis-ci.org/ryankurte/libcem.svg)](https://travis-ci.org/ryankurte/libcem)

## Modules

- [memqueue](./memqueue) is a memory queue (ie. queue containing it's own memory) designed to be inherently thread-safe for a single producer and consumer.

## Building and Testing

Testing is via [googletest](https://github.com/google/googletest) which is automagically imported by [test/gtest.cmake](https://github.com/ryankurte/libcem/blob/master/test/gtest.cmake).

1. `git clone git@github.com:ryankurte/libcem.git` to clone the repository
2. `cd libcem` to change into the correct directory
3. `mkdir build && cd build` to create and move into build directory
4. `cmake ..` to initialise the build
5. `make` to build
5. `./cem-test` to run tests
