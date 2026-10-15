## Using GObject 'notify' signal, property binding

Source code for this chapter is in [/src/gobj-06/](../src/gobj-06/) directory.

In this tutorial we wil create object with two properties `text`and `number`. 
Object is the same as in [previous chapter](gobj-05.md), so this tutorial will
focus on user code. In user code we will handle signals that produced by GObject
instances when its properties change.

Signals are widely used in GTK UI implementation and many other parts of GLib
infrastructure. User interaction, input from external programs and devices,
asynchronous execution of long-running tasks and other similar things are often
done using signals.

### Object implementation

Full source code of object is in
[src/gobj-06/tut-obj06.h](../src/gobj-06/tut-obj06.h) and
[src/gobj-06/tut-obj06.c](../src/gobj-06/tut-obj06.c).

Object TutObj06 has two properties:
- property "text" of string type
- property "number"of int type

Object is almost the same as TutObj05 from previos chapter. The only noticeable
thing in all of its implementation, is use of generic property initialization in
public constructor methods:

```C
TutObj06 *
tut_obj06_new_full (gint number, const gchar *text)
{
    TutObj06 *self;

    g_print ("entered tut_obj06_new_full()\n");
    self = (TutObj06 *) g_object_new (TUT_TYPE_OBJ06, "number", number,
                                      "text", text, NULL);
    return self;
}
```

Here the properties of object are set automatically via so called process of
value collection. When using this form of `g_object_new ()` properties are set
with GObject generic property accessor. Exactly as if someone written something
like:

```C
    self = (TutObj06 *) g_object_new ();
    g_object_set_property (G_OBJECT (self), "number", someintvalue);
    g_object_set_property (G_OBJECT (self), "text", sometextvalue);
```

### Using object -- define signal handlers

Full source code is in [src/gobj-06/main.c](../src/gobj-06/main.c).

We will create two objects, A and B, of class TutObj06 and connect its
properties to four signal handlers (callbacks, as they usually called in
documentaion):

```C
void notify_A_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
void notify_A_text_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
void notify_B_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
void notify_B_text_cb (GObject *self, GParamSpec *pspec, gpointer user_data);
```

Connections are arranged as this:
- `notify_A_number_cb ()` handles notifications on property `number` of object A
- `notify_A_text_cb ()` handles notifications on property `text` of object A
- `notify_B_number_cb ()` handles notifications on property `number` of object B
- `notify_B_text_cb ()` handles notifications on property `text` of object B

```C
    g_signal_connect (A, "notify::number", G_CALLBACK (notify_A_number_cb), NULL);
    g_signal_connect (A, "notify::text", G_CALLBACK (notify_A_text_cb), NULL);
    g_signal_connect (B, "notify::number", G_CALLBACK (notify_B_number_cb), NULL);
    g_signal_connect (B, "notify::text", G_CALLBACK (notify_B_text_cb), NULL);
```

Function `g_signal_connect ()` in second argument accepts name of a signal
("notify" in this case) with possible so called "::detail" string addition. For
"notify" signals this "detail" provides name of a property that handelr of 
signal is interested in. The last argument is arbitrary "user" pointer, passed
to handler function intact.

Handlers are plain C functions, accepting several formal arguments. Exact set
of formal arguments may vary from one signal to other. Appropriate documentation
sections discuss those details. Our handlers accept three arguments:
- pointer to object which is source of the signal
- parameter (or property) specification (same as used when property created)
- above said user supplied pointer

Our handlers just report that signal was "catched" and print corresponding
property value:

```C
void
notify_A_number_cb (GObject *self, GParamSpec *pspec, gpointer user_data)
{
    g_print ("A::notify::number: %d\n",
              tut_obj06_get_number (TUT_OBJ06 (self)));
}
```

These strings in `main ()` function makes all four of our handlers work:

```C
    tut_obj06_set_number (A, 1);
    tut_obj06_set_text (A, "First");
    tut_obj06_set_number (B, 2);
    tut_obj06_set_text (B, "Second");
```

### Using object -- bind properties of objects

Binding is a kind of connection between properties of objects. When binding is
set, a change of a property in so called source object produces automatic change
of property in other so called target object. Simpliest binding just copy
property value from source to property of target via generic property getting
and setting.

Binding may be bidirectional, where bound parties are both source and target.
Any change is transferred to opposite party. Bindings may be complex, where
value of source property is transformed by some transformation function before
putting it to target property. Bidirectional transforming bindings with two
transformation functions are possible too.

And last, source object and target object may be one object. Probably, there is
no sense in binding one property to itself, but pair of properties of object may
be bound with useful for program effect.

Lets construct such complex scheme of bindings:
- Source A.text unudirectinally bound to target B.text
- Source B.text unudirectinally bound to target B.number, with transformation
function "B.number = length of string in B.text"
- Source B.number unidirectionally bound to target A.number

```
   A.text ------> B.text --
                           |
                           v
                     [transformation]
                           |
   A.number <-- B.number <-
```

In main.c code it looks like this:

```C
    GBinding *At_to_Bt;
    GBinding *Bt_to_Bn;
    GBinding *Bn_to_An;
    /* ... */
    At_to_Bt = g_object_bind_property (G_OBJECT (A), "text",
                                       G_OBJECT (B), "text",
                                       G_BINDING_SYNC_CREATE);
    Bt_to_Bn = g_object_bind_property_full (G_OBJECT (B), "text",   /* source */
                                            G_OBJECT (B), "number", /* target */
                                            G_BINDING_SYNC_CREATE,
                                            binding_transform_text_to_num,
                                            NULL, /* no reverse transform */
                                            NULL, 
                                            NULL); /* no destroy notify */
    Bn_to_An = g_object_bind_property (G_OBJECT (B), "number",
                                       G_OBJECT (A), "number",
                                       G_BINDING_SYNC_CREATE);
```

Fifth argument to `g_object_bind_property ()` function contains flag of
synchronized binding. It means that value of source immediately copied to target
on binding creation.

Then implement transformation function for "text" to "number" binding:

```C
gboolean
binding_transform_text_to_num (GBinding *binding, const GValue *from_value,
                               GValue *to_value, gpointer user_data)
{
    const gchar *str;

    g_print ("entered binding_transform_text_to_num ()\n");
    g_return_val_if_fail (from_value != NULL, FALSE);
    g_return_val_if_fail (to_value != NULL, FALSE);
    g_return_val_if_fail (G_VALUE_HOLDS_STRING (from_value), FALSE);
    g_return_val_if_fail (G_VALUE_HOLDS_INT (to_value), FALSE);
    str = g_value_get_string (from_value);
    if (!str)
        g_value_set_int (to_value, 0);
    else
        g_value_set_int (to_value, strlen (str));
    return TRUE;
}
```

There are too many checks... but who knows where and when this function will be
called in future?

Now a single line in main.c produces a bunch of effects:

```C
    tut_obj06_set_text (A, "The quick brown fox jumped over the lazy dog");
```

Bindings are objects too, they are reference counted. We should care of
unbinding and deleting binding objects:

```C
    g_binding_unbind (Bn_to_An);
    g_binding_unbind (Bt_to_Bn);
    g_binding_unbind (At_to_Bt);
```

Created as in our program binding are automatically deleted on unbind. To save
its instances we should `g_object_ref ()` them somewhere before.
