#include "tut-obj05.h"

int
main (void)
{
    TutObj05 *obj;
    GTypeQuery tq;
    GValue gval = G_VALUE_INIT;

    obj = tut_obj05_new_with_text ("whathing");
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_type_query (TUT_TYPE_OBJ05, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);
    g_print ("tut_obj05_get_text_writable(obj): %d\n", tut_obj05_get_text_writable(obj));
    g_print ("tut_obj05_get_text(obj): \"%s\"\n", tut_obj05_get_text(obj));
    tut_obj05_set_text(obj, "something");
    g_print ("tut_obj05_get_text(obj): \"%s\"\n", tut_obj05_get_text(obj));
    tut_obj05_set_text_writable(obj, FALSE);
    tut_obj05_set_text(obj, "anything");
    g_print ("tut_obj05_get_text(obj): \"%s\"\n", tut_obj05_get_text(obj));
    tut_obj05_set_text_writable(obj, TRUE);
    tut_obj05_set_text(obj, "anything");
    g_print ("tut_obj05_get_text(obj): \"%s\"\n", tut_obj05_get_text(obj));
    g_object_get_property (G_OBJECT (obj), "text-writable", &gval);
    g_print ("obj.'text-writable' = %d\n", g_value_get_boolean (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (obj), "text", &gval);
    g_print ("obj.text = \"%s\"\n", g_value_get_string (&gval));
    g_value_unset (&gval);
    g_value_init (&gval, G_TYPE_STRING);
    g_value_set_string (&gval, "thing");
    g_object_set_property (G_OBJECT (obj), "text", &gval);
    g_value_unset (&gval);
    g_value_init (&gval, G_TYPE_BOOLEAN);
    g_value_set_boolean (&gval, FALSE);
    g_object_set_property (G_OBJECT (obj), "text-writable", &gval);
    g_value_unset (&gval);
    g_print ("tut_obj05_get_text_writable(obj): %d\n", tut_obj05_get_text_writable(obj));
    g_print ("tut_obj05_get_text(obj): \"%s\"\n", tut_obj05_get_text(obj));
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (obj)->ref_count);
    g_clear_object (&obj);

    return 0;
}
