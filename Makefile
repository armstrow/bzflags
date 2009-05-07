
CC= g++
FLAGS= -Wall -ggdb -Iinc
EXENAME= robotter
OBJECTS= obj/RobotController.o obj/BZFSCommunicator.o obj/Robot.o


clean:
	rm -f obj/*.o

realclean: clean
	rm -f bin/*




build: bin/$(EXENAME)

run:
	bin/$(EXENAME)



bin/$(EXENAME): src/main.cpp obj/RobotController.o
	$(CC) $(FLAGS) src/main.cpp -o bin/$(EXENAME) $(OBJECTS)

obj/RobotController.o: inc/RobotController.h src/RobotController.cpp obj/BZFSCommunicator.o obj/Robot.o
	$(CC) $(FLAGS) -c src/RobotController.cpp -o obj/RobotController.o obj/BZFSCommunicator.o obj/Robot.o

obj/BZFSCommunicator.o: inc/BZFSCommunicator.h src/BZFSCommunicator.cpp
	$(CC) $(FLAGS) -c src/BZFSCommunicator.cpp -o obj/BZFSCommunicator.o 

obj/Robot.o: inc/Robot.h inc/Robot.h obj/BZFSCommunicator.o
	$(CC) $(FLAGS) -c src/Robot.cpp -o obj/Robot.o obj/BZFSCommunicator.o



