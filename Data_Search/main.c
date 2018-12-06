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

    FILE* stream = fopen("consulta.csv", "r");
    while(fgets(percorre, 1024, stream) !=  NULL)
    {
        sizeStruct++;
    }

    Candidado *candidato = malloc(sizeStruct*sizeof(Candidado));

    fclose(stream);

    bTree b;
    b = bTree_Create();

    int option;
    int option2;
    char line[1024];
    char cpfChar[1024];
    char nomeBTree[256];
    double cpfBTree;
    double CPFsearch;
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

                                t_inicial = clock();

                                stream = fopen("consulta.csv", "r");

                                fgets(line, 1024, stream); ///flag

                                while (!feof(stream) && fgets(line, 1024, stream))
                                {
                                    /*if(test != true)
                                    {
                                        fgets(line, 1024, stream);

                                        test = true;
                                        system("pause");
                                    }*/

                                    char* tmp = strdup(line);
                                    char* tmp2 = strdup(line);

                                    strcpy(candidato[i].nome, getfield(tmp, 18));
                                    strcpy(cpfChar, getfield(tmp2 ,21));

                                    //printf(">>%s\n",cpfChar);
                                    candidato[i].cpf = atof(cpfChar);

                                    //printf("nome : %s\n",candidato[i].nome);
                                    //printf("cpf : %.0f\n",candidato[i].cpf);

                                    i++;
                                    free(tmp);
                                    free(tmp2);
                                }
                                fclose(stream);
                                t_final = clock();
                                tempo = ((double) (t_final - t_inicial)) /CLOCKS_PER_SEC;
                                printf("Time: %lf seg\n",tempo);
                               // getchar();

                                break;


                            case 2:

                                printf("\nDigite o CPF: ");
                                scanf("%lf",&CPFsearch);
                                cpfTest = false;

                                for(i = 0; i < sizeStruct; i++)
                                {
                                    if(CPFsearch == candidato[i].cpf)
                                    {
                                        printf("CPF Encontrado !\n");
                                        printf("Nome : %s\n",candidato[i].nome);
                                        printf("CPF : %lf\n",candidato[i].cpf);
                                        cpfTest = true;
                                        break;
                                    }

                                }

                                if(cpfTest == false)
                                {
                                    printf("CPF Não Encontrado !\n");
                                }

                                break;

                            case 3:
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
                                        printf("CPF  : %0.lf\n",candidato[i].cpf);
                                        //system("pause");
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


                                    cpfBTree = atof(cpfChar);

                                    //printf("cpf %lf \n",cpfBTree);
                                    //printf("nome %s\n" ,nomeBTree);
                                    //system("pause");

                                    bTree_Insert(b, cpfBTree ,nomeBTree);

                                    Btree_Print_Keys(b);

                                    system("pause");
                                    system("cls");

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
                                scanf("%lf",&CPFsearch);

                                t_inicial = clock();

                                bTree_Search(b, CPFsearch);

                                t_final = clock();
                                tempo = ((double) (t_final - t_inicial)) /CLOCKS_PER_SEC;
                                printf("Time: %lf seg\n",tempo);
                                break;

                            case 3:
                                printf("Tamanho : %d\n",bTree_Count_All(b));
                                bTree_Destroy(b);
                                printf("Árvore Deletada com Sucesso !\n");
                                break;

                            case 4:
                                Btree_Print_Keys(b);
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


    return 0;

}
