#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "environnement.h"
#include "interprete.h"
#include "programme.h"


// Couleurs pour l'affichage
char rouge[] = "\033[0;31m";
char normal[] = "\033[00m";
char jaune[] = "\033[0;33m";
char vert[] = "\033[0;32m";

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
            printf("ERREUR PILE VIDE\n");
            break;
        case ERREUR_ADRESSAGE:
            printf("ERREUR ADRESSAGE\n");
            break;
        case ERREUR_DIVISION_PAR_ZERO:
            printf("ERREUR DIVISION PAR ZERO\n");
            break;
    }
}

/* Affiche une erreur d'exécution du test*/
void affiche_erreur(char msg[], char nom_terrain[])
{
    fprintf(stderr, " %s[-] %s%s%s\n%s\n", rouge, jaune, nom_terrain, normal, msg);
}

/* Convertir une orientation en un caractère correspondant*/
char orientation_vers_char(Orientation o)
{
    switch (o)
    {
        case Est: return 'E';
        case Ouest: return 'O';
        case Nord: return 'N';
        case Sud: return 'S';
    }
}

/* 
 * Supprime le retour à la ligne à la fin d'une string.
 * (Peut causer des bugs lors de l'ouverture de fichiers)
*/
void supprime_retour_ligne(char *str)
{
    char *c;
    
    c = strchr(str, '\n');
    if (c != NULL) { *c = '\0'; }
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

    int pas = 0;

    char *nom_test;
    char chemin_fichier_terrain[255], chemin_fichier_prog[255];
    int nb_pas_exec_max = 0;
    char evenement_fin = '\0', evenement_attendu_fin = '\0';
    int x_final = 0, y_final = 0;
    char orientation_finale = '\0';

    // Si on utilise l'option '-q', on attend deux arguments, sinon 1
    if (argc < 2 || (argv[1][0] == '-' && argc < 3))
    {
        fprintf(stderr, "Utilisation : %s [opt: -q (quiet)] <nom_test>\n", argv[0]);
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

    // Récuperer le nom du fichier contenant le terrain
    fgets(chemin_fichier_terrain, 255, f);
    supprime_retour_ligne(chemin_fichier_terrain);

    // Récuperer le nom du fichier contenant le programme
    fgets(chemin_fichier_prog, 255, f);
    supprime_retour_ligne(chemin_fichier_prog);

    // Récupérer les informations pour le test
    fscanf(f, "%d\n", &nb_pas_exec_max);
    fscanf(f, "%c\n", &evenement_attendu_fin);

    // Lire la position et l'orientation finale si nécessaire
    if (evenement_attendu_fin == 'N' || evenement_attendu_fin == 'F')
    {
        fscanf(f, "%d %d\n", &x_final, &y_final);
        fscanf(f, "%c", &orientation_finale);
    }

    fclose(f);

    /* Initialisation de l'environnement : lecture du terrain,
       initialisation de la position du robot */
    errt = initialise_environnement(&envt, chemin_fichier_terrain);
    gestion_erreur_terrain(errt);

    /* Lecture du programme */
    errp = lire_programme(&prog, chemin_fichier_prog);
    gestion_erreur_programme(errp);

    /* Initialisation de l'état */
    init_etat(&etat);

    // Exécution du programme
    pas = 0;
    do {
        res = exec_pas(&prog, &envt, &etat);
        if (argv[1][0] != '-' || argv[1][1] != 'q'){
            afficher_envt(&envt);
            affiche_etat_inter(res);
        }
        pas++;
    } while (res == OK_ROBOT && pas <= nb_pas_exec_max);

    // Affichage du résultat
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

    /// Gestion des erreurs ///

    if (pas > nb_pas_exec_max)
    {
        affiche_erreur("Le nombre de pas d'éxécution maximum à été atteint", nom_test);
        return 1;
    }

    if (evenement_fin != evenement_attendu_fin)
    {
        affiche_erreur("Les évènements d'arrivée ne correspondent pas", nom_test);
        fprintf(stderr, "Attendu : %c, Obtenu : %c\n", evenement_attendu_fin, evenement_fin);
        return 1;
    }
    if (evenement_attendu_fin == 'N' || evenement_attendu_fin == 'F')
    {
        if (x_final != envt.r.x || y_final != envt.r.y)
        {
            affiche_erreur("La position d'arrivée du robot ne correspond pas", nom_test);
            fprintf(stderr, "Attendu : (%d, %d), Obtenu : (%d, %d)\n", x_final, y_final, envt.r.x, envt.r.y);
            return 1;
        }

        if (orientation_finale != orientation_vers_char(envt.r.o))
        {
            affiche_erreur("L'orientation d'arrivée du robot ne correspond pas", nom_test);
            fprintf(stderr, "Attendu : %c, Obtenu : %c\n", orientation_finale, orientation_vers_char(envt.r.o));
            return 1;
        }
    }
    printf(" %s[+] %s%s%s\n", vert, jaune, nom_test, normal);
    return 0;
}
