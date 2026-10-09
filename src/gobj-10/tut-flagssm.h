#pragma once
#include <glib-object.h>

typedef enum _TutSMFlags {
    TUT_SMODE_RELATIVE = 1,
    TUT_SMODE_INTERRUPTIBLE = 2,
    TUT_SMODE_MASK = (TUT_SMODE_RELATIVE | TUT_SMODE_INTERRUPTIBLE)
} TutSMFlags;

#define TUT_TYPE_FLAGSSM (tut_flagssm_get_type())

GType        tut_flagssm_get_type (void);
const gchar *tut_flagssm_get_first_name (guint flagssm);
const gchar *tut_flagssm_get_first_nick (guint flagssm);
