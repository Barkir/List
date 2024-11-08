#ifndef LIST_H
#define LIST_H

const size_t OUTBUF_SIZE = 10000;
const int PZN = -66666;

enum errors
{
    SUCCESS = 0,
    ALLOCATE_ERROR = 1
};

struct List
{
    int * elements;
    int * next;
    int * prev;

    size_t free;
    size_t size;
    size_t cap;

};

int ListCtor(List * lst, size_t capacity);
int ListInsert(List * lst, size_t index, int num);
int ListDelete(List * lst, size_t index);
int ListRealloc(List * lst);

int ListPrint(List * lst, size_t num);
int ListDump(List * lst, const char * OutName);
int ListDtor(List * lst);

int ListSetElem(List * lst, size_t index, int val);
int * ListGetElem(List * lst, size_t index);
int ListGetNext(List * lst, size_t index);
int ListGetPrev(List * lst, size_t index);

#endif
