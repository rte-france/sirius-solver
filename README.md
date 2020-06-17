# Utiliser Sirius

Une fois l'archive de Sirius récupérée
__*** A COMPLETER ***__
```
*** COMPLETE ME PLEASE ***
```
Ou Sirius compilé et installé par vous même (voir [__Compiler Sirius__](#compiler-sirius)).

Désippez l'archive / copiez le répertoire d'install de Sirius.
Ce répertoire (qui contient les sous répertoires cmake, include et lib) sera à faire pointer par __sirius_solver_ROOT__

## cmake
Dans votre cmake, ajoutez la politique de recherche des paquets suivante. Elle permettra à cmake de rechercher Sirius récursivement dans le répertoire pointé par __sirius_solver_ROOT__
```cmake
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
```
Ensuite importez le package Sirius.
```cmake
find_package(sirius_solver CONFIG REQUIRED)
```
Et enfin ajouter la dépendance de votre cible (exe/binaire/lib/...) vers Sirius.
```cmake
target_link_libraries(${EXECUTABLE_NAME} PUBLIC sirius_solver)
```

Une fois le cmake mis à jour, 2 options s'offrent à vous :
- Créer une variable d'environnement __sirius_solver_ROOT__ qui donne le chemin vers le répertoire d'install de Sirius
  - SET __sirius_solver_ROOT__="/chemin/vers/install" _(pour Window, ou via les menus système)_
  - export __sirius_solver_ROOT__="/chemin/vers/install" _(pour Linux)_
- Définir cette variable uniquement pour cmake au moment de l'invocation
  - cmake __-Dsirius_solver_ROOT__="/chemin/vers/install" _[le reste de votre commande cmake ...]_

# Compiler Sirius

## Sur CentOS 7

### Prérequis

#### gcc
```bash
### Avec les droits root
yum install devtoolset-7-gcc*
```

#### cmake
```bash
### Avec les droits root
yum install openssl-devel
sudo yum remove cmake -y
wget https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4.tar.gz
tar xzf cmake-3.16.4.tar.gz
cd cmake-3.16.4
./bootstrap --prefix=/usr/local
make -j 4
make install
```

### Procédure de configuration et de build
```bash
### Activation globale du gcc récent
scl enable devtoolset-7 bash

### Clone du projet d'intégration
rm -rf ortools-integration
git clone https://github.com/rte-france/ortools-integration.git -b unification_2020
cd ortools-integration

### Configuration et build + install de Sirius
git clone https://github.com/rte-france/temp-pne.git -b unification_2020 Sirius
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="install" -B Sirius/buildLinux -S Sirius/src
cmake --build Sirius/buildLinux/ --config Release --target install -j4
```
Le répertoire d'installation étant ici install (dans le répertoire Sirius)

## Sur Windows ( !!! __WORK IN PROGRESS__ !!!)

### Pré-requis
- Visual Studio 15 2017 Win64
- cmake 3.12 ou +
- git 2.8 (& git-bash) ou +

## Procédure de configuration et de build
```bash
### Configuration et build + install de Sirius
git clone https://github.com/rte-france/temp-pne.git -b unification_2020 Sirius
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="install" -B Sirius/buildWindows -S Sirius/src -G "Visual Studio 15 2017 Win64"
cmake --build Sirius/buildWindows/ --config Release --target install -j4
```

Le répertoire d'installation étant ici install (dans le répertoire Sirius)
