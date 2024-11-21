#include "terrain.h"
#include <stdio.h>

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
  while (gestion_erreur_terrain(lire_terrain(f, &t, &x, &y)))
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
