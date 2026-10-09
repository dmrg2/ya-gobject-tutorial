#pragma once
#include <glib-object.h>

/* Simple final type with custom signals, using convenience macros */

G_BEGIN_DECLS

/* We wiil use it as 'detail' for our signal */
#define TUT_OBJ07_DETAIL_STRING "obj07-detail"

#define TUT_TYPE_OBJ07 (tut_obj07_get_type())

G_DECLARE_FINAL_TYPE(TutObj07, tut_obj07, TUT, OBJ07, GObject);

/* Public interface to our object */

TutObj07 *tut_obj07_new (void);
void      tut_obj07_signal_simple (TutObj07 *self);
void      tut_obj07_signal_with_para (TutObj07 *self, gchar *string_para, gint int_para);
void      tut_obj07_signal_with_para_detail (TutObj07 *self, gchar *string_para, gint int_para);

G_END_DECLS
