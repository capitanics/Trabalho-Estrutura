#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 4 prateleiras de aterrisagem (filas)
//
int tempo = 0;

int avioes_aterrissados = 0;
int avioes_decolados = 0;

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

typedef struct filas_decolagem{
    Fila *filas[3];
    int last_id;
} filas_decolagem;

void init_filas_prateleiras(Prateleiras *filas) {
    filas->last_id = 1;
    for (int i = 0; i < 4; i++) {
        filas->filas[i] = (Fila *)malloc(sizeof(Fila));
        init_fila(filas->filas[i]);
    }
}

void init_filas_decolagem(filas_decolagem *filas){
    filas->last_id = 1;
    for (int i = 0; i < 4; i++) {
        filas->filas[i] = (Fila *)malloc(sizeof(Fila));
        init_fila(filas->filas[i]);
    }
}

int check_smallest_line_aterrissagem(Prateleiras *fila) {
    int menor = 0;
    for (int i = 0; i < 4; i++) {
        if (fila->filas[i]->tamanho < fila->filas[menor]->tamanho){
            menor = i;
        }
    }
    return menor;
}

int check_smallest_line_decolagem(filas_decolagem *fila){
    int menor = 0;
    for (int i = 0; i < 3; i++) {
        if (fila->filas[i]->tamanho < fila->filas[menor]->tamanho){
            menor = i;
        }
    }
    return menor;
}

int check_largest_line_aterrissagem(Prateleiras *fila) {
    int maior = 0;
    for (int i = 0; i < 4; i++) {
        if (fila->filas[i]->tamanho > fila->filas[maior]->tamanho) {
            maior = i;
        }
    }
    return maior;
}

int check_largest_line_decolagem(filas_decolagem *fila){
    int maior = 0;
    for (int i = 0; i < 3; i++) {
        if (fila->filas[i]->tamanho > fila->filas[maior]->tamanho) {
            maior = i;
        }
    }
    return maior;
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
    int menor = check_smallest_line_aterrissagem(fila);
    Aviao aviao;
    init_aviao(&aviao, fila->last_id);
    fila->last_id++;

    insert_plane(fila->filas[menor], &aviao);
}

void receive_plane_decolagem(filas_decolagem *fila) {
    int menor = check_smallest_line_decolagem(fila);
    Aviao aviao;
    init_aviao(&aviao, fila->last_id);
    fila->last_id++;

    insert_plane(fila->filas[menor], &aviao);
}

void takeoff_plane(filas_decolagem *fila, int index){
    if (fila->filas[index]->tamanho > 0){
        remove_plane(fila->filas[index]);
        avioes_decolados++;
        return;
    }
}

void land_plane(Prateleiras *fila, int index) {
    if (fila->filas[index]->tamanho > 0) {
        remove_plane(fila->filas[index]);
        avioes_aterrissados++;
        return;
    }
}

void distribuir_pistas(Prateleiras *prateleiras, filas_decolagem *decolagem){
    //Pista 1
    for (int i = 0; i < 2; i++) {
        if (prateleiras->filas[i]->tamanho > decolagem->filas[0]->tamanho){
            land_plane(prateleiras,i);
        }else{
            takeoff_plane(decolagem,0);
        }
    }
    //Pista 2
    for(int i = 2; i < 4; i++){
        if (prateleiras->filas[i]->tamanho > decolagem->filas[1]->tamanho){
            land_plane(prateleiras,i);
        }else{
            takeoff_plane(decolagem,1);
        }
    }
    //Pista 3
    takeoff_plane(decolagem,2);
}
double calcular_tempo_medio_aterrissagem() {
    if (avioes_aterrissados == 0) {
        return 0.0;
    } else {
        return (double)tempo/avioes_aterrissados;
    }
}

double calcular_tempo_medio_decolagem(){
    if (avioes_decolados == 0) {
        return 0.0;
    } else {
        return (double)tempo/avioes_decolados;
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
    printf("%d", check_smallest_line_aterrissagem(&prateleiras));
    receive_plane(&prateleiras);
    printf("%d", check_smallest_line_aterrissagem(&prateleiras));
    while (true) {
        tempo++;
        for (int i = 0; i < rand_planes; i++) {
            receive_plane(&prateleiras);
        }
        // printf("%d", check_smallest_line(&prateleiras));

        // print_prateleiras(&prateleiras);
        printf("%.2lf\n", calcular_tempo_medio_aterrissagem());
        sleep(1);
    }
    return 0;
}
