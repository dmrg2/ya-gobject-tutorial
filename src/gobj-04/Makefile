CC = gcc

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`

LDFLAGS = `pkg-config --libs glib-2.0 gobject-2.0 gio-2.0`

EXEC =  main

SRC =  main.c tut-obj04.c tut-obj04m.c

OBJ =  $(SRC:%.c=%.o)

DEP = $(OBJ:%.o=%.d)

all: $(EXEC)

-include $(DEP)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -MMD -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f  $(DEP) $(OBJ) $(EXEC)
