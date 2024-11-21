
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>

int gestion_erreur_terrain(erreur_terrain e)
{
  switch (e)
  {
  case OK:
    return 0;
  case ERREUR_FICHIER:
    fprintf(stderr, "ERREUR FICHIER\n");
    return 1;
  case ERREUR_LECTURE_LARGEUR:
    fprintf(stderr, "ERREUR LECTURE LARGEUR\n");
    return 1;
  case ERREUR_LARGEUR_INCORRECTE:
    fprintf(stderr, "ERREUR LARGEUR INCORRECTE\n");
    return 1;
  case ERREUR_LECTURE_HAUTEUR:
    fprintf(stderr, "ERREUR LECTURE HAUTEUR\n");
    return 1;
  case ERREUR_HAUTEUR_INCORRECTE:
    fprintf(stderr, "ERREUR HAUTEUR INCORRECTE\n");
    return 1;
  case ERREUR_LIGNES_MANQUANTES:
    fprintf(stderr, "ERREUR LIGNES MANQUANTES\n");
    return 1;
  case ERREUR_LIGNE_TROP_LONGUE:
    fprintf(stderr, "ERREUR LIGNE TROP COURTE\n");
    return 1;
  case ERREUR_LIGNE_TROP_COURTE:
    fprintf(stderr, "ERREUR LIGNE TROP COURTE\n");
    return 1;
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    fprintf(stderr, "ERREUR POSITION ROBOT MANQUANTE\n");
    return 1;
  case ERREUR_CARACTERE_INCORRECT:
    fprintf(stderr, "ERREUR CARACTERE INCORRECT\n");
    return 1;
  }
}

/* Convertit une Case en le caractère qui la représente */
char convertir_type_case_char(Case case_parcourue)
{
  switch (case_parcourue)
  {
    case LIBRE: return '.';
    case EAU: return '~';
    case ROCHER: return '#';
  }
}

erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y)
{
  int l, h; // Dimensions du terrain
  int rx, ry;
  char ligne[DIM_MAX];
  Case case_parcourue;

  rx = -1;
  ry = -1;

  if (f == NULL)
    return ERREUR_FICHIER;

  // Lecture de la largeur
  if (fscanf(f, "%d", &l) == EOF)
    return ERREUR_LECTURE_LARGEUR;
  if (l >= DIM_MAX || l <= 0)
    return ERREUR_LARGEUR_INCORRECTE;

  // Lecture de la hauteur
  if (fscanf(f, "%d", &h) == EOF)
    return ERREUR_HAUTEUR_INCORRECTE;
  if (h >= DIM_MAX || h <= 0)
    return ERREUR_HAUTEUR_INCORRECTE;

  // Lecture du terrain
  fscanf(f, "\n");
  for (int i = 0; i < h; i++)
  {
    if (fgets(ligne, DIM_MAX, f) == NULL)
      return ERREUR_LIGNES_MANQUANTES;

    if (strlen(ligne) < l + 1)
      return ERREUR_LIGNE_TROP_COURTE;
    if (strlen(ligne) > l + 1)
      return ERREUR_LIGNE_TROP_LONGUE;

    for (int j = 0; j < l; j++)
    {
      switch (ligne[j])
      {
      case '~':
        case_parcourue = EAU;
        break;

      case '#':
        case_parcourue = ROCHER;
        break;

      case '.':
        case_parcourue = LIBRE;
        break;

      case 'C':
        case_parcourue = LIBRE;
        rx = j;
        ry = i;
        break;

      default:
        return ERREUR_CARACTERE_INCORRECT;
      }

      t->tab[j][i] = case_parcourue;
    }
  }

  if (rx < 0 || ry < 0)
    return ERREUR_POSITION_ROBOT_MANQUANTE;

  *x = rx;
  *y = ry;

  t->largeur = l;
  t->hauteur = h;

  return OK;
}

/* Largeur d'un terrain */
int largeur(Terrain *t)
{
  return t->largeur;
}

/* Hauteur d'un terrain */
int hauteur(Terrain *t)
{
  return t->hauteur;
}

/* Indique si la case de coordonnées (x,y) est libre
   Renvoie vrai ssi les 3 conditions suivantes sont vraies :
    - 0 <= x < largeur
    - 0 <= y < hauteur
    - t.tab[x][y] = LIBRE
 */
int est_case_libre(Terrain *t, int x, int y)
{
  return (x >= 0 && x <= largeur(t) + 1) && (x >= 0 && x <= hauteur(t) + 1) && (t->tab[x][y] == LIBRE);
}

/* Affichage d'un terrain t sur la sortie standard */
void afficher_terrain(Terrain *t)
{
  int i, j;
  int h, l;

  h = hauteur(t);
  l = largeur(t);

  for (j = 0; j < hauteur(t); j++)
  {
    for (i = 0; i < largeur(t); i++)
    {
      printf("%c", convertir_type_case_char(t->tab[i][j]));
    }
    printf("\n");
  }
}

/* Écriture d'un terrain t dans un fichier f ouvert en écriture.
   x et y contiennent les coordonnées du robot
   Le terrain est écrit au format lisible par lire_terrain */
void ecrire_terrain(FILE *f, Terrain *t, int x, int y)
{
  int i, j;
  int h, l;

  l = largeur(t);
  h = hauteur(t);
  // fprintf(f, "%d\n%d\n", l, h);

  for (i = 0; i < h; i++)
  {
    for (j = 0; j < l; j++)
    {
      if (x == j && y == i)
        fprintf(f, "C");
      else
        fprintf(f, "%c", convertir_type_case_char(t->tab[j][i]));
    }
    fprintf(f, "\n");
  }
}
