#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringdb.h"

extern int g_verbose;

#define INITIAL_MAX_STRINGS 8
int max_strings = INITIAL_MAX_STRINGS;
char **stringdb_arr;
int stringdb_index = 0;

void stringdb_init (void)
{
    max_strings = INITIAL_MAX_STRINGS;
    stringdb_arr = malloc(sizeof(char *) * max_strings);
}

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
STRING_ID
add_string (const char *str)
{
    char *newstr;
    size_t len;
    STRING_ID id;

    if (g_verbose)
        printf("add_string(%s): entry\n", str);
    id = string_get_id(str);
    if (ERROR != id)
    {
        if (g_verbose)
            printf("add_string: returning existing id %d\n", id);
        return id;
    }
    if (stringdb_index >= max_strings)
    {
        int new_siz = max_strings * 2;

        if (g_verbose)
            printf("add_string: increasing size from %d to %d\n", max_strings, new_siz);
        stringdb_arr = realloc(stringdb_arr, new_siz * sizeof(char *));
        if (!stringdb_arr)
        {
            return ERROR;
        }
        max_strings = new_siz;
    }
    len = strlen(str);
    newstr = malloc(len + 1);
    if (!newstr)
    {
        return ERROR;
    }
    newstr[len] = '\0'; /* force NULL-terminated */
    strncpy(newstr, str, len);
    stringdb_arr[stringdb_index] = newstr;
    id = stringdb_index;
    ++stringdb_index;
    if (g_verbose)
        printf("add_string: returning new id %d\n", id);
    return id;
}



/* returns NULL-terminated name of corresponding string */
char *
string_id_get_string (STRING_ID id)
{
    if (g_verbose)
        printf("string_id_get_string(%d): entry\n", id);
    if ((id < 0) || (id >= stringdb_index))
    {
        return "???";
    }
    if (g_verbose)
        printf("string_id_get_string(%d): returning %s\n", id, stringdb_arr[id]);
    return stringdb_arr[id];
}



/* returns ID corresponding to string, ERROR if string is unknown */
STRING_ID
string_get_id (const char * string)
{
    int ix;

    if (g_verbose)
        printf("string_get_id(\"%s\"): entry\n", string);
    for (ix = 0; ix < stringdb_index; ix++)
    {
        if (0 == strcmp(string, stringdb_arr[ix]))
        {
            if (g_verbose)
                printf("string_get_id(\"%s\"): gotta match, returning %d\n", string, ix);
            return ix;
        }
    }
    if (g_verbose)
        printf("string_get_id(\"%s\"): returning ERROR\n", string);
    return ERROR;
}

void stringdb_dump (void)
{
    int ix;

    for (ix = 0; ix < stringdb_index; ix++)
    {
        printf("%03d \"%s\"\n", ix, stringdb_arr[ix]);
    }
}
