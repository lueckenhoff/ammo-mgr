#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringdb.h"

#define MAX_STRINGS 100
char *stringdb_arr[MAX_STRINGS];
int stringdb_index = 0;

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
STRING_ID
add_string (const char *str)
{
    char *newstr;
    size_t len;
    STRING_ID id;

printf("add_string(%s): entry\n", str);
    id = string_get_id(str);
    if (ERROR != id)
    {
printf("add_string: returning existing id %d\n", id);
        return id;
    }
    if (stringdb_index >= MAX_STRINGS)
    {
        return ERROR;
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
printf("add_string: returning new id %d\n", id);
    return id;
}



/* returns NULL-terminated name of corresponding string */
char *
string_id_get_string (STRING_ID id)
{
//printf("string_id_get_string(%d): entry\n", id);
    if ((id < 0) || (id >= stringdb_index))
    {
        return "???";
    }
//printf("string_id_get_string(%d): returning %s\n", id, stringdb_arr[id]);
    return stringdb_arr[id];
}



/* returns ID corresponding to string, ERROR if string is unknown */
STRING_ID
string_get_id (const char * string)
{
    int ix;

//printf("string_get_id(\"%s\"): entry\n", string);
    for (ix = 0; ix < stringdb_index; ix++)
    {
        if (0 == strcmp(string, stringdb_arr[ix]))
        {
//printf("string_get_id(\"%s\"): gotta match, returning %d\n", string, ix);
            return ix;
        }
    }
//printf("string_get_id(\"%s\"): returning ERROR\n", string);
    return ERROR;
}

void stringdb_dump (void)
{
    int ix;

    for (ix = 0; ix < stringdb_index; ix++)
    {
        printf("%03d %s\n", ix, stringdb_arr[ix]);
    }
}
