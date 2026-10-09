## Implementing custom signal, passing parameters with signal

Source code for this chapter is in [/src/gobj-07/](../src/gobj-07/) directory.

In this tutorial we will create object with custom signals, pass parameters and
detail data with our custom sugnals.

Signals are widely used in GTK UI implementation and many other parts of GLib
infrastructure. User interaction, input from external programs and devices,
asynchronous execution of long-running tasks and other similar things are often
done using signals.

### Object declaration

Full source code is in [src/gobj-07/tut-obj07.h](../src/gobj-07/tut-obj07.h).
File consists of
["standard" parts for final class](gobj-05.md#object-declaration) TutObj07
with these additions:

```C
#define TUT_OBJ07_DETAIL_STRING "obj07-detail"
```

We defined a string, that will be used later as "detail string" with our signal.

```C
void      tut_obj07_signal_simple (TutObj07 *self);
void      tut_obj07_signal_with_para (TutObj07 *self, gchar *string_para,
                                      gint int_para);
void      tut_obj07_signal_with_para_detail (TutObj07 *self, gchar *string_para,
                                             gint int_para);
```

We defined public interface to emit simple signal, signal with data parameters,
and signal with data parameters and a detail.

### Object implementation -- instance structure and signal enumeration

Full source code is in [src/gobj-07/tut-obj07.c](../src/gobj-07/tut-obj07.c).

Declarations section of implementation file is
[almost "standard"](gobj-05.md#object-implementation----instance-structure-storage-and-property-enumeration)
too. There is one addition:

```C
typedef enum {
    SIGNAL_SIMPLE,
    SIGNAL_PARA,
    N_SIGNALS
} TutObj07SignalEnum;
```

We defined an enumeration with ordinal numbers of our signals.

### Object implementation -- cached data items

During construnction of object we create complex structures describing signals
of our object. These structures are registered in GObject library. After
registration complete, they have uique identifiers. This identifiers we cache
for use in future.

```C
static guint tut_obj07_signal_id[N_SIGNALS] = {0};
```

We planned to use a string as detail parameter to one of our signals. Such
strings are supposed to be used many times. Library register them in some kind
of internal dictionary. When string is registered it gains a numeric code of
type GQuark (just unsigned integer). This string identifier we cache too.

```C
static GQuark tut_obj07_detail_quark = 0;
```

### Object implementation -- class and instance initialization

During class initialization we should give descriptions for our signals then
register them in library:

```C
static void
tut_obj07_class_init (TutObj07Class *klass) {
    GType signal_param_types[] = {G_TYPE_STRING, G_TYPE_INT};

    g_print ("entered tut_obj07_class_init ()\n");
    tut_obj07_signal_id[SIGNAL_SIMPLE] =
        g_signal_newv ("signal-simple",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                           G_SIGNAL_NO_HOOKS,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       NULL /* C marshaller */,
                       G_TYPE_NONE /* return_type */,
                       0    /* n_params */,
                       NULL /* param_types */);
    tut_obj07_signal_id[SIGNAL_PARA] =
        g_signal_newv ("signal-para",
                       G_TYPE_FROM_CLASS (klass),
                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                           G_SIGNAL_NO_HOOKS | G_SIGNAL_DETAILED,
                       NULL /* closure */,
                       NULL /* accumulator */,
                       NULL /* accumulator data */,
                       NULL /* C marshaller */,
                       G_TYPE_NONE /* return_type */,
                       G_N_ELEMENTS (signal_param_types) /* n_params */,
                       signal_param_types  /* param_types */);
    tut_obj07_detail_quark =
        g_quark_from_static_string (TUT_OBJ07_DETAIL_STRING);
}
```

Signal "descriptors" are created with function `g_signal_newv ()`. Here we
describe two signals:
- "signal-simple" with no attached data parameters
- "signal-para" with two attached data parameters of types string and int, as
stated in `signal_param_types` array

The last signal "signal-para" also may have "detail" parameter. This is enabled
using flag `G_SIGNAL_DETAILED` in creation of signal "descriptor". Both signals
expecting its handlers return no data. This is set by `G_TYPE_NONE` return type.
Functions that handle our signal should be of type `void (*) ()`.

The string that we use as "detail" parameter added to GObject library's internal
dictionary with function `g_quark_from_static_string ()`.

Instance initializtion function `tut_obj07_init ()` is effectively empty, since
instance have nothing to initialize.

### Object implementation -- public interface

There are three public methods to emit signals from our object.

```C
void
tut_obj07_signal_simple (TutObj07 *self)
{
    g_print ("entered tut_obj07_signal_simple ()\n");
    g_return_if_fail (TUT_IS_OBJ07 (self));
    g_signal_emit (G_OBJECT (self), tut_obj07_signal_id[SIGNAL_SIMPLE], 0);
}
```

First method `tut_obj07_signal_simple ()` makes our "signal-simple" signal
emitted. Since signal has no data parameters, it should be handled with callback
function of type `void (*) (TutObj07 *, void *)`. Two arguments to handler are
present always: object that sourced signal and some user-supplied arbitrary
pointer.

Last argument of `g_signal_emit ()` is detail string identifier. There
is no detail here, in our simple signal.

```C
void
tut_obj07_signal_with_para (TutObj07 *self, gchar *string_para, gint int_para)
{
    g_print ("entered tut_obj07_signal_with_para ()\n");
    g_return_if_fail (TUT_IS_OBJ07 (self));
    g_signal_emit (G_OBJECT (self), tut_obj07_signal_id[SIGNAL_PARA], 0, string_para, int_para);
}
```

Second method `tut_obj07_signal_with_para ()` emits signal "signal-para" passing
user-supplied parameters to handler of this signal. Handler should be function
of type `void (TutObj07 *, char *, int, void *)`. There are two arguments more,
and their types are the same that set on signal registration. Again, detail
string is absent.

```C
void
tut_obj07_signal_with_para_detail (TutObj07 *self, gchar *string_para,
                                   gint int_para)
{
    g_print ("entered tut_obj07_signal_with_para_detail ()\n");
    g_return_if_fail (TUT_IS_OBJ07 (self));
    g_signal_emit (G_OBJECT (self), tut_obj07_signal_id[SIGNAL_PARA],
                   tut_obj07_detail_quark, string_para, int_para);
}
```

Third method `tut_obj07_signal_with_para_detail ()` emits signal "signal-para"
passing user-supplied parameters to handler of this signal just like second
method. The only difference is nonzero "detail string identifier" in third
argument to `g_signal_emit ()`. This detail string identifier is passed together
with other data in signal emission. Inside the library this detail is used to
*filter* emissions of same signal by this detail.

For example, we may connect handler to signal "notify" of GObject instance, and
that handler will receive all "notify" emissions. But we may also specify a
property we are interected in, connecting our handler to detailed name of signal
like "notify::some-property". Handler connected last way will receive only
those "notify" emissions which are marked with "some-property" detail.

### Defining signal handlers in user code

Full source code is in [src/gobj-07/main.c](../src/gobj-07/main.c).

We define three handlers to make three different connections to signals of our
object:

```C
static void on_signal_simple (TutObj07 *object, gpointer user_data);
static void on_signal_with_para (TutObj07 *object, gchar *string_para,
                                 gint int_para, gpointer user_data);
static void on_signal_with_para_detail (TutObj07 *object, gchar *string_para,
                                        gint int_para, gpointer user_data);

/* ... */

static void
on_signal_simple (TutObj07 *object, gpointer user_data)
{
    g_print ("got into on_signal_simple () {\"%s\"}\n", (char*) user_data);
}

static void on_signal_with_para (TutObj07 *object, gchar *string_para,
                                 gint int_para, gpointer user_data)
{
    g_print ("got into on_signal_with_para (\"%s\", %d) {\"%s\"}\n",
             string_para, int_para, (char*) user_data);
}

static void on_signal_with_para_detail (TutObj07 *object, gchar *string_para,
                                        gint int_para, gpointer user_data)
{
    g_print ("got into on_signal_with_para_detail (\"%s\", %d) {\"%s\"}\n",
             string_para, int_para, (char*) user_data);
}
```

All three handlers just reporting received signal and data parameters attached
to it (if any). User-supplied arbitrary pointer, which is last argument to 
handlers, is used as textual label of connection we made.

### Connecting signals

There are three connections made with `g_signal_connect ()` function:

```C
    g_signal_connect (obj, "signal-simple", G_CALLBACK (on_signal_simple),
                      "conn. #1");
    g_signal_connect (obj, "signal-para", G_CALLBACK (on_signal_with_para),
                      "conn. #2");
    g_signal_connect (obj, "signal-para::" TUT_OBJ07_DETAIL_STRING,
                      G_CALLBACK (on_signal_with_para_detail), "conn. #3");
```

First connects handler to simple signal "signal-simple" which has no data
parameters. Second connects other handler to signal "signal-para" which has
data parameters passed with him. And third connects yet another handler to
*detailed* signal "signal-para" with detail string our object uses. The last
connection differs from second in that signals it delivers are filtered on that
detail string.

### Emitting and handling signals

Lets try to use public interface of our object to emit all three kinds of
signals:

```C
    g_print ("trying simple signal:\n");
    tut_obj07_signal_simple (obj);
    g_print ("trying signal with parameters:\n");
    tut_obj07_signal_with_para (obj, "string data", 123);
    g_print ("trying signal with parameters and detail:\n");
    tut_obj07_signal_with_para_detail (obj, "another string", 456);
```

First call `tut_obj07_signal_simple (obj);` delivers signal to
`on_signal_simple ()`, as expected.

Second call `tut_obj07_signal_with_para (obj, "string data", 123);` delivers
signal to `on_signal_with_para ()`, passing data parameters with that signal.
Third signal handler `on_signal_with_para_detail ()` is not called, because its
connection filter signals on detail.

And third call `tut_obj07_signal_with_para_detail (obj, "another string", 456);`
delivers signal to both handlers `on_signal_with_para ()` and
`on_signal_with_para_detail ()`. This signal emission is marked with right
detail string for third connection, and second connection does not look at
detail string at all.
