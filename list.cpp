#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

int ListCtor(List * lst, size_t capacity)
{
    lst->cap = capacity;

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

int ListDelete(List * lst, size_t index, int * ret)
{
    if (lst->elements[index] == PZN) return -fprintf(stderr, "Element Already Deleted! Try Another!\n");

    int old_free = lst->free;
    lst->free = index;

    int prev_index = lst->prev[index];
    int next_index = lst->next[index];

    lst->next[prev_index] = next_index;
    lst->prev[next_index] = prev_index;

    lst->next[lst->free] = old_free;
    lst->prev[old_free] = lst->free;

    *ret = lst->elements[index];
    lst->elements[index] = PZN;
    lst->size--;

    return SUCCESS;

}

int * ListGetElem(List * lst, size_t index) // GetElemAdr
{
    return &(lst->elements[index]);
}

int ListGetNext(List * lst, size_t index)
{
    return lst->next[index];
}

int ListGetPrev(List * lst, size_t index)
{
    return lst->prev[index];
}

int * ListGetHead(List * lst)
{
    return &lst->prev[0];
}

int * ListGetTail(List * lst)
{
    return &lst->next[0];
}

int ListPushHead(List * lst, int num)
{
    ListInsert(lst, lst->prev[0], num);
    return SUCCESS;
}

int ListPushTail(List * lst, int num)
{
    ListInsert(lst, 0, num);
    return SUCCESS;
}

int ListPopTail(List * lst, int * ret)
{
    ListDelete(lst, lst->prev[0], ret);
    return SUCCESS;
}

int ListPopHead(List * lst, int * ret)
{
    ListDelete(lst, lst->next[0], ret);
    return SUCCESS;
}
size_t ListGetSize(List * lst, int num)
{
    return lst->size;
}

size_t ListGetCap(List * lst, int num)
{
    return lst->cap;
}

int ListPrint(FILE * src, List * lst, size_t num)
{
    fprintf(src, "-------------------------------------------------------\n");
    for (int i = 0; i < num; i++)
        fprintf(src,"%3d ", lst->elements[i]);

    fprintf(src, "\n");

    for (int i = 0; i < num; i++)
        fprintf(src, "%3d ", lst->next[i]);

    fprintf(src, "\n");

    for (int i = 0; i < num; i++)
        fprintf(src, "%3d ", lst->prev[i]);

    fprintf(src, "\n\n");

    fprintf(src, "    lst.free = %lu\n", lst->free);
    fprintf(src, "    lst.size = %lu\n", lst->size);
    fprintf(src, "    lst.cap = %lu\n", lst->cap);
    fprintf(src, "------------------------------------------------------------\n\n");

    return SUCCESS;
}

int ListDump(List * lst, const char * OutName)
{
    size_t size = strlen(OutName) * 2;
    char  * FileName = (char*) calloc(size, sizeof(char));

    sprintf(FileName, "%s", OutName);

    while (access(FileName, F_OK) == 0)
    {
        if (strlen(FileName) >= size - 1)
        {
            size *= 2;
            FileName = (char*) realloc(FileName, size);
        }
        sprintf(FileName, "%s_", FileName);
    }

    char * HtmName = (char*) calloc(strlen(FileName) + 10, sizeof(char));
    sprintf(HtmName, "%s.htm", FileName);

    FILE * Out = fopen(FileName, "wb");
    FILE * Htm = fopen(HtmName, "wb");

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
            if (i >= lst->size && lst->elements[i + 1] == PZN)
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
    sprintf(command, "dot %s -T png -o %s.png", FileName, FileName);

    fprintf(Htm, "<pre>\n");

    ListPrint(Htm, lst, lst->cap);
    fprintf(Htm, "<img src=%s.png width=20%%>", FileName);

    fclose(Out);
    fclose(Htm);
    system(command);

    free(OutBuf);
    free(command);
    free(FileName);
    free(HtmName);

    return SUCCESS;
}

int ListDtor(List * lst)
{
    free(lst->elements);
    free(lst->next);
    free(lst->prev);

    return SUCCESS;
}
