#include <stdio.h>

#include "list.h"

/*
 *  Use ListCtor(&lst) to init a list
 *  Use ListDtor(&lst) to delete list
 *
 *
 *  Available functions:
 *          ListInsert(&lst, index, num)    - inserts an element into list
 *          ListDelete(&lst, index)         - deletes an element out of list
 *
 *          ListSetElem(&lst, index, num)   - sets an element by index
 *          ListGetElem(&lst, index)        - gets adr of element by index
 *          ListGetNext(&lst, index)        - gets next index of element by index
 *          ListGetPrev(&lst, index)        - gets prev index of element by index
 *
 *          ListPrint(&lst, count)          - dumps list and info in stdout
 *          ListDump(&lst)                  - make a png pic of your list
*/

int main(void)
{
    List lst = {};
    ListCtor(&lst, 10);

    int * val = 0;

    /* Code Sample */

    ListInsert(&lst, 0, 15);
    ListInsert(&lst, 0, 25);
    ListInsert(&lst, 0, 40);
    ListDelete(&lst, 2);
    ListInsert(&lst, 1, 30);

    ListSetElem(&lst, 2, 54);

    ListDump(&lst, "out.dot");
    ListPrint(&lst, 15);

    ListDtor(&lst);


}
