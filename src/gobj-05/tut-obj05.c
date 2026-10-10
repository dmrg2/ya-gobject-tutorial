#include "tut-obj05.h"

typedef enum
{
    PROP_TEXT_WRITABLE = 1,
    PROP_TEXT,
    N_PROPERTIES
} TutObj05PropEnum;

struct _TutObj05 {
    GObject parent_instance;
};

typedef struct _TutObj05Private TutObj05Private;
struct _TutObj05Private {
    gboolean text_writable;
    gchar *text;
};

static void tut_obj05_class_init (TutObj05Class *klass);
static void tut_obj05_init (TutObj05 *self);
static void tut_obj05_finalize (GObject *gobject);
static void tut_obj05_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void tut_obj05_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

G_DEFINE_FINAL_TYPE_WITH_PRIVATE (TutObj05, tut_obj05, G_TYPE_OBJECT);

/* saved property pspecs */

static GParamSpec *tut_obj05_prop_pspec[N_PROPERTIES] = { NULL, };

/* custom part of construction/destruction */

static void
tut_obj05_class_init (TutObj05Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_print ("entered tut_obj05_class_init()\n");
    object_class->finalize = tut_obj05_finalize;
    object_class->set_property = tut_obj05_set_property;
    object_class->get_property = tut_obj05_get_property;
    tut_obj05_prop_pspec[PROP_TEXT_WRITABLE] =
        g_param_spec_boolean ("text-writable",
                              "Text is writable",
                              "Flag indicating that 'text' property currently may be written to.",
                              TRUE, /* default value */
                              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj05_prop_pspec[PROP_TEXT] =
        g_param_spec_string  ("text",
                              "Text",
                              "Property holding arbitrary text.",
                              NULL, /* default value */
                              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj05_prop_pspec);
}

static void
tut_obj05_init (TutObj05 *self) {
    TutObj05Private *priv = tut_obj05_get_instance_private (self);

    g_print ("entered tut_obj05_init()\n");
    priv->text_writable = TRUE;
    priv->text = NULL;
}

static void
tut_obj05_finalize (GObject *gobject)
{
    TutObj05Private *priv = tut_obj05_get_instance_private (TUT_OBJ05 (gobject));

    g_print ("entered tut_obj05_finalize()\n");
    if (priv->text) /* delete on finalize */
    {
        g_print ("\tfreeing\n");
        g_free(priv->text);
        priv->text = NULL;
    }
    G_OBJECT_CLASS (tut_obj05_parent_class)->finalize (gobject);
}

/* property access dispatchers */

static void
tut_obj05_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj05Private *priv;

    g_return_if_fail (TUT_IS_OBJ05 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj05_get_instance_private (TUT_OBJ05 (object));
    switch ((TutObj05PropEnum) property_id)
    {
    case PROP_TEXT_WRITABLE:
        priv->text_writable = g_value_get_boolean (value);
        g_print ("\tproperty 'text-writable' set: %d\n", priv->text_writable);
        break;
    case PROP_TEXT:
        if (!priv->text_writable) {
            /* Note: usually this situation is not programming error, but application logic
               flaw. We should handle such situalions somewhere in calling code. There is
               place where we use GError-based error reporting, gboolean results and so on. */
            g_warning("property 'text' curenntly is read-only");
            break;
        }
        g_free (priv->text);
        priv->text = g_value_dup_string (value);
        g_print ("\tproperty text set: \"%s\"\n", priv->text);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
tut_obj05_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
    TutObj05Private *priv;

    g_return_if_fail (TUT_IS_OBJ05 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj05_get_instance_private (TUT_OBJ05 (object));
    switch ((TutObj05PropEnum) property_id)
    {
    case PROP_TEXT_WRITABLE:
        g_value_set_boolean (value, priv->text_writable);
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
TutObj05 *
tut_obj05_new (void)
{
    TutObj05 *self;

    g_print("entered tut_obj05_new()\n");
    self = (TutObj05 *) g_object_new (TUT_TYPE_OBJ05, NULL);
    return self;
}

TutObj05 *
tut_obj05_new_with_text (const gchar *text)
{
    TutObj05 *self;
    TutObj05Private *priv;

    g_print("entered tut_obj05_new_with_text()\n");
    self = g_object_new (TUT_TYPE_OBJ05, NULL);
    priv = tut_obj05_get_instance_private (self);
    priv->text = g_strdup (text);
    return self;
}

TutObj05 *
tut_obj05_new_full (gboolean text_writable, const gchar *text)
{
    TutObj05 *self;
    TutObj05Private *priv;

    g_print("entered tut_obj05_new_full()\n");
    self = g_object_new (TUT_TYPE_OBJ05, NULL);
    priv = tut_obj05_get_instance_private (self);
    priv->text_writable = text_writable;
    priv->text = g_strdup (text);
    return self;
}

void
tut_obj05_set_text_writable (TutObj05 *self, gboolean text_writable)
{
    TutObj05Private *priv;

    g_print ("entered tut_obj05_set_text_writable()\n");
    g_return_if_fail (TUT_IS_OBJ05 (self));
    priv = tut_obj05_get_instance_private (self);
    priv->text_writable = text_writable;
    g_object_notify_by_pspec(G_OBJECT (self), tut_obj05_prop_pspec[PROP_TEXT_WRITABLE]);
}

gboolean
tut_obj05_get_text_writable (TutObj05 *self)
{
    TutObj05Private *priv;

    g_print ("entered tut_obj05_get_text_writable()\n");
    g_return_val_if_fail (TUT_IS_OBJ05 (self), FALSE);
    priv = tut_obj05_get_instance_private (self);
    return priv->text_writable;
}

void
tut_obj05_set_text (TutObj05 *self, const gchar* text)
{
    TutObj05Private *priv;

    g_print ("entered tut_obj05_set_text()\n");
    g_return_if_fail (TUT_IS_OBJ05 (self));
    priv = tut_obj05_get_instance_private (self);
    if(!priv->text_writable)
    {
        /* Note: usually this situation is not programming error, but application logic
           flaw. We should handle such situalions somewhere in calling code. There is
           place where we use GError-based error reporting, gboolean results and so on. */
        g_warning("property 'text' curenntly is read-only");
        return;
    }
    g_free (priv->text);
    priv->text = g_strdup (text);
    g_object_notify_by_pspec(G_OBJECT (self), tut_obj05_prop_pspec[PROP_TEXT]);
}

const gchar *
tut_obj05_get_text (TutObj05 *self)
{
    TutObj05Private *priv;

    g_print ("entered tut_obj05_get_text()\n");
    g_return_val_if_fail (TUT_IS_OBJ05 (self), NULL);
    priv = tut_obj05_get_instance_private (self);
    return (const gchar*) priv->text;
}
