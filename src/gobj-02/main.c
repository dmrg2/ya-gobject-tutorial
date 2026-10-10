#include <glib-object.h>

#define TUT_TYPE_OBJ02 (tut_obj02_get_type())

/* Simple type with non-virtual method 'whoami(void)' -- minimalistic */

/* BEGIN HEADER */

typedef struct _TutObj02Class TutObj02Class;

struct _TutObj02Class
{
    GObjectClass parent_class;
};

typedef struct _TutObj02 TutObj02;

struct _TutObj02
{
    GObject parent_instance;
};

GType
tut_obj02_get_type (void);

TutObj02 *
tut_obj02_new (void);

void
tut_obj02_whoami (const TutObj02 *self);

/* END HEADER */

/* BEGIN IMPL */

static void tut_obj02_class_init (TutObj02Class *klass); /* "standard  method" for class initialization */
static void tut_obj02_init (TutObj02 *self);             /* "standard  method" for instance initialization */

GType
tut_obj02_get_type (void)
{
    static GType type = 0;

    if (type == 0) /* tupe registered once on demand */
    {
        const GTypeInfo ti =
            {
                .class_size     = sizeof (TutObj02Class),
                .base_init      = NULL,
                .base_finalize  = NULL,
                .class_init     = (GClassInitFunc) tut_obj02_class_init,
                .class_finalize = NULL,
                .class_data     = NULL,
                .instance_size  = sizeof (TutObj02),
                .n_preallocs    = 0,
                .instance_init  = (GInstanceInitFunc) tut_obj02_init,
                .value_table    = NULL
            };
        g_print ("entered tut_obj02_get_type()\n\tRegistering new static type\n");
        type = g_type_register_static (G_TYPE_OBJECT, "TutObj02", &ti, G_TYPE_FLAG_FINAL);
    }
    return type;
}

TutObj02 *
tut_obj02_new (void) {
    g_print ("entered tut_obj02_new()\n");
    return (TutObj02 *) g_object_new (TUT_TYPE_OBJ02, NULL);
}

void
tut_obj02_whoami (const TutObj02 *self)
{
    g_print ("entered tut_obj02_whoami()\n\tI am TutObj02! (maybe)\n");
    (void)self;
}

static void
tut_obj02_class_init (TutObj02Class *klass) {
    g_print ("entered tut_obj02_class_init()\n");
    (void)klass;
}

static void
tut_obj02_init (TutObj02 *self) {
    g_print ("entered tut_obj02_init()\n");
    (void)self;
}

/* END IMPL */

int
main (void)
{
    TutObj02 *obj;
    GTypeQuery tq;

    /* create our object */
    obj = tut_obj02_new ();

    /* some info on our object */
    g_print ("obj -> %p\n", obj);
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_type_query (TUT_TYPE_OBJ02, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);

    /* use public interface to our object */
    tut_obj02_whoami (obj);

    /* delete our object */
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (obj)->ref_count);
    g_clear_object (&obj);

    return 0;
}
