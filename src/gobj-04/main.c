#include "tut-obj04.h"
#include "tut-obj04m.h"

int
main (void)
{
    TutObj04 *obj;
    TutObj04M *objm;
    GTypeQuery tq;

    g_print ("=======================================================\n");
    objm = tut_obj04m_new ();
    g_print ("G_IS_OBJECT(objm) = %d\n", G_IS_OBJECT (objm));
    g_type_query (TUT_TYPE_OBJ04M, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);
    g_print ("trying objm->TutObj04::vwhoami(void):\n");
    tut_obj04m_vwhoami (objm);
    g_print ("trying objm->TutObj04::vpure(void):\n");
    tut_obj04m_vpure (objm);
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (objm)->ref_count);
    g_clear_object (&objm);

    g_print ("=======================================================\n");
    obj = tut_obj04_new ();
    g_print ("G_IS_OBJECT(obj) = %d\n", G_IS_OBJECT (obj));
    g_type_query (TUT_TYPE_OBJ04, &tq);
    g_print ("Type query:\n");
    g_print ("\ttype = 0x%lX\n", tq.type);
    g_print ("\ttype_name = %s\n", tq.type_name);
    g_print ("\tclass_size = %u\n", tq.class_size);
    g_print ("\tinstance_size = %u\n", tq.instance_size);
    g_print ("trying obj->TutObj04::vwhoami(void):\n");
    tut_obj04_vwhoami (obj);
    g_print ("trying obj->TutObj04::vpure(void): (should crash)\n");
    tut_obj04_vpure (obj);
    g_print ("before unref: obj->ref_count = %d\n", G_OBJECT (obj)->ref_count);
    g_clear_object (&obj);

    return 0;
}
