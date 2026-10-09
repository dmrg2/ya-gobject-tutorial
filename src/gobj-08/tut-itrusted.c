#include "tut-itrusted.h"

static void tut_itrusted_default_init (TutITrustedInterface *iface);

/* base type is TUT_TYPE_IKNOWN, so it is a prerequisite for ITrusted */
G_DEFINE_INTERFACE(TutITrusted, tut_itrusted, TUT_TYPE_IKNOWN);
/* see also g_type_interface_add_prerequisite() */

/* custom part of construction/destruction */

static void
tut_itrusted_default_init (TutITrustedInterface *iface)
{
    g_print ("entered tut_itrusted_default_init()\n");
    g_object_interface_install_property (iface,
        g_param_spec_boolean ("trusted",
                              "Trusted",
                              "Indicates this entity' id is trusted.",
                              FALSE,
                              G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));
    iface->vtrusted = NULL; /* pure virtual */
}

/* *** note: we have no implemenation here, public interface is just wrappers *** */

/* public interface */

gboolean
tut_itrusted_vtrusted (TutITrusted *self)
{
    TutITrustedInterface *iface;
    gboolean res;

    g_print ("entered tut_itrusted_vtrusted() [[\n");
    g_return_val_if_fail (TUT_IS_ITRUSTED (self), FALSE);
    iface = TUT_ITRUSTED_GET_IFACE (self);
    g_return_val_if_fail (iface->vtrusted != NULL, FALSE);
    res = iface->vtrusted (self);
    g_print ("]] tut_itrusted_vtrusted()\n");
    return res;
}

gboolean
tut_itrusted_get_trusted (TutITrusted *self)
{
    TutITrustedInterface *iface;
    gboolean res;

    g_print ("entered tut_itrusted_get_trusted() [[\n");
    g_return_val_if_fail (TUT_IS_ITRUSTED (self), FALSE);
    iface = TUT_ITRUSTED_GET_IFACE (self);
    g_return_val_if_fail (iface->vtrusted != NULL, FALSE);
    res = iface->vtrusted (self);
    g_print ("]] tut_itrusted_get_trusted()\n");
    return res;
}
