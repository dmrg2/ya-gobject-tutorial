#pragma once
#include <glib-object.h>

/* Simple final type with properties, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ05 (tut_obj05_get_type())

G_DECLARE_FINAL_TYPE(TutObj05, tut_obj05, TUT, OBJ05, GObject);

/* Public interface to our object */
TutObj05    *tut_obj05_new (void);
TutObj05    *tut_obj05_new_with_text (const gchar *text);
TutObj05    *tut_obj05_new_full (gboolean text_writable, const gchar *text);
/*  ... with property accessors */
void         tut_obj05_set_text_writable (TutObj05 *self, gboolean text_writable);
gboolean     tut_obj05_get_text_writable (TutObj05 *self);
void         tut_obj05_set_text (TutObj05 *self, const gchar* text);
const gchar *tut_obj05_get_text (TutObj05 *self);

G_END_DECLS
