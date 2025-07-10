#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caliber.h"
#include "utlist.h"
#include "item.h"

extern int g_verbose;

#define INITIAL_MAX_CALIBERS 8
char **caliber_db_arr;
int max_calibers = INITIAL_MAX_CALIBERS;
int caliber_db_index = 0;
struct item * caliber_list = NULL;

void caliber_init (void)
{
    max_calibers = INITIAL_MAX_CALIBERS;
    caliber_db_arr = malloc(sizeof(char *) * max_calibers);
    caliber_list = NULL;
}

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
CALIBER_ID caliber_add (const char *str)
{
    char *newstr;
    struct item * item;
    size_t len;
    CALIBER_ID id;

    if (g_verbose)
        printf("add_caliber(%s): entry\n", str);
    id = string_get_caliberid(str);
    if (CALIBER_ERROR != id)
    {
        if (g_verbose)
            printf("add_caliber: returning existing id %d\n", id);
        return id;
    }
    if (caliber_db_index >= max_calibers)
    {
        int new_siz = max_calibers * 2;

        if (g_verbose)
            printf("caliber_add: increasing size from %d to %d\n", max_calibers, new_siz);
        caliber_db_arr = realloc(caliber_db_arr, new_siz * sizeof(char *));
        if (!caliber_db_arr)
        {
            return CALIBER_ERROR;
        }
        max_calibers = new_siz;
    }
    len = strlen(str);
    newstr = malloc(len + 1);
    if (!newstr)
    {
        return CALIBER_ERROR;
    }
    newstr[len] = '\0'; /* force NULL-terminated */
    strncpy(newstr, str, len);
    caliber_db_arr[caliber_db_index] = newstr;
    id = caliber_db_index;
    ++caliber_db_index;
    item = malloc(sizeof(struct item));
    if (!item)
    {
        return CALIBER_ERROR;
    }
    item->str = newstr;
    DL_INSERT_INORDER(caliber_list, item, item_order_ascending);
    if (g_verbose)
        printf("add_caliber: returning new id %d\n", id);
    return id;
}



/* returns NULL-terminated name of corresponding string */
char * caliber_id_get_string (CALIBER_ID id)
{
//printf("string_id_get_string(%d): entry\n", id);
    if ((id < 0) || (id >= caliber_db_index))
    {
        return "???";
    }
//printf("caliber_id_get_string(%d): returning %s\n", id, caliber_db_arr[id]);
    return caliber_db_arr[id];
}



/* returns ID corresponding to string, CALIBER_ERROR if string is unknown */
CALIBER_ID string_get_caliberid (const char * string)
{
    int ix;

//printf("string_get_caliberid(\"%s\"): entry\n", string);
    for (ix = 0; ix < caliber_db_index; ix++)
    {
        if (0 == strcmp(string, caliber_db_arr[ix]))
        {
//printf("string_get_caliberid(\"%s\"): gotta match, returning %d\n", string, ix);
            return ix;
        }
    }
//printf("string_get_caliberid(\"%s\"): returning CALIBER_ERROR\n", string);
    return CALIBER_ERROR;
}



void caliber_list_all (void)
{
#if 0
    struct item * item;
    DL_FOREACH(caliber_list, item)
    {
        printf("'%s'\n", item->str);
    }
#endif
    item_list_all(caliber_db_arr, caliber_db_index);
}



char * caliber_get_next (char *caliber)
{
    struct item * item;
    char *rval = "";

//printf("caliber_get_next('%s'): entry\n", caliber);
    if ((NULL == caliber) || (0 == strlen(caliber)))
    {
        rval = caliber_list->str;
//printf("caliber_get_next: default assigning %s\n", caliber_list->str);
        return rval;
    }

    DL_FOREACH(caliber_list, item)
    {
//printf("caliber_get_next: item->str='%s'\n", item->str);
        if (0 == strcasecmp(caliber, item->str))
        {
            if (item->next)
            {
//printf("caliber_get_next: this item matched, assigning %s\n", item->next->str);
                rval = item->next->str;
            }
            else
            {
//printf("caliber_get_next: this item matched, but end of list assigning NULL\n");
                rval = NULL;
            }
            goto done_searching;
        }
    }
done_searching:
//printf("caliber_get_next: about to free item list\n");
    return rval;
}


