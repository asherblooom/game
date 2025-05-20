linux: main glad
	g++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o game.o main.o glad.o

windows: main glad
	# add something here?

main: src/main.cpp 
	g++ -Wall -Wextra -Iinclude/ src/main.cpp -o main.o -c

glad: src/glad.c
	g++ -Iinclude/ src/glad.c -o glad.o -c

