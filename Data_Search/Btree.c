#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "BTree.h"


bTree bTree_Create(void)
{
    bTree b;

    b = malloc(sizeof(*b));
    assert(b);

    b->isLeaf = 1;
    b->numKeys = 0;

    return b;
}

void bTree_Destroy(bTree b)
{
    int i;

    if(!b->isLeaf)
    {
        for(i = 0; i < b->numKeys + 1; i++)
        {
            bTree_Destroy(b->kids[i]);
        }
    }

    free(b);
}

///retorna o menor indice i no array
 int searchKey(int n, data *a, double key)
{
    int lo;
    int hi;
    int mid;


    lo = -1;
    hi = n;

    while(lo + 1 < hi)
    {
        mid = (lo+hi)/2;
        if(a[mid].key == key)
        {
            return mid;
        } else if(a[mid].key < key)
        {
            lo = mid;
        } else
        {
            hi = mid;
        }
    }

    return hi;
}

int bTree_Search(bTree b, double key)
{
    int pos;
    int interacao;

    if(b->numKeys == 0) ///verifica se a arvore esta vazia///
    {
        return 0;
    }

    ///procura a menor posicao que a key pode entrar///
    pos = searchKey(b->numKeys, b->keys, key);

    if(pos < b->numKeys && b->keys[pos].key == key)
    {
        printf("Nome : %s\n" ,b->keys[pos].nome);
        printf("CPF : %0.lf\n" ,b->keys[pos].key);
        return 1;
    } else
    {
        return(!b->isLeaf && bTree_Search(b->kids[pos], key));
    }

}

///insere na arvore
///retorna o novo nó direito se ele for dividido
///senao retorna 0
bTree bTree_Insert_Internal(bTree b, double key, int *median, char* nome)
{
    int pos;
    int mid;
    bTree b2;


   // printf("funcao internal : %s\n",nome);

    pos = searchKey(b->numKeys, b->keys, key);

    if(pos < b->numKeys && b->keys[pos].key == key) ///se cair aqui, nao ha nada a se fazer///
    {
        return 0;
    }

    if(b->isLeaf)
        {

         //todos os elementos acima do POS sobem um espaco
        memmove(&b->keys[pos+1], &b->keys[pos], sizeof(*(b->keys)) * (b->numKeys - pos));
        /*for(i = 0; i < b->numKeys - pos ; i++)
        {
            b->keys[pos+1+i].key = b->keys[pos+i].key;
            strcpy(b->keys[pos+i].nome,  b->keys[pos+1+i].nome);
            //b->keys[pos+1].nome = b->keys[pos].nome;
        }*/

        //printf("aki2   %s \n",nome);

        strcpy(b->keys[pos].nome, nome);

        //printf("aki 3\n");
        b->keys[pos].key = key;
        //strcpy(b->nome, nome);
        //printf("funcao internal 2 : %s\n",b->keys[pos].nome);
        b->numKeys++;

    } else
    {

       ///insere a creanca///
        b2 = bTree_Insert_Internal(b->kids[pos], key, &mid, nome);

        ///insere uma novo elemento em b
        if(b2)
        {

            //todo elemento acima do POS sobe um espaco
            memmove(&b->keys[pos+1], &b->keys[pos], sizeof(*(b->keys)) * (b->numKeys - pos));

            /*for(i = 0; i < b->numKeys - pos ; i++)
            {
                b->keys[pos+1+i].key = b->keys[pos+i].key;
                 strcpy(b->keys[pos+i].nome,  b->keys[pos+1+i].nome);
                //b->keys[pos+1].nome = b->keys[pos].nome;
            }*/
            //a nova crianca entra na POS + 1
            memmove(&b->kids[pos+2], &b->kids[pos+1], sizeof(*(b->keys)) * (b->numKeys - pos));

            /*for(i = 0; i< b->numKeys - (pos) ; i++)
            {
                b->keys[pos+2+i].key = b->keys[pos+1+i].key;
                 strcpy(b->keys[pos+1+i].nome,  b->keys[pos+2+i].nome);
                //b->keys[pos+2].nome = b->keys[pos+1].nome;
            }*/

            strcpy(b->keys[pos].nome, nome);
            b->keys[pos].key = mid;
            b->kids[pos+1] = b2;

            b->numKeys++;
        }
    }
    ///se atingir o valor maximo cai aqui///
    if(b->numKeys >= MAX_KEYS)
    {
        mid = b->numKeys/2;

        *median = b->keys[mid].key;

        b2 = malloc(sizeof(*b2));

        b2->numKeys = b->numKeys - mid - 1;
        b2->isLeaf = b->isLeaf;

        memmove(b2->keys, &b->keys[mid+1], sizeof(*(b->keys)) * b2->numKeys);
        /*for(i = 0; i < b2->numKeys; i++)
        {
            b2->keys[i].key = b->keys[i+mid+1].key;
            strcpy(b->keys[i].nome,  b->keys[i+mid+1].nome);
        }*/

        if(!b->isLeaf)
        {
            memmove(b2->kids, &b->kids[mid+1], sizeof(*(b->kids)) * (b2->numKeys + 1));

            /*for(i = 0; i < b2->numKeys + 1; i++)
            {
                b2->kids[i] = b->kids[i+mid+1];
            }*/
        }

        b->numKeys = mid;

        return b2;
    } else {
        return 0;
    }
}

void bTree_Insert(bTree b, double key, char* nome)
{
    bTree b1;   ///cria da esquerda///
    bTree b2;   ///cria da direita///
    int median;

    //printf("funcao insert : %s", nome);

    b2 = bTree_Insert_Internal(b, key, &median, nome);

    if(b2)
    {
      ///se for dividido, é criado um novo root

        b1 = malloc(sizeof(*b1));
        assert(b1);

        /// copia o root de b1
        memmove(b1, b, sizeof(*b));

    ///cria os ponteiros de b1 e b2
        b->numKeys = 1;
        b->isLeaf = 0;
        b->keys[0].key = median;
        strcpy(b->keys[0].nome, nome);
        b->kids[0] = b1;
        b->kids[1] = b2;
    }
}

/*
int tabs = -1;
void bTree_Print_Keys_TreeMode(bTree b)
{

    int i, j;
    if(b != NULL)
    {
        tabs++;
        for(i = 0; i < b->numKeys; i++) {
            if(!b->isLeaf) bTree_Print_Keys_TreeMode(b->kids[i]);

            for(j = 0; j < tabs; j++) printf("\t");
            printf("%lf\n", b->keys[i]->key);
        }
        if(!b->isLeaf) bTree_Print_Keys_TreeMode(b->kids[b->numKeys]);
    }
    tabs--;

}*/

void Btree_Print_Keys(bTree b)
{
    int i;
    if(b != NULL)
    {
        for(i=0; i<b->numKeys; i++)
        {
            if(b->isLeaf == 0)
            {
                Btree_Print_Keys(b->kids[i]);

            }
            //printf("Nome : %s \n", b->keys[i].nome);
            printf("CPF :%lf \n", b->keys[i].key);
            printf("\n");
            //system("pause");

        }
        if(b->isLeaf == 0)
       Btree_Print_Keys(b->kids[b->numKeys]);
    }
}

/*
int bTree_MAX (bTree b)
{
    assert(b && b->numKeys > 0);
    for(;;)
    {
        if(b->isLeaf || !b->kids[b->numKeys])
        {
        return b->keys[b->numKeys -1];
        }
        b = b->kids[b->numKeys];
    }
}

int bTree_MIN (bTree b)
{
    assert(b && b->numKeys > 0);
    for(;;)
    {
        if(b->isLeaf || !b->kids[0])
        {
            return b->keys[0];
        }
        b = b->kids[0];
    }
}

int bTree_Height (bTree b)
{
    int count = -1;

    if(b == NULL) return count;

    for(;;)
    {
        count++;
        if(b->isLeaf == 1)
        {
            return count;
        }
        b = b->kids[0];
    }
}
*/

int bTree_Count_All(bTree b)
{
    int i, count = 0;
    if(b != NULL)
    {
        if(b->isLeaf == 0) {
            for(i=0; i<b->numKeys+1; i++)
            {
                count += bTree_Count_All(b->kids[i]);
            }
        }
        count += b->numKeys;
    }

    return count;
}
/*
int bTree_Count_Leaf(bTree b)
{
    int i, count = 0;
    if(b != NULL)
    {
        if(b->isLeaf == 0) {
            for(i=0; i<b->numKeys+1; i++)
            {
                count += bTree_Count_Leaf(b->kids[i]);
            }
        }else
            count++;
    }

    return count;
}
*/

/*bTree *Btree_Read_File(bTree b)
{
    int numero;

    FILE* file = fopen ("values.txt", "r");

    //fscanf (file, "%d", &numero);

    while (!feof(file)){

        fscanf(file, "%d", &numero);

        bTree_Insert(b, numero);

        //fscanf(file, "%d", &numero);

    }
    fclose(file);
    //return b;
}*/

void read_search_txt_B(bTree raiz ,FILE* result_search_Btree)
{
    int number;

    FILE* busca = fopen("search.txt", "r");

    //fscanf(busca, "%d", &number);

    while(!feof(busca))
    {
        fscanf(busca, "%d", &number);

        fprintf(result_search_Btree,"%d \n",bTree_Search(raiz, number));
        //fscanf(busca, "%d", &number);

    }
    fclose(busca);
}
