#ifndef _INC_BRAND_H
#define _INC_BRAND_H

typedef int BRAND_ID;
#define BRAND_ERROR -1
void brand_init(void);
BRAND_ID brand_add(const char * brand_str);
char * brand_id_get_string (BRAND_ID id);
BRAND_ID string_get_brandid(const char * string);
void brand_dump(void);

#endif  /* not _INC_BRAND_H */

