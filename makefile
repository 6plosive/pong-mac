all:
	g++ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -framework ApplicationServices libraylib.a -I ./src/ -std=c++11 main.cpp -o main
	./main