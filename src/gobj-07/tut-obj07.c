#include <stdio.h>
#include "tut-obj07.h"

typedef enum {
    SIGNAL_SIMPLE,
    SIGNAL_PARA,
    N_SIGNALS
} TutObj07SignalEnum;

struct _TutObj07 {
    GObject parent_instance;
};

static void tut_obj07_class_init (TutObj07Class *klass);
static void tut_obj07_init (TutObj07 *self);

G_DEFINE_TYPE(TutObj07, tut_obj07, G_TYPE_OBJECT);

/* saved signal ids */

static guint tut_obj07_signal_id[N_SIGNALS] = {0};

/* saved detail quark for 'signal-para' */

static GQuark tut_obj07_detail_quark = 0;

/* custom part of construction/destruction */

static void
tut_obj07_class_init (TutObj07Class *klass) {
    GType signal_param_types[] = {G_TYPE_STRING, G_TYPE_INT};

    g_print ("entered tut_obj07_class_init()\n");
    tut_obj07_signal_id[SIGNAL_SIMPLE] =
        g_signal_newv ("signal-simple",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       NULL /* C marshaller */,
                       G_TYPE_NONE /* return_type */,
                       0    /* n_params */,
                       NULL /* param_types */);
    tut_obj07_signal_id[SIGNAL_PARA] =
        g_signal_newv ("signal-para",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS | G_SIGNAL_DETAILED,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       NULL /* C marshaller */,
                       G_TYPE_NONE /* return_type */,
                       G_N_ELEMENTS(signal_param_types) /* n_params */,
                       signal_param_types  /* param_types */);
    tut_obj07_detail_quark = g_quark_from_static_string (TUT_OBJ07_DETAIL_STRING);
}

static void
tut_obj07_init (TutObj07 *self) {
    g_print ("entered tut_obj07_init()\n");
    (void)self;
}

/* public interface */
TutObj07 *
tut_obj07_new (void) {
    TutObj07 *self;

    g_print ("entered tut_obj07_new()\n");
    self = (TutObj07 *) g_object_new (TUT_TYPE_OBJ07, NULL);
    return self;
}

void
tut_obj07_signal_simple (TutObj07 *self)
{
    g_print ("entered tut_obj07_signal_simple()\n");
    g_return_if_fail (TUT_IS_OBJ07 (self));
    g_signal_emit (G_OBJECT (self), tut_obj07_signal_id[SIGNAL_SIMPLE], 0);
}

void
tut_obj07_signal_with_para (TutObj07 *self, gchar *string_para, gint int_para)
{
    g_print ("entered tut_obj07_signal_with_para()\n");
    g_return_if_fail (TUT_IS_OBJ07 (self));
    g_signal_emit (G_OBJECT (self), tut_obj07_signal_id[SIGNAL_PARA], 0, string_para, int_para);
}

void
tut_obj07_signal_with_para_detail (TutObj07 *self, gchar *string_para, gint int_para)
{
    g_print ("entered tut_obj07_signal_with_para_detail()\n");
    g_return_if_fail (TUT_IS_OBJ07 (self));
    g_signal_emit (G_OBJECT (self), tut_obj07_signal_id[SIGNAL_PARA], tut_obj07_detail_quark, string_para, int_para);
}
