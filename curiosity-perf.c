#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "generation_terrains.h"
#include "environnement.h"
#include "interprete.h"
#include "programme.h"

void affichage_position_programme(erreur_programme e)
{
    int i;
    printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
    printf("%s\n", e.ligne);
    /* Impression de e.num_colonne-1 espaces */
    for (i = 1; i < e.num_colonne; i++)
    {
        printf(" ");
    }
    /* Impression d'un curseur de position */
    printf("^\n");
}

void gestion_erreur_programme(erreur_programme e)
{
    switch (e.type_err)
    {
    case OK_PROGRAMME:
        break;
    case ERREUR_FICHIER_PROGRAMME:
        printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
        exit(2);
    case ERREUR_BLOC_NON_FERME:
        printf("Erreur lecture du programme : bloc non fermé\n");
        exit(2);
    case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
        printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
        affichage_position_programme(e);
        exit(2);
    case ERREUR_COMMANDE_INCORRECTE:
        printf("Erreur lecture du programme : commande incorrecte\n");
        affichage_position_programme(e);
        exit(2);
    }
}

//    fichier_programme est le fichier contenant le programme-robot évalué
//    N est le nombre de terrains utilisés pour l'évaluation
//    L, impair, la largeur des terrains
//    H, impair, la hauteur des terrains
//    d, la densité d'obstacles
//    graine, la graine du générateur aléatoire
//    nb_step_max, le nombre de pas maximum pour chaque exécution du programme sur un terrain
//    fichier_res, le nom du fichier dans lequel seront écrits les résultats.


typedef struct
{
    int sortie;
    int crash;
    int bloque;
    int pas_total;
    int total;
} stats;


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

    fichier_programme = argv[1];
    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    dObst = strtof(argv[5], NULL);
    seed = strtof(argv[6], NULL);
    nb_step_max = strtol(argv[7], NULL, 10);
    fichier_res = argv[8];


    // test de l et h
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
    
    res_f = fopen(fichier_res, "w");
    srand(getpid());
    fprintf(res_f, "%d\n", N);

    T.hauteur = h;
    T.largeur = l;

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
            s.pas_total++;
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
            break;
        case OK_ROBOT: case ARRET_ROBOT:
            printf("\033[0;31mARRET\033[0m\n");
            fprintf(res_f, "-1\n");
            s.bloque++;
            break;
        case PLOUF_ROBOT:
            printf("\033[0;31mCRASH\033[0m\n");
            fprintf(res_f, "-2\n");
            s.crash++;
            break;
        case CRASH_ROBOT:
            printf("\033[0;31mCRASH\033[0m\n");
            fprintf(res_f, "-3\n");
            s.crash++;
            break;
        default:
            printf("\033[0;31mFAIL %d\033[0m\n", res);
            fprintf(res_f, "-4 : %d\n", res);
            break;
        }
        pas = 0;
    }

    printf("\n");

    printf("==============   STATISTIQUES   ==============\n");
    printf(" - Robot sorti            %d/%d terrains (%3.0f%%)\n", s.sortie, N, ((float)s.sortie/(float)N*100));
    printf(" - Robot obstacle         %d/%d terrains (%3.0f%%)\n", s.crash, N, ((float)s.crash/(float)N*100));
    printf(" - Robot bloqué           %d/%d terrains (%3.0f%%)\n", s.bloque, N, ((float)s.bloque/(float)N*100));
    printf("\n");
    printf("Pas moyen : %3.1f\n", (float)s.pas_total / (float)N);
    printf("pid: %d\n", getpid());
    printf("==============================================\n");

    fclose(res_f);
}