
#ifndef __INC_CONFIG_H
#define __INC_CONFIG_H

int cfg_add_ammo(FILE *input_fp, FILE *output_fp);
int cfg_ingest_file(FILE *input_fp, FILE *output_fp);
int cfg_ingest_path(char *path, FILE *output_fp);

#endif /* not __INC_CONFIG_H */
