#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>

#include "linear.h"

int main()
{
    setlocale(LC_ALL, "portuguese");

    //bool test = false;
    int sizeStruct = 0;
    int i=0;
    char percorre[1024];

    FILE* stream = fopen("consulta.csv", "r");
    while(fgets(percorre, 1024, stream) !=  NULL)
    {
        sizeStruct++;
    }

    //printf("size: %d",sizeStruct);
    //system("pause");

    Candidado *candidato = malloc(sizeStruct*sizeof(Candidado));

    fclose(stream);

    int option;
    int option2;
    char line[1024];
    char cpfChar[1024];
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
                                fgets(line, 1024, stream);

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
                                //printf("valor i %d\n",i);
                                //printf("vrau : %s  cpf:  %lf",candidato[6].nome, candidato[6].cpf);
                                fclose(stream);
                                t_final = clock();
                                tempo = ((double) (t_final - t_inicial)) /CLOCKS_PER_SEC;
                                printf("Time: %lf seg\n",tempo);
                               // getchar();

                                break;




                            case 2:
                                //double CPFsearch;
                                printf("\nDigite o CPF: ");
                                scanf("%lf",&CPFsearch);
                               // printf("%lf",CPFsearch);

                                for(i = 0; i < sizeStruct; i++)
                                {
                                    if(CPFsearch == candidato[i].cpf)
                                    {
                                        printf("CPF Encontrado !\n");
                                        printf("Nome : %s\n",candidato[i].nome);
                                        printf("CPF : %lf\n",candidato[i].cpf);
                                        break;
                                    }

                                    //printf("CPF Não Encontrado\n");


                                }

                                break;

                            case 3:
                                break;

                            case 0:
                                break;

                            default:
                                printf("Opção Inválida !\n");
                                break;
                        }

                    }while(option2);

                case 2:
                    break;
            }

    }while(option !=0);


}
