#include <stdio.h>

#include "list.h"

/*
 *  Use ListCtor(&lst) to init a list
 *  Use ListDtor(&lst) to delete list
 *
 *
 *  Available functions:
 *          ListInsert(&lst, index, num)    - inserts an element into list
 *          ListDelete(&lst, index, ret)    - deletes an element out of list
 *
 *          ListPushTail(&lst, num)         - inserts an element after tail
 *          ListPushHead(&lst, num)         - inserts an element after head
 *
 *          ListPopTail(&lst, ret)          - pop an element in tail
 *          LisPopHead(&lst, ret)           - pop an element in head
 *
 *          ListGetElem(&lst, index)        - returns adr of element by index
 *          ListGetNext(&lst, index)        - returns next index of element by index
 *          ListGetPrev(&lst, index)        - returns prev index of element by index
 *          ListGetCap(&lst)                - returns list capacity
 *          ListGetSize(&lst)               - returns list size
 *          ListGetHead(&lst)               - returns adr of list head
 *          ListGetTail(&lst)               - returns adr of list tail
 *
 *          ListPrint(&lst, count)          - dumps list and info in stdout
 *          ListDump(&lst, filename)        - make a png pic of your list and creates
 *                                            an htm file with dump and png pic
 *
*/

int main(void)
{
    List lst = {};
    ListCtor(&lst, 3);

    int val = 0;

    /* Code Sample */

    ListPushTail(&lst, 20);
    ListPushTail(&lst, 30);
    ListPushHead(&lst, 80);
    ListPushHead(&lst, 70);
    ListPopTail(&lst, &val);

    ListDump(&lst, "out");

    ListDtor(&lst);


}
