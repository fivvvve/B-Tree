#include <stdio.h>
#include <stdlib.h>
#include "bTree.h"

struct no {
    int **vet;
    no **ponteiros;
    no *pai;
    int posPai;
    int folha;
    int n;
};


struct bTree {
    no *sentinela;
    int quantidadeNos;
    int ordem;
};

// Função que cria a B Tree com a ordem passada como parâmetro
bTree *criaBTree(int ordem){
    bTree *arv = (bTree*) malloc(sizeof(bTree));
    arv->ordem = ordem;
    arv->quantidadeNos = 0;
    arv->sentinela = (no*) malloc(sizeof(no));
    arv->sentinela->ponteiros = (no**) malloc(sizeof(no*));
    arv->sentinela->ponteiros[0] = NULL;

    //n igual a -1 indica que é a sentinela
    arv->sentinela->n = -1;
    return arv;
}


//Função que insere um elemento na B Tree
//Retornos:
//1: Elemento Inserido
//0: Falha ao inserir
int insereBTree(bTree *arv, int indice, int linha){
    
    if(arv == NULL) return 0;
    
    if(arv->sentinela->ponteiros[0] == NULL){
        no *raiz = (no*) malloc(sizeof(no));
        arv->sentinela->ponteiros[0] = raiz;
        arv->quantidadeNos++;

        //Aloca vetor de chaves do nó de tamanho [m][2]
        raiz->vet = (int**) malloc(arv->ordem*sizeof(int*));
        for(int i=0; i<arv->ordem; i++){
            raiz->vet[i] = (int*) malloc(2*sizeof(int));
        }
        raiz->vet[0][0] = indice;
        raiz->vet[0][1] = linha;
        raiz->pai = arv->sentinela;
        raiz->folha = 1;
        raiz->n = 1;

        //Aloca vetor de ponteiros do nó de tamanho [m+1]
        raiz->ponteiros = (no**) malloc((arv->ordem+1)*sizeof(no*));

        for(int i=0; i<=arv->ordem; i++){
            raiz->ponteiros[i] = NULL;
        }
        return 1;
    }

    no *atual = arv->sentinela->ponteiros[0];
    int achou, i;

    //Procura nó folha para inserir elemento
    while(atual->folha != 1){

        achou = 0;

        //Percorre vetor até que indice seja maior que posição do vetor
        for(i=0; i<atual->n; i++){
            if(indice < atual->vet[i][0]){
                atual = atual->ponteiros[i];
                achou = 1;
                break;           
            }
        }

        //Caso indice seja maior que todos elementos do vetor nó atual recebe último ponteiro 
        if(!achou){
            atual = atual->ponteiros[atual->n];
            achou = 0;
        }
    }

    //Insere ordenado no nó
    for(i=atual->n; i>0; i--){
        if(indice > atual->vet[i-1][0]){
            break;
        }
        atual->vet[i][0] = atual->vet[i-1][0];
        atual->vet[i][1] = atual->vet[i-1][1];
    }
    atual->vet[i][0] = indice;
    atual->vet[i][1] = linha;
    atual->n++;

    //Verifica se nó atingiu limite de tamanho
    if(atual->n == arv->ordem){
        balanceamentoInsercao(arv, atual);
    }

}

//Função para balancear árvore na inserção
void balanceamentoInsercao(bTree *arv, no *atual){

    no *pai;

    //Operações de SPLIT

    //Verifica se atual é a raiz
    if(arv->sentinela->ponteiros[0] == atual) {

        //Cria nova raiz
        pai = (no*) malloc(sizeof(no));
        pai->vet = (int**) malloc(arv->ordem*sizeof(int*));
        for(int i=0; i<arv->ordem; i++){
            pai->vet[i] = (int*) malloc(2*sizeof(int));
        }
        pai->ponteiros = (no**) malloc((arv->ordem+1)*sizeof(no*));
        for(int i=0; i<=arv->ordem; i++){
            pai->ponteiros[i] = NULL;
        }

        //Cria irmão
        no *irmao = (no*) malloc(sizeof(no));
        irmao->vet = (int**) malloc(arv->ordem*sizeof(int*));
        for(int i=0; i<arv->ordem; i++){
            irmao->vet[i] = (int*) malloc(2*sizeof(int));
        }
        irmao->ponteiros = (no**) malloc((arv->ordem+1)*sizeof(no*));
        for(int i=0; i<=arv->ordem; i++){
            irmao->ponteiros[i] = NULL;
        }

        //Atualiza valores do pai
        pai->folha = 0;
        pai->n = 1;
        pai->pai = arv->sentinela;
        arv->sentinela->ponteiros[0] = pai;
        pai->ponteiros[0] = atual;
        pai->ponteiros[1] = irmao;

        //Move valor central do nó atual para nova raiz
        pai->vet[0][0] = atual->vet[(arv->ordem-1)/2][0];
        pai->vet[0][1] = atual->vet[(arv->ordem-1)/2][1];

        //Atualiza valores do Irmão
        irmao->folha = atual->folha;
        irmao->n = ((arv->ordem)/2);
        irmao->pai = pai;
        irmao->posPai = 1;

        //Insere elementos do nó atual no nó irmão criado
        int j = 0, i;
        for(i=((arv->ordem-1)/2)+1; i<arv->ordem; i++){
            irmao->vet[j][0] = atual->vet[i][0];
            irmao->vet[j][1] = atual->vet[i][1];
            irmao->ponteiros[j] = atual->ponteiros[i];
            if(atual->ponteiros[i] != NULL){
                atual->ponteiros[i]->pai = irmao;
                atual->ponteiros[i]->posPai = j;
                atual->ponteiros[i] = NULL;
            }
            j++;
        }
        //Copia última ponteiro de atual para irmão
        irmao->ponteiros[j] = atual->ponteiros[i];
        if(atual->ponteiros[i] != NULL){
            atual->ponteiros[i]->pai = irmao;
            atual->ponteiros[i]->posPai = j;
            atual->ponteiros[i] = NULL;
        }

        //Atualizando atual
        atual->n = (arv->ordem-1)/2;
        atual->pai = pai;
        atual->posPai = 0;

        arv->quantidadeNos += 2;
    } 
    
    else {

        /*Copia valores e ponteiros uma posição para trás no nó pai a partir da posição que o 
        nó atual estava*/
        pai = atual->pai;
        for(int i=pai->n; i>atual->posPai; i--) {
            pai->vet[i][0] = pai->vet[i-1][0];
            pai->vet[i][1] = pai->vet[i-1][1];
            pai->ponteiros[i+1] = pai->ponteiros[i];
            pai->ponteiros[i+1]->posPai++;
        }

        //Move valor central do nó atual para nó pai
        pai->vet[atual->posPai][0] = atual->vet[(arv->ordem-1)/2][0];
        pai->vet[atual->posPai][1] = atual->vet[(arv->ordem-1)/2][1];

        //Cria irmão
        no *irmao = (no*) malloc(sizeof(no));
        irmao->vet = (int**) malloc(arv->ordem*sizeof(int*));
        for(int i=0; i<arv->ordem; i++){
            irmao->vet[i] = (int*) malloc(2*sizeof(int));
        }
        irmao->ponteiros = (no**) malloc((arv->ordem+1)*sizeof(no*));
        for(int i=0; i<=arv->ordem; i++){
            irmao->ponteiros[i] = NULL;
        }


        /*Faz pai apontar para o irmão criado uma posição após o nó atual no vetor de ponteiros 
        do pai*/
        pai->ponteiros[atual->posPai+1] = irmao;

        //Copia valores de atual para irmão
        irmao->folha = atual->folha;
        irmao->n = (arv->ordem)/2;
        irmao->pai = pai;
        irmao->posPai = atual->posPai+1;
        int j = 0, i;
        for(i=((arv->ordem-1)/2)+1; i<arv->ordem; i++){
            irmao->vet[j][0] = atual->vet[i][0];
            irmao->vet[j][1] = atual->vet[i][1];
            irmao->ponteiros[j] = atual->ponteiros[i];
            if(irmao->ponteiros[j] != NULL){
                irmao->ponteiros[j]->pai = irmao;
                irmao->ponteiros[j]->posPai = j;
                atual->ponteiros[i] = NULL;
            }
            j++;
        }
        //Copia última ponteiro de atual para irmão
        irmao->ponteiros[j] = atual->ponteiros[i];
        if(atual->ponteiros[i] != NULL){
            atual->ponteiros[i]->pai = irmao;
            atual->ponteiros[i]->posPai = j;
            atual->ponteiros[i] = NULL;
        }

        atual->n = (arv->ordem-1)/2;

        pai->n++;
        
        //Verifica se desbalanceou o pai
        if(pai->n == arv->ordem){
            balanceamentoInsercao(arv, pai);

            //Atualiza nó pai caso ele tenha mudado pela chamada da função
            pai = atual->pai;
        }


        arv->quantidadeNos++;

    }

}

//Função que procura um elemento na B Tree de acordo com o indice passado
//Retornos:
//-1: Elemento não encontrado
//Número da linha do indice passado como parâmetro
int procuraBTree(bTree *arv, int indice, no *raiz){

    //Condição de parada da função recursiva
    if(arv==NULL || raiz==NULL) return -1;
    
    for(int i=0; i<raiz->n; i++){
        if(raiz->vet[i][0] == indice) return raiz->vet[i][1];

        /*Caso indice seja maior que o elemento vet[i][0] faz chamada recursiva passando nó filho
        da posição*/
        if(indice < raiz->vet[i][0]){
            return procuraBTree(arv, indice, raiz->ponteiros[i]);
        }
    }

    /*Caso elemento seja maior que todos presente no nó raiz faz chamada recursiva passando último
    ponteiro do nó raiz*/
    return procuraBTree(arv, indice, raiz->ponteiros[raiz->n]);
}


//Função que remove um elemento da B Tree de acordo com o indice passado
//Retornos:
//1: Elemento removido
//0: Elemento não está na árvore
int removeBTree(bTree *arv, int indice){
    if(arv == NULL || arv->sentinela->ponteiros[0] == NULL) return 0;

    int pos = -1, i;
    no *atual = arv->sentinela->ponteiros[0];


    //Procura indice na B Tree
    do {

        for(i=0; i<atual->n; i++){
            if(indice == atual->vet[i][0]) {
                //pos armazena posição do elemento no nó
                pos = i;
                break;
            }

            if(indice < atual->vet[i][0]){
                //Caso não ache no vetor, nó atual recebe um de seus filhos
                atual = atual->ponteiros[i];
                i = -1;
                break;
            }
        }

        //Caso indice seja maior que todos elementos do vetor, atual recebe último ponteiro do nó
        if(atual != NULL && i == atual->n) atual = atual->ponteiros[i];
    } while (pos == -1 && atual != NULL);
    
    //Caso não tenha encontrado indice na árvore
    if(atual == NULL) return 0;
    
    if(atual->folha){ //Caso indice a ser removido esteja em um nó folha
        
        //Copia todos os valores uma posição para frente
        for(i=pos; i<atual->n-1; i++){
            atual->vet[i][0] = atual->vet[i+1][0];
            atual->vet[i][1] = atual->vet[i+1][1];
        }
        atual->n--;

        //Verifica se houve desbalanceamento
        if(atual->n < (arv->ordem-1)/2 && arv->sentinela->ponteiros[0] != atual){
            balanceamentoRemocao(arv, atual);
        } else if(atual->n == 0){ //Significa que não existe mais elementos na árvore
            //Libera estruturas do nó atual
            for(int i=0; i<arv->ordem; i++){
                free(atual->vet[i]);
            }

            free(atual->vet);
            for(int i=0; i<=arv->ordem; i++){
                atual->ponteiros[i] = NULL;
            }
            free(atual->ponteiros);
            free(atual);

            arv->sentinela->ponteiros[0] = NULL;

            arv->quantidadeNos--;
        }
    } else { //Caso indice esteja em um nó interna da árvore

        //Procura sucessor do valor
        no *filho = atual->ponteiros[pos+1];
        while(filho->ponteiros[0] != NULL) filho = filho->ponteiros[0];

        //Copia valor do sucessor para lugar do indice removido
        atual->vet[pos][0] = filho->vet[0][0];
        atual->vet[pos][1] = filho->vet[0][1];

        //Copia valores uma posição para no nó filho
        for(i=0; i<filho->n-1; i++){
            filho->vet[i][0] = filho->vet[i+1][0];
            filho->vet[i][1] = filho->vet[i+1][1];
        }
        filho->n--;

        //Verifica se houve desbalanceamento no nó filho
        if(filho->n < (arv->ordem-1)/2){
            balanceamentoRemocao(arv, filho);
        }
    }

    return 1;
}



//Função para balancear árvore na remoção
void balanceamentoRemocao(bTree *arv, no *atual){

    int esq = 0, dir = 0;
    no *irmao;
    no *pai = atual->pai;

    //Verifica se é possível fazer rotação com irmão
    if(atual->posPai > 0 && pai->ponteiros[atual->posPai-1] != NULL &&
    pai->ponteiros[atual->posPai-1]->n > (arv->ordem-1)/2){ //Indica que é possível fazer rotação esq
        esq = 1;

        //irmao recebe irmão da esquerda do nó
        irmao = pai->ponteiros[atual->posPai-1];
    } else if(pai->n >= atual->posPai+1 &&
    pai->ponteiros[atual->posPai+1]->n > (arv->ordem-1)/2){ //Indica que é possível fazer rotação dir
        dir = 1;

        //irmao recebe irmão da direita do nó
        irmao = pai->ponteiros[atual->posPai+1];
    }

    if(esq) { //Rotação com irmão da esquerda

        //Copia valores e ponteiros do nó atual uma posição para trás
        for(int i=atual->n; i>0; i--){
            atual->vet[i][0] = atual->vet[i-1][0];
            atual->vet[i][1] = atual->vet[i-1][1];
            atual->ponteiros[i+1] = atual->ponteiros[i];
            if(atual->ponteiros[i+1] != NULL) atual->ponteiros[i+1]->posPai++;
        }
        //Copia ponteiro da primeira posição do nó uma posição para trás
        atual->ponteiros[1] = atual->ponteiros[0];
        if(atual->ponteiros[1] != NULL) atual->ponteiros[1]->posPai++;

        //Copia valor do nó pai para a primeira posição do nó atual
        atual->vet[0][0] = pai->vet[atual->posPai-1][0];
        atual->vet[0][1] = pai->vet[atual->posPai-1][1];

        //Copia valor da última posição do irmão para a posição do pai que foi copiada para nó atual
        pai->vet[atual->posPai-1][0] =  irmao->vet[irmao->n-1][0];
        pai->vet[atual->posPai-1][1] =  irmao->vet[irmao->n-1][1];

        //Muda pai do nó que estava na última posição do vetor do irmão
        atual->ponteiros[0] = irmao->ponteiros[irmao->n];
        if(atual->ponteiros[0] != NULL) {
            atual->ponteiros[0]->pai = atual;
            atual->ponteiros[0]->posPai = 0;
        }
        atual->n++;
        irmao->n--;
    } 
    
    else if(dir) { //Rotação com irmão da direita

        //Copia valor do pai para última posição do vetor do nó atual
        atual->vet[atual->n][0] = pai->vet[atual->posPai][0];
        atual->vet[atual->n][1] = pai->vet[atual->posPai][1];

        //Copia valor da primeira posição do irmão para o pai
        pai->vet[atual->posPai][0] =  irmao->vet[0][0];
        pai->vet[atual->posPai][1] =  irmao->vet[0][1];

        //Copia primeiro ponteiro do irmão para última posição do nó atual
        atual->ponteiros[atual->n+1] = irmao->ponteiros[0];
        if(atual->ponteiros[atual->n+1] != NULL) {
            atual->ponteiros[atual->n+1]->pai = atual;
            atual->ponteiros[atual->n+1]->posPai = atual->n+1;
        }

        //Move elementos do nó irmao uma posição para frente
        for(int i=0; i<irmao->n; i++){
            irmao->vet[i][0] = irmao->vet[i+1][0];
            irmao->vet[i][1] = irmao->vet[i+1][1];
            irmao->ponteiros[i] = irmao->ponteiros[i+1];
            if(irmao->ponteiros[i] != NULL) irmao->ponteiros[i]->posPai--;
        }

        //Move último ponteiro do nó irmao uma posição para frente
        irmao->ponteiros[irmao->n-1] = irmao->ponteiros[irmao->n];
        if(irmao->ponteiros[irmao->n-1] != NULL) irmao->ponteiros[irmao->n-1]->posPai--;

        atual->n++;
        irmao->n--;
    }

    else { //Operação de Merge

        if(atual->posPai < pai->n) { //Merge com irmão da direita

            //irmao recebe irmao da direita do nó atual
            irmao = pai->ponteiros[atual->posPai+1];

            //Copia valor do pai para última posição do nó atual
            atual->vet[atual->n][0] = pai->vet[atual->posPai][0];
            atual->vet[atual->n][1] = pai->vet[atual->posPai][1];

            //Copia valores do pai uma posição para frente a partir da posição que foi para nó atual
            for(int i=atual->posPai+1; i<pai->n; i++){
                pai->vet[i-1][0] = pai->vet[i][0];
                pai->vet[i-1][1] = pai->vet[i][1];
                pai->ponteiros[i] = pai->ponteiros[i+1];
                if(pai->ponteiros[i] != NULL) {
                    pai->ponteiros[i]->posPai--;
                }
            }

            //Copia valores do irmão para o nó atual
            int j = atual->n+1;
            for(int i=0; i<irmao->n; i++){
                atual->vet[j][0] = irmao->vet[i][0];
                atual->vet[j][1] = irmao->vet[i][1];
                atual->ponteiros[j] = irmao->ponteiros[i];
                if(atual->ponteiros[j] != NULL) {
                    atual->ponteiros[j]->pai = atual;
                    atual->ponteiros[j]->posPai = j;
                }
                j++;
            }

            //Copia último ponteiro do irmão para o nó atual
            atual->ponteiros[j] = irmao->ponteiros[irmao->n];
            if(atual->ponteiros[j] != NULL) {
                atual->ponteiros[j]->pai = atual;
                atual->ponteiros[j]->posPai = j;
            }

            //Atual recebeu todos os nós de seu irmão e um de seu pai
            atual->n += irmao->n + 1;

            //Libera estruturas do irmão
            for(int i=0; i<arv->ordem; i++){
                free(irmao->vet[i]);
            }
            free(irmao->vet);

            for(int i=0; i<=arv->ordem; i++){
                irmao->ponteiros[i] = NULL;
            }
            free(irmao->ponteiros);
            free(irmao);

            arv->quantidadeNos--;

            //Indica qual nó foi removido o irmão ou o nó atual
            esq = 1;
        }
        
        else { //Merge com irmão da esquerda

            //irmao recebe irmao da esquerda do nó atual
            irmao = pai->ponteiros[atual->posPai-1];

            //Copia valor do pai para última posição do nó irmão
            irmao->vet[irmao->n][0] = pai->vet[atual->posPai-1][0];
            irmao->vet[irmao->n][1] = pai->vet[atual->posPai-1][1];

            //Copia todos os elementos do nó atual para final do nó irmão
            int j = irmao->n+1;
            for(int i=0; i<atual->n; i++){
                irmao->vet[j][0] = atual->vet[i][0];
                irmao->vet[j][1] = atual->vet[i][1];
                irmao->ponteiros[j] = atual->ponteiros[i];
                if(irmao->ponteiros[j] != NULL) {
                    irmao->ponteiros[j]->pai = irmao;
                    irmao->ponteiros[j]->posPai = j;
                }
                j++;
            }

            //Copia último ponteiro do nó atual para o irmão
            irmao->ponteiros[j] = atual->ponteiros[atual->n];
            if(irmao->ponteiros[j] != NULL) {
                irmao->ponteiros[j]->pai = irmao;
                irmao->ponteiros[j]->posPai = j;
            }

            //irmao recebeu todos os nós de atual e um de seu pai
            irmao->n += atual->n + 1;
            
            //Libera estruturas do nó atual
            for(int i=0; i<arv->ordem; i++){
                free(atual->vet[i]);
            }

            free(atual->vet);

            for(int i=0; i<=arv->ordem; i++){
                atual->ponteiros[i] = NULL;
            }
            free(atual->ponteiros);
            free(atual);

            arv->quantidadeNos--;
        }

        //Na operação de Merge o pai sempre perde um elemento
        pai->n--;

        //Verifica se desbalanceou o pai
        if(pai->n < (arv->ordem-1)/2 && arv->sentinela->ponteiros[0] != pai) {
            balanceamentoRemocao(arv, pai);
        } else if(pai->n == 0) {
            //Caso a raiz da árvore tenha ficado com 0 elementos é necessário fazer atualização

            //Verifica qual nó deve se tornar a nova raiz
            if(esq) {
                arv->sentinela->ponteiros[0] = atual;
                atual->pai = arv->sentinela;
            }
            else {
                arv->sentinela->ponteiros[0] = irmao;
                irmao->pai = arv->sentinela;
            }


            //Libera estruturas da antiga raiz
            for(int i=0; i<arv->ordem; i++){
                free(pai->vet[i]);
            }

            free(pai->vet);

            for(int i=0; i<=arv->ordem; i++){
                pai->ponteiros[i] = NULL;
            }
            free(pai->ponteiros);
            free(pai);

            arv->quantidadeNos--;
        }

    }
}


//Função que retorna raiz da árvore
no *retornaRaiz(bTree *arv){
    return arv->sentinela->ponteiros[0];
}


//Função que imprime BTree
void imprimeBTree(no *raiz){

    //Condição de parada da função recursiva
    if(raiz == NULL) return;

    //Imprime indices do nó
    for(int i = 0; i<raiz->n; i++){
        printf("%d ", raiz->vet[i][0]);
    }

    //Imprime quantidade de valores e se nó é folha
    printf("n: %d ", raiz->n);
    if(raiz->folha){
        printf("Folha");
    }
    printf("\n");
    
    //Faz chamada recursiva para cada filho da raiz
    for(int i = 0; i<=raiz->n; i++){
        imprimeBTree(raiz->ponteiros[i]);
    }
}

//Função que retorna número de nós da árvore
int retornaNroNos(bTree *arv){
    return arv->quantidadeNos;
}

//Lê dados de arquivo externo e insere na B Tree
int processaArquivo(bTree *arv, char *nomeArquivo){
    FILE *arq;
    arq = fopen(nomeArquivo, "r");

    if(arq == NULL) return 0;

    int indice, linha = 1;
    char buffer[100];
    while(!feof(arq)){
        //Lê indice a ser inserido na árvore
        fscanf(arq, "%d", &indice);

        //Insere indice e linha que os dados estão no arquivo na B Tree
        insereBTree(arv, indice, linha);

        //Pula para próxima linha no arquivo
        fgets(buffer, 100, arq);

        //Aumenta o número da linha no arquivo
        linha++;
    }

    return 1;
}


//Função que apaga toda a árvore passado como parâmetro
void excluiBTree(bTree *arv, no *raiz){

    //Libera sentinela
    if(arv->sentinela != NULL) {
        arv->sentinela->ponteiros[0] = NULL;
        free(arv->sentinela->ponteiros);
        free(arv->sentinela);
        arv->sentinela = NULL;
    }

    //Condição de parada da função recursiva
    if(raiz == NULL) return;

    //Se raiz não for folha libera cada um de seus filhos antes
    if(!raiz->folha){
        for(int i=0; i<=raiz->n; i++){
            excluiBTree(arv, raiz->ponteiros[i]);
        }
    }

    //Libera estruturas da raiz
    for(int i=0; i<arv->ordem; i++){
        free(raiz->vet[i]);
    }
    free(raiz->vet);
    
    for(int i=0; i<=arv->ordem; i++){
        raiz->ponteiros[i] = NULL;
    }
    free(raiz->ponteiros);
    free(raiz);

}