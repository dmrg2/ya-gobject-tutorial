#include "tut-obj10.h"

typedef enum
{
    PROP_BOXEDTS = 1,
    PROP_CLOCK,
    PROP_FLAGS,
    N_PROPERTIES
} TutObj10PropEnum;

typedef enum {
    SIGNAL_SLEEP,
    N_SIGNALS
} TutObj10SignalEnum;

struct _TutObj10 {
    GObject parent_instance;
};

typedef struct _TutObj10Private TutObj10Private;
struct _TutObj10Private {
    TutBoxedTS *boxedts;
    TutClockEnum clock;
    guint flags;
};

static void tut_obj10_class_init (TutObj10Class *klass);
static void tut_obj10_init (TutObj10 *self);
static void tut_obj10_finalize (GObject *gobject);
static void tut_obj10_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void tut_obj10_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

G_DEFINE_FINAL_TYPE_WITH_PRIVATE(TutObj10, tut_obj10, G_TYPE_OBJECT);

/* saved property pspecs */

static GParamSpec *tut_obj10_prop_pspec[N_PROPERTIES] = { NULL, };

/* saved signal ids */

static guint tut_obj10_signal_id[N_SIGNALS] = {0};

/* custom part of construction/destruction */

static void
tut_obj10_class_init (TutObj10Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GType signal_param_types[] = {TUT_TYPE_BOXEDTS, TUT_TYPE_ENUMCK, TUT_TYPE_FLAGSSM};

    g_print ("entered tut_obj10_class_init()\n");
    object_class->finalize = tut_obj10_finalize;
    object_class->set_property = tut_obj10_set_property;
    object_class->get_property = tut_obj10_get_property;
    tut_obj10_prop_pspec[PROP_BOXEDTS] =
        g_param_spec_boxed ("boxedts",
                            "Boxed timespec",
                            "Property contains boxed struct timespec.",
                            TUT_TYPE_BOXEDTS,
                            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj10_prop_pspec[PROP_CLOCK] =
        g_param_spec_enum  ("clock",
                            "Clock",
                            "Property contains clock type.",
                            TUT_TYPE_ENUMCK,
                            TUT_CLOCK_REALTIME,
                            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj10_prop_pspec[PROP_FLAGS] =
        g_param_spec_flags ("flags",
                            "Flags",
                            "Property contains sleep mode flags.",
                            TUT_TYPE_FLAGSSM,
                            0, /* no flags set */
                            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj10_prop_pspec);
    tut_obj10_signal_id[SIGNAL_SLEEP] =
        g_signal_newv ("sleep",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       NULL /* C marshaller */,
                       G_TYPE_NONE /* return_type */,
                       G_N_ELEMENTS(signal_param_types) /* n_params */,
                       signal_param_types  /* param_types */);
}

static void
tut_obj10_init (TutObj10 *self) {
    TutObj10Private *priv = tut_obj10_get_instance_private (self);

    g_print ("entered tut_obj10_init()\n");
    priv->boxedts = NULL;
}

static void
tut_obj10_finalize (GObject *gobject)
{
    TutObj10Private *priv = tut_obj10_get_instance_private (TUT_OBJ10 (gobject));

    g_print ("entered tut_obj10_finalize()\n");
    if (priv->boxedts) /* delete on finalize */
    {
        g_print ("\tfreeing 'boxedts'\n");
        tut_boxedts_free(priv->boxedts);
        priv->boxedts = NULL;
    }
    G_OBJECT_CLASS (tut_obj10_parent_class)->finalize (gobject);
}

/* property access dispatchers */

static void
tut_obj10_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj10Private *priv;

    g_return_if_fail (TUT_IS_OBJ10 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj10_get_instance_private (TUT_OBJ10 (object));
    switch ((TutObj10PropEnum) property_id)
    {
    case PROP_BOXEDTS:
        if (priv->boxedts) tut_boxedts_free(priv->boxedts);
        priv->boxedts = g_value_dup_boxed (value);
        if (priv->boxedts)
        {
            g_print ("\tproperty 'boxedts' set: {%ld, %ld}\n", priv->boxedts->ts.tv_sec, priv->boxedts->ts.tv_nsec);
        }
        else
        {
            g_print ("\tproperty 'boxedts' set: (null)\n");
        }
        break;
    case PROP_CLOCK:
        g_return_if_fail (g_value_get_enum (value) < TUT_N_CLOCKS);
        priv->clock = g_value_get_enum (value);
        g_print ("\tproperty 'clock' set: %s\n", tut_enumck_get_nick (priv->clock));
        break;
    case PROP_FLAGS:
        g_return_if_fail ((g_value_get_flags (value) & ~TUT_SMODE_MASK) == 0);
        priv->flags = g_value_get_flags (value);
        if (!priv->flags)
        {
            g_print ("\tproperty 'flags' set: (none)\n");
        }
        else
        {
            g_print ("\tproperty 'flags' set: [ ");
            if (priv->flags & TUT_SMODE_RELATIVE) g_print ("%s ", tut_flagssm_get_first_nick (TUT_SMODE_RELATIVE));
            if (priv->flags & TUT_SMODE_INTERRUPTIBLE) g_print ("%s ", tut_flagssm_get_first_nick (TUT_SMODE_INTERRUPTIBLE));
            g_print ("]\n");
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
tut_obj10_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
    TutObj10Private *priv;

    g_return_if_fail (TUT_IS_OBJ10 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj10_get_instance_private (TUT_OBJ10 (object));
    switch ((TutObj10PropEnum) property_id)
    {
    case PROP_BOXEDTS:
        g_value_set_boxed (value, priv->boxedts);
        break;
    case PROP_CLOCK:
        g_value_set_enum (value, priv->clock);
        break;
    case PROP_FLAGS:
        g_value_set_flags (value, priv->flags);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* public interface */

TutObj10 *
tut_obj10_new (void)
{
    TutObj10 *self;

    g_print("entered tut_obj10_new()\n");
    self = g_object_new (TUT_TYPE_OBJ10, NULL);
    return self;
}

TutObj10 *
tut_obj10_new_with_boxedts (const TutBoxedTS *boxedts)
{
    TutObj10 *self;

    g_print("entered tut_obj10_new_with_boxedts()\n");
    self = g_object_new (TUT_TYPE_OBJ10, "boxedts", boxedts, NULL);
    return self;
}

TutObj10 *
tut_obj10_new_full (const TutBoxedTS *boxedts, TutClockEnum enumck, guint /*TutSMFlags*/ flagssm)
{
    TutObj10 *self;

    g_print("entered tut_obj10_new_full()\n");
    self = g_object_new (TUT_TYPE_OBJ10, "boxedts", boxedts, "clock", enumck, "flags", flagssm, NULL);
    return self;
}

gint64
tut_obj10_get_tv_sec (TutObj10 *self)
{
    g_print("entered tut_obj10_get_tv_sec()\n");
    g_return_val_if_fail (TUT_IS_OBJ10 (self), 0);
    return G_PRIVATE_FIELD (TutObj10, self, TutBoxedTS *, boxedts)->ts.tv_sec;
}

gint64
tut_obj10_get_tv_nsec (TutObj10 *self)
{
    g_print("entered tut_obj10_get_tv_nsec()\n");
    g_return_val_if_fail (TUT_IS_OBJ10 (self), 0);
    return G_PRIVATE_FIELD (TutObj10, self, TutBoxedTS *, boxedts)->ts.tv_nsec;
}

TutClockEnum
tut_obj10_get_clock (TutObj10 *self)
{
    g_print("entered tut_obj10_get_clock()\n");
    g_return_val_if_fail (TUT_IS_OBJ10 (self), 0);
    return G_PRIVATE_FIELD (TutObj10, self, TutClockEnum, clock);
}

guint
tut_obj10_get_flags (TutObj10 *self)
{
    g_print("entered tut_obj10_get_flags()\n");
    g_return_val_if_fail (TUT_IS_OBJ10 (self), 0);
    return G_PRIVATE_FIELD (TutObj10, self, guint, flags);
}

void
tut_obj10_do_sleep (TutObj10 *self)
{
    TutObj10Private *priv;

    g_print("entered tut_obj10_do_sleep()\n");
    g_return_if_fail (TUT_IS_OBJ10 (self));
    priv = tut_obj10_get_instance_private (self);
    g_return_if_fail (priv->boxedts != NULL);
    g_signal_emit (G_OBJECT (self), tut_obj10_signal_id[SIGNAL_SLEEP], 0, priv->boxedts, priv->clock, priv->flags);
}
