#pragma once
#include <glib-object.h>
#include "tut-timespec.h"

/* Simple final type with custom signals, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ09 (tut_obj09_get_type())

G_DECLARE_FINAL_TYPE(TutObj09, tut_obj09, TUT, OBJ09, GObject);

/* Public interface to our object */
TutObj09    *tut_obj09_new (void);
TutObj09    *tut_obj09_new_data (TutTimespec tuts);
TutObj09    *tut_obj09_new_rawdata (gint64 tv_sec, gint64 tv_nsec); /* with value collector hack inside */
void         tut_obj09_signal_timespec (TutObj09 *self);
void         tut_obj09_set_timespec (TutObj09 *self, TutTimespec tuts);
TutTimespec  tut_obj09_get_timespec (TutObj09 *self);

G_END_DECLS
