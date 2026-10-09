## GObject itself, creation of GObject and descendants, information on instances and its classes

Source code for this chapter is in <../src/gobj-01/> directory.

### GObject itself

GObject itself has two distinct meanings:

- It is a kind of objects a program may create, delete, pass between parts of code, somehow
 manipulate etc. All that is called "class" in object-oriented programming.
 
- It is one specific instance of such objects.

Since GObject is base class for all other objects, all objects are of GObject class. Objects
derived from GObject form subsets of whole GObject set. That is called subclasses in
object-oriented programming.

Technically, one instance of GObject is plain old C structure with three fields:

- a pointer to class structure (GTypeClass *g_class)
- unsigned reference counter (guint ref_count)
- a pointer to instance's additional memory block (GData *qdata)

Class structure, named above GTypeClass, is is more complex structure. It contains information about
a class of objects (its structure, capabilities, behavior, etc, common for all instances of a class).
Even name of a type "GObject" stored somewhere in memory, and that memory referred (pointed to) in
GTypeClass structure.

Reference counter holds count of refrences (pointers) taken to this instance in different parts of
a program. It is responsibility of every such program part to incremet counter on taking reference
(a pointer) and decrement it when this reference not needed anymore. When reference counter reaches
zero (no part of a program nedds this instance of GObject), GObject may be auto-destroyed.

Additional memory block may be used to store additional data, associated with this specific instance
of GObject. For example, there are several mechanisms inside the GObject library, which makes
use of short character strings, associates witn some instances of GObject. This part of GObject
isn't often used.

Since GObject do not contain any user (or application) data, it has no storage for "payload". All
data inside GObject is for library internal use. Nevertheless, instances of GObject may be created.
These instances will take 3*sizeof(pointer) memory each.

### Programming interfaces to GObject

There are many interfaces to create, destroy and manipulate GObject instances in the library. These
interfaces implemented as plain old C functions. One of the formal arguments of such functions
is a pointer to GObject instance (except object creation functions: they *return* a pointer to GObject).
This GObject * argument looks like this pointer in C++. Actually it *is* almost perfect equivalent to.

Suppose C++ method function:

```C++
GObject * GOBject::ref () {
        this->ref_count++;
        return this;
}
```

The same does GObject library C function:
```C
GObject * g_object_ref (GObject * self) {
        self->ref_count++;
        return self;
}
```

The difference is in explicit self argument only.

Beside GObject "methods" there are other interfaces to manupulate object class information, register
new classes of objects and so on. One of the basic concepts in library is concept of GType. GType
is numeric identificator of any known type in system. In short, GType is a number under which a type
is registered somewhere in an internal table of types in a system. Every class of objects have unique
type identifier. But not all type identifiers refer to objects: there are type identifiers for basic
C types like char, int, double, type identifier for zero-terminated strings and so on.

Some of the programming interfaces use this type identifier to fetch information on types or create
objects of that type. GObject is created using its type identifier, defined as macro G_TYPE_OBJECT.
Descendants of GObject created same way.

See part of [src/gobj-01/main.c](../src/gobj-01/main.c):

```C
void
create_and_report (GType type) {
    GObject        *instance;
    /* skipped */
    instance = g_object_new (type, NULL);
    /* skipped */
```

Second argument of function [g_object_new()](https://docs.gtk.org/gobject/ctor.Object.new.html) is
(or may be, if this is class-defined behavior) used to initialize object data after memory allocation.

Look at the name of function g_object_new(). It consists of three parts, separated by underscores.
This is intentional: the naming convention states that its full name should be combination of

- short module name ("g" in this case)
- type name ("object" it this case)
- and specific function or method name ("new" in this case)

Many convenience macros that automating code generations use this scheme of naming, thus force your
program to adhere to.

### Creation and deletion

Add final steps to the same part of [src/gobj-01/main.c](../src/gobj-01/main.c):

```C
void
create_and_report (GType type) {
    GObject        *instance;
    /* skipped */
    instance = g_object_new (type, NULL);
    /* skipped */
    g_print ("before unref: instance->ref_count = %d\n", instance->ref_count);
    g_object_unref (instance); /* or g_clear_object(&instance) */
    /* skipped */
}
```
- `instance = g_object_new (type, NULL);` is where the object created with initial reference
count 1.
- `g_print ("before unref: instance->ref_count = %d\n", instance->ref_count);` is where the
object refrence count reported.
- `g_object_unref (instance);` is where the object refrence count decremented and reached 0,
object automatically destroyed.

Beside that, library done many internal things: initialized instance memory after creation,
called pre- and post-initialization functions, if they were, freed instance memory after destruction,
freed additional memory, if instance used that, and so on. A small part of details of this process
shown in next chapter [gobj-02](gobj-02.md).

### Using API functions

Again, part of [src/gobj-01/main.c](../src/gobj-01/main.c):

```C
void
create_and_report (GType type) {
    GObject        *instance;
    /* skipped */
    instance = g_object_new (type, NULL);
    type_instance = (GTypeInstance *) instance; /* class GObject is GTypeInstance { ... } */
    gtype = G_TYPE_FROM_INSTANCE (type_instance);
    quick_type = G_OBJECT_TYPE (instance);
    named_type = g_type_from_name (g_type_name (type));
    /* skipped */
}
```

Having pointer to a live instance of object we can cast it to pointer much simplier structure

```C
typedef struct _GTypeInstance
{
  GTypeClass *g_class;
} GTypeInstance;
```
Memory layout of GTypeInstance exatly mathes memory layout of beginning of any GObject. Library uses 
such pointers (GTypeInstance *) to type formal arguments of several APIs. Here, in line
`gtype = G_TYPE_FROM_INSTANCE (type_instance);` we fetching type identifier for object pointed by
instance with corresponding macro. Another way is to use ready macro G_OBJECT_TYPE, accepting
any pointer: `quick_type = G_OBJECT_TYPE (instance);`. We can even fetch the type identifier by the
name of type `named_type = g_type_from_name (g_type_name (type));`. All three type identifiers are the
same, program will report it later:

```
    g_print ("G_TYPE_FROM_INSTANCE (type_instance)  = 0x%lX\n", gtype);
    g_print ("G_OBJECT_TYPE (instance)              = 0x%lX\n", quick_type);
    g_print ("type_from_name (***)                  = 0x%lX\n", named_type);
```

More than that, they are same as formal argument 'type' passed to our function.

Next, we can get a reference to class structure:

```C
void
create_and_report (GType type) {
    GObject        *instance;
    /* skipped */
    instance = g_object_new (type, NULL);
    /* skipped */
    class = G_OBJECT_GET_CLASS (instance);
    klass = g_type_class_get (gtype);
    /* skipped */
    g_print ("G_OBJECT_GET_CLASS (instance) -> %p\n", class);
    g_print ("g_type_class_get (instance)   -> %p\n", klass);
    /* skipped */
}
```

Some specific APIs in library use class structure to get detailed information describing instances
of that class.

Gnerally, we should prefer common-use public interfaces based on GType:

```C
void
create_and_report (GType type) {
    /* skipped */
    GTypeQuery      tq;
    GType          *children;
    guint           n_children;
    GType          *interfaces;
    guint           n_interfaces;
    /* skipped */
    g_type_query (gtype, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);

    children = g_type_children (gtype, &n_children);
    g_print ("Children (%u):\n", n_children);
    for (i = 0; i < n_children; i++)
        g_print("\t%s\n", g_type_name (children[i]));
    g_free (children);

    interfaces = g_type_interfaces (gtype, &n_interfaces);
    g_print ("Interfaces (%u):\n", n_interfaces);
    for (i = 0; i < n_interfaces; i++)
        g_print("\t%s\n", g_type_name (interfaces[i]));
    g_free (interfaces);
    /* skipped */
}
```
We can query type name, instance size and class structure size with `g_type_query (gtype, &tq);`. We can query
interfaces that this instance implements with `interfaces = g_type_interfaces (gtype, &n_interfaces);`. We can
query list of direct descendants of this instance's class with `children = g_type_children (gtype, &n_children);`
and so on.

Note that `g_free (children);` and `g_free (interfaces);`. Some APIs allocating memory to place large-sized
results, then *transfer ownership* of allocated memory to caller. In such case, it is a caller responsibility to
free allocated memory blocks after use. Such aspect API behavior is documented in [GObject reference](https://docs.gtk.org/gobject/).

### Type identifier macro and auto-registration of types

Main function of example program [src/gobj-01/main.c](../src/gobj-01/main.c) calls reporting function
for several types from GObject and GLib libraries:

```C
int
main (void)
{
    /* ! Should only try default constructed (that have xxx_new(void)) ! */
    create_and_report (G_TYPE_OBJECT);
    create_and_report (G_TYPE_BINDING_GROUP);
    create_and_report (G_TYPE_APPLICATION);
    create_and_report (G_TYPE_MENU);
    /* *** note changes in 'Children:' *** */
    create_and_report (G_TYPE_OBJECT);

    return 0;
}
```

Look at last, "duplicate" report for GObject type. It differs from first: class GObject have got children (derived
classes). This is because of type autoregistration, hidden in macros G_TYPE_xxx. Look at some of them:

```C
#define G_TYPE_APPLICATION (g_application_get_type ())
```

`G_TYPE_APPLICATION` expands to function call. Function `g_application_get_type ()` (again, note structure of its
name) *registers* new type for GApplication in library type system. Since GObject base type for GApplication,
class GObject gets new subclass at this moment. The registration of a type is one-time operation, functions like
g_xxx_get_type() cache registered type identifier and return cached value on subsequent calls.
