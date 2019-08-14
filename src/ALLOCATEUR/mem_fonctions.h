// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

# ifdef __CPLUSPLUS
  extern "C"
	{
# endif
# ifndef FONCTION_EXTERNES_MEMOIRE_DEJA_DEFINIES
/*****************************************************************/

void * MEM_Init( void );
void   MEM_Quit( void * );
char * MEM_Malloc( void * , size_t );
void   MEM_Free( void * ); 
char * MEM_Realloc( void * , void * , size_t );
long   MEM_QuantiteLibre( BLOCS_LIBRES * );

/*****************************************************************/
# define FONCTION_EXTERNES_MEMOIRE_DEJA_DEFINIES
# endif
# ifdef __CPLUSPLUS
  }
# endif 
