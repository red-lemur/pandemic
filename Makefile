SRC = src
INCLUDE = include
OBJ = obj
BIN = bin
ETC = etc
DOC = doc

CC = gcc
CFLAGS = -I./$(INCLUDE) -Wall -Wextra -pedantic
LDFLAGS = -lrt -lpthread -lm -lncurses
EXEC = $(BIN)/main $(BIN)/epidemic_sim $(BIN)/citizen_manager $(BIN)/press_agency $(BIN)/timer

all: $(EXEC)

$(EXEC):
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN)/main: $(OBJ)/main.o
$(BIN)/epidemic_sim: $(OBJ)/epidemic_sim.o $(OBJ)/city_sim.o $(OBJ)/map_generator.o $(OBJ)/util.o $(OBJ)/interface.o
$(BIN)/citizen_manager: $(OBJ)/citizen_manager.o $(OBJ)/util.o
$(BIN)/press_agency: $(OBJ)/press_agency.o
$(BIN)/timer: $(OBJ)/timer.o
$(BIN)/interface: $(OBJ)/interface.o

$(OBJ)/%.o:
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJ)/main.o: $(SRC)/main.c $(INCLUDE)/file_paths.h $(INCLUDE)/exchanges_between_processes.h
$(OBJ)/epidemic_sim.o: $(SRC)/epidemic_sim.c $(INCLUDE)/epidemic_sim.h $(SRC)/city_sim.c $(INCLUDE)/city_sim.h $(SRC)/map_generator.c $(INCLUDE)/map_generator.h $(INCLUDE)/city.h $(INCLUDE)/exchanges_between_processes.h $(SRC)/interface.c $(INCLUDE)/interface.h $(INCLUDE)/file_paths.h
$(OBJ)/citizen_manager.o: $(SRC)/citizen_manager.c $(INCLUDE)/citizen_manager.h $(INCLUDE)/epidemic_sim.h $(INCLUDE)/city.h $(INCLUDE)/exchanges_between_processes.h $(SRC)/util.c $(INCLUDE)/util.h $(INCLUDE)/city_sim.h $(INCLUDE)/names.h
$(OBJ)/press_agency.o: $(SRC)/press_agency.c $(INCLUDE)/press_agency.h $(INCLUDE)/exchanges_between_processes.h $(INCLUDE)/citizen_manager.h $(INCLUDE)/city.h
$(OBJ)/timer.o: $(SRC)/timer.c $(INCLUDE)/timer.h
$(OBJ)/city_sim.o: $(SRC)/city_sim.c $(INCLUDE)/city_sim.h $(INCLUDE)/city.h
$(OBJ)/map_generator.o: $(SRC)/map_generator.c $(INCLUDE)/map_generator.h $(SRC)/city_sim.c $(INCLUDE)/city_sim.h $(INCLUDE)/city.h $(SRC)/util.c $(INCLUDE)/util.h
$(OBJ)/util.o: $(SRC)/util.c $(INCLUDE)/util.h
$(OBJ)/interface.o: $(SRC)/interface.c $(INCLUDE)/interface.h $(INCLUDE)/city.h

clean:
	rm -f $(OBJ)/*.o
	rm -f $(INCLUDE)/*~
	rm -f $(SRC)/*~	
	rm -f $(ETC)/*~
	rm -f $(DOC)/*~
	rm -f *~

distclean: clean
	rm -f $(BIN)/*
	rm -f $(ETC)/evolution.txt
	rm -f $(ETC)/sos

.PHONY: all clean distclean
