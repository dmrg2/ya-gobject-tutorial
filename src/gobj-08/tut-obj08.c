#include "tut-inamed.h"
#include "tut-iknown.h"
#include "tut-itrusted.h"
#include "tut-obj08.h"

typedef enum {
    PROP_NAME = 1,
    PROP_ID,
    PROP_TRUSTED,
    N_PROPERTIES
} TutObj08PropEnum;

struct _TutObj08 {
    GObject parent_instance;
};

typedef struct _TutObj08Private TutObj08Private;
struct _TutObj08Private {
    gchar *name;
    gint id;
};

static void tut_obj08_class_init (TutObj08Class *klass);
static void tut_obj08_init (TutObj08 *self);
static void tut_obj08_finalize (GObject *gobject);
static void tut_obj08_inamed_init (TutINamedInterface *iface);
static void tut_obj08_iknown_init (TutIKnownInterface *iface);
static void tut_obj08_itrusted_init (TutITrustedInterface *iface);
static const gchar *tut_obj08_inamed_vname_impl (TutObj08 *self);
static gint tut_obj08_iknown_vid_impl (TutObj08 *self);
static gboolean tut_obj08_itrusted_vtrusted_impl (TutObj08 *self);
static void tut_obj08_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void tut_obj08_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

/* interface definition magic is in that '_WITH_CODE' additions */
G_DEFINE_FINAL_TYPE_WITH_CODE(TutObj08, tut_obj08, G_TYPE_OBJECT,
                              G_ADD_PRIVATE (TutObj08)
                              G_IMPLEMENT_INTERFACE (TUT_TYPE_INAMED,
                                                     tut_obj08_inamed_init)
                              G_IMPLEMENT_INTERFACE (TUT_TYPE_IKNOWN,
                                                     tut_obj08_iknown_init)
                              G_IMPLEMENT_INTERFACE (TUT_TYPE_ITRUSTED,
                                                     tut_obj08_itrusted_init));

/* saved property pspecs */

static GParamSpec *tut_obj08_prop_pspec[N_PROPERTIES] = { NULL, };

/* saved interfce class pointers */

static TutINamedInterface *tut_obj08_TutINamedInterface = NULL;
static TutIKnownInterface *tut_obj08_TutIKnownInterface = NULL;
static TutITrustedInterface *tut_obj08_TutITrustedInterface = NULL;

/* custom part of construction/destruction */

static void
tut_obj08_class_init (TutObj08Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_print ("entered tut_obj08_class_init()\n");
    object_class->finalize = tut_obj08_finalize;
    object_class->set_property = tut_obj08_set_property;
    object_class->get_property = tut_obj08_get_property;

    /* actually implement all properties */
    g_object_class_override_property (object_class, PROP_NAME, "name");
    g_object_class_override_property (object_class, PROP_ID, "id");
    g_object_class_override_property (object_class, PROP_TRUSTED, "trusted");
}

static void
tut_obj08_inamed_init (TutINamedInterface *iface)
{
    g_print ("entered tut_obj08_inamed_init()\n");
    tut_obj08_TutINamedInterface = iface;
    iface->vname = (const gchar* (*) (TutINamed *)) tut_obj08_inamed_vname_impl;
    tut_obj08_prop_pspec[PROP_NAME] = g_object_interface_find_property ((GTypeInterface *) iface, "name");
}

static void
tut_obj08_iknown_init (TutIKnownInterface *iface)
{
    g_print ("entered tut_obj08_iknown_init()\n");
    tut_obj08_TutIKnownInterface = iface;
    iface->vid = (gint (*) (TutIKnown *)) tut_obj08_iknown_vid_impl;
    tut_obj08_prop_pspec[PROP_ID] = g_object_interface_find_property ((GTypeInterface *) iface, "id");
}

static void
tut_obj08_itrusted_init (TutITrustedInterface *iface)
{
    g_print ("entered tut_obj08_itrusted_init()\n");
    tut_obj08_TutITrustedInterface = iface;
    iface->vtrusted = (gboolean (*) (TutITrusted *)) tut_obj08_itrusted_vtrusted_impl;
    tut_obj08_prop_pspec[PROP_TRUSTED] = g_object_interface_find_property ((GTypeInterface *) iface, "trusted");
}

static void
tut_obj08_init (TutObj08 *self) {
    TutObj08Private *priv = tut_obj08_get_instance_private (self);

    g_print ("entered tut_obj08_init()\n");
    priv->name = NULL;
    priv->id = 0;
}

static void
tut_obj08_finalize (GObject *gobject)
{
    TutObj08Private *priv = tut_obj08_get_instance_private (TUT_OBJ08 (gobject));

    g_print ("entered tut_obj08_finalize()\n");
    if (priv->name) /* delete on finalize */
    {
        g_print ("\tfreeing\n");
        g_free(priv->name);
        priv->name = NULL;
    }
    G_OBJECT_CLASS (tut_obj08_parent_class)->finalize (gobject);
}

/* interface implementation */

static const gchar *
tut_obj08_inamed_vname_impl (TutObj08 *self)
{
    TutObj08Private *priv;

    g_print ("entered tut_obj08_inamed_vname_impl()\n");
    g_return_val_if_fail (TUT_IS_OBJ08 (self), NULL);
    priv = tut_obj08_get_instance_private (self);
    return (const gchar*) priv->name;
}

static gint
tut_obj08_iknown_vid_impl (TutObj08 *self)
{
    TutObj08Private *priv;

    g_print ("entered tut_obj08_iknown_vid_impl()\n");
    g_return_val_if_fail (TUT_IS_OBJ08 (self), 0);
    priv = tut_obj08_get_instance_private (self);
    return priv->id;
}

static gboolean
tut_obj08_itrusted_vtrusted_impl (TutObj08 *self)
{
    TutObj08Private *priv;

    g_print ("entered tut_obj08_itrusted_vtrusted_impl()\n");
    g_return_val_if_fail (TUT_IS_OBJ08 (self), FALSE);
    priv = tut_obj08_get_instance_private (self);
    return (priv->id && (priv->id < 10));
}

/* property access dispatchers */

static void
tut_obj08_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj08Private *priv;

    g_return_if_fail (TUT_IS_OBJ08 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj08_get_instance_private (TUT_OBJ08 (object));
    switch ((TutObj08PropEnum) property_id)
    {
    case PROP_NAME:
        g_free (priv->name);
        priv->name = g_value_dup_string (value);
        g_print ("\tproperty 'name' set: \"%s\"\n", priv->name);
        break;
    case PROP_ID:
        priv->id = g_value_get_int (value);
        g_print ("\tproperty 'id' set: %d\n", priv->id);
        break;
    case PROP_TRUSTED:
        g_warning ("\tproperty 'trusted' is read-only\n");
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
tut_obj08_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
    TutObj08Private *priv;

    g_return_if_fail (TUT_IS_OBJ08 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj08_get_instance_private (TUT_OBJ08 (object));
    switch ((TutObj08PropEnum) property_id)
    {
    case PROP_NAME:
        g_value_set_string (value, priv->name);
        break;
    case PROP_ID:
        g_value_set_int (value, priv->id);
        break;
    case PROP_TRUSTED:
        /* this is calculated property */
        g_value_set_boolean (value, (priv->id && (priv->id < 10)));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* public interface */
TutObj08 *
tut_obj08_new (void) {
    TutObj08 *self;

    g_print ("entered tut_obj08_new()\n");
    self = g_object_new (TUT_TYPE_OBJ08, NULL);
    return self;
}

TutObj08 *
tut_obj08_new_full (const gchar* name, gint id) {
    TutObj08 *self;

    g_print ("entered tut_obj08_new_full()\n");
    self = g_object_new (TUT_TYPE_OBJ08, "name", name, "id", id, NULL);
    return self;
}

/* END IMPL */
