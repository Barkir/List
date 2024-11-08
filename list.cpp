#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "list.h"

int ListCtor(List * lst, size_t capacity)
{
    lst->cap = capacity + 1;

    lst->elements =     (int*) calloc(lst->cap, sizeof(int));
    if (!lst->elements)                    return ALLOCATE_ERROR;

    lst->next =         (int*) calloc(lst->cap, sizeof(int));
    if (!lst->next)                        return ALLOCATE_ERROR;

    lst->prev =         (int*) calloc(lst->cap, sizeof(int));
    if (!lst->prev)                        return ALLOCATE_ERROR;


    for (size_t i = 1; i < capacity; i++)
        lst->elements[i] = PZN;

    for (size_t i = 1; i < capacity; i++)
        lst->next[i] = i + 1;

    for (size_t i = 1; i < capacity; i++)
        lst->prev[i] = i - 1;


    lst->free = 1;
    lst->size = 0;

    lst->next[0] = 0;
    lst->prev[0] = 0;

    return SUCCESS;

}

int ListRealloc(List * lst)
{
    // printf("REALLOC\n");

    int * check1 = (int*) realloc(lst->elements, 2 * lst->cap * sizeof(int));
    if (!check1) return ALLOCATE_ERROR;
    lst->elements = check1;

    for (size_t i = lst->cap - 1; i < lst->cap * 2; i++)
        lst->elements[i] = PZN;

    int * check2 = (int*) realloc(lst->next, 2 * lst->cap * sizeof(int));
    if (!check2) return ALLOCATE_ERROR;
    lst->next = check2;

    for (size_t i = lst->cap - 1; i < lst->cap * 2; i++)
        lst->next[i] = i + 1;

    int * check3 = (int*) realloc(lst->prev, 2 * lst->cap * sizeof(int));
    if (!check3) return ALLOCATE_ERROR;
    lst->prev = check3;

    for (size_t i = lst->cap - 1; i < lst->cap * 2; i++)
        lst->prev[i] = i - 1;

    lst->cap *= 2;

    return SUCCESS;
}

int ListInsert(List * lst, size_t index, int num)
{
    int check = 0;
    if (index > lst->size) return -fprintf(stderr, "Wrong Index!\n");
    if (lst->size >= lst->cap - 2) check = ListRealloc(lst);
    if (check != SUCCESS) return -fprintf(stderr, "Error in realloc!");

    int next_free = lst->next[lst->free];

    lst->elements[lst->free] = num;

    int old_next = lst->next[index];

    lst->next[index] = lst->free;
    lst->prev[lst->free] = index;

    lst->next[lst->free] = old_next;
    lst->prev[old_next] = lst->free;

    lst->free = next_free;
    lst->size++;

    return SUCCESS;
}

int ListDelete(List * lst, size_t index)
{
    int old_free = lst->free;
    lst->free = index;

    int prev_index = lst->prev[index];
    int next_index = lst->next[index];

    lst->next[prev_index] = next_index;
    lst->prev[next_index] = prev_index;

    lst->next[lst->free] = old_free;
    lst->prev[old_free] = lst->free;

    lst->elements[index] = PZN;
    lst->size--;

    return SUCCESS;

}

int ListPrint(List * lst, size_t num)
{
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < num; i++)
        printf("%3d ", lst->elements[i]);

    printf("\n");

    for (int i = 0; i < num; i++)
        printf("%3d ", lst->next[i]);

    printf("\n");

    for (int i = 0; i < num; i++)
        printf("%3d ", lst->prev[i]);

    printf("\n\n");

    printf("    lst.free = %lu\n", lst->free);
    printf("    lst.size = %lu\n", lst->size);
    printf("    lst.cap = %lu\n", lst->cap);
    printf("------------------------------------------------------------\n\n");

    return SUCCESS;
}

int * ListGetElem(List * lst, size_t index) // GetElemAdr
{
    return &(lst->elements[index]);
}

int ListSetElem(List * lst, size_t index, int val) // --
{
    lst->elements[index] = val;
    return SUCCESS;
}

int ListGetNext(List * lst, size_t index)
{
    return lst->next[index];
}

int ListGetPrev(List * lst, size_t index)
{
    return lst->prev[index];
}


int ListDump(List * lst, const char * OutName)
{
    FILE * Out = fopen(OutName, "wb");
    FILE * Out = fopen()

    char * OutBuf = (char*) calloc(OUTBUF_SIZE, sizeof(char));
    char * ptr = OutBuf;

    char * command = (char*) calloc(100, sizeof(char));
    sprintf(ptr, "digraph\n{\n");
    while (*ptr) ptr++;

    for (int i = 0; i < lst->cap; i++)
    {
        if (lst->elements[i] != PZN)
        {
            sprintf(ptr, "node%d [shape = Mrecord; label = \"{%d | next = %d | prev = %d}\"]\n", i, lst->elements[i], lst->next[i], lst->prev[i]);
            while (*ptr) ptr++;
        }
    }

    for (int i = 0; i < lst->cap; i++)
    {
        if (lst->elements[i] != PZN)
        {
            if (i == lst->size)
            {
                sprintf(ptr, "node%d [weight = 1000; color = white]\n", i);
                while (*ptr) ptr++;
            }
            else
            {
                sprintf(ptr, "node%d -> ", i);
                while (*ptr) ptr++;
            }
        }
    }

    for (int i = 0; i < lst->cap; i++)
    {
        if (lst->elements[i] != PZN)
        {
            sprintf(ptr, "node%d -> node%d [color = red]\nnode%d -> node%d [color = blue]\n", i, lst->next[i], i, lst->prev[i]);
            while (*ptr) ptr++;
        }
    }

    sprintf(ptr, "\n}\n");
    while (*ptr) ptr++;

    printf("%s", OutBuf);

    fwrite(OutBuf, strlen(OutBuf), sizeof(char), Out);
    sprintf(command, "dot %s -T png -o %s.png", OutName, OutName);

    fclose(Out);
    system(command);

    free(OutBuf);
    free(command);

    return SUCCESS;
}

int ListDtor(List * lst)
{
    free(lst->elements);
    free(lst->next);
    free(lst->prev);

    return SUCCESS;
}

// GetHead, GetTail

