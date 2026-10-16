CC = gcc

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`

LDFLAGS = `pkg-config --libs glib-2.0 gobject-2.0 gio-2.0`

EXEC =  main

DLLIB =  greetermodule.so

DL2LIB =  greetermodulealt.so

SRC =  main.c tut-greeter.c

DLSRC =  tut-module-greeter.c

DL2SRC =  tut-module-greeter-alt.c

OBJ =  $(SRC:%.c=%.o)

DLOBJ =  $(DLSRC:%.c=%.o)

DL2OBJ =  $(DL2SRC:%.c=%.o)

DEP = $(OBJ:%.o=%.d)

DLDEP = $(DLOBJ:%.o=%.d)

DL2DEP = $(DL2OBJ:%.o=%.d)

all: $(EXEC) $(DLLIB) $(DL2LIB)

-include $(DEP)

-include $(DLDEP)

-include $(DL2DEP)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(DLLIB): $(DLOBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(DL2LIB): $(DL2OBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -MMD -fPIC -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f  $(DEP) $(DLDEP) $(DL2DEP) $(OBJ) $(DLOBJ) $(DL2OBJ) $(EXEC) $(DLLIB) $(DL2LIB)
