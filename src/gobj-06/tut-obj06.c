#include "tut-obj06.h"

typedef enum
{
    PROP_NUMBER = 1,
    PROP_TEXT,
    N_PROPERTIES
} TutObj06PropEnum;

struct _TutObj06 {
    GObject parent_instance;
};

typedef struct _TutObj06Private TutObj06Private;
struct _TutObj06Private {
    gboolean number;
    gchar *text;
};

static void tut_obj06_class_init (TutObj06Class *klass);
static void tut_obj06_init (TutObj06 *self);
static void tut_obj06_finalize (GObject *gobject);
static void tut_obj06_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void tut_obj06_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

G_DEFINE_FINAL_TYPE_WITH_PRIVATE(TutObj06, tut_obj06, G_TYPE_OBJECT);

/* saved property pspecs */

static GParamSpec *tut_obj06_prop_pspec[N_PROPERTIES] = { NULL, };

/* custom part of construction/destruction */

static void
tut_obj06_class_init (TutObj06Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_print ("entered tut_obj06_class_init()\n");
    object_class->finalize = tut_obj06_finalize;
    object_class->set_property = tut_obj06_set_property;
    object_class->get_property = tut_obj06_get_property;
    tut_obj06_prop_pspec[PROP_NUMBER] =
        g_param_spec_int     ("number",
                              "Number",
                              "Property holding arbitrary signed integer.",
                              G_MININT32,
                              G_MAXINT32,
                              0,
                              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj06_prop_pspec[PROP_TEXT] =
        g_param_spec_string  ("text",
                              "Text",
                              "Property holding arbitrary text.",
                              NULL,
                              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj06_prop_pspec);
}

static void
tut_obj06_init (TutObj06 *self) {
    TutObj06Private *priv = tut_obj06_get_instance_private (self);

    g_print ("entered tut_obj06_init()\n");
    priv->number = 0;
    priv->text = NULL;
}

static void
tut_obj06_finalize (GObject *gobject)
{
    TutObj06Private *priv = tut_obj06_get_instance_private (TUT_OBJ06 (gobject));

    g_print ("entered tut_obj06_finalize()\n");
    if (priv->text) /* delete on finalize */
    {
        g_print ("\tfreeing\n");
        g_free(priv->text);
        priv->text = NULL;
    }
    G_OBJECT_CLASS (tut_obj06_parent_class)->finalize (gobject);
}

/* property access dispatchers */

static void
tut_obj06_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj06Private *priv;

    g_return_if_fail (TUT_IS_OBJ06 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj06_get_instance_private (TUT_OBJ06 (object));
    switch ((TutObj06PropEnum) property_id)
    {
    case PROP_NUMBER:
        priv->number = g_value_get_int (value);
        g_print ("\tproperty 'number' set: %d\n", priv->number);
        break;
    case PROP_TEXT:
        g_free (priv->text);
        priv->text = g_value_dup_string (value);
        g_print ("\tproperty 'text' set: \"%s\"\n", priv->text);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
tut_obj06_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
    TutObj06Private *priv;

    g_return_if_fail (TUT_IS_OBJ06 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj06_get_instance_private (TUT_OBJ06 (object));
    switch ((TutObj06PropEnum) property_id)
    {
    case PROP_NUMBER:
        g_value_set_int (value, priv->number);
        break;
    case PROP_TEXT:
        g_value_set_string (value, priv->text);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* public interface */

TutObj06 *
tut_obj06_new (void)
{
    TutObj06 *self;

    g_print("entered tut_obj06_new()\n");
    self = g_object_new (TUT_TYPE_OBJ06, NULL);
    return self;
}

TutObj06 *
tut_obj06_new_with_text (const gchar *text)
{
    TutObj06 *self;
    TutObj06Private *priv;

    g_print("entered tut_obj06_new_with_text()\n");
    self = g_object_new (TUT_TYPE_OBJ06, "text", text, NULL);
    priv = tut_obj06_get_instance_private (self);
    priv->number = 0;
    return self;
}

TutObj06 *
tut_obj06_new_full (gint number, const gchar *text)
{
    TutObj06 *self;

    g_print("entered tut_obj06_new_full()\n");
    self = (TutObj06 *) g_object_new (TUT_TYPE_OBJ06, "number", number, "text", text, NULL);
    return self;
}

void
tut_obj06_set_number (TutObj06 *self, gint number)
{
    TutObj06Private *priv;

    g_print ("entered tut_obj06_set_number()\n");
    g_return_if_fail (TUT_IS_OBJ06 (self));
    priv = tut_obj06_get_instance_private (self);
    priv->number = number;
    g_object_notify_by_pspec(G_OBJECT (self), tut_obj06_prop_pspec[PROP_NUMBER]);
}

gint
tut_obj06_get_number (TutObj06 *self)
{
    TutObj06Private *priv;

    g_print ("entered tut_obj06_get_number()\n");
    g_return_val_if_fail (TUT_IS_OBJ06 (self), FALSE);
    priv = tut_obj06_get_instance_private (self);
    return priv->number;
}

void
tut_obj06_set_text (TutObj06 *self, const gchar* text)
{
    TutObj06Private *priv;

    g_print ("entered tut_obj06_set_text()\n");
    g_return_if_fail (TUT_IS_OBJ06 (self));
    priv = tut_obj06_get_instance_private (self);
    g_free (priv->text);
    priv->text = g_strdup (text);
    g_object_notify_by_pspec(G_OBJECT (self), tut_obj06_prop_pspec[PROP_TEXT]);
}

const gchar*
tut_obj06_get_text (TutObj06 *self)
{
    TutObj06Private *priv;

    g_print ("entered tut_obj06_get_text()\n");
    g_return_val_if_fail (TUT_IS_OBJ06 (self), NULL);
    priv = tut_obj06_get_instance_private (self);
    return (const gchar*) priv->text;
}
