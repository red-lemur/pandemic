CC = gcc
CFLAGS = -I./include -Wall -Wextra -pedantic
LDFLAGS = -lrt 
EXEC = bin/epidemic_sim bin/timer

all: $(EXEC)

$(EXEC):
	$(CC) $^ -o $@ $(LDFLAGS)

bin/epidemic_sim: obj/main.o
bin/timer: obj/timer.o

obj/%.o:
	$(CC) $(CFLAGS) $< -c -o $@

obj/main.o: src/main.c
obj/timer.o: src/timer.c

clean:
	rm -f obj/*.o
	rm -f include/*~
	rm -f src/*~
	rm -f *~

distclean: clean
	rm -f bin/*

.PHONY: all clean distclean
