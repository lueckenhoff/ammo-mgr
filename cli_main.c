#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>     /* for strcmp */
#include <strings.h>    /* for strcasecmp */
#include "ammo_pkg.h"
// #include "stringdb.h"

void print_help (void)
{
    printf("commands:\n"
           " a - add ammo\n"
           " h - print this help\n"
           " q - quit the application\n"
          );
}

void add_ammo (void)
{
    char buffer[132];

    printf("for example:\n");
    printf("22 9mm Speer Lawman 124 TMJ 50 /ct\n");
    printf("2 380 ACP Remington Compact Ultimate Defense 102 BJHP 20 ct\n");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;      /* trim off trailing newline */
    printf("line=\"%s\"\n", buffer);
    /* now parse the line */
    ammo_parse(buffer);
}


void dump_everything (void)
{
    stringdb_dump();
    ammo_pkg_dump();
}



void cmd_loop (void)
{
    int keep_going = 1;
    char c;
    char buffer[80];

    while (keep_going)
    {
        printf("enter a command: (h for help): ");
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;      /* trim off trailing newline */
        c = buffer[0];
        c = toupper(c);
        printf("gotta %c (0x%X)\n", c, c);
        switch (c)
        {
        case 'A':
            add_ammo();
            break;
        case 'D':
            dump_everything();
            break;
        case '?':               /* fall-thru on purpose */
        case 'H':
            print_help();
            break;
        case 'Q':
            keep_going = 0;
            break;
        default:
            printf("unknown command %c; ignoring\n", c);
            break;
        }
    }
}



int main (int argc, char **argv)
{
    int account_id;

    if (argc < 2)
    {
        printf("usage: %s <account number>\n", argv[0]);
        return -1;
    }
    printf("Ammo Hoard Inventory Manager, version 1.0.0\n");
    printf("===========================================\n");
    account_id = atoi(argv[1]);
    printf("account ID=%d\n", account_id);
    ammo_pkg_init();
    cmd_loop();

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
