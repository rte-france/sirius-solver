// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#ifndef __RTE_SOLVER_INTERFACE__H_
#define __RTE_SOLVER_INTERFACE__H_

#include <spx_constantes_externes.h>
#include <pi_constantes_externes.h>
#include <pne_constantes_externes.h>
#include <pne_params.h>

class Linear_Problem {
public:
	int  Contexte; /* Contexte dans lequel le simplexe est utilise. Cet argument peut prendre 3 valeurs:
				   BRANCH_AND_BOUND_OU_CUT: le simplexe est appelé dans un contexte de Branch And Bound
				   ou de Branch And Cut
				   BRANCH_AND_BOUND_OU_CUT_NOEUD: le simplexe est appelé dans un contexte de Branch And Bound
				   ou de Branch And Cut mais on ne reinitialise pas le probleme
				   SIMPLEXE_SEUL: le simplexe est appelé hors d'un contexte de Branch and Bound ou de
				   Branch And Cut (dans ce cas, certaines sauvegardes particulières ne sont
				   pas faites) */
	int     NombreMaxDIterations; /* Si < 0 , le simplexe prendre sa valeur par defaut */
	double   DureeMaxDuCalcul;     /* Exprime en secondes (attention c'est du double).
								   Mettre une valeur negative si pas de duree max a prendre en compte */
	double * CoutLineaire;         /* Couts lineaires */
	double * X;                    /* Vecteur des variables */
	double * Xmin;                 /* Bornes min des variables */
	double * Xmax;                 /* Bornes max des variables */
	int     NombreDeVariables;    /* Nombre de variables */
	int   * TypeDeVariable; /* Indicateur du type de variable, il ne doit prendre que les suivantes
							(voir le fichier spx_constantes_externes.h mais ne jamais utiliser les valeurs explicites
							des constantes):
							VARIABLE_FIXE                  ,
							VARIABLE_BORNEE_DES_DEUX_COTES ,
							VARIABLE_BORNEE_INFERIEUREMENT ,
							VARIABLE_BORNEE_SUPERIEUREMENT ,
							VARIABLE_NON_BORNEE
							*/
							/* La matrice des contraintes */
							/* Nombre de contraintes */
	int   NombreDeContraintes;
	/* Pointeur sur le debut de chaque ligne de la matrice des contraintes */
	int * IndicesDebutDeLigne;
	/* Nombre de termes non nuls de chaque ligne */
	int * NombreDeTermesDesLignes;
	/* Indice colonne des termes de la matrice des contraintes.
									Attention, les termes de la ligne doivent etre ranges dans l'ordre
									croissant des indices de colonnes */
									/* Les termes de la matrice des contraintes */
	int * IndicesColonnes;
	double * CoefficientsDeLaMatriceDesContraintes;
	/* Le second membre */
/* Sens de contrainte: '<' ou '>' ou '=' */
	char   * Sens;
	/* Valeurs de second membre */
	double * SecondMembre;

	/**********************************************************************************/
	/* PROBLEME_ANTARES_A_RESOUDRE */
	/**********************************************************************************/

	/* Tableau de pointeur a des doubles. Ce tableau est parallele a X, il permet
	de renseigner directement les structures de description du reseau avec les
	resultats contenus dans X */
	double ** AdresseOuPlacerLaValeurDesVariablesOptimisees;

	/**********************************************************************************/
	/* PROBLEME_A_RESOUDRE */
	/**********************************************************************************/

	char   * VariableBinaire; /* Vaut OUI_PI ou NON_PI */

	/*
	Indicateur du type de variable, il ne doit prendre que les suivantes
	(voir le fichier pne_constantes_externes.h mais ne jamais utiliser les
	valeurs explicites des constantes):
	VARIABLE_FIXE                  ,
	VARIABLE_BORNEE_DES_DEUX_COTES ,
	VARIABLE_BORNEE_INFERIEUREMENT ,
	VARIABLE_BORNEE_SUPERIEUREMENT ,
	VARIABLE_NON_BORNEE
	*/

	int   * TypeDeBorneDeLaVariable;
	/*
	Pointeur sur un vecteur dans lequel le solveur va mettre
	les variables duales des contraintes. Attention, ce tableau
	doit etre alloue par l'appelant
	*/
	double * VariablesDualesDesContraintes;
	/* Options */
	/* Doit valoir SIMPLEXE ou POINT_INTERIEUR */
	/* Attention, le choix POINT_INTERIEUR ne peut être utilise que dans le cas
	d'un probleme ne comportant pas de varaibles entieres */
	char     AlgorithmeDeResolution;
	/* Peut valoir OUI_PNE ou NON_PNE */
	char     AffichageDesTraces;
	/* Peut valoir OUI_PNE ou NON_PNE.
	Mettre OUI_PNE pour sortir les donnees du probleme dans un fichier au format mps */
	char     SortirLesDonneesDuProbleme;
	/* Peut valoir OUI_PNE ou NON_PNE */
	/* La valeur hautement conseillee est OUI_PNE */
	char     FaireDuPresolve;
	/* Temps (en secondes) au bout duquel la resolution du probleme est arretee meme si la
	solution optimale n'a pas ete trouvee. Attention, cette grandeur n'est prise en compte
	que si le probleme contient des variables entieres */
	/* Mettre 0 si le temps est illimite */
	int     TempsDExecutionMaximum;
	/* Lorsque le nombre de solutions entieres est egal à la valeur de ce
	parametre, le solveur s'arrete et donne la meilleure solution rencontree.
	Remarque: mettre une valeur strictement negative pour que ce parametre n'ai pas
	de rôle.
	*/
	int     NombreMaxDeSolutionsEntieres;
	/* Si l'écart relatif entre le cout de la solution entiere trouvee et le plus petit minorant
	est inférieur à ToleranceDOptimalite, le solveur s'arrete et considère que la solution
	entiere trouvee est la solution optimale.
	Convention: ToleranceDOptimalite doit etre exprimé en %.
	Conseil   : mettre 0 %.
	*/
	double   ToleranceDOptimalite;
	/* Utile que s'il y a des variables entieres dans le probleme.
	Peut valoir OUI_PNE ou NON_PNE. Lorsque cette option vaut OUI_PNE
	le calcul des coupes de type lift and project est activé.
	- Choix conseillé: NON_PNE car le calcul de ce type de coupe peut être
	couteux.
	- Mettre OUI_PNE si le probleme est difficile a resoudre.
	*/
	char     CoupesLiftAndProject;
	/**********************************************************************************/
	/* PROBLEME_SIMPLEX */
	/**********************************************************************************/
	/* Choix de l'algorithme */
	/* L'utilisateur doit mettre :  (RQ seul le dual marche)
	SPX_PRIMAL s'il veut utiliser l'algorithme primal
	SPX_DUAL   s'il veut utiliser l'algorithme dual */
	/* Guidage de l'algorithme */
	int ChoixDeLAlgorithme;
	/* Le pricing est l'étape du calcul dans laquelle on choisit la variable sortante
	dans l'algorithme dual (ou la variale entrante dans l'algorithme primal).
	Deux choix sont possibles:
	* PRICING_DANTZIG: c'est la méthode basique, elle est rapide mais dans certains
	cas conduit à faire beaucoup d'itérations pour trouver l'optimum.
	* PRICING_STEEPEST_EDGE: méthode élaborée (Forrest-Goldfarb), elle demande plus de
	calculs mais permet de réduite significativement le nombre d'itérations. Il est
	recommander de l'utiliser pour les problèmes difficiles. */
	int TypeDePricing;
	/* Vaut OUI_SPX ou NON_SPX. Si l'utilisateur positionne la valeur a OUI_SPX,
	le simplexe fait un scaling du probleme dès le début de la résolution.
	Le scaling a pour but d'améliorer le conditionnement du problème. Il est
	recommandé de l'utiliser lorsque les coefficients de la matrice des contraintes
	sont très différents les un des autres (rapport > 100) */
	int FaireDuScaling;
	/* Vaut AGRESSIF ou PEU_AGRESSIF.
	* AGRESSIF: le controle est fait à chaque iterations.
	* PEU_AGRESSIF: le controle est fait moins souvent.
	-> Choix recommandé: AGRESSIF
	*/
	int StrategieAntiDegenerescence;

	/* Vaut OUI_SPX ou NON_SPX */
	int   BaseDeDepartFournie;
	/* Pour chaque variable, sa position vis a vis de la base. Une variable peut etre de 4 type:
	EN_BASE, HORS_BASE_SUR_BORNE_INF,HORS_BASE_SUR_BORNE_SUP,HORS_BASE_A_ZERO*/
	int * PositionDeLaVariable;
	/* Nombre de variables basiques complementaires (c'est une valeur d'entree mais de sortie aussi) */
	int   NbVarDeBaseComplementaires;
	int * ComplementDeLaBase;
	/* En sortie, vaut :
	OUI_SPX s'il y a une solution,
	NON_SPX s'il n'y a pas de solution admissible
	SPX_ERREUR_INTERNE si probleme a l'execution (saturation memoire par exemple), et
	dans ce cas il n'y a pas de solution
	SPX_MATRICE_DE_BASE_SINGULIERE si on n'a pas pu construire de matrice de base reguliere,
	et dans ce cas il n'y a pas de solution
	*/
	int   ExistenceDUneSolution;
	/* En Entree:
	- Si OUI_SPX la memoire est liberee a la fin du simplexe
	- Si NON_SPX la memoire n'est pas liberee a la fin du simplexe. Cette est utile si l'on
	veut conserver temporairement les donnees du probleme pour calculer des coupes de Gomory
	par exemple. Attention, l'appelant doit ensuite liberer le memoire par un appel
	a "SPX_LibererProbleme" */
	int   LibererMemoireALaFin;
	/* En entree: cette information n'est utilisee que si l'algorithme choisi est l'algorithme dual.
	On sait qu'a chaque iteration de l'algorithme dual, le cout courant est un minorant du cout optimal.
	Il est donc possible de comparer ce cout à un Cout Max, seuil au dessus duquel on convient d'arreter les
	calculs (l'algorithme sort alors avec le verdict "pas de solution").
	Quelle en est l'utilite (mais il peut y en avoir d'autres) ?
	Dans un contexte de branch and bound, des que l'on dispose d'une solution entiere, toutes les resolutions de
	probleme relaxé menant a un cout superieur a ce cout sont a rejeter. Donc, si l'on se rend compte au cours de
	l'algorithme dual, que la resolution du probleme relaxe va mener a un cout trop grand il est inutile de
	poursuivre les calculs. Ceci permet de gagner du temps de calcul.

	ATTENTION: comme l'algorithme dual peut etre utilisé en tant que solveur (c'est à dire
	---------  en dehors d'un contexte de branch and bound) ou bien pour resoudre un probleme dont on
	de souhaite pas donner de Cout Max parce qu'on ne le connait pas, l'information "CoutMax"
	n'est utilisee par l'algorithme dual que si l'indicateur "UtiliserCoutMax" (argument suivant)
	est positionne a "OUI-SPX". */
	double CoutMax;
	int    UtiliserCoutMax;
	/* Couts reduits des variables hors-base, longueur nombre de variables passees
	en entree du probleme. Contient la valeur 0 si la variable est basique */
	double * CoutsReduits;
	/* Les coupes: uniquement dans un contexte BRANCH AND BOUND ET CUT */
	int     NombreDeContraintesCoupes;
	/**********************************************************************************/
	/* PROBLEME_POINT_INTERIEUR */
	/**********************************************************************************/
	double *  CoutQuadratique;
	/* Parametres de controle */

	int    UtiliserLaToleranceDAdmissibiliteParDefaut;
	double  ToleranceDAdmissibilite;

	int    UtiliserLaToleranceDeStationnariteParDefaut;
	double  ToleranceDeStationnarite;

	int    UtiliserLaToleranceDeComplementariteParDefaut;
	double  ToleranceDeComplementarite;


	/* Variables duales (sortie) */
	double * CoutsMarginauxDesContraintes;
	double * CoutsMarginauxDesContraintesDeBorneInf;
	double * CoutsMarginauxDesContraintesDeBorneSup;


	/**********************************************************************************/
	/* PROBLEME_LINEAIRE_PARTIE_FIXE */
	/**********************************************************************************/
	int      NombreDeTermesAlloues;

};

typedef Linear_Problem PROBLEME_POINT_INTERIEUR;
typedef Linear_Problem PROBLEME_A_RESOUDRE;
typedef Linear_Problem PROBLEME_SPX;
typedef Linear_Problem PROBLEME_SIMPLEXE;
typedef Linear_Problem PROBLEME_LINEAIRE_PARTIE_VARIABLE;
typedef Linear_Problem PROBLEME_LINEAIRE_PARTIE_FIXE;


void _PNE_Solveur(PROBLEME_A_RESOUDRE * Probleme, PNE_PARAMS * pneParams);
void _PI_Quamin(PROBLEME_POINT_INTERIEUR * Probleme);
void _SPX_LibererProbleme(PROBLEME_SPX * Spx);
void _SPX_ModifierLeVecteurCouts(PROBLEME_SPX *, double *, int);
void _SPX_ModifierLeVecteurSecondMembre(PROBLEME_SPX *, double *, char *, int);
PROBLEME_SPX * _SPX_Simplexe(PROBLEME_SIMPLEXE *, PROBLEME_SPX *);

#endif