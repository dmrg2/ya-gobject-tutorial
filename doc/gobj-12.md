## Closures

Source code for this chapter is in [src/gobj-12/](../src/gobj-12/) directory.

Closures is base concept for storing actions, evaluations, other computations
or actions like that as persistent object, which may be passed between
different evaluation evironments without losing its behavior. Closure represent
something *callable* that may be stored somewhere to call it later or in
other circumstances. Closures in GObject library is not simple addresses of
some functions. They have data associated with them, mechanism of notifying
about processing of a call itself, transformations needed to convert caller's
data form to callee's and vice versa, and so on.

[GObject's closures](https://docs.gtk.org/gobject/struct.Closure.html) used
almost everywhere around signals, language bindings, asynchronous operations,
multithreaded processing and in many other places. There is one standard
implementaion of closure in the library that may be "customized" many ways with
so called marshallers and auxillary data, attached to closure.

Marshallers do a work of transforming data from GObject's native forms to forms
that are acceptable to called entity. They also do reverse transformations.
Marshallers are some kind of "wrappers" on, for example, functions, implemented
in other library or even other language/runtime ecosystem. Marshallers also
used to build "adapters" from GObject signals to signal handlers.

### Closure API

Generic abstarct closure in library is represented by GClosure object, which
provides API to associate closure with data, notification signal handlers, and
marshalling procedure. There is one specific implementaion
[GCClosure](https://docs.gtk.org/gobject/struct.CClosure.html), which is used
to bind ordinary C functions to closure objects.

GCClosure offers a set of ready to use marshallers to connect GObjet signals to
handlers implemented in C, and generic marshaller, capable to process calls to
almost any simple function, given a set of its parameter descriptions.

GCClosure instances is created similar to GObject instances. Closure instances
are reference-counted like other objects and can notify on events of their
lifecycle.

### Marshallers

GCClosure's API has [long list](https://docs.gtk.org/gobject/struct.CClosure.html)
of ready to use signature-specific marshallers, such as marshallers to call
functions like `void (*) (void)`, functions like `char * (*) (GObject *)` and
so on. These marshallers are relatively fast. There is also a generic
marshaller, which is usually used by library with closures, implementing
delivery of different signals with variuos parameters. This generic marshaller
can do parameter passing for GObject descendants that may define arbitrary
signals with custom parameters. Generic marshaller is slower, since it has to
parse and interpret parameter types.

User code may implement its own marshaller and set up closure to use it. In
chapter [gobj-09](gobj-09.md#implementing-property-and-signal-in-object) we
have already done that.

### Closure notifications

There are several notifications issued by closure, that user code may receive.
These are:
- notification before marshallig arguments to called entity
- notification after receiving result, produced by callable entity
- notification on data destruction, issued when call to callable entity is
complete, and data used in that call may be freed
- notification on invalidation of closure, acting approximately same as
disposal for objects
- notification on finalization of closure, just before closure is destroyed and
memory allocated for closure is freed

### Protected members of GClosure interface

As stated in documetation, GClosure has only two public data memebers,
`in_marshal` and `is_invalid`. But there are more data fields which are to be
used by GClosure descendants and marshallers:
- `derivative_flag`, which indicates that user is willing "standard" very
first and very last arguments to closure target to be swapped (signal handlers
used with GObject's signals have GObject * in the first argument and arbitrary
gpointer as user-supplied data in the last)
- `data` of type gpointer, which holds arbitrary user-supplied data that may
be used, for example, in marshaller

`derivative_flag` is set when `GCClosure` is created with `g_cclosure_new_swap ()`
or `g_cclosure_new_object_swap`. There is a macro `G_CCLOSURE_SWAP_DATA` to
test if this flag is set in closure.

`data` pointer assigned from parameters of `GClosure` and `GCClosure`
constructors. When creating closure that is by nature tightly coupled to some
object (e.g., for object's propery change notification) with
`g_cclosure_new_object ()`, `data` is set to point to that object.

### Creating and using simple closure

Full source is in [src/gobj-12/main.c](../src/gobj-12/main.c).

We will use GCClosure to call simple C function that accepts two integers and
returns sum of that integers. Our closure will use custom marshaller to convert
GValue parameters to arguments of C function and to convert return value of
C function to GValue.

We will attach handlers to notifications issued by our closure to see how
process of calling evolves.

First, we declare our callable function, our marshaller, and our notification
handlers:

```C
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
```

Each of these functions (except our simple summing target function, of course)
have at least one auxillary argument that may be used to pass arbitrary
supplemental data from user code. We use that auxillary argumet to pass "text
labels" for closure processing stages.

Our notification handlers are te same, they just print its `user_data` argument.

```C
void
on_pre_marshal (gpointer user_data, GClosure *closure)
{
  g_print ("in on_pre_marshal ()      {%s}\n", (char *) user_data);
}
```

Our marshaller does not care about argument swapping at all, it just unpacks
given arguments from passed in GValue structures and packs return value of
target function to returned GValue.

```C
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
```

Next, define variables to hold closure itself, parameters (arguments) passed to
it, and result that it returns.

```C
  GCClosure    *C_closure;
  GValue        result;
  GValue        arguments[2];

  g_value_init (&result, G_TYPE_INT);
  g_value_init (&arguments[0], G_TYPE_INT);
  g_value_init (&arguments[1], G_TYPE_INT);
```

Create closure with our target summing function, then attach notification
handlers and our custom marshaller to it.

```C
  C_closure = (GCClosure *) g_cclosure_new (G_CALLBACK (func_closure_target),
                                            "user data",
                                            on_data_destroy);
  g_closure_add_marshal_guards ((GClosure *) C_closure,
                                "pre data", on_pre_marshal,
                                "post data", on_post_marshal);
  g_closure_add_finalize_notifier ((GClosure *) C_closure,
                                   "fin data",
                                   on_closure_finalize);
  g_closure_set_marshal ((GClosure *) C_closure,
                         (GClosureMarshal) closure_marshaller);
```

Fill in arguments to callable entity, and invoke our closure.

```C
  g_value_set_int (&arguments[0], 5);
  g_value_set_int (&arguments[1], 6);
  g_closure_invoke ((GClosure *) C_closure,
                    &result,
                    G_N_ELEMENTS (arguments),
                    arguments,
                    "hint");
```

Note that code block above remains the same, whether we implemented our
summing function in C in same file, implemented it in Lua or Python module, or
even implemented it in a web service accessed via REST-like protocol. Closure
hides the details of implemenation, there is only contract "give two numbers,
get a sum" remains.

Now, closure instance is not needed anymore. Unreference it, and get the result
of summing it returned.

```C
  g_closure_unref ((GClosure *) C_closure);

  g_print ("result = %d\n", g_value_get_int (&result));
```

Program outputs the sequence of events in the process of call execution, up to
closure destruction.

```
$ ./main
C_closure->ref_count: 1
in on_pre_marshal ()      {pre data}
in closure_marshaller ()
    n_param_values:   2
    invocation_hint:  {hint}
    marshal_data:     {(null)}
    closure->data:    {user data}
in func_closure_target (5, 6) => 11
in on_post_marshal ()     {post data}
in on_closure_finalize () {fin data}
in on_data_destroy ()     {user data}
result = 11
```
