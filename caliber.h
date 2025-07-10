#ifndef _INC_CALIBER_H
#define _INC_CALIBER_H

typedef int CALIBER_ID;
typedef void (show_f)(void *dst, const void *src);

#define CALIBER_ERROR -1
void caliber_init(void);
CALIBER_ID caliber_add(const char * caliber_str);
char * caliber_id_get_string (CALIBER_ID id);
CALIBER_ID string_get_caliberid(const char * string);
void caliber_list_all(void);
char * caliber_get_next(char *caliber);

#endif  /* not _INC_CALIBER_H */

