#include "tut-greeter.h"

struct _TutGreeter
{
  GObject       parent_instance;
};

static void   tut_greeter_class_init (TutGreeterClass * klass);
static void   tut_greeter_init (TutGreeter * self);

G_DEFINE_TYPE (TutGreeter, tut_greeter, G_TYPE_OBJECT)

/* custom part of construction/destruction */

static void
tut_greeter_class_init (TutGreeterClass *klass)
{
  g_print ("entered tut_greeter_class_init ()\n");
  klass->hello = NULL;          /* leave method uniplemented */
}

static void
tut_greeter_init (TutGreeter *self)
{
  g_print ("entered tut_greeter_init ()\n");
  (void) self;
}

/* public interface */

void
tut_greeter_hello (TutGreeter *self)
{
  TutGreeterClass *klass;

  g_print ("tut_greeter_hello () [[\n");
  g_return_if_fail (TUT_IS_GREETER (self));       /* it is from external code -- check */
  klass = TUT_GREETER_GET_CLASS (self);
  g_return_if_fail (klass->hello != NULL);
  klass->hello (self);
  g_print ("]] tut_greeter_hello ()\n");
}
