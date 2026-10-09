#include "tut-flagssm.h"

G_DEFINE_FLAGS_TYPE (TutFlagsSM, tut_flagssm,
    G_DEFINE_ENUM_VALUE (TUT_SMODE_RELATIVE,      "relative"),
    G_DEFINE_ENUM_VALUE (TUT_SMODE_INTERRUPTIBLE, "interruptible"));

static GFlagsClass *cached_fc = NULL;

const gchar* tut_flagssm_get_first_name (guint flagssm)
{
    GFlagsValue *fv;

    if (!cached_fc) cached_fc = (GFlagsClass *) g_type_class_peek(TUT_TYPE_FLAGSSM);
    if (!cached_fc) return NULL;
    fv = g_flags_get_first_value (cached_fc, flagssm);
    if (!fv) return NULL;
    return fv->value_name;
}

const gchar* tut_flagssm_get_first_nick (guint flagssm)
{
    GFlagsValue *fv;

    if (!cached_fc) cached_fc = (GFlagsClass *) g_type_class_peek(TUT_TYPE_FLAGSSM);
    if (!cached_fc) return NULL;
    fv = g_flags_get_first_value (cached_fc, flagssm);
    if (!fv) return NULL;
    return fv->value_nick;
}
