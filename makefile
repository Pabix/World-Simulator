all: main.o predator.o map_function.o data_struct.o select_lib.o
	gcc -o out main.o predator.o map_function.o data_struct.o select_lib.o

%.o: %.c types.h
	gcc -c $<