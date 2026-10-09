#pragma once
#include <glib-object.h>

/* Interface is like type that 
   - is final or extendable,
   - is noninstantiable,
   - have pure virtual methods,
   - have unimplemented properties. */

G_BEGIN_DECLS

#define TUT_TYPE_INAMED (tut_inamed_get_type())

G_DECLARE_INTERFACE(TutINamed, tut_inamed, TUT, INAMED, GObject);

/* Make vtable structure visible for implementors */
struct _TutINamedInterface {
    GTypeInterface parent_iface;
    const gchar* (*vname) (TutINamed *self); /* Used as: iface->vname (instance) */
    void * __vtable_resv[1];                 /* 1 more slot to add virtual methods without breaking ABI */
};

/* Public interface to our object */
const gchar *tut_inamed_vname (TutINamed *self);
const gchar *tut_inamed_get_name (TutINamed *self);
void         tut_inamed_set_name (TutINamed *self, const gchar *name);

G_END_DECLS
