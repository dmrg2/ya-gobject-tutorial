#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

int                 is_macro_case       (const char *name);
int                 is_macro_type       (const char *name);
int                 is_snake_case       (const char *name);
int                 is_camel_case       (const char *name);
int                 is_valid_pname      (const char *pname);
int                 is_valid_mname      (const char *mname);
char               *to_macro_case       (const char *name);
char               *to_macro_type       (const char *name);
char               *to_snake_case       (const char *name);
char               *to_camel_case       (const char *name);
char               *to_macro_case_ct    (const char *name);
char               *to_macro_type_ct    (const char *name);
char               *to_snake_case_ct    (const char *name);
char               *to_camel_case_ct    (const char *name);
char               *part_modulename     (const char *_name);
char               *part_typename       (const char *_name);
char               *pname_to_snake      (const char *pname);
char               *pname_to_macro      (const char *pname);
char               *to_basename         (const char *_name);
char               *to_dashname         (const char *_name);

#if defined(__cplusplus)
} /* extern "C" */
#endif
