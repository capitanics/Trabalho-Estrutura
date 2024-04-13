#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h> //0/1 false/true


typedef struct NoPilha* PtrNoPilha;

struct NoPilha{
    int elemento;
    PtrNoPilha proximo;
}NoPilha;

typedef struct{
    PtrNoPilha topo;
    int qtde;
}PilhaDinamica;

void iniciaPilhaDinamica(PilhaDinamica *p){
    p->qtde = 0;
    p->topo = NULL;
}

bool estaVaziaPilhaDinamica(PilhaDinamica *p){
    return(p->topo == NULL);
}

int tamanhoPilhaDinamica(PilhaDinamica *p){
    return(p->qtde);
}

void imprimirPilhaDinamica(PilhaDinamica *p){
    printf("Binario = [");
    PtrNoPilha aux;
    for(aux =p->topo; aux != NULL; aux =  aux->proximo){
        printf("%d ", aux->elemento);
    }
    printf("]\n");
}

void inserirPilhaDinamica(PilhaDinamica *p, int num){
    PtrNoPilha aux = malloc(sizeof(NoPilha));
    aux->elemento = num;
    aux->proximo = p->topo;
    p->topo = aux;
    p->qtde++;
}

void removerPilhaDinamica(PilhaDinamica *p){
    if(!estaVaziaPilhaDinamica(p)){
        PtrNoPilha aux;
        aux = p->topo;
        p->topo = p->topo->proximo;
        free(aux);
        p->qtde--;
    }
    else{
        printf("Warning: pilha esta vazia!\n");
    }
}

void conversaoEmBinario(int n, PilhaDinamica *p){ //Divide o número por 2 até chegar a 1, e insere o resto da divisão na pilha.
    do{
        inserirPilhaDinamica(p, n % 2);
        n = n/2;
    } while(n> 0);

}
int main(){
    PilhaDinamica p;
    int n;
    printf("Insira um numero:");
    scanf("%d",&n);
    iniciaPilhaDinamica(&p);
    conversaoEmBinario(n,&p);
    imprimirPilhaDinamica(&p);

    return 0;
}