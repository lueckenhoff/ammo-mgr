#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bullet.h"

extern int g_verbose;

#define INITIAL_MAX_BULLETS 8
char **bullet_db_arr;
int max_bullets = INITIAL_MAX_BULLETS;
int bullet_db_index = 0;

void bullet_init (void)
{
    max_bullets = INITIAL_MAX_BULLETS;
    bullet_db_arr = malloc(sizeof(char *) * max_bullets);
}

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
BULLET_ID bullet_add (const char *str)
{
    char *newstr;
    size_t len;
    BULLET_ID id;

    if (g_verbose)
        printf("add_bullet(%s): entry\n", str);
    id = string_get_bulletid(str);
    if (BULLET_ERROR != id)
    {
        if (g_verbose)
            printf("add_bullet: returning existing id %d\n", id);
        return id;
    }
    if (bullet_db_index >= max_bullets)
    {
        int new_siz = max_bullets * 2;

        if (g_verbose)
            printf("bullet_add: increasing size from %d to %d\n", max_bullets, new_siz);
        bullet_db_arr = realloc(bullet_db_arr, new_siz * sizeof(char *));
        if (!bullet_db_arr)
        {
            return BULLET_ERROR;
        }
        max_bullets = new_siz;
    }
    len = strlen(str);
    newstr = malloc(len + 1);
    if (!newstr)
    {
        return BULLET_ERROR;
    }
    newstr[len] = '\0'; /* force NULL-terminated */
    strncpy(newstr, str, len);
    bullet_db_arr[bullet_db_index] = newstr;
    id = bullet_db_index;
    ++bullet_db_index;
    if (g_verbose)
        printf("add_bullet: returning new id %d\n", id);
    return id;
}



/* returns NULL-terminated name of corresponding string */
char * bullet_id_get_string (BULLET_ID id)
{
//printf("string_id_get_string(%d): entry\n", id);
    if ((id < 0) || (id >= bullet_db_index))
    {
        return "???";
    }
//printf("bullet_id_get_string(%d): returning %s\n", id, bullet_db_arr[id]);
    return bullet_db_arr[id];
}



/* returns ID corresponding to string, BULLET_ERROR if string is unknown */
BULLET_ID string_get_bulletid (const char * string)
{
    int ix;

//printf("string_get_bulletid(\"%s\"): entry\n", string);
    for (ix = 0; ix < bullet_db_index; ix++)
    {
        if (0 == strcmp(string, bullet_db_arr[ix]))
        {
//printf("string_get_bulletid(\"%s\"): gotta match, returning %d\n", string, ix);
            return ix;
        }
    }
//printf("string_get_bulletid(\"%s\"): returning BULLET_ERROR\n", string);
    return BULLET_ERROR;
}



void bullet_dump (void)
{
    int ix;

    for (ix = 0; ix < bullet_db_index; ix++)
    {
        printf("%03d \"%s\"\n", ix, bullet_db_arr[ix]);
    }
}
