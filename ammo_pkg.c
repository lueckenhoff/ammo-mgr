#include <stdio.h>
#include "ammo_pkg.h"
#include "utarray.h"

UT_array *ammo_pkg_arr;
int ammo_pkg_index = 0;



void ammo_pkg_init(void)
{
    utarray_new(ammo_pkg_arr, &ut_ptr_icd);
}


#if 0   /* { */
AMMO_PKG_ID ammo_pkg_lookup (char *caliber, char *vendor, char *product_name, unsigned bullet_grains, char *bullet_descrip, unsigned quantity_per_box)
{
    AMMO_PKG_T *pkg;

    pkg = NULL;
    while ( (pkg = (char**)utarray_next(ammo_pkg_arr, pkg)) )
    {
        if (    (0 == strcmp(caliber, string_id_get_string(pkg->caliber_id)))
             && (0 == strcmp(vendor, string_id_get_string(pkg->vendor_id)))
             && (0 == strcmp(product_name, string_id_get_string(pkg->product_name_id)))
             && (bullet_grains == pkg->bullet_grains)
             && (0 == strcmp(bullet_descrip, string_id_get_string(pkg->bullet_descrip_id)))
             && (quantity_per_box == pkg->quantity_per_box)
           )
        {
            return pkg->id;
        }
    }
    return ERROR;
}
#endif          /* 0 } */



AMMO_PKG_ID
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
            return pkg->id;
        }
    }

    /* nope, go ahead and add strings to the string database */
    caliber_id = add_string(caliber);
    vendor_id = add_string(vendor);
    product_name_id = add_string(product_name);
    bullet_descrip_id = add_string(bullet_descrip);

    /* then allocate a data structure and populate it */
    pkg = malloc(sizeof(AMMO_PKG_T));
    if (!pkg)
    {
        return ERROR;
    }
    pkg->caliber_id        = caliber_id;
    pkg->vendor_id         = vendor_id;
    pkg->product_name_id   = product_name_id;
    pkg->bullet_descrip_id = bullet_descrip_id;
    pkg->bullet_grains     = bullet_grains;
    pkg->quantity_per_box  = quantity_per_box;
    pkg->id                = ammo_pkg_index++;
    /* finally, add this to the dynamic array, and return the new ID */
    utarray_push_back(ammo_pkg_arr, pkg);
    return pkg->id;
}



void ammo_pkg_dump (void)
{
    AMMO_PKG_T *pkg;
    pkg = NULL;
    while ( (pkg = (AMMO_PKG_T *)utarray_next(ammo_pkg_arr, pkg)) )
    {
        printf("id=%d\n", pkg->caliber_id);
        printf("  vendor=\"%s\"\n", string_id_get_string(pkg->vendor_id));
        printf("  product_name=\"%s\"\n", string_id_get_string(pkg->product_name_id));
        printf("  bullet_descrip=\"%s\"\n", string_id_get_string(pkg->bullet_descrip_id));
        printf("  bullet_grains=%u\"\n", pkg->bullet_grains);
        printf("  quantity_per_box=%u\"\n", pkg->quantity_per_box);
    }
}

void ammo_parse (char *line)
{
    char *left;
    char *right;
    char *cur;
    char *prodname_end;

//    printf("22 9mm Speer Lawman 124 TMJ 50 /ct\n");
//    printf("2 380 ACP Remington Compact Ultimate Defense 102 BJHP 20 ct\n");
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
    printf("should be bullet_description, as a string:\n");
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
    printf("should be bullet weight, as a string:\n");
    for (cur = left; cur <= right; cur++)
    {
        printf("%c", *cur);
    }
    printf("\n");

    prodname_end = left - 2;
}

