#include <stdio.h>
#include <string.h>
#include "vendor.h"
#include "uthash.h"

#define MAX_VENDORS 1000
char *vendordb_arr[MAX_VENDORS];
int vendordb_index = 0;

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
VENDOR_ID vendor_add (const char *str)
{
    char *newstr;
    size_t len;
    VENDOR_ID id;

printf("add_vendor(%s): entry\n", str);
    id = string_get_vendorid(str);
    if (VENDOR_ERROR != id)
    {
printf("add_vendor: returning existing id %d\n", id);
        return id;
    }
    if (vendordb_index >= MAX_VENDORS)
    {
        return VENDOR_ERROR;
    }
    len = strlen(str);
    newstr = malloc(len + 1);
    if (!newstr)
    {
        return VENDOR_ERROR;
    }
    newstr[len] = '\0'; /* force NULL-terminated */
    strncpy(newstr, str, len);
    vendordb_arr[vendordb_index] = newstr;
    id = vendordb_index;
    ++vendordb_index;
printf("add_vendor: returning new id %d\n", id);
    return id;
}



/* returns NULL-terminated name of corresponding string */
char * vendor_id_get_string (VENDOR_ID id)
{
//printf("string_id_get_string(%d): entry\n", id);
    if ((id < 0) || (id >= vendordb_index))
    {
        return "???";
    }
//printf("vendor_id_get_string(%d): returning %s\n", id, vendordb_arr[id]);
    return vendordb_arr[id];
}



/* returns ID corresponding to string, VENDOR_ERROR if string is unknown */
VENDOR_ID string_get_vendorid (const char * string)
{
    int ix;

//printf("string_get_vendorid(\"%s\"): entry\n", string);
    for (ix = 0; ix < vendordb_index; ix++)
    {
        if (0 == strcmp(string, vendordb_arr[ix]))
        {
//printf("string_get_vendorid(\"%s\"): gotta match, returning %d\n", string, ix);
            return ix;
        }
    }
//printf("string_get_vendorid(\"%s\"): returning VENDOR_ERROR\n", string);
    return VENDOR_ERROR;
}



void vendordb_dump (void)
{
    int ix;

    for (ix = 0; ix < vendordb_index; ix++)
    {
        printf("%03d \"%s\"\n", ix, vendordb_arr[ix]);
    }
}
