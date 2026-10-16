#include <dlfcn.h>
#include "tut-greeter.h"
#include "tut-module-defs.h"
#include "tut-module-greeter.h"
#include "tut-module-greeter-alt.h"

int
main (void)
{
  TutGreeter                      *greeter;
  TutGreeter                      *greeter_impl, *greeter2_impl;
  void                            *module_lib, *module_lib2;
  GTypeModule                     *type_module, *type_module2;
  tut_module_greeter_fn            module_fn, module2_fn;
  tut_module_greeter_fabricate_fn  fabricate_fn, fabricate2_fn;

  (void)TUT_TYPE_GREETER; /* assure TutGreeter type exist */

  g_print ("=======================================================\n");
  module_lib = dlopen("./greetermodule.so", RTLD_LAZY | RTLD_DEEPBIND);
  module_fn =
    (tut_module_greeter_fn) dlsym(module_lib, TUT_MODULE_GREETER_FN);
  fabricate_fn =
    (tut_module_greeter_fabricate_fn) dlsym(module_lib,
                                            TUT_MODULE_GREETER_FABRICATE_FN);
  type_module = module_fn ();
  g_print ("type_module->name: %s\n", type_module->name);

  g_print ("=======================================================\n");
  module_lib2 = dlopen("./greetermodulealt.so", RTLD_LAZY | RTLD_DEEPBIND);
  module2_fn =
    (tut_module_greeter_fn) dlsym(module_lib2, TUT_MODULE_GREETER_ALT_FN);
  fabricate2_fn =
    (tut_module_greeter_fabricate_fn) dlsym(module_lib2,
                                            TUT_MODULE_GREETER_ALT_FABRICATE_FN);
  type_module2 = module2_fn ();
  g_print ("type_module2->name: %s\n", type_module2->name);

  g_print ("=======================================================\n");
  greeter = (TutGreeter *) g_object_new (TUT_TYPE_GREETER, NULL);
  g_print ("G_IS_OBJECT (greeter) = %d\n", G_IS_OBJECT (greeter));
  g_print ("trying greeter->TutGreeter::hello (void):\n");
  tut_greeter_hello (greeter);
  g_clear_object (&greeter);

  g_print ("=======================================================\n");
  g_type_module_use (G_TYPE_MODULE (type_module));
  g_print ("type_module->use_count: %u\n", type_module->use_count);
  g_print ("type_module->ref_count: %u\n", G_OBJECT (type_module)->ref_count);

  g_print ("=======================================================\n");
  greeter_impl = fabricate_fn();
  g_print ("G_IS_OBJECT (greeter_impl) = %d\n", G_IS_OBJECT (greeter_impl));
  g_print ("trying greeter_impl->TutGreeter::hello (void):\n");
  tut_greeter_hello (greeter_impl);
  g_clear_object (&greeter_impl);

  g_print ("=======================================================\n");
  g_type_module_use (G_TYPE_MODULE (type_module2));
  g_print ("type_module2->use_count: %u\n", type_module2->use_count);
  g_print ("type_module2->ref_count: %u\n", G_OBJECT (type_module2)->ref_count);

  g_print ("=======================================================\n");
  greeter2_impl = fabricate2_fn();
  g_print ("G_IS_OBJECT (greeter2_impl) = %d\n", G_IS_OBJECT (greeter2_impl));
  g_print ("trying greeter2_impl->TutGreeter::hello (void):\n");
  tut_greeter_hello (greeter2_impl);
  g_clear_object (&greeter2_impl);

  g_print ("=======================================================\n");
  g_type_module_unuse (G_TYPE_MODULE (type_module)); /* won't do anything */
  g_type_module_unuse (G_TYPE_MODULE (type_module2)); /* won't do anything */
  /* object classes from this modules is still live and cannot be disposed */
  g_print ("type_module->use_count: %u\n", type_module->use_count);
  g_print ("type_module2->use_count: %u\n", type_module2->use_count);

  dlclose (module_lib);
  dlclose (module_lib2);

  return 0;
}
