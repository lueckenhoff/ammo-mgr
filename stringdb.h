#ifndef __INC_STRINGDB_H
#define __INC_STRINGDB_H

typedef int STRING_ID;
#define ERROR -1

void stringdb_init(void);

/* returns 0 on success, <0 on failure */
/* if the string is already known, return its ID w/o allocating it again */
STRING_ID add_string(const char *str);

/* returns NULL-terminated name of corresponding string */
char * string_id_get_string(STRING_ID id);

/* returns ID corresponding to string, ERROR if string is unknown */
STRING_ID string_get_id(const char * string);

void stringdb_dump(void);

#endif  /* not __INC_STRINGDB_H */
