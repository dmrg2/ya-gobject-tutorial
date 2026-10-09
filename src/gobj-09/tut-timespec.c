#include <gobject/gvaluecollector.h>
#include "tut-timespec.h"

typedef struct _TutTimespecParamSpec {
    GParamSpec parent_param_spec;
} TutTimespecParamSpec;

static GType tut_timespec_get_type_once ();
static GType tut_timespec_get_param_type_once ();

GType
tut_timespec_get_type (void)
{
    static GType static_g_define_type_id = 0;

    if (g_once_init_enter_pointer (&static_g_define_type_id))
    {
        GType g_define_type_id = tut_timespec_get_type_once ();
        g_once_init_leave_pointer (&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}

GType
tut_timespec_get_param_type (void)
{
    static GType static_g_define_type_id = 0;

    if (g_once_init_enter_pointer (&static_g_define_type_id))
    {
        GType g_define_type_id = tut_timespec_get_param_type_once ();
        g_once_init_leave_pointer (&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}

static void tut_timespec_value_init (GValue *value);
static void tut_timespec_value_copy (const GValue *src_value, GValue *dst_value);
static gchar* tut_timespec_collect_value (GValue *value, guint n_collect_values, GTypeCValue *collect_values, guint collect_flags);
static gchar* tut_timespec_lcopy_value (const GValue *value, guint n_collect_values, GTypeCValue *collect_values, guint collect_flags);

static GType tut_timespec_get_type_once () {
    GTypeValueTable vci = {
        .value_init = tut_timespec_value_init,
        .value_free = NULL,
        .value_copy = tut_timespec_value_copy,
        .value_peek_pointer = NULL,
        .collect_format = "qq",
        .collect_value = tut_timespec_collect_value,
        .lcopy_format = "qq",
        .lcopy_value = tut_timespec_lcopy_value
    };
    GTypeInfo ti = {
        .class_size = 0,
        .base_init = NULL,
        .base_finalize = NULL,
        .class_init = NULL,
        .class_finalize = NULL,
        .class_data = NULL,
        .instance_size = 0,
        .n_preallocs = 0,
        .instance_init = NULL,
        .value_table = &vci
    };
    static GTypeFundamentalInfo tfi = {
        .type_flags = 0
    };

    g_print ("tut_timespec_get_type_once(): registering fundamental type\n");
    return  g_type_register_fundamental (g_type_fundamental_next(),
                                         g_intern_static_string ("TutTimespec"),
                                         &ti,
                                         &tfi,
                                         G_TYPE_FLAG_FINAL);
}

static GType tut_timespec_get_param_type_once () {
    GParamSpecTypeInfo psti = {
        .instance_size = sizeof (TutTimespecParamSpec),
        .n_preallocs = 0,
        .value_type = TUT_TYPE_TIMESPEC,
        .instance_init = NULL,
        .finalize = NULL,
        .value_set_default = NULL,
        .value_validate = NULL,
        .values_cmp = NULL
    };

    g_print ("tut_timespec_get_param_type_once(): registering paramspec type\n");
    return  g_param_type_register_static (g_intern_static_string ("TutTimespecParamspec"), &psti);
}

static void tut_timespec_value_init (GValue *value) {
    value->data[1].v_int64 = value->data[0].v_int64 = 0;
    g_print ("\t[timespec] init {%ld, %ld}\n", value->data[0].v_int64, value->data[1].v_int64);
}

static void tut_timespec_value_copy (const GValue *src_value, GValue *dst_value) {
    dst_value->data[0].v_int64 = src_value->data[0].v_int64;
    dst_value->data[1].v_int64 = src_value->data[1].v_int64;
    g_print ("\t[timespec] copied {%ld, %ld}\n", dst_value->data[0].v_int64, dst_value->data[1].v_int64);
}

static gchar* tut_timespec_collect_value (GValue *value, guint n_collect_values, GTypeCValue *collect_values, guint collect_flags) {
    g_return_val_if_fail (n_collect_values == 2, g_strdup_printf ("expected exactly two ints, got %d", n_collect_values));
    value->data[0].v_int64 = collect_values[0].v_int64;
    value->data[1].v_int64 = collect_values[1].v_int64;
    g_print ("\t[timespec] collected {%ld, %ld}\n", value->data[0].v_int64, value->data[1].v_int64);
    return NULL;
}

static gchar* tut_timespec_lcopy_value (const GValue *value, guint n_collect_values, GTypeCValue *collect_values, guint collect_flags) {
    g_return_val_if_fail (n_collect_values == 2, g_strdup_printf ("expected exactly two ints, got %d", n_collect_values));
    collect_values[0].v_int64 = value->data[0].v_int64;
    collect_values[1].v_int64 = value->data[1].v_int64;
    g_print ("\t[timespec] L-copied {%ld, %ld}\n", collect_values[0].v_int64, collect_values[1].v_int64);
    return NULL;
}

GParamSpec *
g_param_spec_timespec (const gchar* name, const gchar* nick, const gchar* blurb, GParamFlags flags)
{
    return g_param_spec_internal (TUT_PARAM_TYPE_TIMESPEC, name, nick, blurb, flags);
}
