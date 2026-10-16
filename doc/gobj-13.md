## GTypeModule and dynamic loading

Source code for this chapter is in [src/gobj-13/](../src/gobj-13/) directory.

GObject library provides API for dynamic load of object classes. This API is
represented by [TypePlugin interface](https://docs.gtk.org/gobject/iface.TypePlugin.html).
Library contains one partial implementation of this interface in abstract
class [GTypeModule](https://docs.gtk.org/gobject/class.TypeModule.html).

GTypeModule is abstract since it has two unimplemented virtual methods, `load`
and `unload`. These methods are supposed to manage dynamic creation and
desctruction of types, implemented in type plugin. Real type plugins derived
from GTypeModule should override these methods and do their registration work
inside them.

Remark: as [stated in documentation](https://docs.gtk.org/gobject/method.TypeClass.unref.html),
since version 2.84 of GObject library there is no way to destroy once
registered class. So, `unload` method may be accounted for historical artifact.
Nevertheless, dynamic loading of type implementations still works.

### Dynamically defined classes in this tutorial

There is little use in classes and objects, which are totally unknown for a
program that will obtain them. Usually, dynamic classes implement some kind of
known before thing. Dynamic class may have well known property names, or
implement well known interface, or be a subclass of a known class, and so on.

In this tutorial we define abstract base class, which declares virtual method,
and then implement that method in derived class. There are two different type
plugins, and each defines some derived class, with own method implementation.
These derived classes are registered on loading corresponding type plugins, and
may be used after that.

Type plugins implemented as shared libraries, exporting simple interface to
instantiate type plugin itself and to instantiate objects, defined in that
shared libraries.

### Base class declaration and definition.

Full source code is in [src/gobj-13/tut-greeter.h](../src/gobj-13/tut-greeter.h)
and [src/gobj-13/tut-greeter.c](../src/gobj-13/tut-greeter.c). We define simple
derivable class `TutGreeter`, with one virtual method `hello`:

```C
struct _TutGreeterClass {
    GObjectClass parent_class;
    void (*hello) (TutGreeter  *self); /* Used as: class->hello (instance) */
    void *__vtable_resv[3];            /* 3 more slots to add virtual methods without breaking ABI */
};
```

Method `hello` is left unimplemented on class initialization of our base class:

```C
static void
tut_greeter_class_init (TutGreeterClass *klass)
{
  g_print ("entered tut_greeter_class_init ()\n");
  klass->hello = NULL;          /* leave method uniplemented */
}
```

### Type plugin API structure

Type plugin exports two functions, with standard signatures declared in [src/gobj-13/tut-module-defs.h](../src/gobj-13/tut-module-defs.h).

```C
typedef GTypeModule * (*tut_module_greeter_fn) (void);
typedef TutGreeter * (*tut_module_greeter_fabricate_fn) (void);
```

First of these functions should return created instance of GTypeModule
implementation. Second creates one object of class that is derived from
`TutGreeter` and have implementation for method `hello`.

Since we have to know name of function is shared library to get its adress
from, these names are defined in corresponding type plugin headers [src/gobj-13/tut-module-greeter.h](../src/gobj-13/tut-module-greeter.h)
and [src/gobj-13/tut-module-greeter-alt.h](../src/gobj-13/tut-module-greeter-alt.h).

```C
#define TUT_MODULE_GREETER_FN "tut_module_greeter"
#define TUT_MODULE_GREETER_FABRICATE_FN "tut_module_greeter_fabricate"
```

```C
#define TUT_MODULE_GREETER_ALT_FN "tut_module_greeter_alt"
#define TUT_MODULE_GREETER_ALT_FABRICATE_FN "tut_module_greeter_alt_fabricate"
```

Note: there may be different custom conventions on naming and structure of API,
that is used with shared library based plugins. All that is needed is to create
GTypeObject, make its `load` called and then create objects of classes, defined
in plugin.  We simply made it similar to GObject naming scheme here. As complex
example you may use [GTK3 input methods](https://gitlab.gnome.org/GNOME/gtk/-/blob/3.24.52/gtk/gtkimmodule.c).

### Declarations in type plugin

Full source code is in [src/gobj-13/tut-module-greeter.c](../src/gobj-13/tut-module-greeter.c).

Each of our type plugins must declare two classes:
- class derived from GTypeModule to implement its `load` method
- class derived from our `TutGreeter` to implement its `hello` method

```C
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
```

There is one additional definition for type identifier of our base type
`TutGreeter`. Dynamiclly loaded type modules have no access to autoregistration
function of our main program, so they will lookup type identifier by name with
special macro `INMODULE_TUT_TYPE_GREETER`.

### Definitions for type module

Type module is defined as usual subclass of GTypeModule

```C
G_DEFINE_FINAL_TYPE            (TutModuleGreeter,
                                tut_module_greeter,
                                G_TYPE_TYPE_MODULE)
```

In initialization function of a class of type module we set up overriden
methods `load` and `unload`

```C
static void
tut_module_greeter_class_init (TutModuleGreeterClass *klass)
{
  GTypeModuleClass *base_class;

  g_print ("entered tut_module_greeter_class_init ()\n");
  base_class = G_TYPE_MODULE_CLASS (klass);
  base_class->load = tut_module_greeter_load_impl;
  base_class->unload = tut_module_greeter_unload_impl;
}
```

In initialization function of instance of type module we set up module name

```C
static void
tut_module_greeter_init (TutModuleGreeter *self)
{
  g_print ("entered tut_module_greeter_init ()\n");
  g_type_module_set_name (G_TYPE_MODULE (self), "greeter-module");
}
```

`load` method implementation uses type registration function for derived from
TutGreeter class, which is defined in this module. (`unload` implementation is
effectively empty as it can not be used.)

```C
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
```

That `tut_greeter_impl_register_type` is automatically defined by dynamic type
registration macro as shown in next section. Static variable
`tut_greeter_impl_type_id` to hold type identifier of our implementation of
base class TutGreeter is defined by that macro too.

### Definitions for derived from TutGreeter class

Our implementing class derived from TutGreeter defined with new macro that we
did not used before

```C
G_DEFINE_DYNAMIC_TYPE_EXTENDED (TutGreeterImpl,
                                tut_greeter_impl,
                                INMODULE_TUT_TYPE_GREETER,
                                G_TYPE_FLAG_FINAL,
                                {});
```

`G_DEFINE_DYNAMIC_TYPE_EXTENDED` works like other `G_DEFINE_TYPE`-like macros,
except that it produces separate function for type registration. Following
naming conventions, for our object it is function
`tut_greeter_impl_register_type`, and this function is used when `load` of type
plugin called.

Class and instance initialization are made as usual. Note that most symbols
produced by macros for our base type TutGreeter are in main program. These
symbols are inaccessible in our type plugin. So, some type casts and
conversions we must do "manual". As cast to `TutGreeterClass` in initialization
function, for example:

```C
static void
tut_greeter_impl_class_init (TutGreeterImplClass * klass)
{
  TutGreeterClass *base_class;

  g_print ("entered tut_greeter_impl_class_init ()\n");
  base_class = (TutGreeterClass *)klass;
  base_class->hello = tut_greeter_impl_hello_impl;
}
```

Dynamic classes registered by default with finalization function. We must
provide definition for this function even if it is empty:

```C
static void
tut_greeter_impl_class_finalize (TutGreeterImplClass * klass)
{
  g_print ("entered tut_greeter_impl_class_finalize ()\n");
  (void)klass;
}
```

Instance initialization for our derived class is effectively empty.

Our implementation for base's `hello` method just prints greeteing.

```C
static void 
tut_greeter_impl_hello_impl (TutGreeter * self)
{
  g_print ("\t Hello from TutGreeterImpl!\n");
}
```

### Implementation of type module public API

There is two functions, one to create and cache single instance of our
type module class, and other to produce one new instance of our implementation
of base class TutGreeter.

```C
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
```

Second type plugin made same and differ only in type names it uses. Its source
code is in
[src/gobj-13/tut-module-greeter-alt.c](../src/gobj-13/tut-module-greeter-alt.c).


### Loading type plugins in user code

Full source code is in [src/gobj-13/main.c](../src/gobj-13/main.c).

Before loading type plugins we must be sure that our base type exists in type
system of GObject library. We just call autoregistration function by touching
type identifier macro.

```C
  (void)TUT_TYPE_GREETER; /* assure TutGreeter type exist */
```

We load our shared libraries with standard .so loading procedure

```C
  void                            *module_lib, *module_lib2;
  GTypeModule                     *type_module, *type_module2;
  tut_module_greeter_fn            module_fn, module2_fn;
  tut_module_greeter_fabricate_fn  fabricate_fn, fabricate2_fn;
  /* ... */
  module_lib = dlopen("./greetermodule.so", RTLD_LAZY | RTLD_DEEPBIND);
  module_fn =
    (tut_module_greeter_fn) dlsym(module_lib, TUT_MODULE_GREETER_FN);
  fabricate_fn =
    (tut_module_greeter_fabricate_fn) dlsym(module_lib,
                                            TUT_MODULE_GREETER_FABRICATE_FN);
  type_module = module_fn ();
```

Names of functions exported from shared library we defined in module interface
headers as macros `TUT_MODULE_GREETER_FN` and
`TUT_MODULE_GREETER_FABRICATE_FN`. Calling first of interface functions we
create instance of type module and obtain pointer to it. Now loaded type plugin
is ready to construct its objects.

```C
  greeter_impl = fabricate_fn();
  g_print ("G_IS_OBJECT (greeter_impl) = %d\n", G_IS_OBJECT (greeter_impl));
  g_print ("trying greeter_impl->TutGreeter::hello (void):\n");
  tut_greeter_hello (greeter_impl);
  g_clear_object (&greeter_impl);
```

Type module `use`d automatically on first attempt to create objects defined in
it. Objects produced by second of plugin's API functions have method `hello`
implemented. We may do steps from beginning of this section for another type
plugin and see another greeting.

Type module's `unuse` call is useless and exists for historical reasons. We
just close dynamically loaded shared objects at the end.

```C
  g_type_module_unuse (G_TYPE_MODULE (type_module)); /* won't do anything */
  g_type_module_unuse (G_TYPE_MODULE (type_module2)); /* won't do anything */
  /* object classes from this modules is still live and cannot be disposed */
  g_print ("type_module->use_count: %u\n", type_module->use_count);
  g_print ("type_module2->use_count: %u\n", type_module2->use_count);

  dlclose (module_lib);
  dlclose (module_lib2);
```
