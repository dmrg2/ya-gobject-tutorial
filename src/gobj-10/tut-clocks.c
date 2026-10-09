#include "tut-clocks.h"

G_DEFINE_ENUM_TYPE (TutEnumCk, tut_enumck,
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_REALTIME,  "clock-realtime"),
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_MONOTONIC, "clock-monotonic"),
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_BOOTTIME,  "clock-boottime"),
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_TAI,       "clock-tai"))

static GEnumClass *cached_ec = NULL;

const gchar* tut_enumck_get_name (gint enumck)
{
    GEnumValue *ev;

    if (!cached_ec) cached_ec = (GEnumClass *) g_type_class_peek(TUT_TYPE_ENUMCK);
    if (!cached_ec) return NULL;
    ev = g_enum_get_value (cached_ec, enumck);
    if (!ev) return NULL;
    return ev->value_name;
}

const gchar* tut_enumck_get_nick (gint enumck)
{
    GEnumValue *ev;

    if (!cached_ec) cached_ec = (GEnumClass *) g_type_class_peek(TUT_TYPE_ENUMCK);
    if (!cached_ec) return NULL;
    ev = g_enum_get_value (cached_ec, enumck);
    if (!ev) return NULL;
    return ev->value_nick;
}
