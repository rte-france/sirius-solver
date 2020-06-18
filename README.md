# Sirius

## Table of Contents

* [About Sirius](#about-sirius)
* [Industrial use cases](#industrial-use-cases)
* [How to build and use Sirius](INSTALL.md)

## About Sirius

Sirius is a linear programming solver, and can also handle mixed binary variables problems. It has been developped by RTE

## Industrial use cases

Sirius has been an integral part of the [Antares Simulator](https://antares-simulator.org/) software design and used by [RTE](https://www.rte-france.com/) the French electrical power transmission system operator.

## Performance comparisons

### Antares Simulator
Antares Simulator among many other things solve a succession of 104 linear problems (2 per week) that are very close to each other. Since only some coefficients in the objective and the bounds of the constraints change between the problems, a hotstart mechanism can be used in some cases to improve the performance of the resolution of the 104 problems.

Here are the performance measure on a laptop without using a hotstart mechanism for Sirius and Clp (COIN-OR) 
![Antares_Sirius_vs_Coin_coldstart](resources/Antares_Sirius_vs_Coin_coldstart.png)

And when we use a hotstart between the optimisations for Sirius and Clp (COIN-OR) 
![Antares_Sirius_vs_Coin_hotstart](resources/Antares_Sirius_vs_Coin_hotstart.png)
