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
  case LARGEUR_INCORRECTE:
    fprintf(stderr, "LARGEUR INCORRECTE");
    break;
  case HAUTEUR_INCORRECTE:
    fprintf(stderr, "HAUTEUR INCORRECTE");
    break;
  case LIGNE_MANQUANTE:
    fprintf(stderr, "LIGNE MANQUANTE");
    break;
  case TAILLE_LIGNE_INCORRECTE:
    fprintf(stderr, "TAILLE LIGNE INCORRECTE");
    break;
  case POSITION_ROBOT_MANQUANTE:
    fprintf(stderr, "POSITION ROBOT MANQUANTE");
    break;
  case CARACTERE_INCORRECT:
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
