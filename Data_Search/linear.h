#ifndef LINEAR_H_INCLUDED
#define LINEAR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

typedef struct
{
    char nome[256];
    double cpf;

}Candidado;

const char* getfield(char* line, int num);

#endif // LINEAR_H_INCLUDED
