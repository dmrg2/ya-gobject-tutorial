#pragma once
#include <glib-object.h>

/* Simple final type with properties, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ06 (tut_obj06_get_type())

G_DECLARE_FINAL_TYPE(TutObj06, tut_obj06, TUT, OBJ06, GObject);

/* Public interface to our object */
TutObj06    *tut_obj06_new (void);
TutObj06    *tut_obj06_new_with_text (const gchar *text);
TutObj06    *tut_obj06_new_full (gint number, const gchar *text);
/*  ... with property accessors */
void         tut_obj06_set_number (TutObj06 *self, gint number);
gint         tut_obj06_get_number (TutObj06 *self);
void         tut_obj06_set_text (TutObj06 *self, const gchar* text);
const gchar *tut_obj06_get_text (TutObj06 *self);

G_END_DECLS
