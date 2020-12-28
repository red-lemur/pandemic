# Pandemic

## Authors : Jérémy Poullain / Guillaume Revel

### Project description :
This project consist in the simulation of the spread of an epidemic in a city.
The city is composed of a tiles of different types representing buildings or places.
Citizens of different professions move around the city, can become infected, get sick and die.
The simulation is done in a certain number of rounds (days) which can be followed in real time.
At the end of the simulation, a synthetic graph appears to show the evolution of the epidemic.

### This project contains :
- README.md:
describe the project
- Makefile:
generate the executable files of the project
- ./src:
contain the source files (.c) of the project
- ./include:
contain the include files (.h) of the project
- ./obj:
contain the object files (.o) of the project
- ./bin:
contain the executable files of the project
- ./etc:
contain useful files to load maps, graphics and to help the user
- ./doc:
contain documentation on the project

### Make commands:
To compile the project:
```{bash}
$ make
```

To delete temporary files:
```{bash}
$ make clean
```

To delete all the files created by the make command:
```{bash}
$ make distclean
```

### You can execute the program with:
```{bash}
$ ./bin/main
```
Do not run the other binary files than 'main', or they may loop endlessly.
If you executed them anyway, you can stop them with the command ```kill -9```
followed by the PID of the processes (you can find their PID with the command ```ps -e```).

### Map:
You can change the map by modifying the 'map.txt' file in the 'etc' folder.

### If you have any issues:
This program is certainly not fully protected against bugs. If a problem occurs, two scripts
can help you.

If your terminal has problems after running the program (shifted lines, wrong colors, etc.),
you can type the following command:
```{bash}
$ ./etc/clear
```

If the program has interrupted and processes are still running in the background, you can type
the following command:
```{bash}
$ ./etc/sos
```

If you have any problem with gnuplot, make sure you installed the 'gnuplot-qt' package on your
device.