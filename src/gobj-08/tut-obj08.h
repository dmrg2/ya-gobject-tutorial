#pragma once
#include <glib-object.h>
#include "tut-inamed.h"
#include "tut-iknown.h"
#include "tut-itrusted.h"

/* Final type implementing interfaces, using convenience macros */
/*
   TutObj08 implements INamed, ITrusted
   ITrusted extends IKnown, so TutObj08 implements IKnown too
 */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ08 (tut_obj08_get_type())

G_DECLARE_FINAL_TYPE(TutObj08, tut_obj08, TUT, OBJ08, GObject);

/* Public interface to our object */
TutObj08 *tut_obj08_new (void);
TutObj08 *tut_obj08_new_full (const gchar* name, gint id);

G_END_DECLS
