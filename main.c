/* Trabalho final da materia de grafos - Corretor automatico
 *
 * Feito por:
 *  Luis Gustavo Sincaruk Vieira
 *  Maurilio da Motta Meireles
 *
 *                              ICMC, 2019*/

#include <stdio.h>
#include "grafos.h"

#define INICIO ".ini"
#define FIM "."

void limpaFrase(char ***frase, int qtd){
    char **fr = *frase;

    for (int i = 0; i < qtd; ++i) {
        free(fr[i]);
    }
    free(fr);
}

int getInput(FILE* file, char ***fr, int* qtd){
    int continuarLendo = 1;

    char **frase = (char**) malloc(sizeof(char*));

    int qtdPalavra = 0;
    char palavra[50];

    char letra;
    int posLetra = 0;
    do{

        if (fscanf(file, "%c", &letra) > 0) {
            if (letra == ' ' || letra == '\n'){
                if (posLetra != 0) {
                    palavra[posLetra] = '\0';
                    posLetra = 0;

                    qtdPalavra++;
                    frase = realloc(frase, qtdPalavra * sizeof(char*));
                    frase[qtdPalavra-1] =(char*) malloc(50 * sizeof(char));
                    strcpy(frase[qtdPalavra-1], palavra);
                }
            } else{
                palavra[posLetra] = letra;
                posLetra++;
            }
        } else{
            continuarLendo = 0;
        }

    }while (letra != '\n' && continuarLendo);

    *fr = frase;
    *qtd = qtdPalavra;

    return continuarLendo;
}


void getFrase(char ***fr, int *qtd){
    char **frase = (char**) malloc(sizeof(char*));

    int qtdPalavra = 0;
    char palavra[50];

    char letra;
    int posLetra = 0;
    do{

    letra =(char) getchar();
    if (letra == ' ' || letra == '\n'){
        if (posLetra != 0) {
            palavra[posLetra] = '\0';
            posLetra = 0;

            qtdPalavra++;
            frase = realloc(frase, qtdPalavra * sizeof(char*));
            frase[qtdPalavra-1] =(char*) malloc(50 * sizeof(char));
            strcpy(frase[qtdPalavra-1], palavra);
        } else{
            frase[0] = (char*) malloc(sizeof(char));
            frase[0][0] = '\n';
        }
    } else{
        palavra[posLetra] = letra;
        posLetra++;
    }

    }while (letra != '\n');

    *fr = frase;
    *qtd = qtdPalavra;

}


void putInGrafo(GRAFO* g, char** frase, int qtdPalavra){
    for(int i = 0; i < qtdPalavra; i++){
        verticeInsere(g, frase[i]);
    }

    for(int i = 0; i < qtdPalavra; i++){
        int j = i+1;
        if(i == 0) arestaInsere(g, verticeBusca(g,INICIO), verticeBusca(g,frase[i]));
        if( j == qtdPalavra) arestaInsere(g, verticeBusca(g, frase[i]), verticeBusca(g, FIM));
        else arestaInsere(g, verticeBusca(g,frase[i]), verticeBusca(g, frase[j]));

    }
}

void readFile(GRAFO* g){
    FILE* fp = fopen("textinput.txt", "r");
    if(fp == NULL){
        if (DEBUG) printf("Erro, nao foi possivel ler o arquivo");
        return;
    }

    char **frase;
    int qtdpalavra;

    while (getInput(fp, &frase,&qtdpalavra)){

        putInGrafo(g, frase, qtdpalavra);

        limpaFrase(&frase, qtdpalavra);
    }

    limpaFrase(&frase, qtdpalavra);


    fclose(fp);
}

void updateFile(char ***fr, int qtdPalavra){
    char **frase = *fr;

    FILE* file = fopen("textinput.txt", "a");
    if (file == NULL) return;
    for (int i = 0; i < qtdPalavra; ++i) {
        int j = 0;
        char space = ' ';
        while(frase[i][j] != '\0'){
            fwrite(&frase[i][j], sizeof(char), 1, file);
            j++;
        }
        if (i == qtdPalavra-1) {
            space = '\n';
        }
        fwrite(&space, sizeof(char), 1, file);
    }
    fclose(file);
}

int main() {

    GRAFO* g = grafoCria();
    verticeInsere(g, INICIO);
    verticeInsere(g, FIM);

    char** frase;
    int qtdPalavra;


    readFile(g);

    //grafoPrint(g);

    int repete = 1;
    while (repete) {
        printf("Digite uma frase\n>\t");
        getFrase(&frase, &qtdPalavra);
        if (frase[0][0] != '\n') {
            printf("\nVoce quis dizer:\t");
            for (int i = 0; i < qtdPalavra; i++) {
                int j = i + 1;
                if (i == 0) Djokstra(g, verticeBusca(g, INICIO), verticeBusca(g, frase[i]));
                if (j == qtdPalavra) Djokstra(g, verticeBusca(g, frase[i]), verticeBusca(g, FIM));
                else Djokstra(g, verticeBusca(g, frase[i]), verticeBusca(g, frase[j]));
            }
            printf("\n\n[s/n]");
            limpaFrase(&frase, qtdPalavra);

            char opcao;
            opcao = (char) getchar();
            getchar(); // Ignora um \n q ta atrapalhando aqui

            if (opcao == 'n') {
                printf("Favor corrigir a frase\n> ");
                getFrase(&frase, &qtdPalavra);
                updateFile(&frase, qtdPalavra);
                putInGrafo(g, frase, qtdPalavra);
                limpaFrase(&frase, qtdPalavra);
            }
        } else {
            limpaFrase(&frase, qtdPalavra);
            repete = 0;
        }
    }


    grafoApaga(g);

    return 0;
}