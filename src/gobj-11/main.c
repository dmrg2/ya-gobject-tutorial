#include <glib-object.h>

/* Weak references */

int
main (void)
{
  GObject      *obj, *ref_s, *ref_d;
  GWeakRef      wr_s = {0};
  GWeakRef     *wr_d;

  wr_d = g_new (GWeakRef, 1);
  obj = g_object_new (G_TYPE_OBJECT, NULL);
  g_weak_ref_set (&wr_s, obj);
  g_weak_ref_init (wr_d, obj);

  g_print ("1. After object_new ():\n");
  g_print ("\tobj->ref_count = %d\n", obj->ref_count);
  g_print ("\tobj -> %p\n", obj);

  ref_s = g_weak_ref_get (&wr_s);
  g_print ("2. After wr_s -> obj, weak_ref_get (wr_s):\n");
  g_print ("\tobj->ref_count = %d\n", obj->ref_count);
  g_print ("\tref_s -> %p\n", ref_s);

  ref_d = g_weak_ref_get (wr_d);
  g_print ("3. After wr_d -> obj, weak_ref_get (wr_d):\n");
  g_print ("\tobj->ref_count = %d\n", obj->ref_count);
  g_print ("\tref_d -> %p\n", ref_d);

  g_object_unref (ref_s);
  g_print ("4. After object_unref (ref_s):\n");
  g_print ("\tobj->ref_count = %d\n", obj->ref_count);
  ref_s = g_weak_ref_get (&wr_s);
  g_print ("\tref_s -> %p\n", ref_s);
  g_object_unref (ref_s);

  g_object_unref (ref_d);
  g_print ("5. After object_unref (ref_d):\n");
  g_print ("\tobj->ref_count = %d\n", obj->ref_count);
  ref_d = g_weak_ref_get (&wr_s);
  g_print ("\tref_d -> %p\n", ref_d);
  g_object_unref (ref_d);

  g_object_unref (obj);
  ref_s = g_weak_ref_get (&wr_s);
  ref_d = g_weak_ref_get (wr_d);
  g_print ("6. After object_unref (obj):\n");
  g_print ("\tref_s -> %p\n", ref_s);
  g_print ("\tref_d -> %p\n", ref_d);
  if (ref_s != NULL) g_object_unref (ref_s);
  if (ref_d != NULL) g_object_unref (ref_d);

  g_weak_ref_set (&wr_s, NULL);
  g_weak_ref_clear (wr_d);
  g_free (wr_d);

  return 0;
}
