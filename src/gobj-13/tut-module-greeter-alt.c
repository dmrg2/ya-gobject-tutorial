#include "tut-greeter.h"

/* Module instance */
struct _TutModuleGreeterAlt
{
  GTypeModule       parent_instance;
};

/* Greeter implementation instance */
struct _TutGreeterImplAlt
{
  TutGreeter        parent_instance;
};

/* We know only name of base type */
#define INMODULE_TUT_TYPE_GREETER ({ \
  g_print ("*** type lookup for TutGreeter\n"); \
  g_type_from_name ("TutGreeter"); })

#define TUT_TYPE_MODULE_GREETER_ALT (tut_module_greeter_alt_get_type ())
#define TUT_TYPE_GREETER_IMPL_ALT (tut_greeter_impl_alt_get_type ())

G_DECLARE_FINAL_TYPE           (TutModuleGreeterAlt,
                                tut_module_greeter_alt,
                                TUT, MODULE_GREETER_ALT,
                                GTypeModule)
G_DECLARE_FINAL_TYPE           (TutGreeterImplAlt,
                                tut_greeter_impl_alt,
                                TUT, GREETER_IMPL_ALT,
                                TutGreeter)
G_DEFINE_FINAL_TYPE            (TutModuleGreeterAlt,
                                tut_module_greeter_alt,
                                G_TYPE_TYPE_MODULE)
G_DEFINE_DYNAMIC_TYPE_EXTENDED (TutGreeterImplAlt,
                                tut_greeter_impl_alt,
                                INMODULE_TUT_TYPE_GREETER,
                                G_TYPE_FLAG_FINAL,
                                {});

static gboolean tut_module_greeter_alt_load_impl   (GTypeModule * module);
static void     tut_module_greeter_alt_unload_impl (GTypeModule * module);

static void     tut_greeter_impl_alt_class_init    (TutGreeterImplAltClass * klass);
static void     tut_greeter_impl_alt_init          (TutGreeterImplAlt * self);
static void     tut_greeter_impl_alt_hello_impl    (TutGreeter * self);

/* Cached module instance pointer */
static TutModuleGreeterAlt *module_greeter      = NULL;

/* Module class initialization */
static void
tut_module_greeter_alt_class_init (TutModuleGreeterAltClass *klass)
{
  GTypeModuleClass *base_class;

  g_print ("entered tut_module_greeter_alt_class_init ()\n");
  base_class = G_TYPE_MODULE_CLASS (klass);
  base_class->load = tut_module_greeter_alt_load_impl;
  base_class->unload = tut_module_greeter_alt_unload_impl;
}

/* Implementation class initialization */
static void
tut_greeter_impl_alt_class_init (TutGreeterImplAltClass * klass)
{
  TutGreeterClass *base_class;

  g_print ("entered tut_greeter_impl_alt_class_init ()\n");
  base_class = (TutGreeterClass *)klass;
  base_class->hello = tut_greeter_impl_alt_hello_impl;
}

/* Implementation class finalization */
/*   this function is autodeclared by macro */
static void
tut_greeter_impl_alt_class_finalize (TutGreeterImplAltClass * klass)
{
  g_print ("entered tut_greeter_impl_alt_class_finalize ()\n");
  (void)klass;
}

/* Module instance initialization */
static void
tut_module_greeter_alt_init (TutModuleGreeterAlt *self)
{
  g_print ("entered tut_module_greeter_alt_init ()\n");
  g_type_module_set_name (G_TYPE_MODULE (self), "greeter-module-alt");
}

/* Implementation instance initialization */
static void
tut_greeter_impl_alt_init (TutGreeterImplAlt * self)
{
  g_print ("entered tut_greeter_impl_alt_init ()\n");
  (void) self;
}

/* Module method 'load' */
static gboolean
tut_module_greeter_alt_load_impl (GTypeModule * module)
{
  g_print ("entered tut_module_greeter_alt_load_impl ()\n");
  g_return_val_if_fail (module_greeter != NULL, FALSE);
  if (!tut_greeter_impl_alt_type_id)
    {
      g_print ("\tregistering dynamic type\n");
      tut_greeter_impl_alt_register_type(G_TYPE_MODULE (module_greeter));
    }
  return tut_greeter_impl_alt_type_id != 0;
}

/* Module method 'unload' */
static void
tut_module_greeter_alt_unload_impl (GTypeModule * module)
{
  g_print ("entered tut_module_greeter_alt_unload_impl ()\n");
  (void)module;
}

/* Implementation method 'hello' */
static void 
tut_greeter_impl_alt_hello_impl (TutGreeter * self)
{
  g_print ("\t Hello from TutGreeterImplAlt!\n");
}

/* Public module singleton access */
GTypeModule *
tut_module_greeter_alt (void)
{
  g_print ("entered tut_module_greeter_alt ()\n");
  if (module_greeter == NULL)
    {
      module_greeter =
        (TutModuleGreeterAlt *) g_object_new (TUT_TYPE_MODULE_GREETER_ALT, NULL);
    }
  return G_TYPE_MODULE (module_greeter);
}

/* Public implemenation instance fabric */
TutGreeter *
tut_module_greeter_alt_fabricate (void)
{
  g_print ("entered tut_module_greeter_alt_fabricate ()\n");
  return (TutGreeter *) g_object_new (tut_greeter_impl_alt_type_id, NULL);
}
