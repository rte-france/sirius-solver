- [Créer et utiliser un objet params](#créer-et-utiliser-un-objet-params)
    - [Préambule](#préambule)
    - [Instancier un nouvel objet params](#instancier-un-nouvel-objet-params)
    - [Modifier un paramètre donné dans une instance d'un objet params](#modifier-un-paramètre-donné-dans-une-instance-dun-objet-params)
    - [Lire un objet params à partir d'un fichier](#lire-un-objet-params-à-partir-dun-fichier)
    - [Écrire un objet params à partir d'un fichier](#Écrire-un-objet-params-à-partir-dun-fichier)
- [Structure PNE_PARAMS](#structure-pne_params)
    - [Paramètres basiques](#paramètres-basiques)
    - [Paramètres avancés](#paramètres-avancés)
    - [SPX_PARAMS dans PNE_PARAMS](#spx_params-dans-pne_params)
- [Structure SPX_PARAMS](#structure-spx_params)
    - [Paramètres basiques](#paramètres-basiques-1)
    - [Paramètres avancés](#paramètres-avancés-1)

# Créer et utiliser un objet params

## Préambule

Ce manuel d'utilisation est écrit en pensant à la résolution se basant sur l'objet **PNE_PARAMS** et donc la résolution d'un problème MIP via la fonction
`PNE_Solveur(PROBLEME_A_RESOUDRE * Probleme , PNE_PARAMS * pneParams);`.  
Mais il est directement applicable à l'objet **SPX_PARAMS** en utilisant la fonction `SPX_Simplexe( PROBLEME_SIMPLEXE * Probleme , PROBLEME_SPX * Spx , SPX_PARAMS * spx_params );` comme point d'entrée.  
>**SPX\_PARAMS** : Les correspondances seront indiquées sous forme de citation, comme cet exemple.

## Instancier un nouvel objet params

Pour obtenir un pointeur sur une nouvelle instance d'un **PNE_PARAMS** il suffit d'appeler la fonction  **newDefaultPneParams()** exemple :  
`PNE_PARAMS * pneParams = newDefaultPneParams();`  

La structure est instanciée avec les valeurs par défaut de tous les paramètres. Ces valeurs sont indiquées dans la documentation de l'objet [**PNE_PARAMS**](pne-params).    
>**SPX\_PARAMS** : `newDefaultSpxParams();`

## Modifier un paramètre donné dans une instance d'un objet params  

Pour modifier un paramètre dans la structure **PNE_PARAMS** avant d'appeler la fonction `PNE_Solveur` il faut appeler une fonction dépendant du type du paramètre. Par exemple pour le paramètre VERBOSE_PNE qui est un entier, on utilisera l'appel suivant :  
`PNE_setIntParam(pneParams, "VERBOSE_PNE", 1);`

## Lire un objet params à partir d'un fichier

## Écrire un objet params à partir d'un fichier

La fonction `void PNE_EcrireJeuDeParametres_AvecNom(PNE_PARAMS* Params, char const * file_name, int DiffSeules)` permet d'écrire le jeu de paramètres contenu dans une struct PNE_PARAMS. _Le dernier argument sert à n'écrire que les paramètres ayant valeur est différente de celle par défaut._

>**SPX_PARAMS** : `SPX_EcrireJeuDeParametres_AvecNom(SPX_PARAMS\* Params, char const \* file_name, int DiffSeules);`

Petit exemple d'utilisation :  
Pour connaitre l'ensemble des valeurs par défaut sans consulter la documentation, vous pouvez utiliser le code suivant :  
`PNE_PARAMS * defaultPneParams = newDefaultPneParams();`  
`PNE_EcrireJeuDeParametres_AvecNom(defaultPneParams, "pne_params_defaut.prm", 0);`  
La liste complète des paramètres et leur valeur par défaut sera écrite dans le fichier pne_params_defaut.prm à l'endroit où l'exécutable sera lancé.

# Structure PNE_PARAMS

## Paramètres basiques
| Nom param |  valeur par défaut | type | description |  
| -------- | -------- | -------- | -------- |  
| **AffichageDesTraces** | **1** | int | _Active (1) ou désactive (0) les traces de plus haut niveau_ |  
| **TempsDExecutionMaximum** | **0** | int | _Arrêt de l'algorithme après  `TempsDExecutionMaximum` secondes. 0 <=> pas de limite._ |  
| **FaireDuPresolve** | **1** | int | _Active (1) ou désactive (0) le présolve._ |
| **ToleranceDOptimalite** | **1e-12** | double | _(Gap relatif) Définit une tolérance relative sur l'écart entre la meilleure fonction objectif en nombres entiers et la fonction objectif du meilleur noeud restant. **Valeur donnée en pourcentage : 0.1 = 0.1%**_ |
| **VERBOSE_PNE** |  **0** | int | _Active (1) ou désactive (0) les traces liées au problème PNE._ |  

## Paramètres avancés

| Nom param |  valeur par défaut | type | description |  
| -------- | -------- | -------- | -------- |  
| **NombreMaxDeSolutionsEntieres** | **1e6** | int | _Arrêt de l'algorithme après avoir trouvé plus de `NombreMaxDeSolutionsEntieres` solutions entières._ |  
| **TOLERANCE_SUR_LES_ENTIERS** | **1.e-6** | double | _Spécifie la quantité absolue dont une variable entière peut différer d'un entier en étant néanmoins considérée comme réalisable._ |
| **SEUIL_DADMISSIBILITE** | **1.e-6** | double | _Tolérance en valeur absolue sur le respect des bornes des contraintes_ |


 



## SPX_PARAMS dans PNE_PARAMS

Les paramètres de la sous partie Simplexe sont accessible directement via les setter `PNE_setXxxParam()` comme il le serait dans l'objet SPX_PARAMS avec les setter `SPX_setXxxParam()`.  
C'est à dire qu'on peut écrire directement `PNE_setIntParam("VERBOSE_SPX", pne_params, 1);` au lieu de `SPX_setIntParam("VERBOSE_SPX", pne_params->spx_params, 1);`.
La structure SPX_PARAMS est décrite en détail ci-après [Structure SPX_PARAMS](#structure-spx_params).

# Structure SPX_PARAMS

## Paramètres basiques

| Nom param | valeur par défaut | type | description |
| --- | --- | --- | --- |
| **VERBOSE_SPX_SCALING** | **0** | int | _Active (1) ou désactive (0) les traces liées au scaling du Simplexe._ |
| **VERBOSE_SPX** | **0** | int | _Active (1) ou désactive (0) les traces liées au Simplexe._ |

## Paramètres avancés

| Nom param | valeur par défaut | type | description |
| --- | --- | --- | --- |
| **SEUIL_DE_VIOLATION_DE_BORNE** | **1.e-7** | double | _Seuil de violation de borne pour l'algorithme dual, c'est le seuil de convergence de l'algorithme dual_ |
