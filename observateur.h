#include <stdbool.h>

#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_

typedef enum
{
    A,
    G,
    D,
    M
} Alphabet;

typedef enum
{
    Init,
    Mes,
    Err
} Obs1Etat;

typedef enum
{
    D0,
    D1,
    D2,
    D3,
    DErr
} Obs2Etat;

/** Retourne l'état initial de l'automate 1 */
Obs1Etat etat_initial_obs1(void);

/** Retourne l'état correspondant à la transition depuis un autre état */
Obs1Etat transition_obs1(Obs1Etat, Alphabet);

/** Retourne vrai si Etat != Err */
bool obs1_est_final(Obs1Etat);

/** Retourne l'état initial de l'automate 2 */
Obs2Etat etat_initial_obs2(void);

/** Retourne l'état correspondant à la transition depuis un autre état */
Obs2Etat transition_obs2(Obs2Etat, Alphabet);

/** Retourne vrai si Etat != Err */
bool obs2_est_final(Obs2Etat);


#endif
