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
    printf("commands:\n"
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



void cmd_loop (FILE *cfg_file)
{
    int keep_going = 1;
    int ix;
    char *ptr;
    char buffer[132];
    char *word2;
    char word1[10];

    while (keep_going)
    {
        printf("enter a command: (h for help): ");
        bzero(buffer, sizeof(buffer));          /* ensure a fresh start */
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;      /* trim off trailing newline */

        /* skip any leading spaces */
        ptr = buffer;
        ix = 0;
        while (isspace(*ptr) && (ix < sizeof(buffer) - 1))
        {
            ++ix;
            ++ptr;
        }

        for (ix = 0; ix < sizeof(word1) - 1; ix++, ptr++)
        {
            if (isspace(*ptr))
            {
                break;
            }
            word1[ix] = *ptr;
        }
        word1[ix] = '\0';
        if (g_verbose)
            printf("read word1 '%s'\n", word1);
        for (     ; ix < sizeof(buffer) - 1; ix++, ptr++)
        {
            if (!isspace(*ptr))
            {
                break;
            }
        }
        word2 = ptr;
        if (g_verbose)
            printf("read word2 '%s'\n", word2);

        if (0 == strncasecmp(word1, "add", 1))
        {
            cfg_add_ammo(stdin, cfg_file);
        }
        else if (0 == strncasecmp(word1, "list", 1))
        {
            if (0 == strncasecmp(word2, "brand", 2))
            {
                brand_list_all();
            }
            else if (0 == strncasecmp(word2, "bullet", 2))
            {
                bullet_list_all();
            }
            else if (0 == strncasecmp(word2, "caliber", 1))
            {
                caliber_list_all();
            }
            else
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
        }
        else if (0 == strncasecmp(word1, "dump", 1))
        {
            if (0 == strncasecmp(word2, "string", 1))
            {
                stringdb_dump();
            }
            else
            {
                ammo_pkg_dump();
            }
        }
        else if (   (0 == strncasecmp(word1, "help", 1))
                 || (0 == strcmp(word1, "?")))
        {
            print_help();
        }
        else if (0 == strncasecmp(word1, "terse", 1))
        {
            printf("will stop being verbose\n");
            g_verbose = 0;
        }
        else if (0 == strncasecmp(word1, "verbose", 1))
        {
            printf("will start being verbose\n");
            g_verbose = 1;
        }
        else if (0 == strncasecmp(word1, "quit", 1))
        {
            keep_going = 0;
        }
        else if (0 == strncasecmp(word1, "read", 1))
        {
            cfg_ingest_path(word2, cfg_file);
        }
        else if (0 == strncasecmp(word1, "multiread", 1))
        {
            char *token;
            while ((token = strsep(&word2, " ")) != NULL)
            {
                cfg_ingest_path(token, cfg_file);
            }
        }
        else if (0 == strncasecmp(word1, "show", 2))
        {
            do_show(word2);
        }
        else
        {
            printf("unknown command %s; ignoring\n", word1);
        }
    }
}



int main (int argc, char **argv)
{
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
    printf("Ammo Hoard Inventory Manager, version 1.0.0\n");
    printf("===========================================\n");
    stringdb_init();
    brand_init();
    bullet_init();
    caliber_init();
    ammo_pkg_init();
    printf("account ID=%d\n", account_id);
    snprintf(file_str, sizeof(file_str) -1 , "%08d.amo", account_id);
    printf("data file='%s'\n", file_str);
    (void) cfg_ingest_path(file_str, NULL);
    cfg_file = fopen(file_str, "a");
    if (NULL == cfg_file)
    {
        fprintf(stderr, "failed to open/create config file %s!\n", file_str);
        return -1;
    }
    cmd_loop(cfg_file);
    fclose(cfg_file);

#if 0
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
#endif

    return 0;
}
