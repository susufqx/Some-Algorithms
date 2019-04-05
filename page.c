//
// Date    : 12 / 11 / 2014
// Cette programme est pour l'etude des algorithmes des deplacements des pages
// main.cpp ...
// 此文件包含 FIFO OPT LRU以及CLOCK四种页面置换算法
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N_TACHE 40

void OPT(int *tache, int n_tache, int n_bloc);
void FIFO(int *tache, int n_tache, int n_bloc);
void LRU(int *tache, int n_tache, int n_bloc);
void CLOCK(int *tache, int n_tache, int n_bloc);

//---------------------------main fonction here----------------------------

int main() {
  int n_tache;
  int *tache;
  int i;
  int n_bloc;
  tache = (int *)malloc(N_TACHE * sizeof(int));

  printf("Entrer le nombre de taches qui executent en meme temps : ");
  scanf("%d", &n_tache);
  printf("Creer %d taches aleatoire ( 1 - %d ) : \n", N_TACHE, n_tache);
  // Pour chaque fois , on peut obtenir les nombres differents
  srand((unsigned)time(NULL));
  // Pour obtenir les nombres
  for (i = 0; i < N_TACHE; i++) {
    tache[i] = (rand() % n_tache) + 1;
  }
  // Pour afficher les nombres
  for (i = 0; i < N_TACHE; i++) {
    if ((i + 1) % 10 != 0) {
      printf("%4d", tache[i]);
    } else {
      printf("%4d\n", tache[i]);
    }
  }

  printf("\nEntrer le nombre de blocs dans la memoire : ");
  scanf("%d", &n_bloc);
  printf("\n---------------- Optimal ----------------\n");
  OPT(tache, n_tache, n_bloc);
  printf("\n-----------------  CLOCK  -----------------\n");
  CLOCK(tache, n_tache, n_bloc);
  printf("\n----------- First In First Out -----------\n");
  FIFO(tache, n_tache, n_bloc);
  printf("\n----------- Least Recently Used -----------\n");
  LRU(tache, n_tache, n_bloc);
  return 0;
}

//----------------------- end main fonction -----------------------------

//----------------------------  OPT  ------------------------------------
int exist(int *bloc, int n_bloc, int x) {
  int iFind = -1;
  int i;
  for (i = 0; i < n_bloc; i++) {
    if (x == bloc[i]) {
      iFind = i;
      break;
    }
  }
  return iFind;
}

void OPT(int *tache, int n_tache, int n_bloc) {
  if (n_tache <= n_bloc) {
    printf("Nombre de pages fautes：%d\n", n_tache);
    return;
  }
  // Commencer à OPT
  int *Liste_Bloc = (int *)malloc(n_bloc * sizeof(int));
  int i, j, k;
  for (i = 0; i < n_bloc; i++) {
    Liste_Bloc[i] = 0;
  }
  int faute = 0;
  for (i = 0; i < N_TACHE; i++) {
    if (exist(Liste_Bloc, n_bloc, tache[i]) !=
        -1) // Cette tache est deja dans la memoire
    {
      // Ne rien faire , laisse executer
      for (k = 0; k < n_bloc; k++) {
        printf(" %d ", Liste_Bloc[k]);
      }
      printf("\n");
    } else if (exist(Liste_Bloc, n_bloc, 0) !=
               -1) // Il existe une memoire libre
    {
      faute++;
      int indice = exist(Liste_Bloc, n_bloc,
                         0); // Chercher ou se trouve la memoire libre
      Liste_Bloc[indice] =
          tache[i]; // Mettre la memoire libre avec le numero de la tache
      for (k = 0; k < n_bloc; k++) {
        printf(" %d ", Liste_Bloc[k]);
      }
      printf(" F\n");
    } else {
      // Chercher dans la memoire s'il existe encore cette tache
      // Si non
      // On remplace directement cette tache avec l'autre
      faute++;
      int iExist = 1;
      for (j = 0; j < n_bloc; j++) {
        if (exist(&(tache[i + 1]), N_TACHE - i - 1, Liste_Bloc[j]) == -1) {
          iExist = 0;
          Liste_Bloc[j] = tache[i];
          break;
        }
      }
      if (iExist == 1) {
        // Il y a une page faute
        // Chercher le plus loin tache pour changer
        int max_indice = 0; // Indice dans le memoire qui est le plus loin tache
                            // ... Liste_Bloc
        int next_indice = 0; // Indice dans la liste de tache qui est le plus
                             // loin tache ...tache
        int *next_tache = (int *)malloc(n_bloc * sizeof(int));
        for (j = 0; j < n_bloc; j++) {
          next_tache[j] =
              exist(&(tache[i + 1]), N_TACHE - i - 1, Liste_Bloc[j]) + i + 1;
        }
        for (j = 0; j < n_bloc; j++) {
          if (next_indice < next_tache[j]) {
            next_indice = next_tache[j];
            max_indice = j;
          }
        }
        Liste_Bloc[max_indice] = tache[i];
      }
      for (k = 0; k < n_bloc; k++) {
        printf(" %d ", Liste_Bloc[k]);
      }
      printf(" F\n");
    }
  }
  printf("\nNombre de pages fautes : %d\n", faute);
}

//----------------------------  end OPT  ------------------------------------

//----------------------------  FIFO  ------------------------------------
int exister(int *tab, int n_tab, int n) {
  int x;
  int i;
  for (i = 0; i < n_tab; i++) {
    x = n - tab[i];
    if (x == 0)
      return 1;
  }
  return 0;
}

void FIFO(int *tache, int n_tache, int n_bloc) {
  int faute = 0;
  int i, j, k, l;
  if (n_tache <=
      n_bloc) // quand le nombre de tache est moins que le nombre de bloc
  {
    int *tab;
    tab = (int *)malloc(sizeof(int) * n_tache);

    for (i = 1; i < n_bloc; i++) {
      tab[i] = 0; // On utilise 0 quand le bloc est vide
    }

    int u = 0;
    int n = 0;

    while (n < N_TACHE) {
      int w = exister(tab, n_bloc, tache[n]);
      if (w == 0) {
        tab[u] = tache[n];
        faute = faute + 1;
        for (j = 0; j < n_bloc; j++) {
          printf(" %d ", tab[j]);
        }
        printf(" F\n");
        u = u + 1;
      } else if (w == 1) {
        for (j = 0; j < n_bloc; j++) {
          printf(" %d ", tab[j]);
        }
        printf("\n");
      }
      n++;
    }
    printf("\nNombre de pages fautes : %d\n", faute);
  } else {
    // procédure de FIFO: quand le nombre de tache est plus que le nombre de
    // bloc

    int *mem_bloc;
    mem_bloc = (int *)malloc(sizeof(int) * n_bloc);

    for (i = 1; i < n_bloc; i++) {
      mem_bloc[i] = 0; // On utilise 0 quand le bloc est vide
    }
    int y = 0;
    int m = 0;

    while (mem_bloc[n_bloc - 1] == 0) {
      int q = exister(mem_bloc, n_bloc, tache[m]);
      if (q == 0) {
        mem_bloc[y] = tache[m];
        faute = faute + 1;
        for (j = 0; j < n_bloc; j++) {
          printf(" %d ", mem_bloc[j]);
        }
        printf(" F\n");
        y = y + 1;
      } else if (q == 1) {
        for (j = 0; j < n_bloc; j++) {
          printf(" %d ", mem_bloc[j]);
        }
        printf("\n");
      }
      m++;
    }

    for (i = m; i < N_TACHE; i++) {
      int p = exister(mem_bloc, n_bloc, tache[i]);
      if (p == 0) {
        for (j = 1; j < n_bloc; j++) {
          mem_bloc[j - 1] = mem_bloc[j];
        }
        mem_bloc[n_bloc - 1] = tache[i];
        faute = faute + 1;
        for (k = 0; k < n_bloc; k++) {
          printf(" %d ", mem_bloc[k]);
        }
        printf(" F\n");
      } else if (p == 1) {
        for (l = 0; l < n_bloc; l++) {
          printf(" %d ", mem_bloc[l]);
        }
        printf("\n");
      }
    }
    printf("\nNombre de pages fautes : %d\n", faute);
  }
}
//----------------------------  end FIFO  ------------------------------------

//----------------------------  LRU  ------------------------------------

int mini(int *a, int b) {
  int i = 0;
  int j;
  for (j = 0; j < b; j++) {
    if (a[i] > a[j]) {
      i = j;
    }
  }
  return i;
}

void LRU(int *tache, int n_tache, int n_bloc) {
  int i, j, k, n_faute, m, out;
  int *mp;
  int *mp_n;
  mp = (int *)malloc(sizeof(int) * n_bloc);
  mp_n = (int *)malloc(sizeof(int) * n_bloc);
  n_faute = 0;

  for (i = 0; i < n_bloc; i++) {
    mp[i] = 0;
    mp_n[i] = -n_tache + i;
  }

  for (j = 0; j < N_TACHE; j++) {
    m = -1;
    for (k = 0; k < n_bloc; k++) {

      if (mp[k] == tache[j]) {
        m = k;
      }
    }
    // if already exist, if yes, refresh mp_n; if not, change  mp and refresh
    // mp_n
    if (m >= 0) {
      mp_n[m] = j;
      for (i = 0; i < n_bloc; i++) {
        printf(" %d ", mp[i]);
      }
      printf("\n");
    } else {
      out = mini(mp_n, n_bloc);
      mp[out] = tache[j];
      mp_n[out] = j;
      n_faute++;
      for (i = 0; i < n_bloc; i++) {
        printf(" %d ", mp[i]);
      }
      printf(" F \n");
    }
  }
  printf("\nnombre de pages fautes est : %d\n", n_faute);
}
//----------------------------  end LRU  ------------------------------------

//----------------------------  CLOCK  ------------------------------------
void CLOCK(int *tache, int n_tache, int n_bloc) {
  if (n_tache <= n_bloc) {
    printf("Nombre de pages fautes：%d\n", n_tache);
    return;
  }
  // Commencer à CLOCK
  // Allouer chaque bloc un status
  int *Liste_Bloc = (int *)malloc(n_bloc * sizeof(int));
  int *Status_Bloc = (int *)malloc(n_tache * sizeof(int));
  int i, k;
  for (i = 0; i < n_bloc; i++) {
    Liste_Bloc[i] = 0;
  }
  for (i = 0; i < n_tache; i++) {
    Status_Bloc[i] = 0;
  }
  int *pMemoire = Liste_Bloc;
  int pIndice = 0;
  int faute = 0;
  for (i = 0; i < N_TACHE; i++) {
    if (exist(Liste_Bloc, n_bloc, tache[i]) != -1) {
      Status_Bloc[tache[i]] = 1;
      for (k = 0; k < n_bloc; k++) {
        printf(" %d ", Liste_Bloc[k]);
      }
      printf("\n");
    } else if (exist(Liste_Bloc, n_bloc, 0) != -1) {
      faute++;
      int indice = exist(Liste_Bloc, n_bloc, 0);
      Liste_Bloc[indice] = tache[i];
      pIndice = (pIndice + 1) % n_bloc;
      pMemoire = Liste_Bloc + pIndice;
      Status_Bloc[tache[i]] = 1;
      for (k = 0; k < n_bloc; k++) {
        printf(" %d ", Liste_Bloc[k]);
      }
      printf(" F\n");
    } else {
      faute++;
      while (Status_Bloc[*pMemoire] == 1) {
        // Quand le status est 1 , mettre le status comme 0 et pMemoire move
        // pour le suivant jusqu'à le status est 0
        Status_Bloc[*pMemoire] = 0;
        pIndice = (pIndice + 1) % n_bloc;
        pMemoire = Liste_Bloc + pIndice;
      }
      // pMemoire ainme à 0 , remplace cette page avec ce qui veut entrer
      *pMemoire = tache[i];
      Status_Bloc[tache[i]] = 1;
      for (k = 0; k < n_bloc; k++) {
        printf(" %d ", Liste_Bloc[k]);
      }
      printf(" F\n");
    }
  }
  printf("\nNombre de pages fautes : %d\n", faute);
}
//----------------------------  end CLOCK  ------------------------------------