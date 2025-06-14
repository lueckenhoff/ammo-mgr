#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caliber.h"

extern int g_verbose;

#define INITIAL_MAX_CALIBERS 8
char **caliber_db_arr;
int max_calibers = INITIAL_MAX_CALIBERS;
int caliber_db_index = 0;

void caliber_init (void)
{
    max_calibers = INITIAL_MAX_CALIBERS;
    caliber_db_arr = malloc(sizeof(char *) * max_calibers);
}

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
CALIBER_ID caliber_add (const char *str)
{
    char *newstr;
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



void caliber_dump (void)
{
    int ix;

    for (ix = 0; ix < caliber_db_index; ix++)
    {
        printf("%03d \"%s\"\n", ix, caliber_db_arr[ix]);
    }
}
