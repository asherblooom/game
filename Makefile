main: src/main.cpp
	gcc -Wall -Wextra -Iinclude/ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl src/main.cpp -o main.o
