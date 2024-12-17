.default: all

all: mazeRunner

clean:
	rm -f mazeRunner *.o

mazeRunner: Agent.o MazeGenerator.o mazeRunner.o MazeValidation.o Menu.o Terraform.o NoTerraform.o Maze.o
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^
