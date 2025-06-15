#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"
#include "item.h"

struct item
{
    char *str;
    struct item *prev, *next;
};



int item_order_asc (struct item *a, struct item *b)
{
    return strcmp(a->str, b->str);
}



void item_list_all (char **db, int max_index)
{
    struct item *list = NULL;
    struct item *item;
    struct item *tmp;
    int ix;

    for (ix = 0; ix < max_index; ix++)
    {
        item = malloc(sizeof(struct item));
        if (!item)
        {
            return;
        }
        item->str = db[ix];
        DL_INSERT_INORDER(list, item, item_order_asc);
    }
    DL_FOREACH_SAFE(list, item, tmp) {
        printf("%s\n", item->str);
        free(item);
    }
}
