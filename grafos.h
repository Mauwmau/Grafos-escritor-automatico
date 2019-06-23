
#ifndef GRAFOPROJETO_GRAFOS_H
#define GRAFOPROJETO_GRAFOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errorgrafos.h"
#include "lista.h"

//Numeros
#define MaxNumVert 500

//Caracteres
#define TRASH '@'

//Cores
#define BRANCO 0
#define CINZA 1
#define PRETO 2

typedef struct _grafos GRAFO;
typedef struct _vertices VERTICE;
typedef struct _arestas ARESTA;

//Grafo
GRAFO* grafoCria();

void grafoApaga(GRAFO* g);

void grafoPrint(GRAFO *g);

//Vertice
void verticeInsere(GRAFO *g, char *palavra);

void verticeRemove(GRAFO *g, char *palavra);

int verticeBusca(GRAFO *g, char *palavra);

//Aresta
void arestaInsere(GRAFO *g, int origem, int destino);

void arestaRemove(GRAFO *g, int origem, int destino);

//Djikstra
void Djokstra(GRAFO *g, int vertInicial, int vertFim);

#endif //GRAFOPROJETO_GRAFOS_H
