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
} Etat;

/** Retourne l'état initial de l'automate */
Etat etat_initial_obs(void);

/** Retourne l'état correspondant à la transition depuis un autre état */
Etat transition(Etat, Alphabet);

/** Retourne vrai si Etat != Err */
bool est_final(Etat);

#endif
