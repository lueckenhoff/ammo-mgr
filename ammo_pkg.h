#ifndef __INC_AMMO_PKG_H
#define __INC_AMMO_PKG_H

#include "stringdb.h"
#include "brand.h"
#include "bullet.h"
#include "caliber.h"

/*
some examples:
9mm Speer Lawman 124 TMJ 50 /ct

380 ACP Remington Compact Ultimate Defense 102 BJHP 20 /ct
    caliber_id="380 ACP",
    brand_id="Remington",
    product_name="Compact Ultimate Defense",
    bullet_grains=102,
    bullet_id="BJHP",
    quantity_per_box=20

9mm Federal HST 147 HP 50 /ct
5.56 NATO Magtech Tactical M193 55 FMJBT 50 /ct
5.56 NATO PMC X-TAC 62 Green Tip 20 /ct
*/

typedef int AMMO_PKG_ID;

typedef struct
{
    AMMO_PKG_ID id;
    CALIBER_ID  caliber_id;
    BRAND_ID    brand_id;
    STRING_ID   product_name_id;
    unsigned    bullet_grains;
    BULLET_ID   bullet_descrip_id;
    unsigned    quantity_per_box;
    int         quantity_held;
} AMMO_PKG_T;
/* this data structure represents one particular packaging of one particular kind of ammo */


void ammo_pkg_init(void);

AMMO_PKG_T * ammo_pkg_add(char *caliber, char *brand, char *product_name, unsigned bullet_grains, char *bullet_id, unsigned quantity_per_box);

AMMO_PKG_T *
ammo_pkg_lookup(char *caliber, char *brand, char *product_name, unsigned bullet_grains, char *bullet_descrip, unsigned quantity_per_box);

void ammo_pkg_dump(void);

void ammo_pkg_query(char *caliber, char *brand, char *bullet_descrip, unsigned int bullet_grains);

int ammo_parse(char *line);
#endif  /* not __INC_AMMO_PKG_H */
