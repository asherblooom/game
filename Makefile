# make rules for main game
linux: main glad
	g++ -o bin/game bin/link/main.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

windows: main glad
	# add something here?

main: src/main.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ src/main.cpp -o bin/link/main.o -c



# make rules for examples
examples: moving-triangle triangle point glad
	g++ -o examples/bin/triangle examples/bin/link/triangle.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	g++ -o examples/bin/point examples/bin/link/point.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	g++ -o examples/bin/moving-triangle examples/bin/link/moving-triangle.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

moving-triangle: examples/src/moving-triangle.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ examples/src/moving-triangle.cpp -o examples/bin/link/moving-triangle.o -c

triangle: examples/src/triangle.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ examples/src/triangle.cpp -o examples/bin/link/triangle.o -c

point: examples/src/point.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ examples/src/point.cpp -o examples/bin/link/point.o -c



# compiles the glad library
glad: src/glad.cpp mkbin
	g++ -Iinclude/ src/glad.cpp -o bin/link/glad.o -c

# creates bin folders if they don't already exist
mkbin:
	mkdir -p bin/link
	mkdir -p examples/bin/link

# deletes all the bin folders (and containing executables)
clean: 
	rm -rf bin/ */bin/

