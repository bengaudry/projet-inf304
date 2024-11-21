#include "observateur.h"
#include <stdbool.h>

/** Retourne l'état initial de l'automate */
Etat etat_initial_obs(void) { return Init; }

/** Retourne l'état correspondant à la transition depuis un autre état */
Etat transition(Etat e, Alphabet c)
{
    switch (e)
    {
    case Init:
        switch (c)
        {
        case A:
            return Err;
        case G:
        case D:
            return Init;
        case M:
            return Mes;
        }
        break;
    case Mes:
        switch (c)
        {
        case A:
        case G:
        case D:
            return Init;
        case M:
            return Mes;
        }
        break;
    case Err:
        return Err;
    }
}

/** Retourne vrai si Etat != Err */
bool est_final(Etat e)
{
    return e != Err;
}
