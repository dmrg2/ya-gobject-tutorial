#include "tut-obj04.h"
#include "tut-obj04m.h"

struct _TutObj04M {
    GObject parent_instance;
};

static void tut_obj04m_class_init (TutObj04MClass *klass);
static void tut_obj04m_init (TutObj04M *self);
static void tut_obj04m_vwhoami_impl (TutObj04 *self);
static void tut_obj04m_vpure_impl (TutObj04 *self);

G_DEFINE_FINAL_TYPE(TutObj04M, tut_obj04m, TUT_TYPE_OBJ04);

/* custom part of construction/destruction */

static void
tut_obj04m_class_init (TutObj04MClass *klass) {
    TutObj04Class *base_class = TUT_OBJ04_CLASS(klass);

    g_print ("entered tut_obj04m_class_init()\n");
    base_class->vwhoami = tut_obj04m_vwhoami_impl; /* override method implemenattion */
    base_class->vpure = tut_obj04m_vpure_impl;     /* override method implemenattion */
}

static void
tut_obj04m_init (TutObj04M *self) {
    g_print ("entered tut_obj04m_init()\n");
    (void)self;
}

/* interface implementation */

static void tut_obj04m_vwhoami_impl (TutObj04 *self)
{
    g_print ("entered tut_obj04m_whoami_impl()\n");
    g_return_if_fail (TUT_IS_OBJ04M (self)); /* it is from external code -- check */
    g_print ("\tI am TutObj04M, descendant of TutObj04!\n");
    (void)self;
}

static void tut_obj04m_vpure_impl (TutObj04 *self)
{
    g_print ("entered tut_obj04m_vpure_impl()\n");
    g_return_if_fail (TUT_IS_OBJ04M (self)); /* it is from external code -- check */
    g_print ("\tI, TutObj04M, implemented vpure!\n");
    (void)self;
}

/* public interface */

TutObj04M *
tut_obj04m_new (void) {
    TutObj04M *self;

    g_print ("entered tut_obj04m_new()\n");
    self = (TutObj04M *) g_object_new (TUT_TYPE_OBJ04M, NULL);
    return self;
}
