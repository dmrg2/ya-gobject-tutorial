#include "tut-greeter.h"

/* Module instance */
struct _TutModuleGreeter
{
  GTypeModule       parent_instance;
};

/* Greeter implementation instance */
struct _TutGreeterImpl
{
  TutGreeter        parent_instance;
};

/* We know only name of base type */
#define INMODULE_TUT_TYPE_GREETER ({ \
  g_print ("*** type lookup for TutGreeter\n"); \
  g_type_from_name ("TutGreeter"); })

#define TUT_TYPE_MODULE_GREETER (tut_module_greeter_get_type ())
#define TUT_TYPE_GREETER_IMPL (tut_greeter_impl_get_type ())

G_DECLARE_FINAL_TYPE           (TutModuleGreeter,
                                tut_module_greeter,
                                TUT, MODULE_GREETER,
                                GTypeModule)
G_DECLARE_FINAL_TYPE           (TutGreeterImpl,
                                tut_greeter_impl,
                                TUT, GREETER_IMPL,
                                TutGreeter)
G_DEFINE_FINAL_TYPE            (TutModuleGreeter,
                                tut_module_greeter,
                                G_TYPE_TYPE_MODULE)
G_DEFINE_DYNAMIC_TYPE_EXTENDED (TutGreeterImpl,
                                tut_greeter_impl,
                                INMODULE_TUT_TYPE_GREETER,
                                G_TYPE_FLAG_FINAL,
                                {});

static gboolean tut_module_greeter_load_impl   (GTypeModule * module);
static void     tut_module_greeter_unload_impl (GTypeModule * module);

static void     tut_greeter_impl_class_init    (TutGreeterImplClass * klass);
static void     tut_greeter_impl_init          (TutGreeterImpl * self);
static void     tut_greeter_impl_hello_impl    (TutGreeter * self);

/* Cached module instance pointer */
static TutModuleGreeter *module_greeter      = NULL;

/* Module class initialization */
static void
tut_module_greeter_class_init (TutModuleGreeterClass *klass)
{
  GTypeModuleClass *base_class;

  g_print ("entered tut_module_greeter_class_init ()\n");
  base_class = G_TYPE_MODULE_CLASS (klass);
  base_class->load = tut_module_greeter_load_impl;
  base_class->unload = tut_module_greeter_unload_impl;
}

/* Implementation class initialization */
static void
tut_greeter_impl_class_init (TutGreeterImplClass * klass)
{
  TutGreeterClass *base_class;

  g_print ("entered tut_greeter_impl_class_init ()\n");
  base_class = (TutGreeterClass *)klass;
  base_class->hello = tut_greeter_impl_hello_impl;
}

/* Implementation class finalization */
/*   this function is autodeclared by macro */
static void
tut_greeter_impl_class_finalize (TutGreeterImplClass * klass)
{
  g_print ("entered tut_greeter_impl_class_finalize ()\n");
  (void)klass;
}

/* Module instance initialization */
static void
tut_module_greeter_init (TutModuleGreeter *self)
{
  g_print ("entered tut_module_greeter_init ()\n");
  g_type_module_set_name (G_TYPE_MODULE (self), "greeter-module");
}

/* Implementation instance initialization */
static void
tut_greeter_impl_init (TutGreeterImpl * self)
{
  g_print ("entered tut_greeter_impl_init ()\n");
  (void) self;
}

/* Module method 'load' */
static gboolean
tut_module_greeter_load_impl (GTypeModule * module)
{
  g_print ("entered tut_module_greeter_load_impl ()\n");
  g_return_val_if_fail (module_greeter != NULL, FALSE);
  if (!tut_greeter_impl_type_id)
    {
      g_print ("\tregistering dynamic type\n");
      tut_greeter_impl_register_type(G_TYPE_MODULE (module_greeter));
    }
  return tut_greeter_impl_type_id != 0;
}

/* Module method 'unload' */
static void
tut_module_greeter_unload_impl (GTypeModule * module)
{
  g_print ("entered tut_module_greeter_unload_impl ()\n");
  (void)module;
}

/* Implementation method 'hello' */
static void 
tut_greeter_impl_hello_impl (TutGreeter * self)
{
  g_print ("\t Hello from TutGreeterImpl!\n");
}

/* Public module singleton access */
GTypeModule *
tut_module_greeter (void)
{
  g_print ("entered tut_module_greeter ()\n");
  if (module_greeter == NULL)
    {
      module_greeter =
        (TutModuleGreeter *) g_object_new (TUT_TYPE_MODULE_GREETER, NULL);
    }
  return G_TYPE_MODULE (module_greeter);
}

/* Public implemenation instance fabric */
TutGreeter *
tut_module_greeter_fabricate (void)
{
  g_print ("entered tut_module_greeter_fabricate ()\n");
  return (TutGreeter *) g_object_new (tut_greeter_impl_type_id, NULL);
}
