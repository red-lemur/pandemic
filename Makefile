CC = gcc
CFLAGS = -I./include -Wall -Wextra -pedantic -g
LDFLAGS = -lrt -lpthread # -lm
EXEC = bin/main bin/epidemic_sim bin/citizen_manager bin/press_agency bin/timer
INTERFACE = bin/interface

all: $(EXEC)
inter: $(INTERFACE)

$(EXEC):
	$(CC) $^ -o $@ $(LDFLAGS)

$(INTERFACE):
	$(CC) $^ -lncurses -o $@

bin/main: obj/main.o
bin/epidemic_sim: obj/epidemic_sim.o obj/city_sim.o obj/map_generator.o obj/util.o
bin/citizen_manager: obj/citizen_manager.o obj/util.o
bin/press_agency: obj/press_agency.o
bin/timer: obj/timer.o
bin/interface: obj/interface.o

obj/%.o:
	$(CC) $(CFLAGS) $< -c -o $@

obj/main.o: src/main.c include/file_paths.h
obj/epidemic_sim.o: src/epidemic_sim.c include/epidemic_sim.h src/city_sim.c include/city_sim.h src/map_generator.c include/map_generator.h include/city.h include/exchanges_between_processes.h
obj/citizen_manager.o: src/citizen_manager.c include/citizen_manager.h include/epidemic_sim.h include/city.h include/exchanges_between_processes.h src/util.c include/util.h include/city_sim.h
obj/press_agency.o: src/press_agency.c
obj/timer.o: src/timer.c include/timer.h
obj/city_sim.o: src/city_sim.c include/city_sim.h include/city.h
obj/map_generator.o: src/map_generator.c include/map_generator.h src/city_sim.c include/city_sim.h include/city.h src/util.c include/util.h
obj/util.o: src/util.c include/util.h
obj/interface.o: src/interface.c include/interface.h

clean:
	rm -f obj/*.o
	rm -f include/*~
	rm -f src/*~	
	rm -f etc/*~
	rm -f *~

distclean: clean
	rm -f bin/*

.PHONY: all clean distclean
