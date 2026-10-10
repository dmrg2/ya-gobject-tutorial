## Object with internal data, creatiion and deletion, freeing allocated memory

Source code for this chapter is in [/src/gobj-03/](../src/gobj-03/) directory.

### Names and standard definitions

All names and structure definitions are [conventional](gobj-02.md#conventional-module-and-object-names).

In this chapter we define much more C symbols. This symbols mimic the definitions that
are produced automatically, when we use conenience macros like G_DECLARE_xxx (). The use
of that convenience macros is shown in [next chapter](gobj-04.md).

```C
#include <glib-object.h>

#define TUT_TYPE_OBJ03 (tut_obj03_get_type())

/* BEGIN HEADER */

/* G_DECLARE_FINAL_TYPE produces code like this: <<< */
    GType tut_obj03_get_type (void); 

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

    typedef struct _TutObj03                       TutObj03;
    typedef struct { GObjectClass parent_class; }  TutObj03Class;
    typedef TutObj03                              *TutObj03_autoptr;
    typedef GList                                 *TutObj03_listautoptr;
    typedef GSList                                *TutObj03_slistautoptr;
    typedef GQueue                                *TutObj03_queueautoptr; 

    static G_GNUC_UNUSED inline void glib_autoptr_clear_TutObj03 (TutObj03 *_ptr)
        { if (_ptr) (glib_autoptr_clear_GObject) ((GObject *) _ptr); }
    static G_GNUC_UNUSED inline void glib_autoptr_cleanup_TutObj03 (TutObj03 **_ptr)
        { glib_autoptr_clear_TutObj03 (*_ptr); }
    static G_GNUC_UNUSED inline void glib_autoptr_destroy_TutObj03 (void *_ptr)
        { (glib_autoptr_clear_GObject) ((GObject *) _ptr); }
    static G_GNUC_UNUSED inline void glib_listautoptr_cleanup_TutObj03 (GList **_l)
        { g_list_free_full (*_l, glib_autoptr_destroy_TutObj03); }
    static G_GNUC_UNUSED inline void glib_slistautoptr_cleanup_TutObj03 (GSList **_l)
        { g_slist_free_full (*_l, glib_autoptr_destroy_TutObj03); }
    static G_GNUC_UNUSED inline void glib_queueautoptr_cleanup_TutObj03 (GQueue **_q)
        { if (*_q) g_queue_free_full (*_q, glib_autoptr_destroy_TutObj03); }

    typedef TutObj03Class *TutObj03Class_autoptr;
    typedef GList         *TutObj03Class_listautoptr;
    typedef GSList        *TutObj03Class_slistautoptr;
    typedef GQueue        *TutObj03Class_queueautoptr; 

    static G_GNUC_UNUSED inline void glib_autoptr_clear_TutObj03Class (TutObj03Class *_ptr)
        { if (_ptr) (g_type_class_unref) ((TutObj03Class *) _ptr); }
    static G_GNUC_UNUSED inline void glib_autoptr_cleanup_TutObj03Class (TutObj03Class **_ptr)
        { glib_autoptr_clear_TutObj03Class (*_ptr); }
    static G_GNUC_UNUSED inline void glib_autoptr_destroy_TutObj03Class (void *_ptr)
        { (g_type_class_unref) ((TutObj03Class *) _ptr); }
    static G_GNUC_UNUSED inline void glib_listautoptr_cleanup_TutObj03Class (GList **_l)
        { g_list_free_full (*_l, glib_autoptr_destroy_TutObj03Class); }
    static G_GNUC_UNUSED inline void glib_slistautoptr_cleanup_TutObj03Class (GSList **_l)
        { g_slist_free_full (*_l, glib_autoptr_destroy_TutObj03Class); }
    static G_GNUC_UNUSED inline void glib_queueautoptr_cleanup_TutObj03Class (GQueue **_q)
        { if (*_q) g_queue_free_full (*_q, glib_autoptr_destroy_TutObj03Class); }

    G_GNUC_UNUSED static inline TutObj03 * TUT_OBJ03 (gpointer ptr)
        { return G_TYPE_CHECK_INSTANCE_CAST(ptr, tut_obj03_get_type (), TutObj03); }
    G_GNUC_UNUSED static inline gboolean TUT_IS_OBJ03 (gpointer ptr)
        { return G_TYPE_CHECK_INSTANCE_TYPE(ptr, tut_obj03_get_type ()); } 

    #pragma GCC diagnostic pop
/* >>> G_DECLARE_FINAL_TYPE */
```

There are all definitions we need in following code, except complete definition of instance
structure `struct _TutObj03`.  We will have to define it itself later. The same approach
used with conventional macros in code.

### API declarations for object type

There still no delrations of APIs to our object. We should add them.

```C
/* Custom defined public interface to out object */
TutObj03    *tut_obj03_new (void);
void         tut_obj03_whoami (const TutObj03 *self);
void         tut_obj03_set_thing (TutObj03 *self, const gchar* thing);
const gchar *tut_obj03_get_thing (TutObj03 *self);
void         tut_obj03_grab (TutObj03 *self);
void         tut_obj03_release (TutObj03 *self);
gboolean     tut_obj03_have_grabbed (const TutObj03 *self);

/* END HEADER */
```

The same approach used with conventional macros in code.

Our object will contain two data items inside:
- boolean value named `grabbed`
- string value named `thing`

Our object implements "resource owner" behavior pattern. When `thing` is set (not NULL),
it can "grab" (take ownership of) it. When `thing` is `grabbed`, it cannot be changed, until
object "release" it. The `thing` is a string allocated in memory. When our object is
destroyed, it should:
- "release" the `thing`
- free memory occupied by string `thing`

Our custom API consist of:
- object creation method `tut_obj03_new ()`
- object application "report" method  `tut_obj03_whoami ()`
- object application `thing` setter `tut_obj03_set_thing ()`
- object application `thing` getter `tut_obj03_get_thing ()`
- object application "grab" method `tut_obj03_grab ()`
- object application "release" method `tut_obj03_release ()`
- object application `grabbed` getter `tut_obj03_have_grabbed ()`

### Instance and its data storage defintions

Our object will use GObject library's "prvate instance data" mechanism. GObject library
provide APIs do declare that objects of a class  have such "private data storage" and to
attach this storage to every created instance of a class.

Technically, library will increase the size of memory block allocated for instance by
the size of instance's private data. This additional memory will be used to store that
private data of instance. Location of additional memory will be remembered and known to
the library and to the methods of object. User code will never know neither location
nor structure of instance's private data. Data is "incapsulated".

```C
* BEGIN IMPL */

/* Custom defined instance structure */
struct _TutObj03 {
    GObject parent_instance; /* head of structure is GObject as is */
};

/* Custom defined instance private data */
typedef struct _TutObj03Private TutObj03Private;
struct _TutObj03Private {
    gchar *thing;     /* Anything you need to */
    gboolean grabbed; /* implement your object */
};
```

### Construction process defintions

Our object has internal data, so it will need a function to initialize it:

```C
/* Forward declare two initialization methods */
static void tut_obj03_class_init (TutObj03Class *klass); /* for class initialization */
static void tut_obj03_init (TutObj03 *self);             /* for instance initialization */
```

Part of initialization process may be generated automatically by convenience macros
like G_DEFINE_TYPE, G_DEFINE_FINAL_TYPE, etc. Whole set of those handy macros
described in [GObject API reference](https://docs.gtk.org/gobject/#function_macros).

Here we manually recreate the code produced by one of that convenience macros:

```C
/* G_DEFINE_FINAL_TYPE_WITH_PRIVATE produces code like this: <<< */
    static void     tut_obj03_init (TutObj03 *self);
    static void     tut_obj03_class_init (TutObj03Class *klass);
    static GType    tut_obj03_get_type_once (void);
    static gpointer tut_obj03_parent_class = NULL;
    static gint     TutObj03_private_offset;

    static void
    tut_obj03_class_intern_init (gpointer klass)
    {
        tut_obj03_parent_class = g_type_class_peek_parent (klass);
        if (TutObj03_private_offset != 0)
            g_type_class_adjust_private_offset (klass, &TutObj03_private_offset);
        tut_obj03_class_init ((TutObj03Class*) klass);
    }

    G_GNUC_UNUSED static inline
    gpointer tut_obj03_get_instance_private (const TutObj03 *self)
    {
        return (G_STRUCT_MEMBER_P (self, TutObj03_private_offset));
    }

    GType
    tut_obj03_get_type (void)
    {
        static GType static_g_define_type_id = 0;

        if (g_once_init_enter_pointer (&static_g_define_type_id))
        {
            GType g_define_type_id = tut_obj03_get_type_once ();
            g_once_init_leave_pointer (&static_g_define_type_id, g_define_type_id);
        }
        return static_g_define_type_id;
    }

    G_NO_INLINE static GType
    tut_obj03_get_type_once (void)
    {
        GType g_define_type_id =
            g_type_register_static_simple (G_TYPE_OBJECT,
                                           g_intern_static_string ("TutObj03"),
                                           sizeof (TutObj03Class),
                                           (GClassInitFunc)(void (*)(void)) tut_obj03_class_intern_init,
                                           sizeof (TutObj03),
                                           (GInstanceInitFunc)(void (*)(void)) tut_obj03_init,
                                           G_TYPE_FLAG_FINAL);
        {
            TutObj03_private_offset =
                g_type_add_instance_private (g_define_type_id, sizeof (TutObj03Private));
        }
        return g_define_type_id;
    }
/* >>> G_DEFINE_FINAL_TYPE_WITH_PRIVATE */
```

Note that function `tut_obj03_get_type ()` already created. This function uses complex logic
to assure type registration is performed atomically and only once. During type registration
process library told that our object has private data of some size. These lines of code do it:

```C
            TutObj03_private_offset =
                g_type_add_instance_private (g_define_type_id, sizeof (TutObj03Private));
```

Class initialization function function is wrapped with `tut_obj03_class_intern_init ()`.
This wrapper function caches reference (pointer) to parent class' structure and location
of instance's private data in memory. Our custom initialization function
`tut_obj03_class_init ()` is called from inside of that wrapper.

### Custom part of class construction

Our object contains data resources that should be freed on instance destruction. We define
two function for this:
- disposal function to "release" resources held `void tut_obj03_dispose ()`
- deletion function to free allocated memory `tut_obj03_finalize ()`

```C
static void tut_obj03_dispose (GObject *gobject);  /* we implement custom Object::dispose() */
static void tut_obj03_finalize (GObject *gobject); /* we implement custom Object::finalize() */
```

These functions attached to our object class' description in class initialization function.
GObject class structure have well known pointer fields `dispose` and `finalize` for such
functions:

```C
static void
tut_obj03_class_init (TutObj03Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_print ("entered tut_obj03_class_init()\n");
    object_class->dispose  = tut_obj03_dispose;
    object_class->finalize = tut_obj03_finalize;
}
```

Remember that we are changing *a copy* of GObject class' structure that is embedded into
our class structure. Original structure of GObject class is stored elsewhere and remains intact.

Disposal and deletion functions are provided in custom part of code. During disposal stage we
"release" resource if we have it "grabbed":

```C
static void
tut_obj03_dispose (GObject *gobject)
{
    TutObj03Private *priv = tut_obj03_get_instance_private (TUT_OBJ03 (gobject));

    g_print ("entered tut_obj03_dispose()\n");
    if (priv->grabbed) /* release on dispose */
    {
        g_print ("\t\"releasing\" 'grabbed' resource\n");
        priv->grabbed = FALSE;
    }
    G_OBJECT_CLASS (tut_obj03_parent_class)->dispose (gobject);
}
```

During deletion stage we free memory that was allocated to store `thing` string:

```C
static void
tut_obj03_finalize (GObject *gobject)
{
    TutObj03Private *priv = tut_obj03_get_instance_private (TUT_OBJ03 (gobject));

    g_print ("entered tut_obj03_finalize()\n");
    if (priv->thing) /* delete on finalize */
    {
        g_print ("\tfreeing dynamically allocated 'thing'\n");
        g_free(priv->thing);
        priv->thing = NULL;
    }
    G_OBJECT_CLASS (tut_obj03_parent_class)->finalize (gobject);
}
```

### Custom part of instance initialization

We have two data items in private data of our objects and want them to have some known
"default" state right after creation. In instance initialization function `tut_obj03_init ()`
we should somehow get a pointer to private data structure and set the default values.
Class definition macro (or, as in this chapter, we itself) generated function
`tut_obj03_get_instance_private ()` to access private data from inside of object
implemetation:

```C
static void
tut_obj03_init (TutObj03 *self) {
    TutObj03Private *priv = tut_obj03_get_instance_private (self);

    g_print ("entered tut_obj03_init()\n");
    priv->thing   = NULL;
    priv->grabbed = FALSE;
}
```

### Rest of API implementation -- public interface

Object creation is made like before:

```C
TutObj03 *
tut_obj03_new (void) {
    TutObj03 *self;

    g_print("entered tut_obj03_new()\n");
    self = (TutObj03 *) g_object_new (TUT_TYPE_OBJ03, NULL);
    return self;
}

```

Note all the following APIs are using assertion macro `g_return_if_fail (TUT_IS_OBJ03 (self));`.
A pointer to instance comes from external code and may contain wrong address. We should
protect itself from programmer errors.

"Reporting method" is like before too:

```C
void
tut_obj03_whoami (const TutObj03 *self)
{
    TutObj03Private *priv;

    g_print("entered tut_obj03_whoami()\n");
    g_return_if_fail (TUT_IS_OBJ03 (self)); /* it is from external code -- check */
    priv  = tut_obj03_get_instance_private (self);
    g_print ("\tI am TutObj03 (\"%s\", %d)!\n", priv->thing, priv->grabbed);
}
```

Setter for `thing` checks[^1] `grabbed` state:

```C
void
tut_obj03_set_thing (TutObj03 *self, const gchar* thing)
{
    TutObj03Private *priv;

    g_print ("entered tut_obj03_set_thing()\n");
    g_return_if_fail (TUT_IS_OBJ03 (self)); /* it is from external code -- check */
    priv = tut_obj03_get_instance_private (self);
    if (priv->grabbed)
    {
        g_warning("can't set while grabbed");
        return;
    }
    if (priv->thing) g_free (priv->thing);
    priv->thing = thing ? (g_strdup(thing)) : NULL;
}
```

Getter for `thing` does not *transfer ownership*:

```C
const gchar*
tut_obj03_get_thing (TutObj03 *self) {
    TutObj03Private *priv;

    g_print ("entered tut_obj03_get_thing()\n");
    g_return_val_if_fail (TUT_IS_OBJ03 (self), NULL); /* it is from external code -- check */
    priv = tut_obj03_get_instance_private (self);
    return (const gchar*) priv->thing;
}
```

"Grab" method checks[^1] if not yet "grabbed" and if `thing` is not NULL:

```C
void
tut_obj03_grab (TutObj03 *self)
{
    TutObj03Private *priv;

    g_print ("entered tut_obj03_grab()\n");
    g_return_if_fail (TUT_IS_OBJ03 (self)); /* it is from external code -- check */
    priv = tut_obj03_get_instance_private (self);
    if (priv->grabbed)
    {
        g_warning("already grabbed");
        return;
    }
    if (!priv->thing)
    {
        g_warning("can't grab nullthing");
        return;
    }
    priv->grabbed = TRUE;
}
```

"Release" method checks[^1] "grabbed" state:

```C
void
tut_obj03_release (TutObj03 *self)
{
    TutObj03Private *priv;

    g_print ("entered tut_obj03_release()\n");
    g_return_if_fail (TUT_IS_OBJ03 (self)); /* it is from external code -- check */
    priv = tut_obj03_get_instance_private (self);
    if (!priv->grabbed)
    {
        g_warning("no thing grabbed");
        return;
    }
    priv->grabbed = FALSE;
}
```

"Have grabbed" getter returns "grabbed" state:

```C
gboolean
tut_obj03_have_grabbed (const TutObj03 *self)
{
    TutObj03Private *priv;

    g_print ("entered tut_obj03_have_grabbed()\n");
    g_return_val_if_fail (TUT_IS_OBJ03 (self), FALSE); /* it is from external code -- check */
    priv = tut_obj03_get_instance_private (self);
    return priv->grabbed;
}

/* END IMPL */
```

### Creating and using object

Now all is ready to use public APIs of our object:

```C
int
main (void)
{
    TutObj03 *obj;
    GTypeQuery tq;

    /* create our object */
    obj = tut_obj03_new ();

    /* some info on our object */
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_type_query (TUT_TYPE_OBJ03, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);

    /* use public interface to our object */
    tut_obj03_whoami (obj);
    tut_obj03_grab (obj);
    tut_obj03_set_thing (obj, "Thing");
    tut_obj03_grab (obj);
    tut_obj03_set_thing (obj, "Nothing");
    tut_obj03_release (obj);
    tut_obj03_release (obj);
    g_print("tut_obj03_get_thing() gave \"%s\"\n", tut_obj03_get_thing (obj));
    tut_obj03_set_thing (obj, "Nothing");
    tut_obj03_grab (obj);
    tut_obj03_whoami (obj);

    /* delete our object */
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (obj)->ref_count);
    g_clear_object (&obj);

    return 0;
}
```

Note the warnings issued by GObject library when we attempt to use APIs in "illegal state" of
object.

[^1]: There is better [GError API](https://docs.gtk.org/glib/error-reporting.html) in the library to report *application* logic errors. 
