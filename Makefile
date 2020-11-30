CC = gcc
CFLAGS = -I./include -Wall -Wextra -pedantic
LDFLAGS = -lrt # -lm
EXEC = bin/main bin/epidemic_sim bin/citizen_manager bin/press_agency bin/timer
INTERFACE = bin/interface

all: $(EXEC)
inter: $(INTERFACE)

$(EXEC):
	$(CC) $^ -o $@ $(LDFLAGS)

$(INTERFACE):
	$(CC) $^ -lncurses -o $@

bin/main: obj/main.o
bin/epidemic_sim: obj/epidemic_sim.o obj/city.o
bin/citizen_manager: obj/citizen_manager.o
bin/press_agency: obj/press_agency.o
bin/timer: obj/timer.o
bin/interface: obj/interface.o

obj/%.o:
	$(CC) $(CFLAGS) $< -c -o $@

obj/main.o: src/main.c
obj/epidemic_sim.o: src/epidemic_sim.c include/epidemic_sim.h include/city.h
obj/citizen_manager.o: src/citizen_manager.c include/epidemic_sim.h include/city.h
obj/press_agency.o: src/press_agency.c
obj/timer.o: src/timer.c include/timer.h
obj/city.o: src/city.c include/city.h
obj/interface.o: src/interface.c

clean:
	rm -f obj/*.o
	rm -f include/*~
	rm -f src/*~
	rm -f *~

distclean: clean
	rm -f bin/*

.PHONY: all clean distclean
