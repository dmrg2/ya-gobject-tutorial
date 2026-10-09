#include <string.h>
#include "tut-obj06.h"

void notify_A_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
void notify_A_text_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
void notify_B_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
void notify_B_text_cb (GObject *self, GParamSpec *pspec, gpointer user_data);

gboolean binding_transform_text_to_num (GBinding *binding,
                                        const GValue *from_value,
                                        GValue *to_value,
                                        gpointer user_data);

int
main (void)
{
    TutObj06 *A, *B;
    GValue gval = G_VALUE_INIT;
    GBinding *At_to_Bt;
    GBinding *Bt_to_Bn;
    GBinding *Bn_to_An;

    A = tut_obj06_new_full (1, "2");
    B = tut_obj06_new_full (3, "4");
    g_print ("G_IS_OBJECT(A) = %d\n", G_IS_OBJECT (A));
    g_print ("G_IS_OBJECT(B) = %d\n", G_IS_OBJECT (B));
    g_print ("=================================================================\nNotify signals:\n");
    g_signal_connect(A, "notify::number", (GCallback) notify_A_number_cb, NULL);
    g_signal_connect(A, "notify::text", (GCallback) notify_A_text_cb, NULL);
    g_signal_connect(B, "notify::number", (GCallback) notify_B_number_cb, NULL);
    g_signal_connect(B, "notify::text", (GCallback) notify_B_text_cb, NULL);
    tut_obj06_set_number (A, 1);
    tut_obj06_set_text (A, "First");
    tut_obj06_set_number (B, 2);
    tut_obj06_set_text (B, "Second");
    g_print ("=================================================================\nBinding via notify:\n");
    At_to_Bt = g_object_bind_property (G_OBJECT (A), "text", G_OBJECT (B), "text", G_BINDING_SYNC_CREATE);
    Bt_to_Bn = g_object_bind_property_full (G_OBJECT (B), "text",   /* source */
                                            G_OBJECT (B), "number", /* target */
                                            G_BINDING_SYNC_CREATE,
                                            binding_transform_text_to_num,
                                            NULL, /* no reverse transform */
                                            NULL, 
                                            NULL); /* no destroy notify */
    Bn_to_An = g_object_bind_property (G_OBJECT (B), "number", G_OBJECT (A), "number", G_BINDING_SYNC_CREATE);
    tut_obj06_set_text (A, "The quick brown fox jumped over the lazy dog");
    g_binding_unbind (Bn_to_An);
    g_binding_unbind (Bt_to_Bn);
    g_binding_unbind (At_to_Bt);
    g_object_get_property (G_OBJECT (A), "text", &gval);
    g_print ("A.text = \"%s\"\n", g_value_get_string (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (A), "number", &gval);
    g_print ("A.number = %d\n", g_value_get_int (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (B), "text", &gval);
    g_print ("B.text = \"%s\"\n", g_value_get_string (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (B), "number", &gval);
    g_print ("B.number = %d\n", g_value_get_int (&gval));
    g_value_unset (&gval);
    g_clear_object (&A);
    g_clear_object (&B);
    return 0;
}

void
notify_A_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data)
{
    g_print ("A::notify::number: %d\n", tut_obj06_get_number(TUT_OBJ06 (self)));
}

void
notify_A_text_cb (GObject *self, GParamSpec *pspec, gpointer user_data)
{
    g_print ("A::notify::text: \"%s\"\n", tut_obj06_get_text(TUT_OBJ06 (self)));
}

void
notify_B_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data)
{
    g_print ("B::notify::number: %d\n", tut_obj06_get_number(TUT_OBJ06 (self)));
}

void
notify_B_text_cb (GObject *self, GParamSpec *pspec, gpointer user_data)
{
    g_print ("B::notify::text: \"%s\"\n", tut_obj06_get_text(TUT_OBJ06 (self)));
}

gboolean
binding_transform_text_to_num (GBinding *binding, const GValue *from_value, GValue *to_value, gpointer user_data)
{
    const gchar *str;

    g_print ("entered binding_transform_text_to_num()\n");
    g_return_val_if_fail (from_value != NULL, FALSE);
    g_return_val_if_fail (to_value != NULL, FALSE);
    g_return_val_if_fail (G_VALUE_HOLDS_STRING (from_value), FALSE);
    g_return_val_if_fail (G_VALUE_HOLDS_INT (to_value), FALSE);
    str = g_value_get_string (from_value);
    if (!str)
    {
        g_value_set_int (to_value, 0);
    }
    else
    {
        g_value_set_int (to_value, strlen(str));
    }
    return TRUE;
}
