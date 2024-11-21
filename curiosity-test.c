#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "environnement.h"
#include "interprete.h"
#include "programme.h"

void affiche_etat_inter(resultat_inter r) {
    switch (r)
    {
    case OK_ROBOT:
        printf("OK ROBOT\n");
        break;
    case SORTIE_ROBOT:
        printf("SORTIE ROBOT\n");
        break;
    case ARRET_ROBOT:
        printf("ARRET ROBOT\n");
        break;
    case PLOUF_ROBOT:
        printf("PLOUF ROBOT\n");
        break;
    case CRASH_ROBOT:
        printf("CRASH ROBOT\n");
        break;
    case ERREUR_PILE_VIDE:
        printf("ERREUR_PILE_VIDE\n");
        break;
    case ERREUR_ADRESSAGE:
        printf("ERREUR_ADRESSAGE\n");
        break;
    case ERREUR_DIVISION_PAR_ZERO:
        printf("ERREUR_DIVISION_PAR_ZERO\n");
        break;
    }
}

void print_fail(char msg[], char nom_terrain[])
{
    fprintf(stderr, " \033[0;31m[-]\033[00m \033[0;33m%s\033[00m\n%s\n", nom_terrain, msg);
}

char orientation_to_char(Orientation o)
{
    switch (o)
    {
    case Est:
        return 'E';
        break;
    case Ouest:
        return 'O';
        break;
    case Nord:
        return 'N';
        break;
    case Sud:
        return 'S';
        break;
    }
}

int main(int argc, char **argv)
{
    FILE *f;

    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;

    int pas;

    char *c;
    char *nom_test;
    char chemin_fichier_terrain[255], chemin_fichier_prog[255];
    int nb_pas_exec_max = 0;
    char evenement_fin = '\0', evenement_attendu_fin = '\0';
    int x_final = 0, y_final = 0;
    char orientation_finale = '\0';

    // Si on utilise l'option '-q', on attend deux arguments, sinon 1
    if ((argv[1][0] == '-' && argc < 3) || argc < 2)
    {
        fprintf(stderr, "Utilisation : %s <nom_test>\n", argv[0]);
        return 1;
    }

    // Si on utilise l'option '-q', on ouvre le fichier dans
    // l'argument 2, sinon 1
    nom_test = (argv[1][0] == '-') ? argv[2] : argv[1];
    f = fopen(nom_test, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Le fichier test donné en argument est incorrect\n");
        return 1;
    }

    fgets(chemin_fichier_terrain, 255, f);
    // Supprime le retour à la ligne à la fin du chemin
    if ((c = strchr(chemin_fichier_terrain, '\n')) != NULL)
        *c = '\0';

    fgets(chemin_fichier_prog, 255, f);
    // Supprime le retour à la ligne à la fin du chemin
    if ((c = strchr(chemin_fichier_prog, '\n')) != NULL)
        *c = '\0';

    fscanf(f, "%d\n", &nb_pas_exec_max);
    fscanf(f, "%c\n", &evenement_attendu_fin);

    if (evenement_attendu_fin == 'N' || evenement_attendu_fin == 'F')
    {
        fscanf(f, "%d %d\n", &x_final, &y_final);
        fscanf(f, "%c", &orientation_finale);
    }

    fclose(f);
    // printf("%s, %s, %d, %c, %d, %d, %c\n", chemin_fichier_terrain, chemin_fichier_prog, nb_pas_exec_max, evenement_attendu_fin, x_final, y_final, orientation_finale);

    /* Initialisation de l'environnement : lecture du terrain,
       initialisation de la position du robot */
    errt = initialise_environnement(&envt, chemin_fichier_terrain);
    gestion_erreur_terrain(errt);

    /* Lecture du programme */
    errp = lire_programme(&prog, chemin_fichier_prog);
    gestion_erreur_programme(errp);

    /* Initialisation de l'état */
    init_etat(&etat);
    pas = 0;
    do {
        res = exec_pas(&prog, &envt, &etat);
        if (argv[1][0] != '-' || argv[1][1] != 'q'){
            afficher_envt(&envt);
            affiche_etat_inter(res);
        }
        pas++;
    } while (res == OK_ROBOT && pas <= nb_pas_exec_max);

    /* Affichage du résultat */
    evenement_fin = 'N';
    switch (res)
    {
    case OK_ROBOT:
        evenement_fin = 'N';
        break;
    case SORTIE_ROBOT:
        evenement_fin = 'S';
        break;
    case ARRET_ROBOT:
        if (pas < nb_pas_exec_max)
            evenement_fin = 'F';
        else 
            evenement_fin = 'N';
        break;
    case PLOUF_ROBOT:
        evenement_fin = 'P';
        break;
    case CRASH_ROBOT:
        evenement_fin = 'O';
        break;
    case ERREUR_PILE_VIDE:
        fprintf(stderr, "pile vide");
        break;
    case ERREUR_ADRESSAGE:
        fprintf(stderr, "erreur d'adressage");
        break;
    case ERREUR_DIVISION_PAR_ZERO:
        fprintf(stderr, "division par 0");
        break;
    }

    if (pas > nb_pas_exec_max)
    {
        print_fail("Le nombre de pas d'éxécution maximum à été atteint", nom_test);
        return 1;
    }

    if (evenement_fin != evenement_attendu_fin)
    {
        print_fail("Les évènements d'arrivée ne correspondent pas", nom_test);
        fprintf(stderr, "Attendu : %c, Obtenu : %c\n", evenement_attendu_fin, evenement_fin);
        return 1;
    }
    if (evenement_attendu_fin == 'N' || evenement_attendu_fin == 'F')
    {
        if (x_final != envt.r.x || y_final != envt.r.y)
        {
            print_fail("La position d'arrivée du robot ne correspond pas", nom_test);
            fprintf(stderr, "Attendu : (%d, %d), Obtenu : (%d, %d)\n", x_final, y_final, envt.r.x, envt.r.y);
            return 1;
        }

        if (orientation_finale != orientation_to_char(envt.r.o))
        {
            print_fail("L'orientation d'arrivée du robot ne correspond pas", nom_test);
            fprintf(stderr, "Attendu : %c, Obtenu : %c\n", orientation_finale, orientation_to_char(envt.r.o));
            return 1;
        }
    }
    printf(" \033[0;32m[+]\033[00m \033[0;33m%s\033[00m\n", nom_test);

    return 0;
}
