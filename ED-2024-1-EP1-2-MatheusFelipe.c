#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int tempo = 0;

int avioes_aterrissados = 0;
int avioes_decolados = 0;

// Estrura do aviao
typedef struct aviao {
  int id;
  int combustivel;
} Aviao;

// Função para inicializar o avião
void init_aviao(Aviao *aviao, int id) {
  aviao->id = id;
  aviao->combustivel = 10;
}

// Estrutura do node da fila
typedef struct fila_node {
  Aviao aviao;
  struct fila_node *next;
} FilaNode;

// Estrutura da fila
typedef struct fila {
  FilaNode *inicio;
  FilaNode *fim;
  int tamanho;
} Fila;

// Função para inicializar a fila
void init_fila(Fila *fila) {
  fila->inicio = NULL;
  fila->fim = NULL;
  fila->tamanho = 0;
}

// Estrutura das prateleiras
typedef struct filas_aterrissagem {
  Fila *filas[4];
  int last_id;
} Prateleiras;

// Estrutura das filas de decolagem
typedef struct filas_decolagem {
  Fila *filas[3];
  int last_id;
} filas_decolagem;

// Função para inicializar as prateleiras
void init_filas_prateleiras(Prateleiras *filas) {
  filas->last_id = 1;
  for (int i = 0; i < 4; i++) {
    filas->filas[i] = (Fila *)malloc(sizeof(Fila));
    init_fila(filas->filas[i]);
  }
}
// WARNING: Versao antia de init_filas_decolagem
//  void init_filas_decolagem(filas_decolagem *filas) {
//    filas->last_id = 0;
//    for (int i = 0; i < 4; i++) {
//      filas->filas[i] = (Fila *)malloc(sizeof(Fila));
//      init_fila(filas->filas[i]);
//    }
//  }
//

// Função para inicializar as filas de decolagem
void init_filas_decolagem(filas_decolagem *filas) {
  filas->last_id = 2; // Start the IDs from 2
  for (int i = 0; i < 3; i++) {
    filas->filas[i] = (Fila *)malloc(sizeof(Fila));
    init_fila(filas->filas[i]);
  }
}

// Função para verificar a menor fila de aterrisagem
int check_smallest_line_aterrissagem(Prateleiras *fila) {
  int menor = 0;
  for (int i = 0; i < 4; i++) {
    if (fila->filas[i]->tamanho < fila->filas[menor]->tamanho) {
      menor = i;
    }
  }
  return menor;
}

// Função para verificar a menor fila de decolagem
int check_smallest_line_decolagem(filas_decolagem *fila) {
  int menor = 0;
  for (int i = 0; i < 3; i++) {
    if (fila->filas[i]->tamanho < fila->filas[menor]->tamanho) {
      menor = i;
    }
  }
  return menor;
}

// Função para verificar a maior fila de aterrisagem
int check_largest_line_aterrissagem(Prateleiras *fila) {
  int maior = 0;
  for (int i = 0; i < 4; i++) {
    if (fila->filas[i]->tamanho > fila->filas[maior]->tamanho) {
      maior = i;
    }
  }
  return maior;
}

// Função para verificar a maior fila de decolagem
int check_largest_line_decolagem(filas_decolagem *fila) {
  int maior = 0;
  for (int i = 0; i < 3; i++) {
    if (fila->filas[i]->tamanho > fila->filas[maior]->tamanho) {
      maior = i;
    }
  }
  return maior;
}

// Função para inserir um avião na fila
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

// Função para remover um avião da fila
void remove_plane(Fila *fila) {
  if (fila->inicio == NULL) {
    return;
  }

  FilaNode *node = fila->inicio;
  fila->inicio = fila->inicio->next;
  free(node);
  fila->tamanho--;
}

// Função para receber um avião na fila de aterrisagem
void receive_plane(Prateleiras *fila) {
  int menor = check_smallest_line_aterrissagem(fila);
  Aviao aviao;
  init_aviao(&aviao, fila->last_id);
  fila->last_id += 2;

  insert_plane(fila->filas[menor], &aviao);
}

// void receive_plane_decolagem(filas_decolagem *fila) {
//   int menor = check_smallest_line_decolagem(fila);
//   Aviao aviao;
//   init_aviao(&aviao, fila->last_id);
//   fila->last_id = fila->last_id + 2;
//
//   insert_plane(fila->filas[menor], &aviao);
// }

// Função para receber um avião na fila de decolagem
void receive_plane_decolagem(filas_decolagem *fila) {
  int menor = check_smallest_line_decolagem(fila);
  Aviao aviao;
  init_aviao(&aviao, fila->last_id);
  fila->last_id += 2; // Increment the ID for the next plane
  insert_plane(fila->filas[menor], &aviao);
}

// Função para decolar um avião
void takeoff_plane(filas_decolagem *fila, int index) {
  if (fila->filas[index]->tamanho > 0) {
    remove_plane(fila->filas[index]);
    avioes_decolados++;
    return;
  }
}

// Função para aterrisar um avião
void land_plane(Prateleiras *fila, int index) {
  if (fila->filas[index]->tamanho > 0) {
    remove_plane(fila->filas[index]);
    avioes_aterrissados++;
    return;
  }
}

// Função para distribuir os aviões nas pistas
void distribuir_pistas(Prateleiras *prateleiras, filas_decolagem *decolagem) {
  // Pista 1
  for (int i = 0; i < 2; i++) {
    if (prateleiras->filas[i]->inicio != NULL &&
        decolagem->filas[0]->inicio != NULL) {
      if (prateleiras->filas[i]->tamanho > decolagem->filas[0]->tamanho) {
        printw("Aterrisando aviao %d\n",
               prateleiras->filas[i]->inicio->aviao.id);
        land_plane(prateleiras, i);
      } else {
        printw("Decolando aviao %d\n", decolagem->filas[0]->inicio->aviao.id);
        takeoff_plane(decolagem, 0);
      }
    }
  }
  // Pista 2
  for (int i = 2; i < 4; i++) {
    if (prateleiras->filas[i]->inicio != NULL &&
        decolagem->filas[1]->inicio != NULL) {
      if (prateleiras->filas[i]->tamanho > decolagem->filas[1]->tamanho) {
        printw("Aterrisando aviao %d\n",
               prateleiras->filas[i]->inicio->aviao.id);
        land_plane(prateleiras, i);
      } else {
        printw("Decolando aviao %d\n", decolagem->filas[1]->inicio->aviao.id);
        takeoff_plane(decolagem, 1);
      }
    }
  }
  // Pista 3
  if (decolagem->filas[2]->inicio != NULL) {
    printw("Decolando aviao %d\n", decolagem->filas[2]->inicio->aviao.id);
    takeoff_plane(decolagem, 2);
  }
}
// Função para calcular o tempo médio de aterrisagem
double calcular_tempo_medio_aterrissagem() {
  if (avioes_aterrissados == 0) {
    return 0.0;
  } else {
    return (double)tempo / avioes_aterrissados;
  }
}

// Função para calcular o tempo médio de decolagem
double calcular_tempo_medio_decolagem() {
  if (avioes_decolados == 0) {
    return 0.0;
  } else {
    return (double)tempo / avioes_decolados;
  }
}

// Função para imprimir as prateleiras
void print_prateleiras(Prateleiras *prateleiras) {
  printw("Prateleira 1: ");
  FilaNode *node = prateleiras->filas[0]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");

  printw("Prateleira 2: ");
  node = prateleiras->filas[1]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");

  printw("Prateleira 3: ");
  node = prateleiras->filas[2]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");

  printw("Prateleira 4: ");
  node = prateleiras->filas[3]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");
}

// Função para imprimir as filas de decolagem
void print_decolagem(filas_decolagem *decolagem) {
  printw("Fila 1: ");
  FilaNode *node = decolagem->filas[0]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");

  printw("Fila 2: ");
  node = decolagem->filas[1]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");

  printw("Fila 3: ");
  node = decolagem->filas[2]->inicio;
  while (node != NULL) {
    printw("%d ", node->aviao.id);
    node = node->next;
  }
  printw("\n");
}

// função para pegar a entrada do usuário
void get_user_input() {
  nodelay(stdscr, TRUE);
  int c = getch();
  if (c == 'q') {
    endwin();
    exit(0);
  }
}

// Função para liberar as filas
void free_queues(Prateleiras *fila_aterrisagem,
                 filas_decolagem *fila_decolagem) {
  for (int i = 0; i < 4; i++) {
    while (fila_aterrisagem->filas[i]->inicio != NULL) {
      remove_plane(fila_aterrisagem->filas[i]);
    }
    free(fila_aterrisagem->filas[i]);
  }

  for (int i = 0; i < 3; i++) {
    while (fila_decolagem->filas[i]->inicio != NULL) {
      remove_plane(fila_decolagem->filas[i]);
    }
    free(fila_decolagem->filas[i]);
  }
}

// Função para simular o aeroporto
void simulation() {
  // Inicialização das filas
  Prateleiras fila_aterrisagem;
  filas_decolagem fila_decolagem;
  init_filas_prateleiras(&fila_aterrisagem);
  init_filas_decolagem(&fila_decolagem);

  // Inicialização do ncurses
  initscr();
  noecho();
  curs_set(0);

  while (true) {
    // Loop  de simulação
    clear();
    int avioes_aterrisando = rand() % 4;
    int avioes_deecolando = rand() % 4;

    for (int i = 0; i < avioes_aterrisando; i++) {
      receive_plane(&fila_aterrisagem);
    }
    for (int i = 0; i < avioes_deecolando; i++) {
      receive_plane_decolagem(&fila_decolagem);
    }

    printw("------------------------------------------------------------\n");
    printw("Tempo medio de espera para aterrisagem: %f\n",
           calcular_tempo_medio_aterrissagem());
    printw("Tempo medio de espera para decolagem: %f\n",
           calcular_tempo_medio_decolagem());
    printw("Numero de aviões aterrisando: %d\n", avioes_aterrisando);
    printw("Numero de aviões decolando: %d\n", avioes_deecolando);
    printw("FILAS DE ATERRISAGEM\n");
    print_prateleiras(&fila_aterrisagem);
    printw("FILAS DE DECOLAGEM\n");
    print_decolagem(&fila_decolagem);
    printw("------------------------------------------------------------\n");

    distribuir_pistas(&fila_aterrisagem, &fila_decolagem);
    get_user_input();

    refresh();
    tempo++;
    sleep(1);
  }

  free_queues(&fila_aterrisagem, &fila_decolagem);

  // End ncurses
  endwin();
}

int main() {
  srand(time(NULL));
  simulation();
  return 0;
}
