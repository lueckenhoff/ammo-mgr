#include <stdio.h>
#define __USE_GNU
#include <string.h>
#include "ammo_pkg.h"
#include "utarray.h"
#include "vendor.h"

extern int g_verbose;

UT_array *ammo_pkg_arr;
UT_icd ammopkg_icd = {sizeof(AMMO_PKG_T), NULL, NULL, NULL};

int ammo_pkg_index = 0;



void ammo_pkg_init(void)
{
    utarray_new(ammo_pkg_arr, &ammopkg_icd);
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
             && (0 == strcmp(vendor, vendor_id_get_string(pkg->vendor_id)))
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
ammo_pkg_add (
    char *caliber,
    char *vendor,
    char *product_name,
    unsigned bullet_grains,
    char *bullet_descrip,
    unsigned quantity_per_box
    )
{
    STRING_ID caliber_id, product_name_id, bullet_descrip_id;
    VENDOR_ID vendor_id;
    AMMO_PKG_T *pkg;

    /* first, check to see if we already have this exact package */
    pkg = ammo_pkg_lookup(caliber, vendor, product_name, bullet_grains, bullet_descrip, quantity_per_box);
    if (pkg)
    {
        if (g_verbose)
            printf("ammo_pkg_add: returning existing struct ptr\n");
        return pkg;
    }

    /* nope, go ahead and add strings to the string database */
    caliber_id        = add_string(caliber);
    vendor_id         = vendor_add(vendor);
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
    pkg->id                = ++ammo_pkg_index;
    pkg->quantity_held     = 0;

    if (g_verbose)
    {
        printf("at time of adding:\n");
        printf("         pkg->caliber_id=%u\n", pkg->caliber_id);
        printf("          pkg->vendor_id=%u\n", pkg->vendor_id);
        printf("    pkg->product_name_id=%u\n", pkg->product_name_id);
        printf("  pkg->bullet_descrip_id=%u\n", pkg->bullet_descrip_id);
        printf("      pkg->bullet_grains=%u\n", pkg->bullet_grains);
        printf("   pkg->quantity_per_box=%u\n", pkg->quantity_per_box);
        printf("      pkg->quantity_held=%d\n", pkg->quantity_held);
        printf("                 pkg->id=%u\n", pkg->id);
    }
    /* finally, add this to the dynamic array, and return the new structure ptr */
    utarray_push_back(ammo_pkg_arr, pkg);
    if (g_verbose)
        printf("ammo_pkg_add: returning new struct ptr %p\n", (void *) pkg);
    pkg = ammo_pkg_lookup(caliber, vendor, product_name, bullet_grains, bullet_descrip, quantity_per_box);
    return pkg;
}



void ammo_pkg_dump (void)
{
    AMMO_PKG_T *pkg;

    for( pkg = (AMMO_PKG_T *) utarray_front(ammo_pkg_arr);
         pkg != NULL;
         pkg = (AMMO_PKG_T *)utarray_next(ammo_pkg_arr, pkg))
    {
        printf("caliber=\"%s\" (id=%u)\n", string_id_get_string(pkg->caliber_id), pkg->caliber_id);
        printf("  vendor=\"%s\" (id=%u)\n", vendor_id_get_string(pkg->vendor_id), pkg->vendor_id);
        printf("  product_name=\"%s\" (id=%u)\n", string_id_get_string(pkg->product_name_id), pkg->product_name_id);
        printf("  bullet_descrip=\"%s\" (id=%u)\n", string_id_get_string(pkg->bullet_descrip_id), pkg->bullet_descrip_id);
        printf("  bullet_grains=%u\"\n", pkg->bullet_grains);
        printf("  quantity_per_box=%u\"\n", pkg->quantity_per_box);
        printf("  quantity_held=%d\"\n", pkg->quantity_held);
    }
}



void ammo_pkg_query (char *caliber, char *vendor, char *bullet_descrip)
{
    AMMO_PKG_T *pkg;
    unsigned total_rounds = 0;

    for( pkg = (AMMO_PKG_T *) utarray_front(ammo_pkg_arr);
         pkg != NULL;
         pkg = (AMMO_PKG_T *)utarray_next(ammo_pkg_arr, pkg))
    {
        if ((strlen(caliber) > 0)
            && strncasecmp(caliber, string_id_get_string(pkg->caliber_id), strlen(caliber)))
        {
            continue;
        }
        if ((strlen(vendor) > 0)
            && strncasecmp(vendor, vendor_id_get_string(pkg->vendor_id), strlen(vendor)))
        {
            continue;
        }
        if ((strlen(bullet_descrip) > 0)
            && strncasecmp(bullet_descrip, string_id_get_string(pkg->bullet_descrip_id), strlen(bullet_descrip)))
        {
            continue;
        }
        // 1 5.56 Nato PMC X-TAC 55 FMJ 20/ct
        printf("%u ", pkg->quantity_held);
        printf("%s ", string_id_get_string(pkg->caliber_id));
        printf("%s ", vendor_id_get_string(pkg->vendor_id));
        printf("%s ", string_id_get_string(pkg->product_name_id));
        printf("%u ", pkg->bullet_grains);
        printf("%s ", string_id_get_string(pkg->bullet_descrip_id));
        printf("%u/ct\n", pkg->quantity_per_box);

#if 0
        printf("caliber=\"%s\" (id=%u)\n", string_id_get_string(pkg->caliber_id), pkg->caliber_id);
        printf("  vendor=\"%s\" (id=%u)\n", vendor_id_get_string(pkg->vendor_id), pkg->vendor_id);
        printf("  product_name=\"%s\" (id=%u)\n", string_id_get_string(pkg->product_name_id), pkg->product_name_id);
        printf("  bullet_descrip=\"%s\" (id=%u)\n", string_id_get_string(pkg->bullet_descrip_id), pkg->bullet_descrip_id);
        printf("  bullet_grains=%u\n", pkg->bullet_grains);
        printf("  quantity_per_box=%u\n", pkg->quantity_per_box);
        printf("  quantity_held=%d\n", pkg->quantity_held);
#endif
        total_rounds += pkg->quantity_per_box * pkg->quantity_held;
    }
    printf("%u rounds\n", total_rounds);
}



int
str_replace (char *line, char *old, char *replace)
{
    char *found;
    char *src;
    char *dst;
    int old_len;
    int replace_len;
    int copy_count;

    found = strcasestr(line, old);
    if (!found)
    {
        return 0;
    }
    if (g_verbose)
        printf("str_replace: before='%s'\n", line);
    old_len = strlen(old);
    replace_len = strlen(replace);
    if (old_len == replace_len)
    {
        memcpy(found, replace, replace_len);
    }
    else if (old_len > replace_len)
    {
        memcpy(found, replace, replace_len);
        src = found + old_len;
        dst = found + replace_len;
        copy_count = strlen(found) - old_len;
        while (copy_count > 0)
        {
            *dst++ = *src++;
            --copy_count;
        }
        *dst = '\0';
    }
    else    /* old_len < replace_len */
    {
        src = found + old_len;
        dst = found + replace_len;
        copy_count = strlen(found) - old_len;
        src = line + strlen(line) - 1;
        dst = src + (replace_len - old_len) + 1;
        *dst-- = '\0';
        while (copy_count > 0)
        {
            *dst-- = *src--;
            --copy_count;
        }
        memcpy(found, replace, replace_len);
    }
    if (g_verbose)
        printf("str_replace:  after='%s'\n", line);
    return 1;
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

    /* step 1: preprocess the line a little */
    str_replace(line, "FMJGT", "Green_Tip");
    str_replace(line, "GT ", "Green_Tip ");
    str_replace(line, "Green Tip", "Green_Tip");
    str_replace(line, "Fort Scott Munitions", "Fort_Scott");
    str_replace(line, "Fort_Scott TUI 95 20", "Fort_Scott 95 TUI 20");
    str_replace(line, "Federated Ordnance Foundation", "Federated_Ordnance");
    str_replace(line, "AMMO Inc", "AMMO_Inc");
    str_replace(line, "Underwood Ammunition", "Underwood_Ammunition");
    str_replace(line, "American Sniper", "American_Sniper");
    str_replace(line, "00BUCK 275", "484 00BUCK275");
    str_replace(line, "00 Buck 2.75\"", "484 00BUCK275");
    str_replace(line, "22 LR ", "22 Long Rifle ");

    /* first, work from end of line to the end of the (highly variable) product name */
    right = line + (strlen(line) - 1);
    cur = right;
    --cur;  /* skip 't' */
    --cur;  /* skip 'c' */
    if ('/' != *cur)
    {
        ++cur;
    }
    left = cur;
    // printf("should be '/ct' or 'ct'\n");
    for (cur = left; cur <= right; cur++)
    {
        ;
        // printf("%c", *cur);
    }
    // printf("\n");

    right = left - 1;
    if (' ' == *right)
    {
        --right;
    }

    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    // printf("should be quantity in the box, as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        tmpstr[ix] = *cur;
        // printf("%c", *cur);
    }
    // printf("\n");
    tmpstr[ix] = '\0';
    quantity_per_box = (unsigned) atoi(tmpstr);
    // printf("quantity_per_box=%u\n", quantity_per_box);

    right = left - 2;
    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    // printf("should be bullet_description, as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        bullet_descrip[ix] = *cur;
        //printf("%c", *cur);
    }
    bullet_descrip[ix] = '\0';
    // printf("\n");
    // printf("bullet_descrip='%s'\n", bullet_descrip);

    right = left - 2;
    cur = right;
    while (' ' != *cur)
    {
        --cur;
    }
    left = cur + 1;
    // printf("should be bullet weight, as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        tmpstr[ix] = *cur;
        // printf("%c", *cur);
    }
    // printf("\n");
    tmpstr[ix] = '\0';
    bullet_grains = (unsigned) atoi(tmpstr);
    // printf("bullet_grains=%u\n", bullet_grains);

    prodname_end = left - 2;

    /* next, work from beginning of line to the beginning of the (highly variable) product name */
    left = line;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    //printf("should be quantity (number of packages), as a string:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        tmpstr[ix] = *cur;
        //printf("%c", *cur);
    }
    tmpstr[ix] = '\0';
    quantity = atoi(tmpstr);
    // printf("quantity=%d\n", quantity);
    //printf("\n");

    left = right + 2;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    //printf("should be 1st word of caliber (only word for '9mm'):\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        caliber[ix] = *cur;
        //printf("%c", *cur);
    }
    //printf("\n");

    if (('9' == left[0]) && ('m' == left[1]) && ('m' == left[2]))
    {
        goto skip_second_word;
    }
    else if (('2' == left[0]) && ('2' == left[1]))
    {
        int do_skip = 0;
        int skip_len = 0;
        if (0 == strncasecmp(right + 2, "Long Rifle ", 11))
        {
            do_skip = 1;
            skip_len = 10;
        }
        else if (0 == strncasecmp(right + 2, "LR ", 3))
        {
            do_skip = 1;
            skip_len = 20;
        }
        if (do_skip)
        {
            caliber[ix++] = ' ';
            caliber[ix++] = 'L';
            caliber[ix++] = 'o';
            caliber[ix++] = 'n';
            caliber[ix++] = 'g';
            caliber[ix++] = ' ';
            caliber[ix++] = 'R';
            caliber[ix++] = 'i';
            caliber[ix++] = 'f';
            caliber[ix++] = 'l';
            caliber[ix++] = 'e';
            right += skip_len;
            goto skip_second_word;
        }
    }

    left = right + 2;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    //printf("should be 2nd word of caliber (but never for '9mm'):\n");
    caliber[ix++] = ' ';
    for (cur = left; cur <= right; cur++, ix++)
    {
        caliber[ix] = *cur;
        //printf("%c", *cur);
    }
    //printf("\n");

skip_second_word:
    caliber[ix] = '\0';
    // printf("caliber='%s'\n", caliber);

    left = right + 2;
    cur = left;
    while (' ' != *cur)
    {
        ++cur;
    }
    right = cur - 1;
    //printf("should be vendor:\n");
    for (cur = left, ix = 0; cur <= right; cur++, ix++)
    {
        vendor[ix] = *cur;
        //printf("%c", *cur);
    }
    vendor[ix] = '\0';
    //printf("\n");
    // printf("vendor='%s'\n", vendor);

    prodname_start = right + 2;
    //printf("product name (variable number of words):\n");
    for (cur = prodname_start, ix = 0; cur <= prodname_end; cur++, ix++)
    {
        product_name[ix] = *cur;
        //printf("%c", *cur);
    }
    product_name[ix] = '\0';
    //printf("\n");
    // printf("product_name='%s'\n", product_name);

    pkg = ammo_pkg_add(caliber, vendor, product_name, bullet_grains, bullet_descrip, quantity_per_box);
    // printf("ammo_pkg_add ret'd pkg=%p\n", (void *) pkg);
//    pkg = ammo_pkg_add(caliber, vendor, product_name, bullet_grains, bullet_descrip, quantity_per_box);
//    printf("ammo_pkg_add ret'd pkg=%p\n", pkg);
    if (!pkg)
    {
        return ERROR;
    }
    if (g_verbose)
        printf("before: pkg=%p pkg->quantity_held=%d\n", (void *) pkg, pkg->quantity_held);
    pkg->quantity_held += quantity;
    if (g_verbose)
        printf("after: pkg=%p pkg->quantity_held=%d\n", (void *) pkg, pkg->quantity_held);
    return 0;
}

