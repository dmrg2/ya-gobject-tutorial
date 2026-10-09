## Using boxed types, enumeration types, flag types

Source code for this chapter is in [/src/gobj-10/](../src/gobj-10/) directory.

In this tutorial we will use so called boxed types, enumerations and flags, how
they all supported by GObject library. Boxed types is GObject built-in way to
use plain old C structures and other composite types that may be represented
as compact blocks of memory. Enumerations and flags are widely used by library
itself. Registering such types in GObject type system allows for more strict
type checking in objects, and for pretty-printing values of flags and
enumerations.

### Declaring enumeration

Full source is in [src/gobj-10/tut-clocks.h](../src/gobj-10/tut-clocks.h).

To declare new enumeration type we need one standart type identifier macro
macro and one function that registers this type in GObject library. In our
header file we also declare C enumeration itself and simple APIs to get text
represenation of that enumeration values:

```C
typedef enum _TutClockEnum {
    TUT_CLOCK_REALTIME,
    TUT_CLOCK_MONOTONIC,
    TUT_CLOCK_BOOTTIME,
    TUT_CLOCK_TAI,
    TUT_N_CLOCKS
} TutClockEnum;

#define TUT_TYPE_ENUMCK (tut_enumck_get_type ())

GType        tut_enumck_get_type (void);
const gchar *tut_enumck_get_name (gint enumck);
const gchar *tut_enumck_get_nick (gint enumck);
```

On registration in library every enumeration value gets "source-like" name and
human readable nickname to allow pretty-printing values. In our public API we
declare functions get either.

### Registering enumeration and its values

Full source is in [src/gobj-10/tut-clocks.c](../src/gobj-10/tut-clocks.c).

Registration of enumerated type is just one big multiline macro:

```C
G_DEFINE_ENUM_TYPE (TutEnumCk, tut_enumck,
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_REALTIME,  "clock-realtime"),
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_MONOTONIC, "clock-monotonic"),
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_BOOTTIME,  "clock-boottime"),
    G_DEFINE_ENUM_VALUE (TUT_CLOCK_TAI,       "clock-tai"))
```

Here we give two forms of type name and list of values with human-readable
nicknames for each of the enumeration values. snake_case form of type name, as
usual, is a prefix for function names this macro assumes. Macro implements
complete function `tut_enumck_get_type ()` for us.

### Simple API for enumeration

In our API we have two functions:
- `tut_enumck_get_name ()` to translate enumeration value to its C source name
- `tut_enumck_get_nick ()` to translate enumeration value to human-readable
nickname that we used on this value registration

```C
static GEnumClass *cached_ec = NULL;

const gchar* tut_enumck_get_name (gint enumck)
{
    GEnumValue *ev;

    if (!cached_ec)
        cached_ec = (GEnumClass *) g_type_class_peek (TUT_TYPE_ENUMCK);
    if (!cached_ec) return NULL;
    ev = g_enum_get_value (cached_ec, enumck);
    if (!ev) return NULL;
    return ev->value_name;
}

const gchar* tut_enumck_get_nick (gint enumck)
{
    GEnumValue *ev;

    if (!cached_ec)
        cached_ec = (GEnumClass *) g_type_class_peek (TUT_TYPE_ENUMCK);
    if (!cached_ec) return NULL;
    ev = g_enum_get_value (cached_ec, enumck);
    if (!ev) return NULL;
    return ev->value_nick;
}
```

Both functions access structure of type `GEnumClass` that created during
registration. To save calls to library we cache pointer to this structure in
static variable and reuse cached pointer.

### Declaring flags type

Full source is in [src/gobj-10/tut-flagssm.h](../src/gobj-10/tut-flagssm.h).

Flags type declared same as enumeration type. There is no difference in
declaration part of these types:

```C
typedef enum _TutSMFlags {
    TUT_SMODE_RELATIVE = 1,
    TUT_SMODE_INTERRUPTIBLE = 2,
    TUT_SMODE_MASK = (TUT_SMODE_RELATIVE | TUT_SMODE_INTERRUPTIBLE)
} TutSMFlags;

#define TUT_TYPE_FLAGSSM (tut_flagssm_get_type ())

GType        tut_flagssm_get_type (void);
const gchar *tut_flagssm_get_first_name (guint flagssm);
const gchar *tut_flagssm_get_first_nick (guint flagssm);
```

To give flag values one common type in C program we define them as enumeraion
members, just giving them values appropriate for integer bit opertions. Public
API is same as declared for enumeration.

### Registering flags type and its values

Full source is in [src/gobj-10/tut-flagssm.c](../src/gobj-10/tut-flagssm.c).

Again, registering is same as for enumeration, except that the name of main
macro differs:

```C
G_DEFINE_FLAGS_TYPE (TutFlagsSM, tut_flagssm,
    G_DEFINE_ENUM_VALUE (TUT_SMODE_RELATIVE,      "relative"),
    G_DEFINE_ENUM_VALUE (TUT_SMODE_INTERRUPTIBLE, "interruptible"))
```

### Simple API for flags type

And again, we have two functions which are almost the same as with enumeration.
The difference is that flags are registered in library with `GFlagsClass` 
structure:

```C
static GFlagsClass *cached_fc = NULL;

const gchar* tut_flagssm_get_first_name (guint flagssm)
{
    GFlagsValue *fv;

    if (!cached_fc)
        cached_fc = (GFlagsClass *) g_type_class_peek (TUT_TYPE_FLAGSSM);
    if (!cached_fc) return NULL;
    fv = g_flags_get_first_value (cached_fc, flagssm);
    if (!fv) return NULL;
    return fv->value_name;
}

const gchar* tut_flagssm_get_first_nick (guint flagssm)
{
    GFlagsValue *fv;

    if (!cached_fc)
        cached_fc = (GFlagsClass *) g_type_class_peek (TUT_TYPE_FLAGSSM);
    if (!cached_fc) return NULL;
    fv = g_flags_get_first_value (cached_fc, flagssm);
    if (!fv) return NULL;
    return fv->value_nick;
}
```

### Declaring boxed type

Full source is in [src/gobj-10/tut-boxedts.h](../src/gobj-10/tut-boxedts.h).

Declaration is similar to previos flags and enumeration. We define some
structure, wrapping one or more compact C objects (for example, array of fixed
length), then define type identifier macro. In addition, we declare functions
to copy contents of our "box" to other "box" and function to free memory when
"box" deleted. Our public API is to create new default-initialized "box" and to
create "box" with given content.

```C
typedef struct _TutBoxedTS {
    struct timespec ts;
} TutBoxedTS;

#define TUT_TYPE_BOXEDTS (tut_boxedts_get_type ())

GType           tut_boxedts_get_type (void);
TutBoxedTS     *tut_boxedts_new (void);
TutBoxedTS     *tut_boxedts_new_data (gint64 tv_sec, gint64 tv_nsec);
TutBoxedTS     *tut_boxedts_copy (TutBoxedTS *boxedts);
void            tut_boxedts_free (TutBoxedTS *boxedts);
```

Note that functions `tut_boxedts_copy ()` and `tut_boxedts_free ()` may be
hidden in implemetation part. We just decided to make them available to user
too.

### Registering boxed type

Full source is in [src/gobj-10/tut-boxedts.c](../src/gobj-10/tut-boxedts.c).

To register boxed type we use single macro, passing copying and freeing
functions to it. These functions are used inside library to make a copies of
our object and to free memory when that copies are deleted. Usually, this
happen when boxed values are passed as arguments or results of functions via
GValue containers.

```C
G_DEFINE_BOXED_TYPE (TutBoxedTS, tut_boxedts,
                     tut_boxedts_copy, tut_boxedts_free)

TutBoxedTS *
tut_boxedts_copy (TutBoxedTS *boxedts)
{
    TutBoxedTS *res;

    g_print ("entered tut_boxedts_copy ()\n");
    if (!boxedts) return NULL;
    res = g_new (TutBoxedTS, 1);
    if (res) *res = *boxedts;
    return res;
}

void
tut_boxedts_free (TutBoxedTS *boxedts)
{
    g_print ("entered tut_boxedts_free ()\n");
    g_free (boxedts);
}
```

### API for boxed type construction

To construct boxed type we make two fully custom functions
`tut_boxedts_new ()` and `tut_boxedts_new_data ()`. The later constructs boxed
with internal data initialized with given values:

```C
TutBoxedTS *
tut_boxedts_new (void)
{
    TutBoxedTS *self;

    g_print ("entered tut_boxedts_new ()\n");
    self = g_new0 (TutBoxedTS, 1);
    return self;
}

TutBoxedTS *
tut_boxedts_new_data (gint64 tv_sec, gint64 tv_nsec)
{
    TutBoxedTS *self;

    g_print ("entered tut_boxedts_new_data ()\n");
    self = g_new (TutBoxedTS, 1);
    if (self)
    {
        self->ts.tv_sec = tv_sec;
        self->ts.tv_nsec = tv_nsec;
    }
    return self;
}
```

### Declaring object with enumeration, flags and boxed

Full source is in [src/gobj-10/tut-gobj10.h](../src/gobj-10/tut-gobj10.h).

Object declaration is [usual three parts](gobj-04.md#derived-object-declarations)
with just a bit more of a public API:

```C
#define TUT_TYPE_OBJ10 (tut_obj10_get_type ())

G_DECLARE_FINAL_TYPE (TutObj10, tut_obj10, TUT, OBJ10, GObject)

/* Public interface to our object */
TutObj10     *tut_obj10_new (void);
TutObj10     *tut_obj10_new_with_boxedts (const TutBoxedTS *boxedts);
TutObj10     *tut_obj10_new_full (const TutBoxedTS *boxedts,
                                  TutClockEnum enumck,
                                  guint /*TutSMFlags*/ flagssm);
gint64        tut_obj10_get_tv_sec (TutObj10 *self);
gint64        tut_obj10_get_tv_nsec (TutObj10 *self);
TutClockEnum  tut_obj10_get_clock (TutObj10 *self);
guint         tut_obj10_get_flags (TutObj10 *self);
void          tut_obj10_do_sleep (TutObj10 *self);
```

We define constructors with property initialization and convenient property
accessors for C programs. Types TutBoxedTS, TutClockEnum are used as is, and
TutSMFlags bit set is represented by guint type.

### Implementing object with enumeration, flags and boxed

Full source is in [src/gobj-10/tut-gobj10.c](../src/gobj-10/tut-gobj10.c).

We define object [with properties and internal data](gobj-05.md) and
[with custom signal](gobj-07.md) exactly as we did it before. Properties and
signal registered at class initialization:

```C
static void
tut_obj10_class_init (TutObj10Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GType signal_param_types[] = {TUT_TYPE_BOXEDTS, TUT_TYPE_ENUMCK,
                                  TUT_TYPE_FLAGSSM};

    g_print ("entered tut_obj10_class_init ()\n");
    object_class->finalize = tut_obj10_finalize;
    object_class->set_property = tut_obj10_set_property;
    object_class->get_property = tut_obj10_get_property;
    tut_obj10_prop_pspec[PROP_BOXEDTS] =
        g_param_spec_boxed ("boxedts",
                            "Boxed timespec",
                            "Property contains boxed struct timespec.",
                            TUT_TYPE_BOXEDTS,
                            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj10_prop_pspec[PROP_CLOCK] =
        g_param_spec_enum  ("clock",
                            "Clock",
                            "Property contains clock type.",
                            TUT_TYPE_ENUMCK,
                            TUT_CLOCK_REALTIME,
                            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj10_prop_pspec[PROP_FLAGS] =
        g_param_spec_flags ("flags",
                            "Flags",
                            "Property contains sleep mode flags.",
                            TUT_TYPE_FLAGSSM,
                            0, /* no flags set */
                            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj10_prop_pspec);
    tut_obj10_signal_id[SIGNAL_SLEEP] =
        g_signal_newv ("sleep",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                           G_SIGNAL_NO_HOOKS,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       NULL /* C marshaller */,
                       G_TYPE_NONE /* return_type */,
                       G_N_ELEMENTS (signal_param_types) /* n_params */,
                       signal_param_types  /* param_types */);
}
```

All our properties are of one of the defined before custom types. Signal uses
all those types at once.

### Passing enumeration, flags and boxed with GValue

In our property accessors all our custom types are passed via GValue. GValue
has ready APIs to store and fetch such flavours of types:

```C
static void
tut_obj10_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj10Private *priv;

    g_return_if_fail (TUT_IS_OBJ10 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj10_get_instance_private (TUT_OBJ10 (object));
    switch ((TutObj10PropEnum) property_id)
    {
    case PROP_BOXEDTS:
        if (priv->boxedts) tut_boxedts_free (priv->boxedts);
        priv->boxedts = g_value_dup_boxed (value);
        if (priv->boxedts)
            g_print ("\tproperty 'boxedts' set: {%ld, %ld}\n",
                     priv->boxedts->ts.tv_sec, priv->boxedts->ts.tv_nsec);
        else
            g_print ("\tproperty 'boxedts' set: (null)\n");
        break;
    case PROP_CLOCK:
        g_return_if_fail (g_value_get_enum (value) < TUT_N_CLOCKS);
        priv->clock = g_value_get_enum (value);
        g_print ("\tproperty 'clock' set: %s\n",
                 tut_enumck_get_nick (priv->clock));
        break;
    case PROP_FLAGS:
        g_return_if_fail ((g_value_get_flags (value) & ~TUT_SMODE_MASK) == 0);
        priv->flags = g_value_get_flags (value);
        if (!priv->flags)
        {
            g_print ("\tproperty 'flags' set: (none)\n");
        }
        else
        {
            g_print ("\tproperty 'flags' set: [ ");
            if (priv->flags & TUT_SMODE_RELATIVE)
                g_print ("%s ",
                         tut_flagssm_get_first_nick (TUT_SMODE_RELATIVE));
            if (priv->flags & TUT_SMODE_INTERRUPTIBLE)
                g_print ("%s ",
                         tut_flagssm_get_first_nick (TUT_SMODE_INTERRUPTIBLE));
            g_print ("]\n");
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}
```

Note that boxed type is passed by reference, so the values itself of a boxed
type are stored in dynamically allocated (and later freed) memory. We should
not forget to `g_value_dup_boxed ()` and `..._free ()` our boxed items when and
where appropriate.

More than that, in internal storage we keep a pointer to instance of boxed
type. Thus, we need a deletion function in our object to free dynamically
allocated memory. This is the difference from custom fundamental type we
defined in [previous chapter](gobj-09.md).

### Another convenience macro G_PRIVATE_FIELD

In our public API we used another macro `G_PRIVATE_FIELD` for convenient access
to private structure filelds:

```C
gint64
tut_obj10_get_tv_sec (TutObj10 *self)
{
    g_print ("entered tut_obj10_get_tv_sec ()\n");
    g_return_val_if_fail (TUT_IS_OBJ10 (self), 0);
    return G_PRIVATE_FIELD (TutObj10, self, TutBoxedTS *, boxedts)->ts.tv_sec;
}
```

This macro hides inside:
- getting pointer to private data
- casting that pointer to pointer to private struct type
- applying member access operator `->` to that pointer

Field from private structure is returned from macro as lvalue.

### Creating object with enumeration, flags and boxed

At creation of our object our custom types may be passed as usual parameters
to generic GObject constructor:

```C
TutObj10 *
tut_obj10_new_full (const TutBoxedTS *boxedts,
                    TutClockEnum enumck,
                    guint /*TutSMFlags*/ flagssm)
{
    TutObj10 *self;

    g_print ("entered tut_obj10_new_full ()\n");
    self = g_object_new (TUT_TYPE_OBJ10,
                         "boxedts", boxedts,
                         "clock", enumck,
                         "flags", flagssm,
                         NULL);
    return self;
}
```

Again, boxed type may be passed by reference only. We can not use memoryless
temporary value.

### Emitting signal with enumeration, flags and boxed parameters

And again, boxed type as a parameter of signal is passed via pointer too:

```C
void
tut_obj10_do_sleep (TutObj10 *self)
{
    TutObj10Private *priv;

    g_print ("entered tut_obj10_do_sleep ()\n");
    g_return_if_fail (TUT_IS_OBJ10 (self));
    priv = tut_obj10_get_instance_private (self);
    g_return_if_fail (priv->boxedts != NULL);
    g_signal_emit (G_OBJECT (self), tut_obj10_signal_id[SIGNAL_SLEEP], 0,
                   priv->boxedts, priv->clock, priv->flags);
}
```

Handler for this signal must have signature like
`void (*) (TutObj *, TutBoxedTS *, TutClockEnum, guint, void *)`. But there is
no need to implemet a custom marshaller. Pointer type is primitive enough for
GValue to pass anywhere.

### User code with values of enumeration, flags and boxed

Full source code is in [src/gobj-10/main.c](../src/gobj-10/main.c).

Enumeration and flags are ordinary C enum types. There is nothing special with
them, they treated as usual integer values in C. Boxed type is, at one side,
plain old C structure, and may be used as that. At other side, it is kind of
items the library can create

```C
    TutBoxedTS *boxedts = tut_boxedts_new_data (5, 6);
```

and pass via GValue

```C
    GValue gval = G_VALUE_INIT;
    /* ... */
    g_value_init (&gval, TUT_TYPE_BOXEDTS);
    g_value_set_boxed (&gval, boxedts);
    g_object_set_property (G_OBJECT (obj), "boxedts", &gval);
    g_object_get_property (G_OBJECT (obj), "boxedts", &gval);
```

provided that user code do not forget to free memory allocated for it:

```C
    /* ... */
    tut_boxedts_free (boxedts);
```

We had defined public API to retrieve user-readable names for pretty-printing
values of enumeration and flags types:

```C
    g_print ("obj.clock = %s\n",
             tut_enumck_get_nick (g_value_get_enum (&gval)));
    /* ... */
    if (g_value_get_flags (&gval) & TUT_SMODE_RELATIVE)
        g_print ("%s ", tut_flagssm_get_first_nick (TUT_SMODE_RELATIVE));
```

Handling signals with parameters of these custom types is straightforward:

```C
static void
on_signal_sleep (TutObj10 *object,
                 TutBoxedTS *boxedts,
                 TutClockEnum enumck,
                 guint /*TutSMFlags*/ flags,
                 gpointer user_data)
{
    g_print ("got into on_signal_sleep ({%ld, %ld}, %d, 0x%08X)\n",
             boxedts->ts.tv_sec,
             boxedts->ts.tv_nsec,
             enumck,
             flags);
}
```

As said before, boxed type can be passed by pointer only. The *owner* of data
pointed by that pointer is some GValue inside the library. So, there is no need
to free memory pointed by `boxedts` argument of handler function.

Running a program shows how many allocations and destructions of boxed values
happen while passing that boxed values around.
