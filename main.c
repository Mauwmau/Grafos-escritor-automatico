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


/* Função que delasoca memória de um vetor de strings passado por parâmetro (frase) de tamanho (qtd)
 *  @parametros
 *      char ***frase - ponteiro de um vetor de strings que será desalocado
 *      int qtd - quantidade de strings existentes no vetor
 */
void limpaFrase(char ***frase, int qtd){
    char **fr = *frase;

    for (int i = 0; i < qtd; ++i) {
        free(fr[i]);
    }
    free(fr);
}


/* Função que lê uma frase de um arquivo (file) e o armazena as (qtd) palavras em um vetor de strings (fr) passado como parametro
 *  @parametros
 *      FILE *file - arquivo de texto a ser lido
 *      char ***fr - ponteiro para um vetors de strings que será alocado e armazenará as palavras
 *      int *qtd - ponteiro para um int que receberá a quantidade de palavras lidas no arquivo
 *
 *  @retorno
 *      1 - foi possivel ler algo no arquivo
 *      0 - não foi possivel ler do arquivo, provavelmente chegou em EOF
 */
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


/* Função que lê frases da entrada padrão e armazena as (qtd) palavras em um vetor de strings
 *  @parametros
 *      char ***fr - ponteiro para o vetor de strings que armazenará as palavras lidas na saída padrão
 *      int *qtd - ponteiro para um inteiro que armazenará a quantida de palavras lidas
 */
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


/* Função que insere vertices e arestas em um grafo (g) conforme as palavras armazenadas em um vetor de strings (frase) de tamanho (qtdPalavra)
 * Insere de forma que todas as palavras sao inseridas como vértices e a palavra logo após é usada para inserir como uma aresta
 * Exemplo de arestas inseridas: palavra[0] -> palavra[1], palvra[i]->palavra[i+1], palavra[qtdPalavra-2]->palavra[qtdPalavra-1]
 *  @parametros
 *      GRAFO* g - grafo em que os vértices e arestas serão inseridos
 *      char **frase - vetor de strings, cujos valores serão usados para inserir vetores e arestas no grafo
 *      int qtdpalavra - tamanho do vetor de strings
 */
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


/* Função que lê todas as frases de um arquivo e já usa as palavras lidas para inserir os vértices e arestas em um grafo (g)
 *  @parametros
 *      GRAFO* g - grafo em que as palavras serão usadas para inserir vértices e arestas
*/
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


/* Função que usa o escreve novas frases escritas pelo usuário no arquivo de base para a formação do grafo inicial
 *  @parametros
 *      char ***fr - ponteiro para vetor de strings que armazena as palavras escritas pelo usuário
 *      qtdpalavra - quantida de palavras na frase escrita pelo usuario
 */
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

    /* Cria o grafo e já insere dois vértice muito importantes, o de inicio da frase e o de fim da frase*/
    GRAFO* g = grafoCria();
    verticeInsere(g, INICIO);
    verticeInsere(g, FIM);

    /* Declaração da variáveis que mais serão usadas, o vetor de strings e a quantidade de strings no vetor*/
    char** frase;
    int qtdPalavra;

    /* Lê um arquivo e constrói um grafo inicial baseado nas frases do arquivo*/
    readFile(g);

    /* Laço de repetição principal, pede para o usuário digitar algo, tenta corrigir ou completar a frase
     * se errar pede para que o usuário digite a frase corretamente e atualiza o grafo*/
    int repete = 1;
    while (repete) {
        
        // Pega uma frase da entrada padrão e guarda em (frase)
        printf("Digite uma frase\n>\t");
        getFrase(&frase, &qtdPalavra);


        /* Se a palavra lida for não apenas uma quebra de linha (ou seja, o usuário digitou nenhuma palavra) realize as ações normalente
         * caso contrario, apenas desaloca o vetor de strings e nao permite repeticao*/
        if (frase[0][0] != '\n') {

            /* Tenta corrigir ou completar o que o usuário digitou por meio de algoritmos de caminho mínimo entra cada palavra digita
             * considera tambem o caminho minimo do vertice de inicio à primeira palavra digitada e o da ultima palavra digitada com o vertice de fim*/
            printf("\nVoce quis dizer:\t");
            for (int i = 0; i < qtdPalavra; i++) {
                int j = i + 1;
                if (i == 0) Djokstra(g, verticeBusca(g, INICIO), verticeBusca(g, frase[i]));
                if (j == qtdPalavra) Djokstra(g, verticeBusca(g, frase[i]), verticeBusca(g, FIM));
                else Djokstra(g, verticeBusca(g, frase[i]), verticeBusca(g, frase[j]));
            }
            printf("\n\n[s/n]");
            limpaFrase(&frase, qtdPalavra);

            /* Lê uma char pra indicar se o corretor funcionou corretamente, sendo que 'n' significa que não*/
            char opcao;
            opcao = (char) getchar();
            getchar(); // Ignora um \n q ta atrapalhando aqui

            /* Em caso de o corretor ter errado, pede para que o usuário escreva o que realmente desejava e insere a frase no grafo,
             * atualizando também o arquivo de base para a criação do grafo*/
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