## Creating new fundamental type, passing its value between parts of code

Source code for this chapter is in [/src/gobj-09/](../src/gobj-09/) directory.

In this tutorial we will register new so called fundamental type, see how
*value collection* works, and create an object which uses that new fundamental
type.

GObject library already have all primitive types that are common to C language.
These types, like `int`, `double`, `char`, etc, are treated special way in some
parts of library. Particularly, these types in most situalions may be passed
by their value, while complex compound types like GObject are passed by
reference (using pointer). In rare cases you will need a new fundamental type,
but library provides API to create it, and we will try to use this API.

### About GValue type

GValue type in the library used as universal container for simple (primitive)
values, including pointers of any kind. This container is used throughout the
library to store values and pass that values when types and number of values
are unknown beforehead. For example, some class may be instaintiated using
variadic function `g_object_new (type, ...)` where variadic part of parameter
list may contain any set of properties of any known type, each together with
some value. These values are stored in GValue before being passed to to
`g_object_set_property ()` do do actual value assignment.

In its internals, GValue is simple C structure, holding type identifier of a 
value that is currently stored in it, and small storage to keep the value in.
This small storage is sized as 2 * sizeof (int64_t), so, when holding a pointer
to something, GValue may have some additional information about that pointer or
about memory that pointer points to. This additional information is sometimes
used in library indeed.

There are many ready library-implemented APIs to fetch values of different 
primitive types from GValue and to put those values into GValue. Part of these
APIs is so called *value collection*. Library knows, for any primitive type,
how  to read its value from C function argument list (including variadic list)
and  how to build such C function argument list from set of GValue objects.
This part of library does the work when we use generic constructor in form of
`g_object_new (type, "properyA", valueA, "propertyB", valueB, ...)`.

Any new primitive type must be capable of value collection for those APIs to
work. APIs to simply copy value between C variable and GValue forth and back
are needed too. Library provides means to implement all of this.

### Timespec as primitive

There is known time type `struct timespec` in standart C header `time.h`. It is
simple structure with two integer fields. Technically, it fits well in internal 
storage of GValue. C variables of type `struct timespec` behave almost as any
other variables of non-decomposable types. Value of `struct timespec` may be 
assigned to C variable as simple as `one_timespec = another_timespec`, may be
passed to C function as argument by value, may be returned as a result of C 
function, and so on.

We just implement the other parts needed for `timespec` to be primitive from
point of view of GObject library.

### Timespec type declaration

Full source is in [src/gobj-09/tut-timespec.h](../src/gobj-09/tut-timespec.h).

New type for Timespec is declared exactly as for new object type. We define a 
macro expanding to type registration function:

```C
#define TUT_TYPE_TIMESPEC (tut_timespec_get_type ())
```

But, apart from that, GObect library requires one additional type: "*parameter*
of type Timespec". Library uses second to treat our Timespec "as argument to
function", "as property type descriptor" and things like it. So, we define one
more macro:

```C
#define TUT_PARAM_TYPE_TIMESPEC (tut_timespec_get_param_type ())
```

Next, we define data structure of our Timespec and declare type registration
functions, as we did it with objects:

```C
typedef struct _TutTimespec {
    gint64 tv_sec;
    gint64 tv_nsec;
} TutTimespec;

GType                      tut_timespec_get_type (void);
GType                      tut_timespec_get_param_type (void);
```

Next we need all of those handy APIs to create and delete Timespec, put it into
GValue and define properties of type Timespec in objects:

```C
GParamSpec                *g_param_spec_timespec (const gchar* name,
                                                  const gchar* nick,
                                                  const gchar* blurb,
                                                  GParamFlags flags);
static inline TutTimespec *tut_timespec_new (void)
    { return (TutTimespec *) g_malloc0 (sizeof (TutTimespec)); }
static inline TutTimespec *tut_timespec_new_data (gint64 tv_sec, gint64 tv_nsec)
    { TutTimespec *tuts = (TutTimespec *) g_malloc (sizeof (TutTimespec));
      if (tuts) { tuts->tv_sec = tv_sec; tuts->tv_nsec = tv_nsec; }
      return tuts; }
static inline void         tut_timespec_delete (TutTimespec *tuts)
    { g_free (tuts); }
static inline TutTimespec  g_value_get_tut_timespec (const GValue *gval)
    { TutTimespec res = {0, 0};
      g_return_val_if_fail (G_VALUE_HOLDS (gval, TUT_TYPE_TIMESPEC), res);
      res.tv_sec = gval->data[0].v_int64; res.tv_nsec = gval->data[1].v_int64;
      return res; }
static inline void         g_value_set_tut_timespec (GValue *gval,
                                                     TutTimespec tuts)
    { g_return_if_fail (G_VALUE_HOLDS (gval, TUT_TYPE_TIMESPEC));
      gval->data[0].v_int64 = tuts.tv_sec;
      gval->data[1].v_int64 = tuts.tv_nsec; }
```

Most of these APIs are trivial, so we defined them as inline functions in
header.

### Timespec implemetation -- registering types

Full source is in [src/gobj-09/tut-timespec.c](../src/gobj-09/tut-timespec.c).

To register both declared types, we manually recreate registration mechanism
that is made automatically by macros for "ordinary" object types:

```C
typedef struct _TutTimespecParamSpec {
    GParamSpec parent_param_spec;
} TutTimespecParamSpec;

static GType tut_timespec_get_type_once ();
static GType tut_timespec_get_param_type_once ();

GType
tut_timespec_get_type (void)
{
    static GType static_g_define_type_id = 0;

    if (g_once_init_enter_pointer (&static_g_define_type_id))
    {
        GType g_define_type_id = tut_timespec_get_type_once ();
        g_once_init_leave_pointer (&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}

GType
tut_timespec_get_param_type (void)
{
    static GType static_g_define_type_id = 0;

    if (g_once_init_enter_pointer (&static_g_define_type_id))
    {
        GType g_define_type_id = tut_timespec_get_param_type_once ();
        g_once_init_leave_pointer (&static_g_define_type_id, g_define_type_id);
    }
    return static_g_define_type_id;
}
```

Like object classes, parameters of our new type have its own storage structure,
here called `TutTimespecParamSpec`. Actual type definitions are made in
"oneshot" functions below:

```C
static void tut_timespec_value_init      (GValue *value);
static void tut_timespec_value_copy      (const GValue *src_value,
                                          GValue *dst_value);
static gchar* tut_timespec_collect_value (GValue *value,
                                          guint n_collect_values,
                                          GTypeCValue *collect_values,
                                          guint collect_flags);
static gchar* tut_timespec_lcopy_value   (const GValue *value,
                                          guint n_collect_values,
                                          GTypeCValue *collect_values,
                                          guint collect_flags);

static GType tut_timespec_get_type_once () {
    GTypeValueTable vci = {
        .value_init = tut_timespec_value_init,
        .value_free = NULL,
        .value_copy = tut_timespec_value_copy,
        .value_peek_pointer = NULL,
        .collect_format = "qq",
        .collect_value = tut_timespec_collect_value,
        .lcopy_format = "p",
        .lcopy_value = tut_timespec_lcopy_value
    };
    GTypeInfo ti = {
        .class_size = 0,
        .base_init = NULL,
        .base_finalize = NULL,
        .class_init = NULL,
        .class_finalize = NULL,
        .class_data = NULL,
        .instance_size = 0,
        .n_preallocs = 0,
        .instance_init = NULL,
        .value_table = &vci
    };
    static GTypeFundamentalInfo tfi = {
        .type_flags = 0
    };

    g_print ("tut_timespec_get_type_once (): registering fundamental type\n");
    return g_type_register_fundamental (g_type_fundamental_next (),
                                        g_intern_static_string ("TutTimespec"),
                                       &ti,
                                       &tfi,
                                        G_TYPE_FLAG_NONE);
}

static GType tut_timespec_get_param_type_once () {
    GParamSpecTypeInfo psti = {
        .instance_size = sizeof (TutTimespecParamSpec),
        .n_preallocs = 0,
        .value_type = TUT_TYPE_TIMESPEC,
        .instance_init = NULL,
        .finalize = NULL,
        .value_set_default = NULL,
        .value_validate = NULL,
        .values_cmp = NULL
    };

    g_print ("tut_timespec_get_param_type_once ():"
             " registering paramspec type\n");
    return g_param_type_register_static
               (g_intern_static_string ("TutTimespecParamspec"), &psti);
}
```

To do type registration for fundamental type we should provede value
manupulation functions:
- `tut_timespec_value_init ()` to initialize value with some contents on
creation or on initialization of GValue container
- `tut_timespec_value_copy ()` to copy vale between two GValue containers
- `tut_timespec_collect_value ()` to read (*collect*) value from list of
"really primitive" values like 'char', 'short', and likes
- `tut_timespec_lcopy_value ()` to write (*lcopy* or decompose) value to list
of "really primitive" values

These functions form interface to handle our new fundamental type inside
GObject library.

Next is to define a format of storage of our new fundamental type in argument
list of a C function or in its return value. This is used to support automatic
binding arguments of C functions like `g_object_new ()` to lists of primitives
used in collection/lcopy process. The format is defined by a format string in
fields `collect_format` and `lcopy_format`. Here we define the format as two
int64_t values, placed in memory sequentially. Our structure, indeed, pushed
on stack of called C function like that.

Next, we fill TypeInfo structure. Our fundamental type is not class, it has no
class structure and need not to inialize something in it, so, TypeInfo is
almost empty. Only one field, table of value manipulation methods, is set.

And the last for fundamental type, we define type flags. Again, our type is not
class, it can not be abstract or final, so our flags are empty.

To do a registration itself we use `g_type_register_fundamental ()` which
requires caller-supplied value of type identifier. Next free identifier from
set of reserved for fundamentals we get with `g_type_fundamental_next ()`.

Registering parameter specification is more like registering class.
`GParamSpecTypeInfo` structure has fields similar to class structure. From all
these fields we need only memory size of our type and its type identifier. No
special initialization of parameters of our simple type needed.

Note that we *may* supply a function to validate values *of parameters* of our
type Timespec and function to set default values to that parameters. But this
is an option.

### Timespec implemetation -- value manipulation functions

Our Timespec looks like array of two big integers in C program memory, so, let
it look the same for GObject library. Our data manipulation functions are
simple:

```C
static void tut_timespec_value_init (GValue *value) {
    value->data[1].v_int64 = value->data[0].v_int64 = 0;
    g_print ("\t[timespec] init {%ld, %ld}\n",
             value->data[0].v_int64, value->data[1].v_int64);
}

static void tut_timespec_value_copy (const GValue *src_value,
                                     GValue *dst_value) {
    dst_value->data[0].v_int64 = src_value->data[0].v_int64;
    dst_value->data[1].v_int64 = src_value->data[1].v_int64;
    g_print ("\t[timespec] copied {%ld, %ld}\n",
             dst_value->data[0].v_int64, dst_value->data[1].v_int64);
}

static gchar* tut_timespec_collect_value (GValue *value,
                                          guint n_collect_values,
                                          GTypeCValue *collect_values,
                                          guint collect_flags) {
    g_return_val_if_fail (n_collect_values == 2,
                          g_strdup_printf ("expected exactly two ints, got %d",
                                           n_collect_values));
    value->data[0].v_int64 = collect_values[0].v_int64;
    value->data[1].v_int64 = collect_values[1].v_int64;
    g_print ("\t[timespec] collected {%ld, %ld}\n",
             value->data[0].v_int64, value->data[1].v_int64);
    return NULL;
}

static gchar* tut_timespec_lcopy_value (const GValue *value,
                                        guint n_collect_values,
                                        GTypeCValue *collect_values,
                                        guint collect_flags) {
    TutTimespec *ts_p;
    g_return_val_if_fail (n_collect_values == 1,
                          g_strdup_printf ("expected one pointer, got %d",
                                            n_collect_values));
    ts_p = (TutTimespec *) collect_values[0].v_pointer;
    g_return_val_if_fail (ts_p != NULL,
                          g_strdup_printf ("value location for %s is null",
                                            G_VALUE_TYPE_NAME (value)));
    ts_p->tv_sec = value->data[0].v_int64;
    ts_p->tv_nsec = value->data[1].v_int64;
    g_print ("\t[timespec] L-copied {%ld, %ld}\n",
             ts_p->tv_sec, ts_p->tv_nsec);
    return NULL;
}
```

### Timespec implemetation -- creating parameter specification

Since all what is needed to pass value of Timespec is ready, we may define our
parameter specification with default behavior:

```C
GParamSpec *
g_param_spec_timespec (const gchar* name, const gchar* nick,
                       const gchar* blurb, GParamFlags flags)
{
    return g_param_spec_internal (TUT_PARAM_TYPE_TIMESPEC, name,
                                  nick, blurb, flags);
}
```

And that is all with our "primitive type". Phew.

### Declaring object using Timespec

Full source code is in [src/gobj-09/tut-obj09.h](../src/gobj-09/tut-obj09.h).

We want to define an object with single property "timespec" of type Timespec,
and attach single signal "timespec" with Timespec parameter to it. This give
us enough use cases for Timespec:

```C
#define TUT_TYPE_OBJ09 (tut_obj09_get_type ())

G_DECLARE_FINAL_TYPE (TutObj09, tut_obj09, TUT, OBJ09, GObject)

/* Public interface to our object */
TutObj09    *tut_obj09_new (void);
TutObj09    *tut_obj09_new_data (TutTimespec tuts);
TutObj09    *tut_obj09_new_rawdata (gint64 tv_sec, gint64 tv_nsec); /* with value collector hack inside */
void         tut_obj09_signal_timespec (TutObj09 *self);
void         tut_obj09_set_timespec (TutObj09 *self, TutTimespec tuts);
TutTimespec  tut_obj09_get_timespec (TutObj09 *self);
```

### Implementing property and signal in object

Full source code is in [src/gobj-09/tut-obj09.c](../src/gobj-09/tut-obj09.c).

Most parts of object implementation are usual. Here is how we add a property
of our new fundamental type and a signal using that type:

```C
static void
tut_obj09_class_init (TutObj09Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GType signal_param_types[] = {TUT_TYPE_TIMESPEC};

    g_print ("entered tut_obj09_class_init ()\n");
    object_class->set_property = tut_obj09_set_property;
    object_class->get_property = tut_obj09_get_property;
    tut_obj09_prop_pspec[PROP_TIMESPEC] =
        g_param_spec_timespec ("timespec",
                               "Timespec",
                               "Property holding classic struct timespec.",
                               G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj09_prop_pspec);
    tut_obj09_signal_id[SIGNAL_TIMESPEC] =
        g_signal_newv ("timespec",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                           G_SIGNAL_NO_HOOKS,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       tut_obj09_timespec_marshaller, /* we need custom marshaller for
                                                         our non-ffi type */
                       G_TYPE_NONE /* return_type */,
                       1    /* n_params */,
                       signal_param_types);
    /* We can also decompose timespec to two int64 primitives in 'signal_param_types'
       and use generic marshaller out of the box, as we did it in tut_obj09_new_rawdata ().
       This hack works too. */
}
```

All is like it was before, except that new "marshaller" parameter in signal
definition. Since GObject library's low-level code, based on FFI library does
not understand our new "primitive" type, we must supply a function that
"marshal" Timespec. Marshalling is moving program data between parties in a
form that all these parties can understand. We have to do it ourself:

```C
static void
tut_obj09_timespec_marshaller (GClosure     *closure,
                               GValue       *return_value,
                               guint         n_param_values,
                               const GValue *param_values,
                               gpointer      invocation_hint,
                               gpointer      marshal_data)
{
    typedef void (*tut_obj09_timespec_callback) (TutObj09 *object,
                                                 TutTimespec tuts,
                                                 gpointer user_data);

    /* assume marshal_data may contain callback address only (as it is usual with simple signal) */

    GCClosure *C_closure;
    TutObj09 *object;
    gpointer user_data;
    TutTimespec tuts;
    tut_obj09_timespec_callback callback;

    g_print ("entered tut_obj09_timespec_marshaller ()\n");
    C_closure = (GCClosure*) closure;
    callback = marshal_data ?
               (tut_obj09_timespec_callback) marshal_data :
               (tut_obj09_timespec_callback) C_closure->callback;
    tuts = g_value_get_tut_timespec (&param_values[1]);
    if (G_CCLOSURE_SWAP_DATA (C_closure))
    {
        user_data = g_value_peek_pointer (&param_values[0]);
        object = (TutObj09 *) closure->data;
    }
    else
    {
        object = (TutObj09 *) g_value_peek_pointer (&param_values[0]);
        user_data = closure->data;
    }
    callback (object, tuts, user_data);
}
```

I can not say "exhaustive" talking of documentation on
[closures](https://docs.gtk.org/gobject/struct.Closure.html) and
[marshalling](https://docs.gtk.org/gobject/alias.SignalCMarshaller.html), but,
in short words, we do the following:
- find address of C calback function (which may be either in given closure, or
in argument of our marshaller function)
- fetch callback function argument values, which are supplied by GObject
library in list of universal GValue containers, `param_values`
- arrange that arguments in right for C callback function order
- do a traditional C function call (which is pushing agruments on a stack) of
callback function

### Implementing object creation

Since our object has property, it is straightforward to implement a constructor
that will create objects with that property set:

```C
TutObj09 *
tut_obj09_new_data (TutTimespec tuts)
{
    TutObj09 *self;

    g_print ("entered tut_obj09_new_data ()\n");
    self = (TutObj09 *) g_object_new (TUT_TYPE_OBJ09, "timespec", tuts, NULL);
    return self;
}
```

Data collection for such property initialization is implemented in our Timespec
type. It is interesting that we can use this universal data collection mechanim
slightly "non-standard" way:

```C
TutObj09 *
tut_obj09_new_rawdata (gint64 tv_sec, gint64 tv_nsec)
{
    TutObj09 *self;

    g_print ("entered tut_obj09_new_rawdata ()\n");
    /* This hack works because TutTimespec 'collect_format' set to "qq". Underlying value
       collector of GObject library collects structure as two distict int64 primitives. */
    self = (TutObj09 *) g_object_new (TUT_TYPE_OBJ09,
                                      "timespec", tv_sec, tv_nsec,
                                      NULL);
    return self;
}
```

As said in comment, decomposed value of Timespec seamless go to the property
too.

### Creating and using Timespec and object with Timespec onboard

Full source code is in [src/gobj-09/main.c](../src/gobj-09/main.c).

Timespec itself is plain old C structure. It will do all that any other C
structure do:

```C
    TutTimespec ts_first = {1, 0};
    TutTimespec ts_second = {2, 0};
    /* ... */
    g_print ("first = {%ld, %ld}\n", ts_first.tv_sec, ts_first.tv_nsec);
    g_print ("second = {%ld, %ld}\n", ts_second.tv_sec, ts_second.tv_nsec);
```

Public API knows type `TutTimespec` and allows to create it dynamically:

```C
    TutTimespec *ts_third_p = tut_timespec_new_data (3, 0);
    /* ... */
    g_print ("Type id %lu, name %s\n", TUT_TYPE_TIMESPEC,
             g_type_name (TUT_TYPE_TIMESPEC));
    /* ... */
    tut_timespec_delete (ts_third_p);
```

Our object with Timespec may take and give Timespec values via its property,
and can emit signals with timespec parameter:

```C
static void on_signal_timespec (TutObj09 *object,
                                TutTimespec param_timespec,
                                gpointer user_data);

int main (void) {
    /* ... */
    TutTimespec ts_test = {0};
    TutObj09 *obj, *objdata, *objraw;
    /* ... */
    obj = tut_obj09_new ();
    objdata = tut_obj09_new_data (ts_test);
    objraw = tut_obj09_new_rawdata ((gint64)4, (gint64)0);
    /* ... */
    g_signal_connect (obj, "timespec",
                      G_CALLBACK (on_signal_timespec), "from obj");
    g_signal_connect (objdata, "timespec",
                      G_CALLBACK (on_signal_timespec), "from objdata");
    g_signal_connect (objraw, "timespec",
                      G_CALLBACK (on_signal_timespec), "from objraw");
    tut_obj09_signal_timespec (obj);
    tut_obj09_signal_timespec (objdata);
    tut_obj09_signal_timespec (objraw);
    /* ... */
}
```

We can also get TutTimespec-typed property value with value lcopying:

```C
    TutTimespec ts_test = {0};
    /* ... */
    /* ... */
    /* ... */
    g_object_get (objdata, "timespec", &ts_test, NULL);
    g_print ("g_object_get (objdata) -> {%ld, %ld}\n",
             ts_test.tv_sec, ts_test.tv_nsec);
    g_object_get (objraw, "timespec", &ts_test, NULL);
    g_print ("g_object_get (objraw) -> {%ld, %ld}\n",
             ts_test.tv_sec, ts_test.tv_nsec);
    /* ... */
```

Thanks GObject library design, signal "notify" from GObject do not pass values
of properties. Else we would have to implement another very big marshaller. )
