linux: main glad
	g++ -o game.o main.o glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

windows: main glad
	# add something here?

main: src/main.cpp 
	g++ -Wall -Wextra -Iinclude/ src/main.cpp -o main.o -c

glad: src/glad.cpp
	g++ -Iinclude/ src/glad.cpp -o glad.o -c

clean: 
	rm *.o

