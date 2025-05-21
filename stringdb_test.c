#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* for strcmp */
#include <strings.h>    /* for strcasecmp */
#include "stringdb.h"

int main (int argc, char **argv)
{
    int ix;
    STRING_ID id, id2;
    char *str;

    printf("argc=%d\n", argc);
    for (ix = 1; ix < argc; ix++)
    {
        printf("argv[%d]=\"%s\"\n", ix, argv[ix]);
        id = add_string(argv[ix]);
        printf("add_string ret'd %d\n", id);
        str = string_id_get_string(id);
        printf("string_id_get_string(%d) != %s ", id, argv[ix]);
        if (strcmp(str, argv[ix]))
        {
            printf("FAIL\n");
        }
        else
        {
            printf("pass\n");
        }
        id2 = string_get_id(argv[ix]);
        printf("string_get_id(%s) == id (%d) ", argv[ix], id);
        if (id2 != id)
        {
            printf("FAIL\n");
        }
        else
        {
            printf("pass\n");
        }
    }
    stringdb_dump();
    return 0;
}
