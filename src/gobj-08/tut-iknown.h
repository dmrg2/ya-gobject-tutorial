#pragma once
#include <glib-object.h>

/* Interface is like type that 
   - is final or extendable,
   - is noninstantiable,
   - have pure virtual methods,
   - have unimplemented properties. */

G_BEGIN_DECLS

#define TUT_TYPE_IKNOWN (tut_iknown_get_type())

G_DECLARE_INTERFACE(TutIKnown, tut_iknown, TUT, IKNOWN, GObject);

/* Make vtable structure visible for implementors */
struct _TutIKnownInterface {
    GTypeInterface parent_iface;
    gint (*vid) (TutIKnown *self); /* Used as: iface->vid (instance) */
    void * __vtable_resv[1];       /* 1 more slot to add virtual methods without breaking ABI */
};

/* Public interface to our object */
gint tut_iknown_vid (TutIKnown *self);
gint tut_iknown_get_id (TutIKnown *self);

G_END_DECLS
