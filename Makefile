CC = gcc
CFLAGS = -I./include -Wall -Wextra -pedantic
LDFLAGS = -lrt # -lm
EXEC = bin/epidemic_sim bin/timer
INTERFACE = bin/interface

all: $(EXEC)
inter: $(INTERFACE)

$(EXEC):
	$(CC) $^ -o $@ $(LDFLAGS)

$(INTERFACE):
	$(CC) $^ -lncurses -o $@

bin/epidemic_sim: obj/main.o

bin/timer: obj/timer.o
bin/interface: obj/interface.o

obj/%.o:
	$(CC) $(CFLAGS) $< -c -o $@

obj/main.o: src/main.c
obj/timer.o: src/timer.c

obj/interface.o: src/interface.c

clean:
	rm -f obj/*.o
	rm -f include/*~
	rm -f src/*~
	rm -f *~

distclean: clean
	rm -f bin/*

.PHONY: all clean distclean
