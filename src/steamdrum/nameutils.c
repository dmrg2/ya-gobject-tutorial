#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameutils.h"

static void       __macro_type_to_macro_case (char *to, const char *from);
static void       __snake_case_to_macro_case (char *to, const char *from);
static void       __camel_case_to_macro_case (char *to, const char *from);
static void       __macro_case_to_macro_type (char *to, const char *from);
static void       __snake_case_to_macro_type (char *to, const char *from);
static void       __camel_case_to_macro_type (char *to, const char *from);
static void       __macro_case_to_snake_case (char *to, const char *from);
static void       __macro_type_to_snake_case (char *to, const char *from);
static void       __camel_case_to_snake_case (char *to, const char *from);
static void       __macro_case_to_camel_case (char *to, const char *from);
static void       __macro_type_to_camel_case (char *to, const char *from);
static void       __snake_case_to_camel_case (char *to, const char *from);

int is_macro_case (const char *name) {
    const char *p = name;
    int part_len = 0, seen_us = 0;

    if (!name) return 0;
    if (!*name) return 0;
    if (!isupper (*name)) return 0;
    do {
        if (*p == '_') {
            seen_us = 1;
            if (part_len == 0) return 0;
            part_len = 0;
        } else {
            if (!isupper (*p) && !isdigit (*p)) return 0;
            part_len++;
        }
    } while (*++p);
    return seen_us && (part_len > 0);
}

int is_macro_type (const char *name) {
    const char *p = name;
    int part_no = 0;
    int part_len = 0;
    int seen_type = 0;

    if (!name) return 0;
    if (!*name) return 0;
    if (!isupper (*name)) return 0;
    do {
        if (*p == '_') {
            if (part_len == 0) return 0;
            part_len = 0;
            part_no++;
            if (part_no == 1) {
                if (strncmp (p, "_TYPE_", 6) == 0) seen_type = 1;
            }
        } else {
            if (islower (*p) && !isdigit (*p)) return 0;
            part_len++;
        }
    } while (*++p);
    return seen_type && (part_len > 0) && (part_no > 1);
}

int is_snake_case (const char *name) {
    const char *p = name;
    int part_len = 0, seen_us = 0;

    if (!name) return 0;
    if (!*name) return 0;
    if (!islower (*name)) return 0;
    do {
        if (*p == '_') {
            seen_us = 1;
            if (part_len == 0) return 0;
            part_len = 0;
        } else {
            if (!islower (*p) && !isdigit (*p)) return 0;
            part_len++;
        }
    } while (*++p);
    return seen_us && (part_len > 0);
}

int is_camel_case (const char *name) {
    const char *p = name;
    int upper_count = 0, part_no = 0;

    if (!name) return 0;
    if (!*name) return 0;
    if (!*(name + 1)) return 0;
    if (!isupper (*name)) return 0;
    upper_count = 1;
    p++;
    do {
        if (!isalnum (*p)) return 0;
        if (isupper (*p)) {
            if (part_no == 0) {
               if ((upper_count >= 1) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                }
            } else {
               if ((upper_count >= 2) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                }
            }
            upper_count++;
        } else {
            upper_count = 0;
        }
    } while (*++p);
    return (part_no > 0);
}

int is_valid_pname (const char *pname) {
    const char *p = pname;

    if (!pname) return 0;
    if (!*pname) return 0;
    if (!isalpha (*pname)) return 0;
    do {
        if (!isalnum (*p) && (*p != '-') && (*p != '_')) return 0;
    } while (*++p);
    return 1;
}

int is_valid_mname (const char *mname) {
    const char *p = mname;

    if (!mname) return 0;
    if (!*mname) return 0;
    if (!isalpha (*mname)) return 0;
    do {
        if (!isalnum (*p) && (*p != '_')) return 0;
    } while (*++p);
    return 1;
}

char * to_macro_case (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __snake_case_to_macro_case (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __camel_case_to_macro_case (res, name);
    }
    return res;
}

char * to_macro_type (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_case_to_macro_type (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __snake_case_to_macro_type (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __camel_case_to_macro_type (res, name);
    }
    return res;
}

char * to_snake_case (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_case_to_snake_case (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __camel_case_to_snake_case (res, name);
    }
    return res;
}

char * to_camel_case (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_case_to_camel_case (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __snake_case_to_camel_case (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    }
    return res;
}

char * to_macro_case_ct (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_type (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_type_to_macro_case (res, name);
    } else if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __snake_case_to_macro_case (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __camel_case_to_macro_case (res, name);
    }
    return res;
}

char * to_macro_type_ct (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_type (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    } else if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_case_to_macro_type (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __snake_case_to_macro_type (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __camel_case_to_macro_type (res, name);
    }
    return res;
}

char * to_snake_case_ct (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_type (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_type_to_snake_case (res, name);
    } else if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_case_to_snake_case (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __camel_case_to_snake_case (res, name);
    }
    return res;
}

char * to_camel_case_ct (const char *name) {
    char *res = NULL;

    if (!name) return NULL;
    if (!*name) return NULL;
    if (is_macro_type (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_type_to_camel_case (res, name);
    } else if (is_macro_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __macro_case_to_camel_case (res, name);
    } else if (is_snake_case (name)) {
        res = calloc (2, strlen (name) + 3);
        __snake_case_to_camel_case (res, name);
    } else if (is_camel_case (name)) {
        res = calloc (2, strlen (name) + 3);
        strcpy (res, name);
    }
    return res;
}

char * part_modulename (const char *_name) {
    const char *p;
    char *res = NULL;

    if (!_name) return NULL;
    if (!*_name) return NULL;
    if ((p = strchr (_name, '_')) == NULL) return NULL;
    res = calloc (1, (int)(p - _name) + 1);
    strncpy (res, _name, (int)(p - _name));
    return res;
}

char * part_typename (const char *_name) {
    int len;
    const char *p;
    char *res = NULL;

    if (!_name) return NULL;
    if (!*_name) return NULL;
    if ((p = strchr (_name, '_')) == NULL) return NULL;
    p++;
    len = strlen (p);
    if (len == 0) return NULL;
    res = calloc (1, len + 1);
    strncpy (res, p, len);
    return res;
}

char * pname_to_snake (const char *pname) {
    int len;
    char *p, *res;

    if (!pname) return NULL;
    if (!*pname) return NULL;
    len = strlen (pname);
    if (len == 0) return NULL;
    p = res = calloc (1, len + 1);
    while (*pname) {
        if (*pname == '-')
            (*p++ = '_'), pname++;
        else
            *p++ = tolower (*pname++);
    }
    return res;
}

char * pname_to_macro (const char *pname) {
    int len;
    char *p, *res;

    if (!pname) return NULL;
    if (!*pname) return NULL;
    len = strlen (pname);
    if (len == 0) return NULL;
    p = res = calloc (1, len + 1);
    while (*pname) {
        if (*pname == '-')
            (*p++ = '_'), pname++;
        else
            *p++ = toupper (*pname++);
    }
    return res;
}

char * to_basename (const char *_name) {
    int len;
    char *p, *res;

    if (!_name) return NULL;
    if (!*_name) return NULL;
    len = strlen (_name);
    if (len == 0) return NULL;
    p = res = calloc (1, len + 1);
    while (*_name) {
        if (*_name == '_')
            _name++;
        else
            *p++ = tolower (*_name++);
    }
    return res;
}

char * to_dashname (const char *_name) {
    int seen_us = 0, len;
    char *p, *res;

    if (!_name) return NULL;
    if (!*_name) return NULL;
    len = strlen (_name);
    if (len == 0) return NULL;
    p = res = calloc (1, len + 1);
    while (*_name) {
        if (*_name == '_') {
            if (!seen_us) {
                seen_us = 1;
                *p++ = '-';
            }
            _name++;
        } else
            *p++ = tolower (*_name++);
    }
    return res;
}

static void __macro_type_to_macro_case (char *to, const char *from) {
    int type_seen = 0;
    while ((*to++ = toupper (*from++)))
        if (!type_seen && (strncmp (from, "_TYPE_", 6) == 0)) {
            from += 5;
            type_seen = 1;
        }
}

static void __snake_case_to_macro_case (char *to, const char *from) {
    while ((*to++ = toupper (*from++))) ;
}

static void __camel_case_to_macro_case (char *to, const char *from) {
    int upper_count = 0, part_no = 0;

    upper_count = 1;
    *to++ = toupper (*from++);
    do {
        if (isupper (*from)) {
            if (part_no == 0) {
               if ((upper_count >= 1) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                    *to++ = '_';
                }
            } else {
               if ((upper_count >= 2) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                    *to++ = '_';
                }
            }
            upper_count++;
        } else {
            upper_count = 0;
        }
    } while ((*to++ = toupper (*from++)));
}

static void __macro_case_to_macro_type (char *to, const char *from) {
    int part_no = 0;

    do {
        if (!*from) continue;
        if (*from == '_') {
            part_no++;
            if (part_no == 1) {
                strcat (to, "_TYPE");
                to += 5;
            }
        }
    } while ((*to++ = toupper (*from++)));
}

static void __snake_case_to_macro_type (char *to, const char *from) {
    int part_no = 0;

    do {
        if (!*from) continue;
        if (*from == '_') {
            part_no++;
            if (part_no == 1) {
                strcat (to, "_TYPE");
                to += 5;
            }
        }
    } while ((*to++ = toupper (*from++)));
}

static void __camel_case_to_macro_type (char *to, const char *from) {
    int upper_count = 0, part_no = 0, type_ins = 0;

    upper_count = 1;
    *to++ = toupper (*from++);
    do {
        if (isupper (*from)) {
            if (part_no == 0) {
               if ((upper_count >= 1) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                    *to++ = '_';
                }
            } else {
               if ((upper_count >= 2) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                    *to++ = '_';
                }
            }
            if ((part_no == 1) && !type_ins) {
                strcat (to, "TYPE_");
                to += 5;
                type_ins = 1;
            }
            upper_count++;
        } else {
            upper_count = 0;
        }
    } while ((*to++ = toupper (*from++)));
}

static void __macro_case_to_snake_case (char *to, const char *from) {
    while ((*to++ = tolower (*from++))) ;
}

static void __macro_type_to_snake_case (char *to, const char *from) {
    int type_seen = 0;
    while ((*to++ = tolower (*from++)))
        if (!type_seen && (strncmp (from, "_TYPE_", 6) == 0)) {
            from += 5;
            type_seen = 1;
        }
}

static void __camel_case_to_snake_case (char *to, const char *from) {
    int upper_count = 0, part_no = 0;

    upper_count = 1;
    *to++ = tolower (*from++);
    do {
        if (isupper (*from)) {
            if (part_no == 0) {
               if ((upper_count >= 1) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                    *to++ = '_';
                }
            } else {
               if ((upper_count >= 2) || (upper_count == 0)) {
                    upper_count = 0;
                    part_no++;
                    *to++ = '_';
                }
            }
            upper_count++;
        } else {
            upper_count = 0;
        }
    } while ((*to++ = tolower (*from++)));
}

static void __macro_case_to_camel_case (char *to, const char *from) {
    int first_letter = 1;
    do {
        if (*from == '_') {
            from++;
            first_letter = 1;
        }
        if (!*from) continue;
        if (first_letter)
            *to++ = toupper (*from++);
        else
            *to++ = tolower (*from++);
        first_letter = 0;
    } while (*from);
}

static void __macro_type_to_camel_case (char *to, const char *from) {
    int type_seen = 0;
    int first_letter = 1;
    do {
        if (!type_seen && (strncmp (from, "_TYPE_", 6) == 0)) {
            from += 5;
            type_seen = 1;
        }
        if (*from == '_') {
            from++;
            first_letter = 1;
        }
        if (!*from) continue;
        if (first_letter)
            *to++ = toupper (*from++);
        else
            *to++ = tolower (*from++);
        first_letter = 0;
    } while (*from);
}

static void __snake_case_to_camel_case (char *to, const char *from) {
    int first_letter = 1;
    do {
        if (*from == '_') {
            from++;
            first_letter = 1;
        }
        if (!*from) continue;
        if (first_letter)
            *to++ = toupper (*from++);
        else
            *to++ = tolower (*from++);
        first_letter = 0;
    } while (*from);
}
