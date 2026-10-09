#include "tut-obj08.h"

int
main (void)
{
    TutObj08 *Alice, *Bob;
    GValue gval = G_VALUE_INIT;

    g_print ("Alice:\n");
    Alice = tut_obj08_new ();
    g_value_init (&gval, G_TYPE_STRING);
    g_value_set_string (&gval, "Alice");
    g_object_set_property (G_OBJECT (Alice), "name", &gval);
    g_value_unset (&gval);
    g_value_init (&gval, G_TYPE_INT);
    g_value_set_int (&gval, 3);
    g_object_set_property (G_OBJECT (Alice), "id", &gval);
    g_value_unset (&gval);
    g_print ("tut_inamed_vname(Alice): \"%s\"\n", tut_inamed_vname(TUT_INAMED (Alice)));
    g_print ("tut_iknown_vid(Alice): %d\n", tut_iknown_vid(TUT_IKNOWN (Alice)));
    g_print ("tut_itrusted_vtrusted(Alice): %d\n", tut_itrusted_vtrusted(TUT_ITRUSTED (Alice)));
    g_object_get_property (G_OBJECT (Alice), "name", &gval);
    g_print ("Alice.name = %s\n", g_value_get_string (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (Alice), "id", &gval);
    g_print ("Alice.id = %d\n", g_value_get_int (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (Alice), "trusted", &gval);
    g_print ("Alice.trusted = %d\n", g_value_get_boolean (&gval));
    g_value_unset (&gval);
    g_print ("Bob:\n");
    Bob = tut_obj08_new_full ("Bob", 5);
    g_print ("tut_inamed_vname(Bob): \"%s\"\n", tut_inamed_vname(TUT_INAMED (Bob)));
    g_print ("tut_iknown_vid(Bob): %d\n", tut_iknown_vid(TUT_IKNOWN (Bob)));
    g_print ("tut_itrusted_vtrusted(Bob): %d\n", tut_itrusted_vtrusted(TUT_ITRUSTED (Bob)));
    g_object_get_property (G_OBJECT (Bob), "name", &gval);
    g_print ("Bob.name = %s\n", g_value_get_string (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (Bob), "id", &gval);
    g_print ("Bob.id = %d\n", g_value_get_int (&gval));
    g_value_unset (&gval);
    g_object_get_property (G_OBJECT (Bob), "trusted", &gval);
    g_print ("Bob.trusted = %d\n", g_value_get_boolean (&gval));
    g_value_unset (&gval);
    g_print ("Alice before unref: Alice.ref_count = %d\n", G_OBJECT (Alice)->ref_count);
    g_print ("Bob before unref: Bob.ref_count = %d\n", G_OBJECT (Bob)->ref_count);
    g_clear_object (&Alice);
    g_clear_object (&Bob);

    return 0;
}
