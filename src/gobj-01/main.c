#include <glib-object.h>
#include <gio/gio.h>

/* GObject itself and some descendants */

void
create_and_report (GType type) {
    GObject        *instance;
    GTypeInstance  *type_instance;
    GObjectClass   *class;
    GTypeClass     *klass;
    GType           gtype;
    GType           quick_type;
    GType           named_type;
    GTypeQuery      tq;
    GType          *children;
    guint           n_children;
    GType          *interfaces;
    guint           n_interfaces;
    int i;

    g_print ("===========================================================================\n");
    g_print ("trying to create instance of type 0x%lX (%s):\n", type, g_type_name (type));
    instance = g_object_new (type, NULL);
    type_instance = (GTypeInstance *) instance; /* class GObject is GTypeInstance { ... } */
    gtype = G_TYPE_FROM_INSTANCE (type_instance);
    quick_type = G_OBJECT_TYPE (instance);
    named_type = g_type_from_name (g_type_name (type));
    class = G_OBJECT_GET_CLASS (instance);
    klass = g_type_class_get (gtype);

    g_print ("G_IS_OBJECT (instance) = %d\n", G_IS_OBJECT (instance));
    g_print ("instance      -> %p\n", instance);
    g_print ("type_instance -> %p\n", type_instance);
    g_print ("G_TYPE_FROM_INSTANCE (type_instance)  = 0x%lX\n", gtype);
    g_print ("G_OBJECT_TYPE (instance)              = 0x%lX\n", quick_type);
    g_print ("type_from_name (***)                  = 0x%lX\n", named_type);
    g_print ("G_OBJECT_GET_CLASS (instance) -> %p\n", class);
    g_print ("g_type_class_get (instance)   -> %p\n", klass);
    g_print ("type_name_from_class (klass)            = %s\n", g_type_name_from_class (klass));
    g_print ("type_name_from_instance (type_instance) = %s\n", g_type_name_from_instance (type_instance));
    g_print ("type_depth (gtype)  = %u\n", g_type_depth (gtype));
    g_print ("type_parent (gtype) = 0x%lX (\"%s\")\n", g_type_parent (gtype), g_type_name (g_type_parent (gtype)));

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

    g_print ("before unref: instance->ref_count = %d\n", instance->ref_count);
    g_object_unref (instance); /* or g_clear_object(&instance) */
    g_print ("===========================================================================\n\n");
}

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
