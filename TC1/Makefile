all: inicia

TC1Compile: TC1.cpp 
	g++ -c TC1.cpp -lmath

tinystr: tinystr.cpp
	g++ -c tinystr.cpp

tinyxml: tinyxml.cpp
	g++ -c tinyxml.cpp

tinyxmlerror: tinyxmlerror.cpp
	g++ -c tinyxmlerror.cpp

tinyxmlparser: tinyxmlparser.cpp
	g++ -c tinyxmlparser.cpp

inicia: TC1Compile tinystr tinyxml tinyxmlerror tinyxmlparser
	g++ -o trabalhocg TC1.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o -framework OpenGL -framework GLUT

clean:
	rm *.o 
	rm  trabalhocg
	clear
