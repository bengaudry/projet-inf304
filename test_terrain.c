#include "terrain.h"
#include <stdio.h>

int affiche_erreur(erreur_terrain err)
{
  fprintf(stderr, "\033[0;31m");
  switch (err)
  {
  case ERREUR_FICHIER:
    fprintf(stderr, "ERREUR FICHIER");
    break;
  case ERREUR_LECTURE_LARGEUR:
    fprintf(stderr, "ERREUR LECTURE LARGEUR");
    break;
  case ERREUR_LARGEUR_INCORRECTE:
    fprintf(stderr, "LARGEUR INCORRECTE");
    break;
  case ERREUR_LECTURE_HAUTEUR:
    fprintf(stderr, "ERREUR LECTURE HAUTEUR");
    break;
  case ERREUR_HAUTEUR_INCORRECTE:
    fprintf(stderr, "HAUTEUR INCORRECTE");
    break;
  case ERREUR_LIGNES_MANQUANTES:
    fprintf(stderr, "LIGNE MANQUANTE");
    break;
  case ERREUR_LIGNE_TROP_LONGUE:
    fprintf(stderr, "ERREUR LIGNE TROP LONGUE");
    break;
  case ERREUR_LIGNE_TROP_COURTE:
    fprintf(stderr, "ERREUR LIGNE TROP COURTE");
    break;
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    fprintf(stderr, "POSITION ROBOT MANQUANTE");
    break;
  case ERREUR_CARACTERE_INCORRECT:
    fprintf(stderr, "CARACTERE INCORRECT");
    break;
  case OK:
    fprintf(stderr, "\033[00m\n");
    return 0;
  }
  fprintf(stderr, "\033[00m\n");
  return 1;
}

int main(int argc, char **argv)
{
  FILE *f;
  Terrain t;
  int x, y;

  if (argc < 2)
  {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  while (affiche_erreur(
      lire_terrain(f, &t, &x, &y)))
  {

    printf("Donner un nouveau fichier :\n");
    scanf("%s", argv[1]);
    f = fopen(argv[1], "r");
  }
  fclose(f);

  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
  printf("Largeur: %d, Hauteur: %d\n", largeur(&t), hauteur(&t));

  f = fopen("test_ecriture.txt", "w");
  ecrire_terrain(f, &t, 4, 2);
  fclose(f);
}
