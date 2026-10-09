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
    ...
    instance = g_object_new (type, NULL);
    ...
```

Second argument of function [g_object_new()](https://docs.gtk.org/gobject/ctor.Object.new.html) is
(or may be, if this is class-defined behavior) used to initialize object data after memory allocation.

