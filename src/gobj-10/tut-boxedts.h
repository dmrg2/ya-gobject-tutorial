#pragma once
#include <time.h>
#include <glib-object.h>

typedef struct _TutBoxedTS {
    struct timespec ts;
} TutBoxedTS;

#define TUT_TYPE_BOXEDTS (tut_boxedts_get_type())

GType           tut_boxedts_get_type (void);
TutBoxedTS     *tut_boxedts_new (void);
TutBoxedTS     *tut_boxedts_new_data (gint64 tv_sec, gint64 tv_nsec);
TutBoxedTS     *tut_boxedts_copy (TutBoxedTS *boxedts);
void            tut_boxedts_free (TutBoxedTS *boxedts);
