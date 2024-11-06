#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

int ListCtor(List * lst, size_t capacity)
{
    lst->elements =     (int*) calloc(capacity * 2, sizeof(int));
    if (!lst->elements)                    return ALLOCATE_ERROR;

    lst->next =         (int*) calloc(capacity * 2, sizeof(int));
    if (!lst->next)                        return ALLOCATE_ERROR;

    lst->prev =         (int*) calloc(capacity * 2, sizeof(int));
    if (!lst->prev)                        return ALLOCATE_ERROR;

    lst->cap = capacity;

    lst->next[0] = 1;
    lst->next[capacity - 1] = 0;
    for (size_t i = 1; i < capacity * 2; i++)
        lst->elements[i] = PZN;

    for (size_t i = 1; i < capacity - 1; i++)
        lst->next[i] = i + 1;

    for (size_t i = capacity; i < capacity * 2; i++)
        lst->next[i] = i + 1;

    lst->prev[0] = (int) (capacity - 1);
    lst->prev[1] = 0;
    for (size_t i = 2; i < capacity; i++)
        lst->prev[i] = i - 1;

    lst->free = 1;
    lst->size = 1;

    return SUCCESS;

}

int ListInsert(List * lst, size_t index, int num)
{
    if ((lst->cap - lst->size) <= 0) return fprintf(stderr, "List Overflow! Insert Unavailable.");

    int new_free = lst->next[lst->free];
    int old_next = lst->next[index];

    if (lst->next[index] == -1 && lst->elements[index] != PZN)
    {
        lst->elements[lst->free] = num;
        lst->next[index] = lst->free;

        lst->next[lst->free] = -1;
        lst->prev[lst->free] = index;
        lst->free = new_free;
        lst->size++;
        return SUCCESS;
    }

    if (lst->elements[old_next] && lst->elements[old_next] != PZN)
    {
        lst->elements[lst->free] = num;
        lst->next[index] = lst->free;

        lst->prev[lst->free] = index;

        lst->next[lst->free] = old_next;
        lst->prev[old_next] = lst->free;
        if (lst->next[old_next] == lst->prev[old_next]) lst->next[old_next] = new_free;
        lst->free = new_free;
        lst->size++;

        return SUCCESS;
    }

    lst->elements[lst->free] = num;
    lst->next[index] = lst->free;
    lst->prev[lst->free] = index;
    lst->next[lst->free] = -1;
    lst->free = new_free;
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

    lst->elements[index] = -111;
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
    printf("------------------------------------------------------------\n\n");

    return SUCCESS;
}

int * ListGetElem(List * lst, size_t index)
{
    return &(lst->elements[index]);
}

int ListSetElem(List * lst, size_t index, int val)
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
            if (i == lst->size - 1)
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
        if (lst->next[i] != -1 && lst->prev[i] != -1 && lst->elements[i] != PZN)
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
    if (lst->elements)  free(lst->elements);
    if (lst->next)      free(lst->next);
    if (lst->prev)      free(lst->prev);
    return SUCCESS;
}

