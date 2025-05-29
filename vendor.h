#ifndef _INC_VENDOR_H
#define _INC_VENDOR_H

typedef int VENDOR_ID;
#define VENDOR_ERROR -1
void vendor_init(void);
VENDOR_ID vendor_add(const char * vendor_str);
char * vendor_id_get_string (VENDOR_ID id);
VENDOR_ID string_get_vendorid(const char * string);
void vendor_dump(void);

#endif  /* not _INC_VENDOR_H */

