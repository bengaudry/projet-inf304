#include "observateur.h"
#include <stdbool.h>

////// OBSERVATEUR 1 //////
// Observateur vérifiant que l'on effectue une mesure avant d'avancer

/* Retourne l'état initial de l'automate */
Obs1Etat etat_initial_obs1(void) { return Init; }

/* Retourne l'état correspondant à la transition depuis un autre état */
Obs1Etat transition_obs1(Obs1Etat e, Alphabet c)
{
    switch (e)
    {
    case Init:
        switch (c)
        {
            case A: return Err;
            case G: case D: return Init;
            case M: return Mes;
        }
        break;
    case Mes:
        switch (c)
        {
            case A: case G: case D: return Init;
            case M:  return Mes;
        }
        break;
    case Err:
        return Err;
    }
}

/** Retourne vrai si Etat != Err */
bool obs1_est_final(Obs1Etat e)
{
    return e != Err;
}


////// OBSERVATEUR 2 //////
// Observateur vérifiant que l'on ne tourne pas en rond

/* Retourne l'état initial de l'automate */
Obs2Etat etat_initial_obs2(void) { return D0; }

/* Retourne l'état correspondant à la transition depuis un autre état */
Obs2Etat transition_obs2(Obs2Etat e, Alphabet c)
{
    switch (c)
    {
        case D:
            switch (e) {
                case D0: return D1;
                case D1: return D2;
                case D2: return D3;
                case D3: return DErr;
                case DErr: return DErr;
            }
            break;
        case A: case M:
            return e;
        case G:
            switch (e) {
                case D0: return D0;
                case D1: return D0;
                case D2: return D1;
                case D3: return D2;
                case DErr: return DErr;
            }
            break;
    }
}

/* Retourne vrai si Etat != Err */
bool obs2_est_final(Obs2Etat e)
{
    return e != DErr;
}
