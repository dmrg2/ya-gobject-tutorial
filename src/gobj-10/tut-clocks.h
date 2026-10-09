#pragma once
#include <glib-object.h>

typedef enum _TutClockEnum {
    TUT_CLOCK_REALTIME,
    TUT_CLOCK_MONOTONIC,
    TUT_CLOCK_BOOTTIME,
    TUT_CLOCK_TAI,
    TUT_N_CLOCKS
} TutClockEnum;

#define TUT_TYPE_ENUMCK (tut_enumck_get_type())

GType        tut_enumck_get_type (void);
const gchar *tut_enumck_get_name (gint enumck);
const gchar *tut_enumck_get_nick (gint enumck);
