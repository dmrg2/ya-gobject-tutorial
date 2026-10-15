#include <glib-object.h>

/* Closures */


void         on_pre_marshal      (gpointer user_data, GClosure *closure);
void         on_post_marshal     (gpointer user_data, GClosure *closure);
void         on_data_destroy     (gpointer user_data, GClosure *closure);
void         on_closure_finalize (gpointer user_data, GClosure *closure);
void         closure_marshaller  (GClosure     *closure,
                                  GValue       *return_value,
                                  guint         n_param_values,
                                  const GValue *param_values,
                                  gpointer      invocation_hint,
                                  gpointer      marshal_data);
int          func_closure_target (int a, int b); /* => a + b */

int
main (void)
{
  GCClosure    *C_closure;
  GValue        result;
  GValue        arguments[2];

  g_value_init (&result, G_TYPE_INT);
  g_value_init (&arguments[0], G_TYPE_INT);
  g_value_init (&arguments[1], G_TYPE_INT);

  C_closure = (GCClosure *) g_cclosure_new (G_CALLBACK (func_closure_target),
                                            "user data",
                                            on_data_destroy);
  g_print ("C_closure->ref_count: %d\n", C_closure->closure.ref_count);
  g_closure_add_marshal_guards ((GClosure *) C_closure,
                                "pre data", on_pre_marshal,
                                "post data", on_post_marshal);
  g_closure_set_marshal ((GClosure *) C_closure,
                         (GClosureMarshal) closure_marshaller);
  g_closure_add_finalize_notifier ((GClosure *) C_closure,
                                   "fin data",
                                   on_closure_finalize);

  g_value_set_int (&arguments[0], 5);
  g_value_set_int (&arguments[1], 6);
  g_closure_invoke ((GClosure *) C_closure,
                    &result,
                    G_N_ELEMENTS (arguments),
                    arguments,
                    "hint");

  g_closure_unref ((GClosure *) C_closure);

  g_print ("result = %d\n", g_value_get_int (&result));

  return 0;
}

void
on_pre_marshal (gpointer user_data, GClosure *closure)
{
  g_print ("in on_pre_marshal ()      {%s}\n", (char *) user_data);
}

void
on_post_marshal (gpointer user_data, GClosure *closure)
{
  g_print ("in on_post_marshal ()     {%s}\n", (char *) user_data);
}

void
on_data_destroy (gpointer user_data, GClosure *closure)
{
  g_print ("in on_data_destroy ()     {%s}\n", (char *) user_data);
}

void
on_closure_finalize (gpointer user_data, GClosure *closure)
{
  g_print ("in on_closure_finalize () {%s}\n", (char *) user_data);
}

void
closure_marshaller (GClosure     *closure,
                    GValue       *return_value,
                    guint         n_param_values,
                    const GValue *param_values,
                    gpointer      invocation_hint,
                    gpointer      marshal_data)
{
  typedef int (*target_func_t) (int, int);
  GCClosure    *C_closure;
  target_func_t target_func;
  int           result;

  g_print ("in closure_marshaller ()\n");
  g_print ("\tn_param_values:   %d\n", n_param_values);
  g_print ("\tinvocation_hint:  {%s}\n", (char *) invocation_hint);
  g_print ("\tmarshal_data:     {%s}\n", (char *) marshal_data);
  g_print ("\tclosure->data:    {%s}\n", (char *) closure->data);
  C_closure = (GCClosure *) closure;
  target_func = (target_func_t) C_closure->callback;
  result = target_func (g_value_get_int (&param_values[0]),
                        g_value_get_int (&param_values[1]));
  g_value_set_int (return_value, result);
}

int
func_closure_target (int a, int b)
{
  g_print ("in func_closure_target (%d, %d) => %d\n", a, b, a + b);
  return a + b;
}
