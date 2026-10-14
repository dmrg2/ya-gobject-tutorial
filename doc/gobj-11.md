## Weak references

Source code for this chapter is in [src/gobj-11/](../src/gobj-11/) directory.

[Weak references](https://docs.gtk.org/gobject/struct.WeakRef.html) are used in complex object-oriented systems to simplify object
lifetime management. For example, part of a program may cache items it recently
worked with, while allowing memory management subsystem do delete that items
in arbitrary moments without notifying that part of a program. Or, in a doubly
linked list, references to preceding items may be weak, thus breaking circular
reference loop between preceding and following item.

GObject library provide weak references it its API. However, note that
maintaining a weak reference set costs a certain amount of calculations.
GObject instances are reference-counted and notified on lifetime events. So,
weak references in their internals need to install notification callbacks to
be aware of object disappearing.

### GObject API for weak references

Weak references API is relatively simple. It has one type `GWeakRef` which is
internally a plain old C structure and four methods:

- to initialize newly allocated weak reference item
- to clear weak reference item that was initialized before
- to set weak reference value (pointer)
- to get weak reference value (pointer)

Last method automatically increments reference count of object pointed by weak
reference, so returned pointer is a strong reference and is owned by caller of
the method. When this last method fails to found "live" object, it returns NULL.

Inside `GWeakRef` structure there is simple pointer, holding NULL or some valid
value. This pointer is automatically cleared (NULL) on deletion of object it
had pointed to.

### Using weak references

Full source is in [src/gobj-11/main.c](../src/gobj-11/main.c).

Weak references may be created either as static objects or allocated with
memory allocation functions. Here we defined static `wr_s` initialized to NULL,
and allocated uninitialized `wr_d`, which has undefined state.

```C
  GWeakRef      wr_s = {0};
  GWeakRef     *wr_d;

  wr_d = g_new (GWeakRef, 1);
```

Initialized weak referense may be set to point at some object, while
uninitialized must be once initialized with some object pointer or NULL. The
last is because library do not know if current content of uninitialized is
pointer to something, or just arbitrary value, and should be told about.

```C
  g_weak_ref_set (&wr_s, obj);
  /* ... */
  g_weak_ref_init (wr_d, obj);
```

Cleared weak reference becomes uninitialized, no matter it is static or
dynamically allocated. To use it again it must be initialized once more.

```C
  g_weak_ref_clear (wr_d);
```

Pointer that user code gets from weak reference is *owned* by user code. Object
pointed by this pointer is automatically referenced (its reference count
incremented). When this pointer comes out of use, user code should unreference
it.

```C
  GObject      *obj, *ref_s, *ref_d;
  /* ... */
  obj = g_object_new (G_TYPE_OBJECT, NULL);
  g_weak_ref_set (&wr_s, obj);
  g_weak_ref_init (wr_d, obj);
  /* ... */
  ref_s = g_weak_ref_get (&wr_s);
  ref_d = g_weak_ref_get (wr_d);
  /* ... */
  g_object_unref (ref_s);
  g_object_unref (ref_d);
```

Running `main` from this chapter shows how reference count of object changes
on getting strong references from weak reference.
