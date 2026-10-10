## Object properties and interface to access them

Source code for this chapter is in [/src/gobj-05/](../src/gobj-05/) directory.

In this tutorial we wil create object with two properties `text`and `text-writable`.
Property `text` is just string, and property `text-writable` is boolean, controlling
possibility to change `text`. When `text-writable` is FALSE, `text` can not be set.

Source code for this tutorial is structured in separate heder and source files.

### Object declaration

Full source code is in [src/gobj-05/tut-obj05.h](../src/gobj-05/tut-obj05.h). File
consists of three "standard" parts:
- type identifier declaration
- final type declaration wit convenience macro
- public interface declarations

```C
/* Public interface to our object */
TutObj05    *tut_obj05_new (void);
TutObj05    *tut_obj05_new_with_text (const gchar *text);
TutObj05    *tut_obj05_new_full (gboolean text_writable, const gchar *text);
/*  ... with property accessors */
void         tut_obj05_set_text_writable (TutObj05 *self, gboolean text_writable);
gboolean     tut_obj05_get_text_writable (TutObj05 *self);
void         tut_obj05_set_text (TutObj05 *self, const gchar* text);
const gchar *tut_obj05_get_text (TutObj05 *self);
```

There are three methods of object creation, roughly equivalent to C++ constructors
`TutObject05::TutObject05(void)`, `TutObject05::TutObject05(char *text)` and
`TutObject05::TutObject05(bool text_writable, char *text)`.

Other four methods are convenient property accessors for C programs. They can be used
instead of generic GObject's accesors `g_object_get_property (self, name)` and
`g_object_set_property (self, name, value)`.

### Object implementation -- instance structure, storage and property enumeration

Full source code is in [src/gobj-05/tut-obj05.c](../src/gobj-05/tut-obj05.c)

There are more definitions at the top of implementation file than it was before:

```C
typedef enum
{
    PROP_TEXT_WRITABLE = 1,
    PROP_TEXT,
    N_PROPERTIES
} TutObj05PropEnum;

struct _TutObj05 {
    GObject parent_instance;
};

typedef struct _TutObj05Private TutObj05Private;
struct _TutObj05Private {
    gboolean text_writable;
    gchar *text;
};
```

`TutObj05PropEnum` is just ordinal numbers of properties, used in property dispatcher methods.
`struct _TutObj05` and `struct _TutObj05Private` are structures of instance and private data,
similar to seen before. Our class is final, so there is no explicit class structure definition.

### Object implementation -- type definition and internal declarations

There are more than was before forward declarations of "standard" functions in this part:

```C
static void tut_obj05_class_init (TutObj05Class *klass);
static void tut_obj05_init (TutObj05 *self);
static void tut_obj05_finalize (GObject *gobject);
static void tut_obj05_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void tut_obj05_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

G_DEFINE_FINAL_TYPE_WITH_PRIVATE(TutObj05, tut_obj05, G_TYPE_OBJECT);
```

Again names of functions are conventional, as macro `G_DEFINE_FINAL_TYPE_WITH_PRIVATE`
expect. `tut_obj05_finalize ()` frees memory [as before](gobj-03.md#object-deletion-declarations).
Two new names are:
- `tut_obj05_set_property ()` to implement property setter for our properties
- `tut_obj05_get_property ()` to implement property getter

### Object implementation -- cached data items

During construnction of object we will create complex structures describing properties
of our object. These complex structures are cached for reuse in future.

```C
static GParamSpec *tut_obj05_prop_pspec[N_PROPERTIES] = { NULL, };
```

### Object implementation -- class and instance initialization

During class initialization we:
- set up well known pointer to deletion method in GObject class structure
- set up well known pointers to property dispather methods GObject class structure
- register list of properties in GObject class structure

```C
static void
tut_obj05_class_init (TutObj05Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_print ("entered tut_obj05_class_init()\n");
    object_class->finalize = tut_obj05_finalize;
    object_class->set_property = tut_obj05_set_property;
    object_class->get_property = tut_obj05_get_property;
    tut_obj05_prop_pspec[PROP_TEXT_WRITABLE] =
        g_param_spec_boolean ("text-writable",
                              "Text is writable",
                              "Flag indicating that 'text' property currently may be written to.",
                              TRUE, /* default value */
                              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    tut_obj05_prop_pspec[PROP_TEXT] =
        g_param_spec_string  ("text",
                              "Text",
                              "Property holding arbitrary text.",
                              NULL, /* default value */
                              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       tut_obj05_prop_pspec);
}
```

`g_param_spec_xxx ()` functions create complex so called property specifications. This
property specifications are used by library to look up properties by name, determine its
types, organize data passing and so on. `g_object_class_install_properties ()`
associates list of properties with our class.

During instance initialization we give initial values to private data items as usual:

```C
static void
tut_obj05_init (TutObj05 *self) {
    TutObj05Private *priv = tut_obj05_get_instance_private (self);

    g_print ("entered tut_obj05_init()\n");
    priv->text_writable = TRUE;
    priv->text = NULL;
}
```

### Object implementation -- deletion method

On destruction of our object we free allocated for our private data items memory:

```C
static void
tut_obj05_finalize (GObject *gobject)
{
    TutObj05Private *priv = tut_obj05_get_instance_private (TUT_OBJ05 (gobject));

    g_print ("entered tut_obj05_finalize()\n");
    if (priv->text) /* delete on finalize */
    {
        g_print ("\tfreeing\n");
        g_free(priv->text);
        priv->text = NULL;
    }
    G_OBJECT_CLASS (tut_obj05_parent_class)->finalize (gobject);
}
```

### Object implementation -- property dispathers

To speed up things, property name lookup is performed only once, somewhere in the
GObject library. Our property dispathers called with ready ordinal numbers of
properties. Property value passing is done via [GValue](https://docs.gtk.org/gobject/struct.Value.html)
-- universal container for all primitive value types known to library. Since complex
objects are passed by refrences, they are primitive pointers for GValue too.

So, our setter property dispathcher should associate property numbers to data items,
fetch value from given GValue and put it to right private data item:

```C
static void
tut_obj05_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    TutObj05Private *priv;

    g_return_if_fail (TUT_IS_OBJ05 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj05_get_instance_private (TUT_OBJ05 (object));
    switch ((TutObj05PropEnum) property_id)
    {
    case PROP_TEXT_WRITABLE:
        priv->text_writable = g_value_get_boolean (value);
        g_print ("\tproperty 'text-writable' set: %d\n", priv->text_writable);
        break;
    case PROP_TEXT:
        if (!priv->text_writable) {
            /* Note: usually this situation is not programming error, but application logic
               flaw. We should handle such situalions somewhere in calling code. There is
               place where we use GError-based error reporting, gboolean results and so on. */
            g_warning("property 'text' curenntly is read-only");
            break;
        }
        g_free (priv->text);
        priv->text = g_value_dup_string (value);
        g_print ("\tproperty text set: \"%s\"\n", priv->text);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}
```

In our setter we implement logic of writability of `text` as it depends on state of
`text-writable` property.

On contrary, our getter property dispathcher should associate property numbers to data items,
fetch value from right private data item and put it to given GValue:

```C
static void
tut_obj05_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
    TutObj05Private *priv;

    g_return_if_fail (TUT_IS_OBJ05 (object));
    g_return_if_fail (value != NULL);
    g_return_if_fail (pspec != NULL);
    priv = tut_obj05_get_instance_private (TUT_OBJ05 (object));
    switch ((TutObj05PropEnum) property_id)
    {
    case PROP_TEXT_WRITABLE:
        g_value_set_boolean (value, priv->text_writable);
        break;
    case PROP_TEXT:
        g_value_set_string (value, priv->text);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}
```

Both setter and getter should consider *transfer of ownership* of data. Strings, etc
should be deleted (freed) or duplicated when appropriate.

### Object implementation -- construction methods

There are two new "constructors" with additional data in our public interface. Let's
see one of them:

```C
TutObj05 *
tut_obj05_new_full (gboolean text_writable, const gchar *text)
{
    TutObj05 *self;
    TutObj05Private *priv;

    g_print("entered tut_obj05_new_full()\n");
    self = g_object_new (TUT_TYPE_OBJ05, NULL);
    priv = tut_obj05_get_instance_private (self);
    priv->text_writable = text_writable;
    priv->text = g_strdup (text);
    return self;
}
```

Generally, construction of object with properties may be done much simplier, using
universal form of object constructor:

```C
    self = g_object_new (TUT_TYPE_OBJ05,
                         "text", <some string>,
                         "text-writable", <some bool>,
                         NULL);
```

But property `text` in our object is not always writable, so we using direct initialization
of private data items here.

### Object implementation -- convenience property setters and getters

In setterrs and getters we choose to use direct access to private data items too. When we do
this we should remember that normal process of setting property in GObject produces so called
notification signal of property change. In our custom setters we reproduce this behvior:

```C
void
tut_obj05_set_text_writable (TutObj05 *self, gboolean text_writable)
{
    TutObj05Private *priv;

    g_print ("entered tut_obj05_set_text_writable()\n");
    g_return_if_fail (TUT_IS_OBJ05 (self));
    priv = tut_obj05_get_instance_private (self);
    priv->text_writable = text_writable;
    g_object_notify_by_pspec(G_OBJECT (self), tut_obj05_prop_pspec[PROP_TEXT_WRITABLE]);
}
```

Line `g_object_notify_by_pspec(G_OBJECT (self), tut_obj05_prop_pspec[PROP_TEXT_WRITABLE]);`
does the work of producing notification signal. Here we use property specifications that
were cached during construction of our class.

### Creating and using object

Now all is ready to use public APIs of both our objects. This is done in separate
file [src/gobj-05/main.c](../src/gobj-05/main.c). Both ways of getting setting object
properties are used. Via public convenience accessors:

```C
    /* ... */
    tut_obj05_set_text(obj, "something");
    g_print ("tut_obj05_get_text(obj): \"%s\"\n", tut_obj05_get_text(obj));
    /* ... */
```

... and via generic GObject interface:

```C
    /* ... */
    GValue gval = G_VALUE_INIT;
    /* ... */
    g_value_init (&gval, G_TYPE_STRING);
    g_value_set_string (&gval, "thing");
    g_object_set_property (G_OBJECT (obj), "text", &gval);
    g_value_unset (&gval);
    /* ... */
```

Note that warnings on attempts to set `text` property when `text-writable` is FALSE.
