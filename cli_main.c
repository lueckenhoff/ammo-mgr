#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>     /* for strcmp */
#include <strings.h>    /* for strcasecmp */
#include "ammo_pkg.h"
#include "stringdb.h"

void print_help (void)
{
    printf("commands:\n"
           " add - interactively add ammo\n"
           " dump - dump state\n"
           " help - show this help\n"
           " read - read in a ammo transaction file from disk\n"
           " ? - show this help\n"
           " quit - quit the application\n"
           "\n"
           "(commands may be abbreviated)\n"
          );
}



/* syntax:
 * YYYY.MM.DD
 * line1
 * line2
 * line3
 * blank line terminates the transaction
 *
 * for example:
2025.05.23
22 9mm Speer Lawman 124 TMJ 50 /ct
20 9mm Winchester USA Ready 115 FMJ 50 /ct
2 9mm Federal HST 147 HP 50/ct
2 380 ACP Remington Compact Ultimate Defense 102 BJHP 20 ct

 * would add two packages of ammo, three 9mm, one 380 ACP
 *
 * another example:
2025.04.01
-3 9mm Speer Lawman 124 TMJ 50 /ct

 * would subtract (use) 3 packages of Speer 9mm

 * @returns 1 if more coming, 0 if input is terminated
 */
int add_ammo (FILE *input_fp, FILE *output_fp)
{
    char line[132];
    int year, month, day;
    char str[5];
    char *ptr;

    if (stdin == input_fp)
    {
        printf("for example:\n");
        printf("22 9mm Speer Lawman 124 TMJ 50 /ct\n");
        printf("2 380 ACP Remington Compact Ultimate Defense 102 BJHP 20 ct\n");
        printf("enter timestamp in YYYY/MM/DD format:\n");
    }
    ptr = fgets(line, sizeof(line), input_fp);
    if (!ptr)
    {
        return 0;
    }
    line[strcspn(line, "\n")] = 0;      /* trim off trailing newline */
    printf("line=\"%s\"\n", line);
    str[0] = line[0];
    str[1] = line[1];
    str[2] = line[2];
    str[3] = line[3];
    str[4] = '\0';
    year = atoi(str);
    str[0] = line[5];
    str[1] = line[6];
    str[2] = '\0';
    month = atoi(str);
    str[0] = line[8];
    str[1] = line[9];
    str[2] = '\0';
    day = atoi(str);
    printf("parsed year=%d month=%d day=%d", year, month, day);
    if (output_fp)
    {
        fprintf(output_fp, "%04d/%02d/%02d\n", year, month, day);
    }

    /* now parse the line */
    for ( ; ; )
    {
        ptr = fgets(line, sizeof(line), input_fp);
        if (!ptr)
        {
            return 0;
        }
        line[strcspn(line, "\n")] = 0;      /* trim off trailing newline */
        printf("line=\"%s\"\n", line);
        if ('\0' == line[0])
        {
            if (output_fp)
            {
                fprintf(output_fp, "\n");
            }
            break;
        }
        ammo_parse(line);
        if (output_fp)
        {
            fprintf(output_fp, "%s\n", line);
        }
    }
    return 1;
}

int ingest_file (FILE *input_fp, FILE *output_fp)
{
    int more_coming = 1;

    /* read state from the config file until there is no more */
    while (more_coming)
    {
        more_coming = add_ammo(input_fp, output_fp);
    }
    return 0;
}

int ingest_path (char *path, FILE *output_fp)
{
    FILE *input_fp;
    int rval = -1;

    input_fp = fopen(path, "r");
    if (input_fp)
    {
        rval = ingest_file(input_fp, output_fp);
        fclose(input_fp);
    }
    return rval;
}

void dump_everything (void)
{
    stringdb_dump();
    ammo_pkg_dump();
}



void cmd_loop (FILE *cfg_file)
{
    int keep_going = 1;
    int ix;
    char buffer[80];
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
        for (ix = 0; ix < sizeof(word1) - 1; ix++)
        {
            if (isspace(buffer[ix]))
            {
                break;
            }
            word1[ix] = buffer[ix];
        }
        word1[ix] = '\0';
        printf("read word1 '%s'\n", word1);
        for ( ; ix < sizeof(buffer) - 1; ix++)
        {
            if (!isspace(buffer[ix]))
            {
                break;
            }
        }
        word2 = buffer + ix;
        printf("read word2 '%s'\n", word2);

        if (0 == strncasecmp(word1, "add", 1))
        {
            add_ammo(stdin, cfg_file);
        }
        else if (0 == strncasecmp(word1, "dump", 1))
        {
            dump_everything();
        }
        else if (   (0 == strncasecmp(word1, "help", 1))
                 || (0 == strcmp(word1, "?")))
        {
            print_help();
        }
        else if (0 == strncasecmp(word1, "quit", 1))
        {
            keep_going = 0;
        }
        else if (0 == strncasecmp(word1, "read", 1))
        {
            ingest_path(word2, cfg_file);
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

    if (argc < 2)
    {
        printf("usage: %s <account number>\n", argv[0]);
        return -1;
    }
    printf("Ammo Hoard Inventory Manager, version 1.0.0\n");
    printf("===========================================\n");
    ammo_pkg_init();
    account_id = atoi(argv[1]);
    printf("account ID=%d\n", account_id);
    snprintf(file_str, sizeof(file_str) -1 , "%08d.amo", account_id);
    printf("config file='%s'\n", file_str);
    (void) ingest_path(file_str, NULL);
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
