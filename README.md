# Sirius

## Table of Contents

* [About Sirius](#about-sirius)
* [Industrial use cases](#industrial-use-cases)
* [Integration](#integration)
  * [OR-Tools](#or-tools)
  * [Standalone API](#standalone-api)
* [Performances](#performances)
  * [On Antares Simulator problems](#on-antares-simulator-problems)
* [How to build and use Sirius](INSTALL.md)

## About Sirius

Sirius has been developed by [RTE](https://www.rte-france.com/) and published as an open-source solver which license is Apache 2.0. The Sirius solver is a C implementation of the dual simplex algorithm (for continuous problems) and the branch-and-bound algorithm (problems with binary variables). The tool can be used as a standalone C library or through a fork of [OR-Tools](https://github.com/google/or-tools), Google's software suite for combinatorial optimization.  

## Industrial use cases

Sirius has been an integral part of the [Antares Simulator](https://antares-simulator.org/) software design and used by RTE the French electrical power transmission system operator.

Sirius is also used on several other project at RTE like[...].SiriusAPI.md)

## Performances

### On Antares Simulator problems

Antares Simulator among many other things solve a succession of 104 linear problems (2 per week) that are very close to each other. Since only some coefficients in the objective and the bounds of the constraints change between the problems, a hotstart mechanism can be used in some cases to improve the performance of the resolution of the 104 problems.

Here are the performance measure on a laptop without using a hotstart mechanism for Sirius and Clp (COIN-OR)
![Antares_Sirius_vs_Coin_coldstart](resources/Antares_Sirius_vs_Coin_coldstart.png)

And when we use a hotstart between the optimisations for Sirius and Clp (COIN-OR)
![Antares_Sirius_vs_Coin_hotstart](resources/Antares_Sirius_vs_Coin_hotstart.png)

## How to build and use Sirius

### cmake configuration scripts

Please refer to the command detailed in [Sirius solver INSTALL.md](INSTALL.md) in order to compile and integrate the Sirius distribution in any cmake project using suitable find_package procedure.

### OR-Tools

OR-Tools allow to use Sirius solver in a variety of programming languages such as C++, C#, Python and Java. More details on [OR-Tools](https://github.com/google/or-tools).

This fork is available on github here: [OR-Tools with Sirius](https://github.com/rte-france/or-tools/tree/unification_2020)

Sirius can be used through OR-Tools like any other solver (see OR-Tools [Quick start](https://developers.google.com/optimization/introduction/get_started) and more in depth [Documentation](https://developers.google.com/optimization/)).  
And then for instance in C++, you can specify that you want to use Sirius as a solver by using ```MPSolver::SIRIUS_LINEAR_PROGRAMMING``` or ```MPSolver::SIRIUS_MIXED_INTEGER_PROGRAMMING```.

### Standalone C API

Sirius can also be used as a standalone solver for linear problems with continuous variables or binary variables. The API is described here [Work in progress - SiriusAPI.md](