CC = gcc

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`

LDFLAGS = `pkg-config --libs glib-2.0 gobject-2.0 gio-2.0` -lconfig

EXEC =  steamdrum

TEST =  tester

SRC =  steamdrum.c nameutils.c

TSRC =  tester.c nameutils.c

OBJ =  $(SRC:%.c=%.o)

TOBJ =  $(TSRC:%.c=%.o)

DEP = $(OBJ:%.o=%.d)

TDEP = $(TOBJ:%.o=%.d)

all: $(EXEC)

test: $(EXEC) $(TEST)
	./$(TEST) > tests/tester.out
	./$(EXEC) -o tests -d tests/test.cfg
	gcc -c -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0` -o tests/tut-obj01.o tests/tut-obj01.c
	gcc -c -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0` -o tests/tut-obj02.o tests/tut-obj02.c
	gcc -c -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0` -o tests/ttt-inamed.o tests/ttt-inamed.c
	gcc -c -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0` -o tests/ttt-iknown.o tests/ttt-iknown.c
	gcc -c -Wall -g `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0` -o tests/ttt-itrusted.o tests/ttt-itrusted.c

-include $(DEP)

-include $(TDEP)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(TEST): $(TOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -MMD -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f  $(DEP) $(TDEP) $(OBJ) $(TOBJ) $(EXEC) $(TEST) tests/tester.out tests/tut-obj* tests/ttt-i*
