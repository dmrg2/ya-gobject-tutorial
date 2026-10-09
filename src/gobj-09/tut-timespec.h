#pragma once
#include <glib-object.h>

#define TUT_TYPE_TIMESPEC (tut_timespec_get_type ())
#define TUT_PARAM_TYPE_TIMESPEC (tut_timespec_get_param_type ())

/* boxed-by-value, no allocs */

typedef struct _TutTimespec {
    gint64 tv_sec;
    gint64 tv_nsec;
} TutTimespec;

GType                      tut_timespec_get_type (void);
GType                      tut_timespec_get_param_type (void);
GParamSpec                *g_param_spec_timespec (const gchar* name,
                                                  const gchar* nick,
                                                  const gchar* blurb,
                                                  GParamFlags flags);
static inline TutTimespec *tut_timespec_new (void)
    { return (TutTimespec *) g_malloc0 (sizeof (TutTimespec)); }
static inline TutTimespec *tut_timespec_new_data (gint64 tv_sec, gint64 tv_nsec)
    { TutTimespec *tuts = (TutTimespec *) g_malloc (sizeof (TutTimespec));
      if (tuts) { tuts->tv_sec = tv_sec; tuts->tv_nsec = tv_nsec; } return tuts; }
static inline void         tut_timespec_delete (TutTimespec *tuts)
    { g_free (tuts); }
static inline TutTimespec  g_value_get_tut_timespec (const GValue *gval)
    { TutTimespec res = {0, 0}; g_return_val_if_fail(G_VALUE_HOLDS (gval, TUT_TYPE_TIMESPEC), res);
      res.tv_sec = gval->data[0].v_int64; res.tv_nsec = gval->data[1].v_int64; return res; }
static inline void         g_value_set_tut_timespec (GValue *gval, TutTimespec tuts)
    { g_return_if_fail(G_VALUE_HOLDS (gval, TUT_TYPE_TIMESPEC)); gval->data[0].v_int64 = tuts.tv_sec;
      gval->data[1].v_int64 = tuts.tv_nsec; }

/* See also: g_new0 (TutTimespec, n_timespecs); ... */
