#include "tut-obj07.h"

static void on_signal_simple (TutObj07 *object, gpointer user_data);
static void on_signal_with_para (TutObj07 *object, gchar *string_para, gint int_para, gpointer user_data);
static void on_signal_with_para_detail (TutObj07 *object, gchar *string_para, gint int_para, gpointer user_data);

int
main (void)
{
    TutObj07 *obj;
    GTypeQuery tq;

    obj = tut_obj07_new ();
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_type_query (TUT_TYPE_OBJ07, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);
    g_signal_connect (obj, "signal-simple", G_CALLBACK (on_signal_simple), "conn. #1");
    g_signal_connect (obj, "signal-para", G_CALLBACK (on_signal_with_para), "conn. #2");
    g_signal_connect (obj, "signal-para::" TUT_OBJ07_DETAIL_STRING, G_CALLBACK (on_signal_with_para_detail), "conn. #3");
    g_print ("trying simple signal:\n");
    tut_obj07_signal_simple (obj);
    g_print ("trying signal with parameters:\n");
    tut_obj07_signal_with_para (obj, "string data", 123);
    g_print ("trying signal with parameters and detail:\n");
    tut_obj07_signal_with_para_detail (obj, "another string", 456);
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (obj)->ref_count);
    g_clear_object (&obj);

    return 0;
}

static void
on_signal_simple (TutObj07 *object, gpointer user_data)
{
    g_print ("got into on_signal_simple() {\"%s\"}\n", (char*) user_data);
}

static void on_signal_with_para (TutObj07 *object, gchar *string_para, gint int_para, gpointer user_data)
{
    g_print ("got into on_signal_with_para(\"%s\", %d) {\"%s\"}\n", string_para, int_para, (char*) user_data);
}

static void on_signal_with_para_detail (TutObj07 *object, gchar *string_para, gint int_para, gpointer user_data)
{
    g_print ("got into on_signal_with_para_detail(\"%s\", %d) {\"%s\"}\n", string_para, int_para, (char*) user_data);
}
