#include <stdlib.h>
#include <stdio.h>
#include "programme.h"
#include "environnement.h"
#include "interprete.h"
#include "terrain.h"

int main(int argc, char **argv)
{
    int res;
    erreur_terrain errt;
    Environnement envt;
    erreur_programme errp;
    Programme prog;
    etat_inter etat_int;

    if (argc < 3)
    {
        fprintf(stderr, "Utilisation: %s <fichier_terrain> <fichier_programme>\n", argv[0]);
        return 1;
    }

    /* Initialisation de l'environnement : lecture du terrain,
    initialisation de la position du robot */
    errt = initialise_environnement(&envt, argv[1]);
    gestion_erreur_terrain(errt);

    /* Lecture du programme */
    errp = lire_programme(&prog, argv[2]);
    gestion_erreur_programme(errp);

    /* Initialisation de l'état */
    init_etat(&etat_int);

    // Exécution du programme sur le terrain
    do
    {
        afficher_envt(&envt);
        res = exec_pas(&prog, &envt, &etat_int);
    } while (res == OK_ROBOT);

    if (!obs1_est_final(envt.etat_obs1))
    {
        fprintf(stderr, "Le programme n'est pas correct (avance sans mesurer).\n");
        return 1;
    }

    if (!obs2_est_final(envt.etat_obs2))
    {
        fprintf(stderr, "Le programme n'est pas correct (tourne en rond).\n");
        return 1;
    }

    printf("Programme OK\n");
    return 0;
}
