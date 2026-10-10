## Object inheritance, virtual methods, using GObject library convenience macros

Source code for this chapter is in [/src/gobj-04/](../src/gobj-04/) directory.

In this tutorial we wil create two objects with virtual methods `vwhoami`and `vpure`and no internal
data. First object will be derived from GObject. It will implement method `vwhoami`, but leave
method `vpure` unimplemented (like pure virtual in C++). Second object will be derived from our
first object. It vill override both `vwhoami` and `vpure` methods of first objct.

Source code for this tutorial is structured: each of the objects have separate header and
inmlementation files, and "user" code will be in its own file too. The sources now use convenience
macros from GOject library. With these macros large parts of boilerplate code is generated
automatically. This automatic generation assumes strict adherence to [conventions](gobj-02.md#conventional-module-and-object-names) of naming,
structuring and using objects and classes.

Our objects are:
- "base" TutObj04 descends directly from GObject
- "derived" TutObj04M descends from TutObj04

### Base object declarations

Full source code is in [src/gobj-04/tut-obj04.h](../src/gobj-04/tut-obj04.h) (which is also
conventinally named). File consists of four basic parts described below.

First part is type identifier declaration with typical use of autoregistration function:

```C
#define TUT_TYPE_OBJ04 (tut_obj04_get_type())
```

Second part is declaration of derivable class of objects via library macro:

```C
G_DECLARE_DERIVABLE_TYPE(TutObj04, tut_obj04, TUT, OBJ04, GObject);
```

Note using all the form of naming in this single macro. All three forms are used in this
macro to generate "standard" names for many C objects defined by macro expansion.

Third part is declarations for public interface to TutObj04 objects:

```C
TutObj04 *tut_obj04_new (void);
void tut_obj04_vwhoami (TutObj04 *self);
void tut_obj04_vpure (TutObj04 *self);
```

And last fourth part is definition of class structure:

```C
struct _TutObj04Class {
    GObjectClass parent_class;
    void (*vwhoami) (TutObj04  *self); /* Used as: class->vwhoami (instance) */
    void (*vpure) (TutObj04  *self);   /* Used as: class->vpure (instance) */
    void *__vtable_resv[2];            /* 2 more slots to add virtual methods without breaking ABI */
};
```

It is important to have this definition in header file. This structure contains pointers
to virtual functions that may be overriden by derived classes. Implementation of derived
class must know where these pointers located to be able to replace its values. This part of
structure itself is a "table of virtual functions" in C++ sense.

Note also small array of two "dummy" pointers at the end of "virtual table". It is common
practice to reserve some storage for future modifications. If we add new virtual method
in this reserved storage, we won't break neither API nor ABI to object. "Old" derived
objects and user code will continue to work with unchanged part of virtual table.

### Base object implementation -- instance structure

Full source code is in [src/gobj-04/tut-obj04.c](../src/gobj-04/tut-obj04.c)

Structure of instance data is defined in implementation part and thus hidden from implementations
of derived classes and from user code.

```C
struct _TutObj04 {
    GObject parent_instance;
};
```

If derived classes would need acces to internal data of instances of this base class, we should
place this definition in header file, of provide some other (protected in terms of C++) interface
to access such data.

### Base object implementation -- type definition and internal declarations

There are forward declaration of standard functions used in library type definition macro.
Forward declaration of implementation of our virtual method `vwhoami` is placed here too, 
just to make code compact.

```C
static void tut_obj04_class_init (TutObj04Class *klass);
static void tut_obj04_init (TutObj04 *self);
static void tut_obj04_vwhoami_impl (TutObj04 *self);

G_DEFINE_TYPE(TutObj04, tut_obj04, G_TYPE_OBJECT);
```

Expansion of macro `G_DEFINE_TYPE` uses exactly that "conventional" names of initialization
functions which are used in forward declaration.

### Base object implementation -- class and instance initialization

Class initialization should set up pointers to real implementations of virtual methods in
virtual table:

```C
static void
tut_obj04_class_init (TutObj04Class *klass) {
    g_print ("entered tut_obj04_class_init()\n");
    klass->vwhoami = tut_obj04_vwhoami_impl; /* install method implemenattion */
    klass->vpure = NULL;                     /* leave method uniplemented */
}
```

We have implementation for method `vwhoami` only. Method `vpure` left NULL (pure virtual).

Instance initialization does nothing, since instance has no initializable data:

```C
static void
tut_obj04_init (TutObj04 *self) {
    g_print ("entered tut_obj04_init()\n");
    (void)self;
}
```

### Base object implementation -- virtual methods implementations

Only one implementation for method `vwhoami`:

```C
static void tut_obj04_vwhoami_impl (TutObj04 *self)
{
    g_print ("entered tut_obj04_whoami_impl()\n");
    g_return_if_fail (TUT_IS_OBJ04 (self)); /* it is from external code -- check */
    g_print ("\tI am TutObj04!\n");
    (void)self;
}
```

Passed from external code argument `self` is, as usual, checked for validity.

### Base object implementation -- public interface

Public creation interface is made exactly the same as before. Public interface to
any of virtual methods is just thin wrapper that:
- fetcches virtual table
- finding in that virtual table address of implemetation
- checking that addres is valid (not NULL, address of implementing function)
- chaining call to function at that address, passing all function formal parameters

All wrappers look same, so show only one:

```C
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
```

`TUT_OBJ04_GET_CLASS ()` is automatically generated by `G_DEFINE_TYPE` macro
It gets pointer to class structure from given instance.

***Stop! Why not just use cached pointer to our class structure that was created during
class initialization?***

The answer is: we can not use our *original* class structure. It always contain our
original, intact, virtual table. But derived object have *copies* of our class structure.
Those *copies* may have been modified by derived classes. Particularly, address of function
implementing virtual method may have been changed (method is overriden by derived class).
That is why we should access a *copy* of our class structure, embedded in class structure
of derived class, and use virtual table stored in that *copy*. See also [memory structure
and initialization](gobj-02.md#memory-structure-and-init) from previous chapter.
`TUT_OBJ04_GET_CLASS ()` does this for us.

And again, as usual, check arguments that came from external code.

### Derived object declarations

Full source code is in [src/gobj-04/tut-obj04.h](../src/gobj-04/tut-obj04m.h). File
consists of three basic parts described below.

First part is type identifier declaration with typical use of autoregistration function:

```C
#define TUT_TYPE_OBJ04M (tut_obj04m_get_type())
```

Second part is declaration of final class of objects via library macro:

```C
G_DECLARE_FINAL_TYPE(TutObj04M, tut_obj04m, TUT, OBJ04M, TutObj04);
```

Declaration of final class assumes no descendants will need acces to this class structure.
So, definition of class structure should go to implementation file. Base class for
our new class is TutObj04, not GObject as it was before.

Third part is declarations for public interface to TutObj04 objects:

```C
TutObj04M *tut_obj04m_new (void);
#define tut_obj04m_vwhoami(__self) tut_obj04_vwhoami (TUT_OBJ04 (__self))
#define tut_obj04m_vpure(__self) tut_obj04_vpure (TUT_OBJ04 (__self))
```

When implementing public interface to overriden methods of a base class in derived class
we have choices:
- do not implement it at all, let user somehow obtain pointer to base class instance
- write a macro that casts derived to base then use base's public method
- do full implementation with functions in derived class

We choose second method here, as it allows single implementation for public wrapper
functions and also gives us "expected function names".

### Derived object implementation -- instance structure

Full source code is in [src/gobj-04/tut-obj04m.c](../src/gobj-04/tut-obj04m.c)

Structure of instance data is defined in implementation part and thus hidden from implementations
of derived classes and from user code. We defined final class, so no descendants may have nedd to
access our data

```C
struct _TutObj04M {
    GObject parent_instance;
};
```

### Derived object implementation -- type definition and internal declarations

There are forward declaration of standard functions used in library type definition macro.
Forward declarations of or (overriding) implementations of our virtual methods `vwhoami`
and `vpure` is placed here too.

```C
static void tut_obj04_class_init (TutObj04Class *klass);
static void tut_obj04_init (TutObj04 *self);
static void tut_obj04_vwhoami_impl (TutObj04 *self);

G_DEFINE_TYPE(TutObj04, tut_obj04, G_TYPE_OBJECT);
```

There are no custom definition of class structure at all. Since our class is final, it
will not define new virtual methods. So, its structure is defined in macro by default,
somewhat like:

```C
typedef struct _TutObj04MClass {
    TutObj04Class parent_class;
} TutObj04M;
```

### Derived object implementation -- class and instance initialization

Class initialization should set up pointers to real implementations of virtual methods in
*our copy* of virtual table from TutObj04Class:

```C
static void
tut_obj04m_class_init (TutObj04MClass *klass) {
    TutObj04Class *base_class = TUT_OBJ04_CLASS(klass);

    g_print ("entered tut_obj04m_class_init()\n");
    base_class->vwhoami = tut_obj04m_vwhoami_impl; /* override method implemenattion */
    base_class->vpure = tut_obj04m_vpure_impl;     /* override method implemenattion */
}
```

Instance initialization does nothing, since instance has no initializable data:

```C
static void
tut_obj04m_init (TutObj04M *self) {
    g_print ("entered tut_obj04m_init()\n");
    (void)self;
}
```

### Derived object implementation -- virtual methods implementations

Our derived object implements both methods `vwhoami`and `vpure`:

```C
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
```

Passed from external `self` is checked like before.

### Derived object implementation -- public interface

Real implementation needed only for creation of object:

```C
TutObj04M *
tut_obj04m_new (void) {
    TutObj04M *self;

    g_print ("entered tut_obj04m_new()\n");
    self = (TutObj04M *) g_object_new (TUT_TYPE_OBJ04M, NULL);
    return self;
}
```

### Creating and using objects

Now all is ready to use public APIs of both our objects. This is done in separate
file [src/gobj-04/main.c](../src/gobj-04/main.c):

```C
#include "tut-obj04.h"
#include "tut-obj04m.h"

int
main (void)
{
    TutObj04 *obj;
    TutObj04M *objm;
    GTypeQuery tq;

    g_print ("=======================================================\n");
    objm = tut_obj04m_new ();
    g_print ("G_IS_OBJECT(objm) = %d\n", G_IS_OBJECT (objm));
    g_type_query (TUT_TYPE_OBJ04M, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);
    g_print ("trying objm->TutObj04::vwhoami(void):\n");
    tut_obj04m_vwhoami (objm);
    g_print ("trying objm->TutObj04::vpure(void):\n");
    tut_obj04m_vpure (objm);
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (objm)->ref_count);
    g_clear_object (&objm);

    g_print ("=======================================================\n");
    obj = tut_obj04_new ();
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_type_query (TUT_TYPE_OBJ04, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);
    g_print ("trying obj->TutObj04::vwhoami(void):\n");
    tut_obj04_vwhoami (obj);
    g_print ("trying obj->TutObj04::vpure(void): (should crash)\n");
    tut_obj04_vpure (obj);
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (obj)->ref_count);
    g_clear_object (&obj);

    return 0;
}
```

Note that warning on execution of line `tut_obj04_vpure (obj);`. TutObj04 have not implemented
`vpure` method, so its public wrapper failed gracefully.
