#pragma once
#include <glib-object.h>

/* Simple extendable type with virtual methods, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_OBJ04 (tut_obj04_get_type())

G_DECLARE_DERIVABLE_TYPE(TutObj04, tut_obj04, TUT, OBJ04, GObject);

/* Public interface to our object */
TutObj04 *tut_obj04_new (void);
void tut_obj04_vwhoami (TutObj04 *self);
void tut_obj04_vpure (TutObj04 *self);

/* Make vtable structure visible for possible descendants */
struct _TutObj04Class {
    GObjectClass parent_class;
    void (*vwhoami) (TutObj04  *self); /* Used as: class->vwhoami (instance) */
    void (*vpure) (TutObj04  *self);   /* Used as: class->vpure (instance) */
    void *__vtable_resv[2];            /* 2 more slots to add virtual methods without breaking ABI */
};

G_END_DECLS
