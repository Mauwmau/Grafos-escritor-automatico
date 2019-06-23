
#include "lista.h"

struct _no{
    int id;
    int toCompare;
    NO* prox;
};

struct _lista{
    NO* cabeca;
};

LISTA *ListaCria() {
    LISTA* lista;
    lista = (LISTA*)malloc(sizeof(LISTA));

    if(lista == NULL) {
        printf("Erro, nao foi possivel criar uma lista\n");
        return NULL;
    }

    NO* cabeca;
    cabeca = (NO*)malloc(sizeof(NO));

    if (cabeca == NULL) {
        printf("Erro, nao foi possivel criar um no cabeca\n");
        return NULL;
    }

    cabeca->id = -1;
    cabeca->toCompare = -1;
    cabeca->prox = NULL;

    lista->cabeca = cabeca;

    return lista;

}

void ListaApaga(LISTA *lista) {
    if (lista == NULL) {
        printf("Erro, lista ja foi removida\n");
        return;
    }

    NO* aux;
    NO* toDelete;
    aux = lista->cabeca;
    while (aux != NULL){
        toDelete = aux;
        aux = aux->prox;
        free(toDelete);
    }

    free(lista);
}

int ListaVazia(LISTA *lista) {
    return (lista->cabeca->prox == NULL ? 1 : 0);
}

void ListaInsere(LISTA *lista, int id, int valor) {

    NO* no = (NO*)malloc(sizeof(NO));
    if(no == NULL) {
        printf("Erro, Nao foi possivel criar um nó");
        return;
    }

    no->id = id;
    no->toCompare = valor;
    no->prox = NULL;

    NO* aux = lista->cabeca;
    while (aux->prox != NULL && (aux->prox->toCompare > no->toCompare)){
        aux = aux->prox;
    }

    no->prox = aux->prox;
    aux->prox = no;

}

int ListaRemove(LISTA *lista) {
    if(ListaVazia(lista)){
        printf("Erro, a lista ta vazia, nao seu pra remover nada\n");
        return -1;
    }

    NO* no = lista->cabeca->prox;
    lista->cabeca->prox = no->prox;

    int indexToReturn = no->id;
    no->prox = NULL;
    free(no);

    return indexToReturn;

}
