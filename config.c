#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>     /* for strcmp */
#include <strings.h>    /* for strcasecmp */
#include "ammo_pkg.h"
#include "stringdb.h"
#include "config.h"

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
 * Note: we don't care about the separator in the timestamp;
 * it can be dots or dashes or slashes or whatever
 *
 * another example:
2025.04.01
-3 9mm Speer Lawman 124 TMJ 50 /ct

 * would subtract (use) 3 packages of 9mm Speer "Lawman" 124 grain TMJ

 * @returns 1 if more coming, 0 if input is terminated
 */
int cfg_add_ammo (FILE *input_fp, FILE *output_fp)
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

int cfg_ingest_file (FILE *input_fp, FILE *output_fp)
{
    int more_coming = 1;

    /* read state from the config file until there is no more */
    while (more_coming)
    {
        more_coming = cfg_add_ammo(input_fp, output_fp);
    }
    return 0;
}

int cfg_ingest_path (char *path, FILE *output_fp)
{
    FILE *input_fp;
    int rval = -1;

    input_fp = fopen(path, "r");
    if (input_fp)
    {
        rval = cfg_ingest_file(input_fp, output_fp);
        fclose(input_fp);
    }
    return rval;
}



