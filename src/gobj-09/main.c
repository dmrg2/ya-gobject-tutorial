#include <glib-object.h>
#include "tut-obj09.h"

static void on_signal_timespec (TutObj09 *object, TutTimespec param_timespec, gpointer user_data);

int main (void) {
    GValue gval = G_VALUE_INIT;
    TutTimespec ts_test = {0};
    TutTimespec ts_first = {1, 0};
    TutTimespec ts_second = {2, 0};
    TutTimespec *ts_third_p = tut_timespec_new_data (3, 0);
    TutObj09 *obj, *objdata, *objraw;

    g_print ("=================================================================\nTutTimespec:\n");
    g_print ("Type id %lu, name %s\n", TUT_TYPE_TIMESPEC, g_type_name (TUT_TYPE_TIMESPEC));
    g_value_init(&gval, TUT_TYPE_TIMESPEC);
    g_value_set_tut_timespec (&gval, ts_first);
    ts_test = g_value_get_tut_timespec (&gval);
    g_print ("first = {%ld, %ld}\n", ts_first.tv_sec, ts_first.tv_nsec);
    g_print ("second = {%ld, %ld}\n", ts_second.tv_sec, ts_second.tv_nsec);
    g_print ("*third = {%ld, %ld}\n", ts_third_p->tv_sec, ts_third_p->tv_nsec);
    g_print ("test = {%ld, %ld}\n", ts_test.tv_sec, ts_test.tv_nsec);
    tut_timespec_delete (ts_third_p);

    g_print ("=================================================================\nUsing TutTimespec:\n");
    obj = tut_obj09_new ();
    objdata = tut_obj09_new_data (ts_test);
    objraw = tut_obj09_new_rawdata ((gint64)4, (gint64)0);
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_print ("G_IS_OBJECT(objdata) = %d\n", G_IS_OBJECT (objdata));
    g_print ("G_IS_OBJECT(objraw) = %d\n", G_IS_OBJECT (objraw));

    g_print ("=================================================================\nPassing TutTimespec to callback:\n");
    g_signal_connect (obj, "timespec", G_CALLBACK (on_signal_timespec), "from obj");
    g_signal_connect (objdata, "timespec", G_CALLBACK (on_signal_timespec), "from objdata");
    g_signal_connect (objraw, "timespec", G_CALLBACK (on_signal_timespec), "from objraw");
    tut_obj09_signal_timespec (obj);
    tut_obj09_signal_timespec (objdata);
    tut_obj09_signal_timespec (objraw);

    g_clear_object (&obj);
    g_clear_object (&objdata);
    g_clear_object (&objraw);
    return 0;
}

static void on_signal_timespec (TutObj09 *object, TutTimespec param_timespec, gpointer user_data)
{
    g_print("got into on_signal_timespec({%ld, %ld}) {\"%s\"}\n", param_timespec.tv_sec, param_timespec.tv_nsec, (char*) user_data);
}
