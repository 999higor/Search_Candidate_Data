#include "linear.h"


int contador = 0;

int CandidatoCMP(void *a, void *b)
{
    contador++;
    Candidato* ca = (Candidato* )a;
    Candidato* cb = (Candidato* )b;

    return strncmp(ca->cpf, cb->cpf, TAMANHO);
}

Candidato *newCandidato(const char *cpf, const char *name)
{
    Candidato *r = (Candidato*)malloc(sizeof(Candidato));
    strncpy(r->cpf, cpf, TAMANHO);
    strncpy(r->nome, name, TAMANHO);

    return r;
}

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
