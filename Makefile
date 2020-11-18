CC = gcc
CFLAGS = -I./include -Wall -Wextra -pedantic
#LDFLAGS = -lm 
EXEC = bin/timer
INTER = bin/interface

all: $(EXEC)
inter: $(INTER)

$(EXEC):
	$(CC) $^ -o $@ #$(LDFLAGS)

$(INTER):
	$(CC) $^ -lncurses -o $@


bin/timer: obj/timer.o
bin/interface: obj/interface.o

obj/%.o:
	$(CC) $(CFLAGS) $< -c -o $@

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
