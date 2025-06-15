#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brand.h"
#include "item.h"

extern int g_verbose;

#define INITIAL_MAX_BRANDS 8
char **brand_db_arr;
int max_brands = INITIAL_MAX_BRANDS;
int brand_db_index = 0;

void brand_init (void)
{
    max_brands = INITIAL_MAX_BRANDS;
    brand_db_arr = malloc(sizeof(char *) * max_brands);
}

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
BRAND_ID brand_add (const char *str)
{
    char *newstr;
    size_t len;
    BRAND_ID id;

    if (g_verbose)
        printf("add_brand(%s): entry\n", str);
    id = string_get_brandid(str);
    if (BRAND_ERROR != id)
    {
        if (g_verbose)
            printf("add_brand: returning existing id %d\n", id);
        return id;
    }
    if (brand_db_index >= max_brands)
    {
        int new_siz = max_brands * 2;

        if (g_verbose)
            printf("brand_add: increasing size from %d to %d\n", max_brands, new_siz);
        brand_db_arr = realloc(brand_db_arr, new_siz * sizeof(char *));
        if (!brand_db_arr)
        {
            return BRAND_ERROR;
        }
        max_brands = new_siz;
    }
    len = strlen(str);
    newstr = malloc(len + 1);
    if (!newstr)
    {
        return BRAND_ERROR;
    }
    newstr[len] = '\0'; /* force NULL-terminated */
    strncpy(newstr, str, len);
    brand_db_arr[brand_db_index] = newstr;
    id = brand_db_index;
    ++brand_db_index;
    if (g_verbose)
        printf("add_brand: returning new id %d\n", id);
    return id;
}



/* returns NULL-terminated name of corresponding string */
char * brand_id_get_string (BRAND_ID id)
{
//printf("string_id_get_string(%d): entry\n", id);
    if ((id < 0) || (id >= brand_db_index))
    {
        return "???";
    }
//printf("brand_id_get_string(%d): returning %s\n", id, brand_db_arr[id]);
    return brand_db_arr[id];
}



/* returns ID corresponding to string, BRAND_ERROR if string is unknown */
BRAND_ID string_get_brandid (const char * string)
{
    int ix;

//printf("string_get_brandid(\"%s\"): entry\n", string);
    for (ix = 0; ix < brand_db_index; ix++)
    {
        if (0 == strcmp(string, brand_db_arr[ix]))
        {
//printf("string_get_brandid(\"%s\"): gotta match, returning %d\n", string, ix);
            return ix;
        }
    }
//printf("string_get_brandid(\"%s\"): returning BRAND_ERROR\n", string);
    return BRAND_ERROR;
}



void brand_list_all (void)
{
    item_list_all(brand_db_arr, brand_db_index);
}
