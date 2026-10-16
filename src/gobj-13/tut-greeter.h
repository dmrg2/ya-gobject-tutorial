#pragma once
#include <glib-object.h>

/* Simple extendable type with virtual methods, using convenience macros */

G_BEGIN_DECLS

#define TUT_TYPE_GREETER (tut_greeter_get_type ())

G_DECLARE_DERIVABLE_TYPE (TutGreeter, tut_greeter, TUT, GREETER, GObject);

/* Make vtable structure visible for possible descendants */
struct _TutGreeterClass {
    GObjectClass parent_class;
    void (*hello) (TutGreeter  *self); /* Used as: class->hello (instance) */
    void *__vtable_resv[3];            /* 3 more slots to add virtual methods without breaking ABI */
};

/* Public interface */
void tut_greeter_hello (TutGreeter *self);

G_END_DECLS
