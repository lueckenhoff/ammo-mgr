#ifndef _INC_BULLET_H
#define _INC_BULLET_H

typedef int BULLET_ID;
#define BULLET_ERROR -1
void bullet_init(void);
BULLET_ID bullet_add(const char * bullet_str);
char * bullet_id_get_string (BULLET_ID id);
BULLET_ID string_get_bulletid(const char * string);
void bullet_dump(void);

#endif  /* not _INC_BULLET_H */

