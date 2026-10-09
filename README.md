## About this tutorial

This tutorial written and tested with GObject-2.0 and GLib-2.0, both version 2.90.1 under Linux.
Programs in this tutorial written in C. Tools used to compile are make, gcc and pkg-config.

This tutorial offers basic examples of GObject implementation and usage for those who learn
programming with GTK. GObject is base type system and library for object-oriented programs in
GTK environment. GObject provides infrastructure for type and class management, property and
method incapsulation, class inheritance, polymorphism, interfaces and so on. GTK library objects
are based on GObject.

There is [GObject API Reference](https://docs.gtk.org/gobject/), which provides detailed
information on parts of GObject API. The tutorial does not go beyond that documentation. It is
a collection of examples showing parts of API "brought together and working". I hope this
will help beginners to understand how GObject work and to learn how to use it.

## What you will need

To compile and change programs from this tutorial you will need:

1. A Linux installation.
2. Simple text editor (examples aren't so much in size) or IDE with C support.
3. Installed gcc, make and pkgconf.
4. Installed glib2.

## Getting source code

Clone repository to local folder:

```bash
git clone https://github.com/dmrg2/ya-gobject-tutorial.git
```

Or [download zip](https://github.com/dmrg2/ya-gobject-tutorial/archive/refs/heads/main.zip) and unpack it.

Both ways give you folder named after repository: ya-gobject-tutorial. Source code is inside that folder.

```bash
cd ya-gobject-tutorial
```

## Building and running

Tutorial code is divided to independed sections under src directory. No preparation or configuration needed.
Given current directory is root of repository, you should go to section directory and do make:

```bash
cd src/gobj-01
make
```
Makefile in each section produces single executable "main". Run it in current directory as usual:

```bash
./main
```

## Contributing

Feel free to post an issue. If you found an error in source and have fix for it, post pull-request.

## Table of contents

1. [GObject itself, creation of GObject and descendants, information on instances and its classes.](doc/gobj-01.md)
2. Create and run your first object.
3. Object with internal data, creatiion and deletion, freeing allocated memory.
4. Object inheritance, virtual methods, using GObject library convenience macros.
5. Object properties and interface to access them.
6. Using GObject 'notify' [of property change] signal.
7. Implementing custom signal on object, passing parameters wit signal.
8. Interfaces, interface inheritance, implementing interfaces in object.
9. Creating new fundamental type, passing its value between parts of code.
10. Using boxed types, enumeration types, flag types, more convenience macros.

## Other useful information sources

Other good tutorials:

["Official" GObject tutorial](https://docs.gtk.org/gobject/tutorial.html)\
[ToshioCP/Gobject-tutorial](https://github.com/ToshioCP/Gobject-tutorial)

API references and official documenation:

[GLib-2.0 documentation](https://docs.gtk.org/glib/)\
[GObject-2.0 documentation](https://docs.gtk.org/gobject/)\
[GModule-2.0 documentation](https://docs.gtk.org/gmodule/) -- about dynamic loading of objects.
