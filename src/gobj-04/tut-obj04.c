#include "tut-obj04.h"

struct _TutObj04 {
    GObject parent_instance;
};

static void tut_obj04_class_init (TutObj04Class *klass);
static void tut_obj04_init (TutObj04 *self);
static void tut_obj04_vwhoami_impl (TutObj04 *self);

G_DEFINE_TYPE(TutObj04, tut_obj04, G_TYPE_OBJECT);

/* custom part of construction/destruction */

static void
tut_obj04_class_init (TutObj04Class *klass) {
    g_print ("entered tut_obj04_class_init()\n");
    klass->vwhoami = tut_obj04_vwhoami_impl; /* install method implemenattion */
    klass->vpure = NULL;                     /* leave method uniplemented */
}

static void
tut_obj04_init (TutObj04 *self) {
    g_print ("entered tut_obj04_init()\n");
    (void)self;
}

/* interface implementation */

static void tut_obj04_vwhoami_impl (TutObj04 *self)
{
    g_print ("entered tut_obj04_whoami_impl()\n");
    g_return_if_fail (TUT_IS_OBJ04 (self)); /* it is from external code -- check */
    g_print ("\tI am TutObj04!\n");
    (void)self;
}

/* public interface */

TutObj04 *
tut_obj04_new (void) {
    TutObj04 *self;

    g_print ("entered tut_obj04_new()\n");
    self = (TutObj04 *) g_object_new (TUT_TYPE_OBJ04, NULL);
    return self;
}

void
tut_obj04_vwhoami (TutObj04 *self)
{
    TutObj04Class *klass;

    g_print ("tut_obj04_vwhoami() [[\n");
    g_return_if_fail (TUT_IS_OBJ04 (self)); /* it is from external code -- check */
    klass = TUT_OBJ04_GET_CLASS (self);
    g_return_if_fail (klass->vwhoami != NULL);
    klass->vwhoami (self);
    g_print ("]] tut_obj04_vwhoami()\n");
}

void
tut_obj04_vpure (TutObj04 *self)
{
    TutObj04Class *klass;

    g_print ("tut_obj04_vpure() [[\n");
    g_return_if_fail (TUT_IS_OBJ04 (self)); /* it is from external code -- check */
    klass = TUT_OBJ04_GET_CLASS (self);
    g_return_if_fail (klass->vpure != NULL);
    klass->vpure (self);
    g_print ("]] tut_obj04_vpure()\n");
}
