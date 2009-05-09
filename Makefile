



CC= g++
FLAGS= -Wall -ggdb -Iinc -lpthread -lm
EXENAME= robotter
OBJECTS= obj/RobotController.o obj/BZFSCommunicator.o obj/Robot.o obj/EnvironmentData.o obj/PersonalData.o obj/MyTank.o obj/OtherTank.o obj/Shot.o obj/Obstacle.o obj/Flag.o obj/Team.o obj/Constant.o obj/Base.o






bin: bin/$(EXENAME)

run: bin
	bin/$(EXENAME)

run-nodebug: bin
	bin/$(EXENAME) localhost 12345

run-debug: bin
	bin/$(EXENAME) -d localhost 12345

clean:
	rm -f obj/*.o

realclean: clean
	rm -f bin/*







bin/$(EXENAME): src/main.cpp $(OBJECTS)
	$(CC) $(FLAGS) src/main.cpp -o bin/$(EXENAME) $(OBJECTS)

obj/RobotController.o: inc/RobotController.h src/RobotController.cpp obj/BZFSCommunicator.o obj/Robot.o obj/EnvironmentData.o
	$(CC) $(FLAGS) -c src/RobotController.cpp -o obj/RobotController.o 

obj/BZFSCommunicator.o: inc/BZFSCommunicator.h src/BZFSCommunicator.cpp
	$(CC) $(FLAGS) -c src/BZFSCommunicator.cpp -o obj/BZFSCommunicator.o 

obj/Robot.o: inc/Robot.h inc/Robot.h obj/BZFSCommunicator.o obj/PersonalData.o obj/EnvironmentData.o
	$(CC) $(FLAGS) -c src/Robot.cpp -o obj/Robot.o 

obj/PersonalData.o: inc/PersonalData.h src/PersonalData.cpp
	$(CC) $(FLAGS) -c src/PersonalData.cpp -o obj/PersonalData.o

obj/EnvironmentData.o: inc/EnvironmentData.h src/EnvironmentData.cpp
	$(CC) $(FLAGS) -c src/EnvironmentData.cpp -o obj/EnvironmentData.o

obj/Team.o: inc/Team.h src/Team.cpp
	$(CC) $(FLAGS) -c src/Team.cpp -o obj/Team.o

obj/Flag.o: inc/Flag.h src/Flag.cpp
	$(CC) $(FLAGS) -c src/Flag.cpp -o obj/Flag.o

obj/Shot.o: inc/Shot.h src/Shot.cpp
	$(CC) $(FLAGS) -c src/Shot.cpp -o obj/Shot.o

obj/MyTank.o: inc/MyTank.h src/MyTank.cpp
	$(CC) $(FLAGS) -c src/MyTank.cpp -o obj/MyTank.o

obj/OtherTank.o: inc/OtherTank.h src/OtherTank.cpp
	$(CC) $(FLAGS) -c src/OtherTank.cpp -o obj/OtherTank.o

obj/Obstacle.o: inc/Obstacle.h src/Obstacle.cpp
	$(CC) $(FLAGS) -c src/Obstacle.cpp -o obj/Obstacle.o

obj/Constant.o: inc/Constant.h src/Constant.cpp
	$(CC) $(FLAGS) -c src/Constant.cpp -o obj/Constant.o

obj/Base.o: inc/Base.h src/Base.cpp
	$(CC) $(FLAGS) -c src/Base.cpp -o obj/Base.o


