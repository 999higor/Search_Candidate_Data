#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>

#include "linear.h"
#include "BTree.h"


int main()
{
    setlocale(LC_ALL, "portuguese");

    bool cpfTest = false;
    int sizeStruct = 0;
    int i=0;
    char percorre[1024];

    FILE* stream;

    Candidato *candidato = NULL;

    bt_Tree *bt = bt_newTree(CandidatoCMP, free);

    int option;
    int option2;
    char line[1024];
    char cpfChar[1024];
    char nomeBTree[256];
    char CPFsearch[256];
    double tempo=0;

    clock_t t_inicial, t_final;

    do{
            printf("[ - - - - - - - - - - Sistema de Candidados -- Inserção e Busca de Dados - - - - - - - - - ]\n");
            printf("1 - Vetor\n");
            printf("2 - Árvore B\n");
            printf("0 - Sair\n> ");
            scanf("%d",&option);

            switch(option)
            {
                case 1:

                    do{
                        printf("[ - - - - - - - - - - Sistema de Candidados -- Vetor - - - - - - - - - ]\n");
                        printf("1 - INSERT - Insere Dados no Vetor\n");
                        printf("2 - SEARCH - Realizar Busca no Vetor\n");
                        printf("3 - DESTROY - Desalocar a Memória do Vetor\n");
                        printf("0 - Sair\n> ");
                        scanf("%d",&option2);

                        switch(option2)
                        {
                            case 1:

                                stream = fopen("consulta.csv", "r");
                                while(fgets(percorre, 1024, stream) !=  NULL)
                                {
                                    sizeStruct++;
                                }

                                if(candidato != NULL)
                                {
                                    free(candidato);
                                }
                                candidato = malloc(sizeStruct*sizeof(Candidato));
                                fclose(stream);


                                t_inicial = clock();

                                stream = fopen("consulta.csv", "r");

                                fgets(line, 1024, stream); ///flag

                                i = 0;

                                while (!feof(stream) && fgets(line, 1024, stream))
                                {

                                    char* tmp = strdup(line);
                                    char* tmp2 = strdup(line);

                                    strcpy(candidato[i].nome, getfield(tmp, 18));
                                    strcpy(candidato[i].cpf, getfield(tmp2 ,21));

                                    i++;
                                    free(tmp);
                                    free(tmp2);
                                }
                                fclose(stream);
                                t_final = clock();
                                tempo = ((double) (t_final - t_inicial)) /CLOCKS_PER_SEC;
                                printf("Time: %lf seg\n",tempo);

                                break;


                            case 2:

                                printf("\nDigite o CPF: ");
                                scanf("%s" ,CPFsearch);
                                cpfTest = false;

                                for(i = 0; i < sizeStruct; i++)
                                {
                                    if(strcmp(CPFsearch, candidato[i].cpf) == 0)
                                    {
                                        printf("CPF Encontrado !\n");
                                        printf("Nome : %s\n",candidato[i].nome);
                                        printf("CPF : %s\n",candidato[i].cpf);
                                        printf("Comparações: %d\n",i+1);
                                        cpfTest = true;
                                        break;
                                    }

                                }
                                //i = 0;

                                if(cpfTest == false)
                                {
                                    printf("CPF Não Encontrado !\n");
                                }

                                break;

                            case 3:
                                free(candidato);
                                candidato = NULL;
                                break;

                            case 4:

                                if(candidato == NULL)
                                {
                                    printf("Vetor Vazio !\n");
                                }else
                                {
                                    for(i = 0; i < sizeStruct; i++)
                                    {
                                        printf("Nome : %s\n",candidato[i].nome);
                                        printf("CPF  : %s\n",candidato[i].cpf);
                                    }
                                }
                                printf("Comparações: %d\n",i);
                                break;

                            case 0:
                                break;

                            default:
                                printf("Opção Inválida !\n");
                                break;
                        }
                    }while(option2 != 0);
                    break;



                case 2:

                    do{
                        printf("[ - - - - - - - - - - Sistema de Candidados -- B TREE - - - - - - - - - ]\n");
                        printf("1 - INSERT - Insere Dados na Árvore\n");
                        printf("2 - SEARCH - Realizar Busca na Árvore\n");
                        printf("3 - DESTROY - Desalocar a Memória da Árvore\n");
                        printf("0 - Sair\n> ");
                        scanf("%d",&option2);

                        switch(option2)
                        {
                            case 1:

                                t_inicial = clock();

                                stream = fopen("consulta.csv", "r");

                                fgets(line, 1024, stream); ///flag

                                while (!feof(stream) && fgets(line, 1024, stream))
                                {

                                    char* tmp = strdup(line);
                                    char* tmp2 = strdup(line);

                                    strcpy(nomeBTree, getfield(tmp, 18));
                                    strcpy(cpfChar, getfield(tmp2 ,21));

                                     bt_insert(bt, newCandidato(cpfChar, nomeBTree));

                                    //i++;
                                    free(tmp);
                                    free(tmp2);
                                }
                                fclose(stream);
                                t_final = clock();
                                tempo = ((double) (t_final - t_inicial)) /CLOCKS_PER_SEC;
                                printf("Time: %lf seg\n",tempo);

                                break;

                            case 2:
                                printf("\nDigite o CPF: ");
                                scanf("%s",CPFsearch);

                                t_inicial = clock();

                                Candidato c;
                                strcpy(c.cpf, CPFsearch);

                                contador = 0;
                                Candidato* result = bt_search(bt, &c);
                                if(result == NULL)
                                {
                                    printf("deu ruim\n");
                                }else
                                {
                                    printf("Nome : %s\n" ,result->nome);
                                    printf("CPF : %s\n" ,result->cpf);
                                    printf("Comparações : %d\n",contador);
                                }

                                t_final = clock();
                                tempo = ((double) (t_final - t_inicial)) /CLOCKS_PER_SEC;
                                printf("Time: %lf seg\n",tempo);
                                break;

                            case 3:

                                bt_free(bt);
                                printf("Árvore Deletada com Sucesso !\n");
                                break;

                            case 4:

                                break;

                            case 0:
                                break;

                            default:
                                printf("Opção Inválida !\n");
                                break;

                        }


                    }while(option2 !=0);
                    break;

                 case 0:
                    break;

                default:
                    printf("Opção Inválida !\n");
                    break;
            }



    }while(option !=0);


}
