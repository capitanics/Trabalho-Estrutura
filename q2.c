#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 4 prateleiras de aterrisagem (filas)
//

typedef struct aviao {
  int id;
  int combustivel;
} Aviao;

void init_aviao(Aviao *aviao, int id) {
  aviao->id = id;
  aviao->combustivel = 10;
}

typedef struct fila_node {
  Aviao aviao;
  struct fila_node *next;
} FilaNode;

typedef struct fila {
  FilaNode *inicio;
  FilaNode *fim;
  int tamanho;
} Fila;

void init_fila(Fila *fila) {
  fila->inicio = NULL;
  fila->fim = NULL;
  fila->tamanho = 0;
}

typedef struct filas_aterrissagem {
  Fila *filas[4];
  int last_id;
} Prateleiras;

void init_filas_prateleiras(Prateleiras *filas) {
  filas->last_id = 1;
  for (int i = 0; i < 4; i++) {
    filas->filas[i] = (Fila *)malloc(sizeof(Fila));
    init_fila(filas->filas[i]);
  }
}

int check_smallest_line(Prateleiras *fila) {
  int menor = fila->filas[0]->tamanho;
  for (int i = 0; i < 4; i++) {
    if (fila->filas[i]->tamanho < menor) {
      menor = fila->filas[i]->tamanho;
    }
  }
  return menor;
}

void insert_plane(Fila *fila, Aviao *aviao) {
  FilaNode *node = (FilaNode *)malloc(sizeof(FilaNode));
  node->aviao = *aviao;
  node->next = NULL;

  if (fila->inicio == NULL) {
    fila->inicio = node;
    fila->fim = node;
  } else {
    fila->fim->next = node;
    fila->fim = node;
  }
  fila->tamanho++;
}

void remove_plane(Fila *fila) {
  if (fila->inicio == NULL) {
    return;
  }

  FilaNode *node = fila->inicio;
  fila->inicio = fila->inicio->next;
  free(node);
  fila->tamanho--;
}

void receive_plane(Prateleiras *fila) {
  int menor = check_smallest_line(fila);
  Aviao aviao;
  init_aviao(&aviao, fila->last_id);
  fila->last_id++;

  insert_plane(fila->filas[menor], &aviao);
}

void land_plane(Prateleiras *fila) {
  for (int i = 0; i < 4; i++) {
    if (fila->filas[i]->tamanho > 0) {
      remove_plane(fila->filas[i]);
      return;
    }
  }
}

}
// Pista 1: 0 - 1
// Pista 2: 2 - 3
//
void print_prateleiras(Prateleiras *fila) {
  for (int i = 0; i < 4; i++) {
    printf("Fila %d: ", i);
    FilaNode *node = fila->filas[i]->inicio;
    while (node != NULL) {
      printf("%d ", node->aviao.id);
      node = node->next;
    }
    printf("\n");
  }
}

void simulation() {}

int main() {
  srand(time(NULL));
  Prateleiras prateleiras;
  init_filas_prateleiras(&prateleiras);

  int rand_planes = rand() % 4;
  printf("%d", check_smallest_line(&prateleiras));
  receive_plane(&prateleiras);
  printf("%d", check_smallest_line(&prateleiras));
  while (true) {

    for (int i = 0; i < rand_planes; i++) {
      receive_plane(&prateleiras);
    }
    land_plane(&prateleiras);
    // printf("%d", check_smallest_line(&prateleiras));

    // print_prateleiras(&prateleiras);

    sleep(1);
  }
  return 0;
}
