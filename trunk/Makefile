
CC= g++
FLAGS= -Wall -ggdb -Iinc
EXENAME= bin/robotter
OBJECTS= 


clean:
	rm -f obj/*.o

realclean: clean
	rm -f bin/*

build: src/main.cpp
	$(CC) $(FLAGS) -o $(EXENAME) src/main.cpp $(OBJECTS)







