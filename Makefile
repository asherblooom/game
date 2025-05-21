linux: main glad
	g++ -o bin/game bin/main.o bin/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

windows: main glad
	# add something here?

examples: triangle glad
	g++ -o examples/bin/triangle examples/bin/triangle.o bin/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

main: src/main.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ src/main.cpp -o bin/main.o -c

triangle: examples/src/triangle.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ examples/src/triangle.cpp -o examples/bin/triangle.o -c

glad: src/glad.cpp mkbin
	g++ -Iinclude/ src/glad.cpp -o bin/glad.o -c

mkbin:
	mkdir -p bin
	mkdir -p examples/bin

clean: 
	rm -rf bin/ */bin/

