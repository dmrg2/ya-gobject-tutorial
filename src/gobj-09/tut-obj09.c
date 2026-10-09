#include "tut-obj09.h"

typedef enum {
    SIGNAL_TIMESPEC,
    N_SIGNALS
} Obj09SignalEnum;

typedef enum
{
    PROP_TIMESPEC = 1,
    N_PROPERTIES
} TutObj09PropEnum;

struct _TutObj09 {
    GObject parent_instance;
};

typedef struct _TutObj09Private TutObj09Private;
struct _TutObj09Private {
    TutTimespec timespec;
};

static void tut_obj09_class_init (TutObj09Class *klass);
static void tut_obj09_init (TutObj09 *self);
static void tut_obj09_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void tut_obj09_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

G_DEFINE_FINAL_TYPE_WITH_PRIVATE(TutObj09, tut_obj09, G_TYPE_OBJECT);

/* saved signal ids */

static guint tut_obj09_signal_id[N_SIGNALS] = {0};

/* saved property pspecs */

static GParamSpec *tut_obj09_prop_pspec[N_PROPERTIES] = { NULL, };

/* custom part of construction/destruction */

static void tut_obj09_timespec_marshaller(GClosure *closure, GValue *return_value, guint n_param_values, const GValue *param_values, gpointer invocation_hint, gpointer marshal_data);

static void
tut_obj09_class_init (TutObj09Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GType signal_param_types[] = {TUT_TYPE_TIMESPEC};

    g_print ("entered tut_obj09_class_init()\n");
    object_class->set_property = tut_obj09_set_property;
    object_class->get_property = tut_obj09_get_property;
    tut_obj09_prop_pspec[PROP_TIMESPEC] =
        g_param_spec_timespec ("timespec",
                               "Timespec",
                               "Property holding classic struct timespec.",
                               G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj09_prop_pspec);
    tut_obj09_signal_id[SIGNAL_TIMESPEC] =
        g_signal_newv ("timespec",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       tut_obj09_timespec_marshaller, /* we need custom marshaller for
                                                         our non-ffi type */
                       G_TYPE_NONE /* return_type */,
                       1    /* n_params */,
                       signal_param_types);
    /* We can also decompose timespec to two int64 primitives in 'signal_param_types'
       and use generic marshaller out of the box, as we did it in tut_obj09_new_rawdata().
       This hack works too. */
}

/* custom callback-specific marshaller */
static void
tut_obj09_timespec_marshaller(GClosure     *closure,
                              GValue       *return_value,
                              guint         n_param_values,
                              const GValue *param_values,
                              gpointer      invocation_hint,
                              gpointer      marshal_data)
{
    typedef void (*tut_obj09_timespec_callback) (TutObj09 *object, TutTimespec tuts, gpointer user_data);

    /* assume marshal_data may contain callback address only (as it is usual with simple signal) */

    GCClosure *C_closure;
    TutObj09 *object;
    gpointer user_data;
    TutTimespec tuts;
    tut_obj09_timespec_callback callback;

    g_print ("entered tut_obj09_timespec_marshaller()\n");
    C_closure = (GCClosure*) closure;
    callback = marshal_data ?
               (tut_obj09_timespec_callback) marshal_data :
               (tut_obj09_timespec_callback) C_closure->callback;
    tuts = g_value_get_tut_timespec (&param_values[1]);
    if (G_CCLOSURE_SWAP_DATA (C_closure))
    {
        user_data = g_value_peek_pointer (&param_values[0]);
        object = (TutObj09 *) closure->data;
    }
    else
    {
        object = (TutObj09 *) g_value_peek_pointer (&param_values[0]);
        user_data = closure->data;
    }
    callback (object, tuts, user_data);
}

static void
tut_obj09_init (TutObj09 *self) {
    TutObj09Private *priv = tut_obj09_get_instance_private (self);

    g_print ("entered tut_obj09_init()\n");
    priv->timespec.tv_nsec = priv->timespec.tv_sec = 0;
}

/* property access dispatchers */

static void
tut_obj09_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj09Private *priv;

    g_return_if_fail (TUT_IS_OBJ09 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj09_get_instance_private (TUT_OBJ09 (object));
    switch ((TutObj09PropEnum) property_id)
    {
    case PROP_TIMESPEC:
        priv->timespec = g_value_get_tut_timespec (value);
        g_print ("property timespec set: {%ld, %ld}\n", priv->timespec.tv_sec, priv->timespec.tv_nsec);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
tut_obj09_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
    TutObj09Private *priv;

    g_return_if_fail (TUT_IS_OBJ09 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj09_get_instance_private (TUT_OBJ09 (object));
    switch ((TutObj09PropEnum) property_id)
    {
    case PROP_TIMESPEC:
        g_value_set_tut_timespec (value, priv->timespec);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* public interface */

TutObj09 *
tut_obj09_new (void) {
    TutObj09 *self;

    g_print ("entered tut_obj09_new()\n");
    self = (TutObj09 *) g_object_new (TUT_TYPE_OBJ09, NULL);
    return self;
}

TutObj09 *
tut_obj09_new_data (TutTimespec tuts)
{
    TutObj09 *self;

    g_print ("entered tut_obj09_new_data()\n");
    self = (TutObj09 *) g_object_new (TUT_TYPE_OBJ09, "timespec", tuts, NULL);
    return self;
}

TutObj09 *
tut_obj09_new_rawdata (gint64 tv_sec, gint64 tv_nsec)
{
    TutObj09 *self;

    g_print ("entered tut_obj09_new_rawdata()\n");
    /* This hack works because TutTimespec 'collect_format' set to "qq". Underlying value
       collector of GObject library collects structure as two distict int64 primitives. */
    self = (TutObj09 *) g_object_new (TUT_TYPE_OBJ09, "timespec", tv_sec, tv_nsec, NULL);
    return self;
}

void
tut_obj09_signal_timespec (TutObj09 *self)
{
    TutObj09Private *priv;

    g_print ("entered tut_obj09_signal_timespec()\n");
    g_return_if_fail (TUT_IS_OBJ09 (self));
    priv = tut_obj09_get_instance_private (self);
    g_signal_emit (G_OBJECT (self), tut_obj09_signal_id[SIGNAL_TIMESPEC], 0, priv->timespec);
}

void
tut_obj09_set_timespec (TutObj09 *self, TutTimespec timespec)
{
    TutObj09Private *priv;

    g_print ("entered tut_obj09_set_timespec()\n");
    g_return_if_fail (TUT_IS_OBJ09 (self));
    priv = tut_obj09_get_instance_private (self);
    priv->timespec = timespec;
    g_object_notify_by_pspec(G_OBJECT (self), tut_obj09_prop_pspec[PROP_TIMESPEC]);
}

TutTimespec
tut_obj09_get_timespec (TutObj09 *self)
{
    TutObj09Private *priv;
    TutTimespec badres = {0};

    g_print ("entered tut_obj09_get_timespec()\n");
    g_return_val_if_fail (TUT_IS_OBJ09 (self), badres);
    priv = tut_obj09_get_instance_private (self);
    return priv->timespec;
}
