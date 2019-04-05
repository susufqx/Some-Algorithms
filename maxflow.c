//
//  name of this file is Max_Flow.c
//  Created by LI Rui.
//  This programe is used to find the max-flow of a given graph
//

//------------------programe--------------------

#include <stdio.h>
#include <stdlib.h>
void Ford_Fulkerson(int m);
int pre[99];
int Q[99];
int t[99];
//-------there are some arrays--------------
int p[6][6] = {{0, 8, 0, 6, 0, 0}, {0, 0, 4, 0, 8, 0}, {0, 0, 0, 0, 0, 6},
               {0, 0, 0, 0, 6, 0}, {0, 0, 2, 0, 0, 8}, {0, 0, 0, 0, 0, 0}};
// int
// p[7][7]={{0,4,0,6,5,0,0},{0,0,7,8,0,0,0},{0,0,0,0,0,6,6},{0,0,0,0,6,4,0},{0,0,2,0,0,0,8},{0,0,0,0,0,0,6},{0,0,0,0,0,0,0}};
// int p[5][5]={{0,4,0,5,0},{0,0,7,4,0},{0,0,0,0,9},{0,0,4,0,8},{0,0,0,0,0}};
// int
// p[6][6]={{0,2,4,4,0,0},{0,0,4,0,7,0},{0,0,0,0,0,3},{0,0,0,0,6,2},{0,0,2,0,0,8},{0,0,0,0,0,0}};

int main() {
  int m = 6, i,
      j; // the number of  m   can be changed due to the number of the array p.
  for (i = 0; i < 99; i++) {
    pre[i] = 0;
  }
  printf("The original graph is:\n");
  for (i = 0; i < m; i++) {
    for (j = 0; j < m; j++) {
      printf("%d    ", p[i][j]);
    }
    printf("\n");
  }
  Ford_Fulkerson(m);
  return 0;
}

int find_augment_path(int m) // use the method of BFS(Edmond-Karp)
{
  int i, x, u = 0;
  int first = 0, last = 0;
  x = m - 1;
  int *check = (int *)malloc(m * sizeof(int));
  for (i = 0; i < m; i++) {
    check[i] = 0;
  }
  for (i = 0; i < 99; i++) {
    pre[i] = 0;
  }
  check[0] = 1;
  last++;
  Q[0] = 0;

  while (first != last) {
    u = Q[first];
    first++;
    for (i = 0; i < m; i++) {
      if (check[i] == 0 && p[u][i] > 0) {
        pre[i] = u;
        Q[last] = i;
        last++;
        check[i] = 1;
        if (i == x)
          return 1;
      }
    }
  }
  return 0;
}

void Ford_Fulkerson(int m) {
  int i, j, k, flow = 0, c = 9999, s = 0;
  k = m;
  printf("\nThe Process of the Max_Flow is\n");
  while (1) {
    if (!find_augment_path(m)) // =0 thers is no augment path in the graph
    {
      printf("\nNow we can't find an augment path, we have got the Maximum "
             "Flow of this graph.\n");
      printf("----- The maximum flow in this graph is   %d   -----\n\n", flow);
      break;
    } else // draw the augment path and update the graph
    {
      i = m - 1;
      while (i != 0) {
        if (0 < p[pre[i]][i] && p[pre[i]][i] < c) {
          c = p[pre[i]][i];
        }
        k = k - 1;
        t[k] = i;
        i = pre[i];
      }
      s++;
      printf("The augment path %d is "
             ":\n-----------------------------------------------------------\n",
             s);
      printf("The flow is %d\n", c);
      while (k < m) {
        p[pre[t[k]]][t[k]] = p[pre[t[k]]][t[k]] - c;
        p[t[k]][pre[t[k]]] = p[t[k]][pre[t[k]]] + c;
        printf("The path %d ------> %d , the capacity is %d\n", pre[t[k]], t[k],
               p[pre[t[k]]][t[k]]);
        k = k + 1;
      }

      flow = flow + c;
      printf("\n-----------------------------------------------------------\n");
    }
  }
}

// End
