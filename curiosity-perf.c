#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "generation_terrains.h"
#include "environnement.h"
#include "interprete.h"
#include "programme.h"


// On définit une structure pour stocker les statistiques de génération de terrains
typedef struct
{
    int sortie;
    int crash;
    int bloque;
    int pas_total;
    int total;
} stats;

//    fichier_programme est le fichier contenant le programme-robot évalué
//    N est le nombre de terrains utilisés pour l'évaluation
//    L, impair, la largeur des terrains
//    H, impair, la hauteur des terrains
//    d, la densité d'obstacles
//    graine, la graine du générateur aléatoire
//    nb_step_max, le nombre de pas maximum pour chaque exécution du programme sur un terrain
//    fichier_res, le nom du fichier dans lequel seront écrits les résultats.

int main(int argc, char **argv)
{
    char *fichier_programme;
    int N, l, h;
    float dObst;
    float seed;
    int pas, nb_step_max;
    char *fichier_res;

    stats s;

    Terrain T;
    Environnement envt;
    Programme prog;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;

    // Initialisation des statistiques
    s.sortie = 0;
    s.crash = 0;
    s.bloque = 0;
    s.pas_total = 0;
    s.total = 0;

    FILE *res_f;

    if (argc != 9)
    {
        printf("Usage: %s <fichier_programme> <N> <L> <H> <d> <graine> <nb_step_max> <fichier_res>\n", argv[0]);
        return 1;
    }

    // Récupération des informations pour la génération de terrains
    fichier_programme = argv[1];
    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    dObst = strtof(argv[5], NULL);
    seed = strtof(argv[6], NULL);
    nb_step_max = strtol(argv[7], NULL, 10);
    fichier_res = argv[8];


    // Gestion d'erreurs sur les paramètres
    if (l > DIM_MAX || l % 2 == 0)
    {
        printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if (h > DIM_MAX || h % 2 == 0)
    {
        printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if ((dObst > 1) || (dObst < 0))
    {
        printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
        return 1;
    }
    
    // Écriture du nombre de terrains dans le fichier resultats
    res_f = fopen(fichier_res, "w");
    fprintf(res_f, "%d\n", N);

    // Initialisation de la seed
    srand(seed);

    // Initialisation des paramètres constants du terrain
    T.hauteur = h;
    T.largeur = l;


    // Génération des terrains
    for (int i = 1; i <= N; i++)
    {
        printf("TERRAIN %d\n", i);
        /* Création d'un terrain correct */
        do generation_aleatoire(&T, l, h, dObst);
        while (!existe_chemin_vers_sortie(&T));
        s.total++;

        /* Lecture du programme */
        prog.lg = 0;
        errp = lire_programme(&prog, fichier_programme);
        gestion_erreur_programme(errp);

        /* Initialisation de l'environnement */
        envt.t = T;
        envt.r.o = Est;
        envt.r.x = l / 2;
        envt.r.y = h / 2;

        afficher_envt(&envt);
        pas = 0;
        /* Initialisation de l'état */
        init_etat(&etat);
        do
        {
            res = exec_pas(&prog, &envt, &etat);
            pas++;
            /* Affichage du terrain et du robot */
            afficher_envt(&envt);
            printf("%d\n", pas);
        } while (res == OK_ROBOT && pas < nb_step_max);

        /* Affichage du résultat */
        switch (res)
        {
        case SORTIE_ROBOT:
            fprintf(res_f, "%d\n", pas);
            s.sortie++;
            s.pas_total += pas;
            break;
        case PLOUF_ROBOT:
            fprintf(res_f, "-2\n");
            s.crash++;
            break;
        case CRASH_ROBOT:
            fprintf(res_f, "-3\n");
            s.crash++;
            break;
        default:
            fprintf(res_f, "-1 : %d\n", res);
            s.bloque++;
            break;
        }
        pas = 0;
    }

    // Affichage des statistiques
    printf("\n");

    printf("==============   STATISTIQUES   ==============\n");
    printf(" - Robot sorti            %d/%d terrains (%3.0f%%)\n", s.sortie, N, ((float)s.sortie/(float)N*100));
    printf(" - Robot obstacle         %d/%d terrains (%3.0f%%)\n", s.crash, N, ((float)s.crash/(float)N*100));
    printf(" - Robot bloqué           %d/%d terrains (%3.0f%%)\n", s.bloque, N, ((float)s.bloque/(float)N*100));
    printf("\n");
    printf("Pas moyen : %3.1f\n", (float)s.pas_total / (float)s.sortie);
    printf("pid: %d\n", getpid());
    printf("==============================================\n");

    fclose(res_f);
}
