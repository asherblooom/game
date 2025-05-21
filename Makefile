linux: main glad
	g++ -o bin/game bin/link/main.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

windows: main glad
	# add something here?

examples: triangle point glad
	g++ -o examples/bin/triangle examples/bin/link/triangle.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	g++ -o examples/bin/point examples/bin/link/point.o bin/link/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

main: src/main.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ src/main.cpp -o bin/link/main.o -c

triangle: examples/src/triangle.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ examples/src/triangle.cpp -o examples/bin/link/triangle.o -c

point: examples/src/point.cpp mkbin
	g++ -Wall -Wextra -Iinclude/ examples/src/point.cpp -o examples/bin/link/point.o -c

glad: src/glad.cpp mkbin
	g++ -Iinclude/ src/glad.cpp -o bin/link/glad.o -c

mkbin:
	mkdir -p bin/link
	mkdir -p examples/bin/link

clean: 
	rm -rf bin/ */bin/

