#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>     /* for strcmp */
#include <strings.h>    /* for strcasecmp */
#include "ammo_pkg.h"
#include "stringdb.h"
#include "brand.h"
#include "bullet.h"
#include "caliber.h"
#include "config.h"

int g_verbose = 0;

void print_help (void)
{
    printf("usage:\n"
           "add - interactively add ammo\n"
           "dump - dump state\n"
           "  dump strings - show all strings\n"
           "list - list various things:\n"
           "  list brands - show all brands\n"
           "  list bullets - show all bullet descriptions (FMJ, JHP, ...)\n"
           "  list calibers - show all calibers\n"
           "help - show this help\n"
           "   ? - show this help\n"
           "read - read in a ammo transaction file from disk\n"
           "multiread - read multiple ammo transaction file2 from disk\n"
           "show - perform queries against the inventory.\n"
           "  For example:\n"
           "  show caliber=9mm               (show all 9mm)\n"
           "  show caliber=9mm,brand=federal (show all 9mm from Federal)\n"
           "  show caliber=9mm,bullet=jhp    (show all 9mm Jacketed Hollow Points)\n"
           "  show brand=speer               (show everything from Speer, in all calibers)\n"
           "terse - stop being verbose\n"
           "verbose - be verbose\n"
           "quit - quit the application\n"
           "(commands may be abbreviated)\n"
          );
}



int main (int argc, char **argv)
{
    int ix;
    int account_id;
    FILE *cfg_file;
    char file_str[80];

    if (argc >= 2)
    {
        account_id = atoi(argv[1]);
    }
    else
    {
        account_id = 0; /* just default to zero */
    }
//    printf("Ammo Hoard Inventory Manager, version 1.0.0\n");
//    printf("===========================================\n");
    stringdb_init();
    brand_init();
    bullet_init();
    caliber_init();
    ammo_pkg_init();
//    printf("account ID=%d\n", account_id);
    snprintf(file_str, sizeof(file_str) -1 , "%08d.amo", account_id);
//    printf("data file='%s'\n", file_str);
    (void) cfg_ingest_path(file_str, NULL);
    cfg_file = fopen(file_str, "a");
    if (NULL == cfg_file)
    {
        fprintf(stderr, "failed to open/create config file %s!\n", file_str);
        return -1;
    }

    if (0 == strncasecmp(argv[2], "add", 1))
    {
        cfg_add_ammo(stdin, cfg_file);
    }
    else if (0 == strncasecmp(argv[2], "list", 1))
    {
        if (argc < 4)
        {
            printf("please re-issue command and specify brand/bullet/caliber\n"
                   "  for example:\n"
                   "list brands\n"
                   "  or\n"
                   "list bullets\n"
                   "  or\n"
                   "list calibers\n"
                   );
        }
        else if (0 == strncasecmp(argv[3], "brand", 2))
        {
            brand_list_all();
        }
        else if (0 == strncasecmp(argv[3], "bullet", 2))
        {
            bullet_list_all();
        }
        else if (0 == strncasecmp(argv[3], "caliber", 1))
        {
            caliber_list_all();
        }
    }
    else if (0 == strncasecmp(argv[2], "dump", 1))
    {
        if (0 == strncasecmp(argv[3], "string", 1))
        {
            stringdb_dump();
        }
        else
        {
            ammo_pkg_dump();
        }
    }
    else if (   (0 == strncasecmp(argv[2], "help", 1))
             || (0 == strcmp(argv[2], "?")))
    {
        print_help();
    }
    else if (0 == strncasecmp(argv[2], "read", 1))
    {
        cfg_ingest_path(argv[3], cfg_file);
    }
    else if (0 == strncasecmp(argv[2], "multiread", 1))
    {
        for (ix = 4; ix < argc; ix++)
        {
            cfg_ingest_path(argv[ix], NULL);
        }
    }
    else if (0 == strncasecmp(argv[2], "show", 2))
    {
        do_show(argv[3]);
    }
    else
    {
        printf("unknown command %s; ignoring\n", argv[2]);
    }

    fclose(cfg_file);

    return 0;
}
