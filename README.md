# Pandemic

## Authors : Jérémy Poullain / Guillaume Revel

### This project contains :
- README.md:
Describe the project
- Makefile:
Generate the executable files of the project
- ./src:
Contain the source files (.c) of the project
- ./include:
Contain the include files (.h) of the project
- ./obj:
Contain the object files (.o) of the project
- ./bin:
Contain the executable files of the project
- ./etc:
Contain useful files to load maps, graphics and to help the user
- ./doc:
Contain documentation on the project

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