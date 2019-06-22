
#include "grafos.h"
/*####################################################################################################################*/
/*                                              DEFINICOES                                                            */
/*####################################################################################################################*/
struct _arestas{
    int vizinho;
    int peso;
    ARESTA* prox;
};

struct _vertices{
    int cor;
    char palavra[50];
    ARESTA* ini;
    ARESTA* fim;
};

struct _grafos{
    VERTICE vertices[MaxNumVert];
    int qtdVertices;
};
/*####################################################################################################################*/





/*====================================================================================================================*/
/*                                              FUNCOES                                                               */
/*====================================================================================================================*/

//| Grafo |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GRAFO* grafoCria(){
    GRAFO* g = (GRAFO*) malloc(sizeof(GRAFO));

    if(g == NULL) {
        _ERROR_GRAFO_NULO_
        return NULL;
    }

    /* inicializa todos os vertices do grafo */
    for(int i = 0; i < MaxNumVert; i++){
        /* cria um nó cabeca de arestas para serem os nós iniciais dos vertices recem-criados*/
        ARESTA* cabeca = (ARESTA*) malloc(sizeof(ARESTA));
        if(cabeca == NULL){
            _ERROR_ARESTA_NULA_
            _iteraction_end(i);
            return NULL;
        }

        cabeca->prox = NULL;
        cabeca->peso = -1;
        cabeca->vizinho = -1;

        g->vertices[i].ini = cabeca;
        g->vertices[i].fim = cabeca;
        g->vertices[i].cor = BRANCO;
        g->vertices[i].palavra[0] = TRASH;
        g->vertices[i].palavra[1] = '\0';
    }

    /* nao ha vertices no grafo */
    g->qtdVertices = 0;

    return g;
}

void grafoApaga(GRAFO* g){
    if(g == NULL){
        _ERROR_GRAFO_NULO_
        return;
    }

    /* da free em todas as arestas ja alocadas */
    for(int i = 0; i < MaxNumVert; i++){

        ARESTA* aux = g->vertices[i].ini;
        ARESTA* toDelete;

        while(aux != NULL){
            toDelete = aux;
            aux = aux->prox;
            free(toDelete);
        }
        g->vertices[i].ini = NULL;
        g->vertices[i].fim = NULL;
    }

    /* da free na estrutura grafo */
    free(g);
}

void grafoPrint(GRAFO *g){
    for(int i = 0; i < MaxNumVert; i++){
        if(g->vertices[i].ini->prox != NULL){
            printf("-----------------------------------------------------------------------------------------------\n");
            printf("Vertice: %s\n", g->vertices[i].palavra);
            printf("Vizinhos:\t");
            ARESTA* aux;
            aux = g->vertices[i].ini->prox;
            while(aux != NULL){
                printf("[%s,%d] ",g->vertices[aux->vizinho].palavra, aux->peso);
                aux = aux->prox;
            }
            printf("\n");
            printf("-----------------------------------------------------------------------------------------------\n");
        }
    }
}

//| Vertice |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int verticeBusca(GRAFO *g, char *palavra){

    char keyword[50];

    if(strcmp(palavra,"") == 0){
        keyword[0] = TRASH;
        keyword[1] = '\0';
    }else{
        strcpy(keyword,palavra);
    }

    for(int i = 0; i < MaxNumVert; i++){
        if(strcmp(g->vertices[i].palavra, keyword) == 0){
            return i;
        }
    }

    _BUSCA_FAIL_
    return -1;
}

void verticeInsere(GRAFO *g, char *palavra) {
    int posicao = verticeBusca(g, palavra);

    if(posicao != -1){
        if(DEBUG){printf("- Vertice ja inserido no grafo!\n");}
        return;
    }

    posicao = verticeBusca(g, "");

    if(posicao > -1){
        strcpy(g->vertices[posicao].palavra, palavra);
        g->qtdVertices++;
    } else{
        if(DEBUG){printf("- Nao ha vertices nulos no grafo!\n");}
    }
}

void verticeRemove(GRAFO *g, char *palavra) {
    int posicao = verticeBusca(g, palavra);

    if(posicao > -1){
        g->vertices[posicao].palavra[0] = TRASH;
        g->vertices[posicao].palavra[1] = '\0';
        g->qtdVertices--;
    } else{
        if(DEBUG){printf("- Nao existe o vertice procurado no grafo!\n");}
    }
}

//| Aresta |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arestaInsere(GRAFO *g, int origem, int destino) {

    ARESTA* aresta = g->vertices[origem].ini->prox;

    while(aresta != NULL && aresta->vizinho != destino){
        aresta = aresta->prox;
    }

    //Verifica se aresta ja existe
    if(aresta != NULL){
        //Sim - atualiza peso

        aresta->peso++;

    }else{
        //Nao - insere aresta

        aresta = (ARESTA*) malloc(sizeof(ARESTA));
        if(aresta == NULL){
            _ERROR_ARESTA_NULA_
            return;
        }
        aresta->vizinho = destino;
        aresta->peso = 1;
        aresta->prox = NULL;

        g->vertices[origem].fim->prox = aresta;
        g->vertices[origem].fim = aresta;
    }

}

void arestaRemove(GRAFO *g, int origem, int destino) {
    ARESTA* aux = g->vertices[origem].ini;
    ARESTA* aresta;

    while(aux->prox != NULL && aux->prox->vizinho != destino){
        aux = aux->prox;
    }
    aresta = aux->prox;

    if(aresta == NULL){
        _ERROR_ARESTA_NULA_
        return;
    }

    if(aresta->peso > 1){
        aresta->peso--;
    }else{
        if(g->vertices[origem].fim == aresta){
            g->vertices[origem].fim = aux;
        }
        aux->prox = aresta->prox;
        aresta->prox = NULL;
        free(aresta);
    }

}

//Tentar Djikstra
void Djokstra(GRAFO *g, int vertInicial, int vertFim){
    char[50][50] reves;
    if(g->qtdVertices < 1){
        if (DEBUG) printf("Erro, não há vertices suficientes para realizar a ação!");
        return;
    }

    if(vertInicial < 0 || vertInicial > MaxNumVert){
        if (DEBUG) printf("Erro, o vertice inicial escolhido não é valido!");
        return;
    }

    if(vertFim < 0 || vertFim > MaxNumVert){
        if (DEBUG) printf("Erro, o vertice final escolhido não é valido!");
        return;
    }

    printf("qtd de vertices: %d\n", g->qtdVertices);

    int *antecessor = (int*)malloc(g->qtdVertices * sizeof(int));
    int *distancia = (int*)malloc(g->qtdVertices * sizeof(int));

    antecessor[vertInicial] = vertInicial;

    for(int i = 0; i < g->qtdVertices; i++){
        distancia[i] = ValorBemGrande;
    }

    distancia[vertInicial] = 0;
    antecessor[vertInicial] = -1;

    LISTA* lista = ListaCria();
    ListaInsere(lista, vertInicial, 0);
    g->vertices[vertInicial].cor = CINZA;

    while(!ListaVazia(lista)){
        int atual = ListaRemove(lista);
        int pesoComparacao = 100;

        ARESTA* aresta = g->vertices[atual].ini->prox;
        while (aresta != NULL){
            pesoComparacao = MaxNumVert - aresta->peso;

            if(g->vertices[aresta->vizinho].cor == BRANCO){
                g->vertices[aresta->vizinho].cor = CINZA;
                ListaInsere(lista, aresta->vizinho, pesoComparacao);
            }

            if(distancia[aresta->vizinho] > distancia[atual] + pesoComparacao){
                distancia[aresta->vizinho] = distancia[atual] + pesoComparacao;
                antecessor[aresta->vizinho] = atual;
            }

            aresta = aresta->prox;
        }
        g->vertices[atual].cor = PRETO;

    }
    ListaApaga(lista);

    printf("%s[%d] ->",g->vertices[vertFim].palavra, vertFim);
    int eu = antecessor[vertFim];
    int cont = 0;
    while(eu != -1){
        sprintf(reves[cont],"%s"g->vertices[eu].palavra);
        eu = antecessor[eu];
        cont++;
    }
    for ( i = reves-1; i > -1; i--)
    {
        printf("%s ",reves[i]);
    }

    free(antecessor);
    free(distancia);

    for (int i = 0; i < MaxNumVert; ++i) {
        g->vertices[i].cor = BRANCO;
    }

}

/*====================================================================================================================*/
