#include <stdio.h>
#include "ammo_pkg.h"
#include "utarray.h"

UT_array *ammo_pkg_arr;
int ammo_pkg_index = 0;



void ammo_pkg_init(void)
{
    utarray_new(ammo_pkg_arr, &ut_ptr_icd);
}


#if 1   /* { */
AMMO_PKG_T *
ammo_pkg_lookup (char *caliber, char *vendor, char *product_name, unsigned bullet_grains, char *bullet_descrip, unsigned quantity_per_box)
{
    AMMO_PKG_T *pkg;

    pkg = NULL;
    while ( (pkg = (AMMO_PKG_T *)utarray_next(ammo_pkg_arr, pkg)) )
    {
        if (    (0 == strcmp(caliber, string_id_get_string(pkg->caliber_id)))
             && (0 == strcmp(vendor, string_id_get_string(pkg->vendor_id)))
             && (0 == strcmp(product_name, string_id_get_string(pkg->product_name_id)))
             && (bullet_grains == pkg->bullet_grains)
             && (0 == strcmp(bullet_descrip, string_id_get_string(pkg->bullet_descrip_id)))
             && (quantity_per_box == pkg->quantity_per_box)
           )
        {
            return pkg;
        }
    }
    return NULL;
}
#endif          /* 1 } */



AMMO_PKG_T *
ammo_pkg_add
    (
    char *caliber,
    char *vendor,
    char *product_name,
    unsigned bullet_grains,
    char *bullet_descrip,
    unsigned quantity_per_box
    )
{
    STRING_ID caliber_id, vendor_id, product_name_id, bullet_descrip_id;
    AMMO_PKG_T *pkg;

    /* first, check to see if we already have this exact package */
    pkg = NULL;
    while ( (pkg = (AMMO_PKG_T *)utarray_next(ammo_pkg_arr, pkg)) )
    {
        if (    (0 == strcmp(caliber, string_id_get_string(pkg->caliber_id)))
             && (0 == strcmp(vendor, string_id_get_string(pkg->vendor_id)))
             && (0 == strcmp(product_name, string_id_get_string(pkg->product_name_id)))
             && (bullet_grains == pkg->bullet_grains)
             && (0 == strcmp(bullet_descrip, string_id_get_string(pkg->bullet_descrip_id)))
             && (quantity_per_box == pkg->quantity_per_box)
           )
        {
printf("ammo_pkg_add: returning existing struct ptr\n");
            return pkg;
        }
    }

    /* nope, go ahead and add strings to the string database */
    caliber_id        = add_string(caliber);
    vendor_id         = add_string(vendor);
    product_name_id   = add_string(product_name);
    bullet_descrip_id = add_string(bullet_descrip);

    /* then allocate a data structure and populate it */
    pkg = malloc(sizeof(AMMO_PKG_T));
    if (!pkg)
    {
        return NULL;
    }
    pkg->caliber_id        = caliber_id;
    pkg->vendor_id         = vendor_id;
    pkg->product_name_id   = product_name_id;
    pkg->bullet_descrip_id = bullet_descrip_id;
    pkg->bullet_grains     = bullet_grains;
    pkg->quantity_per_box  = quantity_per_box;
    pkg->id                = ammo_pkg_index++;
    pkg->quantity_held     = 0;
    /* finally, add this to the dynamic array, and return the new structure ptr */
    utarray_push_back(ammo_pkg_arr, pkg);
printf("ammo_pkg_add: returning new struct ptr\n");
    return pkg;
}



void ammo_pkg_dump (void)
{
    AMMO_PKG_T *pkg;
    pkg = NULL;
    while ( (pkg = (AMMO_PKG_T *)utarray_next(ammo_pkg_arr, pkg)) )
    {
        printf("caliber=\"%s\" (id=%u)\n", string_id_get_string(pkg->caliber_id), pkg->caliber_id);
        printf("  vendor=\"%s\" (id=%u)\n", string_id_get_string(pkg->vendor_id), pkg->vendor_id);
        printf("  product_name=\"%s\" (id=%u)\n", string_id_get_string(pkg->product_name_id), pkg->product_name_id);
        printf("  bullet_descrip=\"%s\" (id=%u)\n", string_id_get_string(pkg->bullet_descrip_id), pkg->bullet_descrip_id);
        printf("  bullet_grains=%u\"\n", pkg->bullet_grains);
        printf("  quantity_per_box=%u\"\n", pkg->quantity_per_box);
        printf("  quantity_held=%d\"\n", pkg->quantity_held);
    }
}

int ammo_parse (char *line)
{
    int ix;
    int quantity;
    unsigned bullet_grains;
    unsigned quantity_per_box;
    char *left;
    char *right;
    char *cur;
    char *prodname_start;
    char *prodname_end;
    char caliber[40];
    char vendor[40];
    char product_name[80];
    char bullet_descrip[40];
    char tmpstr[40];
    AMMO_PKG_T *pkg;

//    printf("22 9mm Speer Lawman 124 TMJ 50 /ct\n");
//    printf("2 380 ACP Remington Compact Ultimate Defense 102 BJHP 20 ct\n");

    /* first, work from end of line to the end of the (highly variable) product name */
    right = line + (strlen(line) - 1);
    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    printf("should be '/ct' or 'ct'\n");
    for (cur = left; cur <= right; cur++)
    {
        printf("%c", *cur);
    }
    printf("\n");

    right = left - 2;
    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    printf("should be quantity in the box, as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        tmpstr[ix] = *cur;
        printf("%c", *cur);
    }
    tmpstr[ix] = '\0';
    quantity_per_box = atoi(tmpstr);
    printf("\n");

    right = left - 2;
    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    printf("should be bullet_description, as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        bullet_descrip[ix] = *cur;
        printf("%c", *cur);
    }
    bullet_descrip[ix] = '\0';
    printf("\n");

    right = left - 2;
    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    printf("should be bullet weight, as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        tmpstr[ix] = *cur;
        printf("%c", *cur);
    }
    tmpstr[ix] = '\0';
    bullet_grains = (unsigned) atoi(tmpstr);
    printf("\n");

    prodname_end = left - 2;

    /* next, work from beginning of line to the beginning of the (highly variable) product name */
    left = line;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    printf("should be quantity (number of packages), as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        tmpstr[ix] = *cur;
        printf("%c", *cur);
    }
    tmpstr[ix] = '\0';
    quantity = (unsigned) atoi(tmpstr);
    printf("\n");

    left = right + 2;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    printf("should be 1st word of caliber (only word for '9mm'):\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        caliber[ix] = *cur;
        printf("%c", *cur);
    }
    printf("\n");

    if (('9' != left[0]) || ('m' != left[1]) || ('m' != left[2]))
    {
        left = right + 2;
        cur = left;
        while (' ' != *cur)
        {
            ++cur;
        }
        right = cur - 1;
        printf("should be 2nd word of caliber (but never for '9mm'):\n");
        caliber[ix++] = ' ';
        for (cur = left; cur <= right; cur++, ix++)
        {
            caliber[ix] = *cur;
            printf("%c", *cur);
        }
        printf("\n");
    }
    caliber[ix] = '\0';

    left = right + 2;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    printf("should be vendor:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        vendor[ix] = *cur;
        printf("%c", *cur);
    }
    vendor[ix] = '\0';
    printf("\n");

    prodname_start = right + 2;
    printf("product name (variable number of words):\n");
    for (cur = prodname_start, ix = 0; cur <= prodname_end; cur++, ix++)
    {
        product_name[ix] = *cur;
        printf("%c", *cur);
    }
    printf("\n");

    pkg = ammo_pkg_add(caliber, vendor, product_name, bullet_grains, bullet_descrip, quantity_per_box);
    if (!pkg)
    {
        return ERROR;
    }
    pkg->quantity_held += quantity;
    return 0;
}

