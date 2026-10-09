#pragma once
#include <glib-object.h>
#include "tut-iknown.h"

/* Interface is like type that 
   - is final or extendable,
   - is noninstantiable,
   - have pure virtual methods,
   - have unimplemented properties. */

G_BEGIN_DECLS

#define TUT_TYPE_ITRUSTED (tut_itrusted_get_type())

G_DECLARE_INTERFACE(TutITrusted, tut_itrusted, TUT, ITRUSTED, TutIKnown);

/* Make vtable structure visible for implementors */
struct _TutITrustedInterface {
    GTypeInterface parent_iface;
    gboolean (*vtrusted) (TutITrusted *self); /* Used as: iface->vtrusted (instance) */
    void * __vtable_resv[1];                  /* 1 more slot to add virtual methods without breaking ABI */
};

/* Public interface to our object */
gboolean tut_itrusted_vtrusted (TutITrusted *self);
gboolean tut_itrusted_get_trusted (TutITrusted *self);

G_END_DECLS
