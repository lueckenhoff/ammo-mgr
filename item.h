#ifndef _INC_ITEM_H

#define _INC_ITEM_H

struct item
{
    char *str;
    struct item *prev, *next;
};

int item_order_ascending(struct item *a, struct item *b);
void item_list_all(char **db, int max_index);
struct item * item_list_alloc(char **db, int max_index);
void item_list_free(struct item *list);

#endif  /* not _INC_ITEM_H */
