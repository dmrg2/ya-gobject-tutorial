#include "tut-inamed.h"

static void tut_inamed_default_init (TutINamedInterface *iface);

G_DEFINE_INTERFACE(TutINamed, tut_inamed, G_TYPE_OBJECT);

/* custom part of construction/destruction */

static void
tut_inamed_default_init (TutINamedInterface *iface)
{
    g_print ("entered tut_inamed_default_init()\n");
    g_object_interface_install_property (iface,
        g_param_spec_string ("name",
                             "Name",
                             "Arbitrary name.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    iface->vname = NULL; /* pure virtual */
}

/* *** note: we have no implemenation here, public interface is just wrappers *** */

/* public interface */

const gchar*
tut_inamed_vname (TutINamed *self)
{
    TutINamedInterface *iface;
    const gchar *res;

    g_print ("entered tut_inamed_vname() [[\n");
    g_return_val_if_fail (TUT_IS_INAMED (self), NULL);
    iface = TUT_INAMED_GET_IFACE (self);
    g_return_val_if_fail (iface->vname != NULL, NULL);
    res = iface->vname (self);
    g_print ("]] tut_inamed_vname()\n");
    return res;
}

const gchar*
tut_inamed_get_name (TutINamed *self)
{
    TutINamedInterface *iface;
    const gchar *res;

    g_print ("entered tut_inamed_get_name() [[\n");
    g_return_val_if_fail (TUT_IS_INAMED (self), NULL);
    iface = TUT_INAMED_GET_IFACE (self);
    g_return_val_if_fail (iface->vname != NULL, NULL);
    res = iface->vname (self);
    g_print ("]] tut_inamed_get_name()\n");
    return res;
}

void
tut_inamed_set_name (TutINamed *self, const gchar *name)
{
    GValue gval = G_VALUE_INIT;

    g_print ("entered tut_inamed_set_name() [[\n");
    g_return_if_fail (TUT_IS_INAMED (self));
    g_value_init (&gval, G_TYPE_STRING);
    g_value_set_string (&gval, name);
    g_object_set_property (G_OBJECT (self), "name", &gval);
    g_value_unset (&gval);
    g_print ("]] tut_inamed_set_name()\n");
}
