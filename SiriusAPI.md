# Sirius Solver API

## Table of Contents

## Creating / Destroying a problem

### SRScreateprob
### SRSfreeprob
### SRSfreempsprob

## Filling a problem

### SRScreatecols
### SRScreaterows
### SRSsetcoefs

## Modifying a problem

### SRSchgbds
### SRSchgobj
### SRSchgrhs
### SRSchgsens
### SRSchgrangeval

## Getting informations about a problem

### SRSversion
### SRSgetnbcols
### SRSgetnbrows

## Setting parameters

### SRSsetintparams
### SRSsetdoubleparams

## Optimizing

### SRSoptimize

## Getting results

### SRSgetobjval
### SRSgetspxitercount
### SRSgetmipitercount
### SRSgetmipnodecount
### SRSgetx
### SRSgetproblemstatus
### SRSgetreducedcosts
### SRSgetdualvalues
### SRSgetcolbasisstatus
### SRSgetbestbound
### SRSgetrowbasisstatus

## Utils

### SRSreadmpsprob
### SRSwritempsprob
### SPXcopy_problem
### SRScopy_from_problem_simplexe