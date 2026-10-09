#include <glib-object.h>

/* Simple final type with disposable and freeable internal data */

/* Custom defined convenience macro */
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

/* Custom defined public interface to out object */
TutObj03    *tut_obj03_new (void);
void         tut_obj03_whoami (const TutObj03 *self);
void         tut_obj03_set_thing (TutObj03 *self, const gchar* thing);
const gchar *tut_obj03_get_thing (TutObj03 *self);
void         tut_obj03_grab (TutObj03 *self);
void         tut_obj03_release (TutObj03 *self);
gboolean     tut_obj03_have_grabbed (const TutObj03 *self);

/* END HEADER */

/* BEGIN IMPL */

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

/* Forward declare two initialization methods */
static void tut_obj03_class_init (TutObj03Class *klass); /* for class initialization */
static void tut_obj03_init (TutObj03 *self);             /* for instance initialization */

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

/* Custom supplied part of object logic starts here */

static void tut_obj03_dispose (GObject *gobject);  /* we implement custom Object::dispose() */
static void tut_obj03_finalize (GObject *gobject); /* we implement custom Object::finalize() */

/* internals of construction/destruction */

static void
tut_obj03_class_init (TutObj03Class *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_print ("entered tut_obj03_class_init()\n");
    object_class->dispose  = tut_obj03_dispose;
    object_class->finalize = tut_obj03_finalize;
}

static void
tut_obj03_init (TutObj03 *self) {
    TutObj03Private *priv = tut_obj03_get_instance_private (self);

    g_print ("entered tut_obj03_init()\n");
    priv->thing   = NULL;
    priv->grabbed = FALSE;
}

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

/* public interface to our object */

TutObj03 *
tut_obj03_new (void) {
    TutObj03 *self;

    g_print("entered tut_obj03_new()\n");
    self = (TutObj03 *) g_object_new (TUT_TYPE_OBJ03, NULL);
    return self;
}

void
tut_obj03_whoami (const TutObj03 *self)
{
    TutObj03Private *priv;

    g_print("entered tut_obj03_whoami()\n");
    g_return_if_fail (TUT_IS_OBJ03 (self)); /* it is from external code -- check */
    priv  = tut_obj03_get_instance_private (self);
    g_print ("\tI am TutObj03 (\"%s\", %d)!\n", priv->thing, priv->grabbed);
}

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

const gchar*
tut_obj03_get_thing (TutObj03 *self) {
    TutObj03Private *priv;

    g_print ("entered tut_obj03_get_thing()\n");
    g_return_val_if_fail (TUT_IS_OBJ03 (self), NULL); /* it is from external code -- check */
    priv = tut_obj03_get_instance_private (self);
    return (const gchar*) priv->thing;
}

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
