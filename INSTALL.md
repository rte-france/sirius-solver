# How to build and use Sirius in a C++ cmake project

## Table of Contents

* [How to build Sirius](#how-to-build-sirius)
* [How to link Sirius library in a C++ cmake project](#how-to-link-sirius-library-in-a-c-cmake-project)

## How to build Sirius

### Prequisites

_Those are the minimal version that have been tested to work, but Sirius should also work with more recent versions of those tools._

- gcc 6.3 or Visual Studio 15 2017
- cMake 3.12
- git 1.8

### Clone, configure, build and install

First, clone the project:

```bash
git clone https://github.com/rte-france/sirius-solver.git -b master Sirius
```

Then you can configure it with cmake:

```bash
cd Sirius
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="SiriusInstall" -B build -S src
```

And finally build and install:

```bash
cmake --build build/ --config Release --target install
```

A directory named __SiriusInstall__ will be created, containing the Sirius solver library to be used with cmake projects.

## How to link Sirius library in a C++ cmake project

### Locate your Sirius install directory

First, locate your Sirius install directory (generated with the above process, or downloaded from your nexus). It must contains the cmake, lib and include directories.


This directory will be refered to as __sirius_solver_ROOT__ from now on.

### Make cmake aware of where the Sirius library is located

There are two ways of doing so.

You can define an environnement variable named __sirius_solver_ROOT__ pointing to your Sirius install directory.

Or you can define a __sirius_solver_ROOT__ variable while calling cmake

```bash
cmake -Dsirius_solver_ROOT="/path/to/sirius_solver_ROOT" [the rest of your cmake configuration command] ...
```

### Add Sirius to your cmake

#### Import with find_package

First, you need to activate the following cmake policy. It will allow cmake to find everything it needs to use the Sirius library from the __sirius_solver_ROOT__ path.

```cmake
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
```

You can then just import the sirius_solver package.

```cmake
find_package(sirius_solver CONFIG REQUIRED)
```

#### Link with target_link_libraries

Then, you can link you target (binary, library, ...) with sirius_solver.

```cmake
target_link_libraries(${EXECUTABLE_NAME} PUBLIC sirius_solver)
```

#### Use the Sirius solver API in your C++ code

Please refer to the [Sirius solver API user guide](SiriusAPI.md).
