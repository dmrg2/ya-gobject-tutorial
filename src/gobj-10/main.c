#include <glib-object.h>
#include "tut-obj10.h"

static void on_signal_sleep (TutObj10 *object,
                             TutBoxedTS *boxedts,
                             TutClockEnum enumck,
                             guint /*TutSMFlags*/ flags, gpointer user_data);

int main (void) {
    TutObj10 *obj = tut_obj10_new ();
    TutBoxedTS *boxedts = tut_boxedts_new_data(5, 6);
    GValue gval = G_VALUE_INIT;

    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_value_init (&gval, TUT_TYPE_BOXEDTS);
    g_value_set_boxed (&gval, boxedts);
    g_object_set_property (G_OBJECT (obj), "boxedts", &gval);
    g_object_get_property (G_OBJECT (obj), "boxedts", &gval);
    g_print ("obj.boxedts.tv_sec = %ld\n", ((TutBoxedTS *) g_value_get_boxed (&gval))->ts.tv_sec);
    g_print ("obj.boxedts.tv_nsec = %ld\n", ((TutBoxedTS *) g_value_get_boxed (&gval))->ts.tv_nsec);
    g_value_unset (&gval);
    g_value_init (&gval, TUT_TYPE_ENUMCK);
    g_value_set_enum (&gval, TUT_CLOCK_BOOTTIME);
    g_object_set_property (G_OBJECT (obj), "clock", &gval);
    g_object_get_property (G_OBJECT (obj), "clock", &gval);
    g_print ("obj.clock = %s\n", tut_enumck_get_nick (g_value_get_enum (&gval)));
    g_value_unset (&gval);
    g_value_init (&gval, TUT_TYPE_FLAGSSM);
    g_value_set_flags (&gval, TUT_SMODE_RELATIVE | TUT_SMODE_INTERRUPTIBLE);
    g_object_set_property (G_OBJECT (obj), "flags", &gval);
    g_object_get_property (G_OBJECT (obj), "flags", &gval);
    g_print ("obj.flags = ");
    if (g_value_get_flags (&gval) & TUT_SMODE_RELATIVE) g_print ("%s ", tut_flagssm_get_first_nick (TUT_SMODE_RELATIVE));
    if (g_value_get_flags (&gval) & TUT_SMODE_INTERRUPTIBLE) g_print ("%s ", tut_flagssm_get_first_nick (TUT_SMODE_INTERRUPTIBLE));
    g_print ("\n");
    g_value_unset (&gval);
    g_print ("tut_obj10_get_tv_sec(obj) = %ld\n", tut_obj10_get_tv_sec (obj));
    g_print ("tut_obj10_get_tv_nsec(obj) = %ld\n", tut_obj10_get_tv_nsec (obj));
    g_print ("tut_obj10_get_clock(obj) = %d\n", tut_obj10_get_clock (obj));
    g_print ("tut_obj10_get_flags(obj) = 0x%08x\n", tut_obj10_get_flags (obj));
    tut_boxedts_free (boxedts);
    g_signal_connect (G_OBJECT (obj), "sleep", G_CALLBACK (on_signal_sleep), NULL);
    tut_obj10_do_sleep (obj);
    g_clear_object (&obj);
    return 0;
}

static void
on_signal_sleep (TutObj10 *object,
                 TutBoxedTS *boxedts,
                 TutClockEnum enumck,
                 guint /*TutSMFlags*/ flags, gpointer user_data)
{
    g_print ("got into on_signal_sleep({%ld, %ld}, %d, 0x%08X)\n",
             boxedts->ts.tv_sec,
             boxedts->ts.tv_nsec,
             enumck,
             flags);
}
