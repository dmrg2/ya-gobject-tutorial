#pragma once
#include <glib-object.h>
#include "tut-boxedts.h"
#include "tut-clocks.h"
#include "tut-flagssm.h"

/* Simple final type with properties, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ10 (tut_obj10_get_type())

G_DECLARE_FINAL_TYPE(TutObj10, tut_obj10, TUT, OBJ10, GObject);

/* Public interface to our object */
TutObj10     *tut_obj10_new (void);
TutObj10     *tut_obj10_new_with_boxedts (const TutBoxedTS *boxedts);
TutObj10     *tut_obj10_new_full (const TutBoxedTS *boxedts, TutClockEnum enumck, guint /*TutSMFlags*/ flagssm);
gint64        tut_obj10_get_tv_sec (TutObj10 *self);
gint64        tut_obj10_get_tv_nsec (TutObj10 *self);
TutClockEnum  tut_obj10_get_clock (TutObj10 *self);
guint         tut_obj10_get_flags (TutObj10 *self);
void          tut_obj10_do_sleep (TutObj10 *self);

G_END_DECLS
