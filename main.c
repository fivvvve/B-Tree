#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bTree.h"
#include <time.h>

int main(){

    int indice, indiceBusca, opt, linha, ret, ordem, begin, end;
    float time_spent;
    char nomeArquivo[50] = {"dados.txt"}, result1[11], result2[11], result3[11];
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

            FILE *arq;
            arq = fopen(nomeArquivo, "r");

            if(arq == NULL) {
                printf("O arquivo não está disponível para consulta.");
                break;
            }

            printf("Digite o elemento que deseja procurar: ");
            scanf("%d", &indice);

            //Buca na árvore
            printf("\nBusca na árvore:\n");
            begin = clock();
            linha = procuraBTree(arv, indice, retornaRaiz(arv));
            
            if(linha != -1) {
                printf("Elemento está na linha: %d\n", linha);
                fseek(arq, 36*(linha-1)+6, 0);
                fscanf(arq, " %[^\t] %[^\t] %[^\t]", result1, result2, result3);
                printf("Valores encontrados: %s, %s, %s\n", result1, result2, result3);
            }
            else printf("Elemento não encontrado na árvore\n");

            end = clock();
            time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
            printf("Tempo: %.10f segundos\n", time_spent);

            //Busca no arquivo
            printf("\nBusca no arquivo:\n");
            rewind(arq);
            begin = clock();
            
            while(!feof(arq)){
                fscanf(arq, " %d", &indiceBusca);
                if(indiceBusca == indice) {
                    fscanf(arq, " %[^\t] %[^\t] %[^\t]", result1, result2, result3);
                    printf("Valores encontrados: %s, %s, %s\n", result1, result2, result3);
                    break;
                } else {
                    fseek(arq, 30, 1);
                }
                fgetc(arq);
            }
            if (feof(arq)) printf("Elemento não encontrado no arquivo\n");

            end = clock();
            time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
            printf("Tempo: %.10f segundos\n\n", time_spent);
            fclose(arq);
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