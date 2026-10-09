#include "tut-iknown.h"

static void tut_iknown_default_init (TutIKnownInterface *iface);

G_DEFINE_INTERFACE(TutIKnown, tut_iknown, G_TYPE_OBJECT);

/* custom part of construction/destruction */
static void
tut_iknown_default_init (TutIKnownInterface *iface)
{
    g_print ("entered tut_iknown_default_init()\n");
    g_object_interface_install_property (iface,
        g_param_spec_int ("id",
                          "Id",
                          "This entity id.",
                          G_MININT32,
                          G_MAXINT32,
                          0,
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    iface->vid = NULL; /* pure virtual */
}

/* *** note: we have no implemenation here, public interface is just wrappers *** */

/* public interface */

gint
tut_iknown_vid (TutIKnown *self)
{
    TutIKnownInterface *iface;
    gint res;

    g_print ("entered tut_iknown_vid() [[\n");
    g_return_val_if_fail (TUT_IS_IKNOWN (self), 0);
    iface = TUT_IKNOWN_GET_IFACE (self);
    g_return_val_if_fail (iface->vid != NULL, 0);
    res = iface->vid (self);
    g_print ("]] entered tut_iknown_vid()\n");
    return res;
}

gint
tut_iknown_get_id (TutIKnown *self)
{
    TutIKnownInterface *iface;
    gint res;

    g_print ("entered tut_iknown_get_id() [[\n");
    g_return_val_if_fail (TUT_IS_IKNOWN (self), 0);
    iface = TUT_IKNOWN_GET_IFACE (self);
    g_return_val_if_fail (iface->vid != NULL, 0);
    res = iface->vid (self);
    g_print ("]] entered tut_iknown_get_id()\n");
    return res;
}
