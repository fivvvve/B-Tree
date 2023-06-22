#include <stdio.h>
#include <stdlib.h>
#include "bTree.h"

int main(){

    int indice, opt, linha, ret, ordem;
    char nomeArquivo[50];
    bTree *arv = NULL;
    
    do {
        printf("1. Criar Índice\n");
        printf("2. Procurar elementos\n");
        printf("3. Remover Registro\n");
        printf("4. Sair\n");
        printf("O que deseja fazer: ");
        scanf("%d", &opt);

        switch (opt) {
        case 1:
            if(arv != NULL){
                excluiBTree(arv, retornaRaiz(arv));
                free(arv);
            }

            arv = NULL;
            system("clear || cls");

            //Leitura da ordem desejada pelo usuário
            printf("Informe a ordem desejada da árvore (max 100): ");
            scanf("%d", &ordem);
            while (ordem < 3 || ordem > 100){
                printf("Valor inválido.\n");
                printf("Informe a ordem desejada da árvore (max 10): ");
                scanf("%d", &ordem);
            }

            //Criar B Tree
            arv = criaBTree(ordem);

            printf("Digite o nome do arquivo: ");
            scanf(" %s", nomeArquivo);
            ret = processaArquivo(arv, nomeArquivo);
            if(ret){
                printf("\nÁrvore:\n");
                imprimeBTree(retornaRaiz(arv));
                printf("Quantidade de nós: %d\n\n", retornaNroNos(arv));
            } else {
                printf("Erro ao ler do arquivo de entrada.\n\n");
            }

            break;
        
        case 2:
            system("clear || cls");

            if(arv == NULL){
                printf("A árvore não foi criada\n\n");
                break;
            }

            printf("Digite o elemento que deseja procurar: ");
            scanf("%d", &indice);
            linha = procuraBTree(arv, indice, retornaRaiz(arv));
            if(linha != -1) printf("Elemento está na linha: %d\n\n", linha);
            else printf("Elemento não encontrado na árvore\n\n");
            break;

        case 3:
            system("clear || cls");

            if(arv == NULL){
                printf("A árvore não foi criada\n\n");
                break;
            }

            printf("Digite o índice que deseja remover: ");
            scanf("%d", &indice);
            ret = removeBTree(arv, indice);
            if(ret) printf("Elemento Removido\n");
            else printf("Falha ao remover elemento\n");

            printf("\nÁrvore:\n");
                imprimeBTree(retornaRaiz(arv));
                printf("Quantidade de nós: %d\n\n", retornaNroNos(arv));

            break;

        case 4:
            system("clear || cls");
            printf("Saindo...\n\n");
            break;

        default:
            system("clear || cls");
            printf("Valor inválido\n\n");
            break;
        }
    } while (opt != 4);

    if(arv == NULL){
        return 0;
    }
    excluiBTree(arv, retornaRaiz(arv));
    free(arv);
    return 0;
}