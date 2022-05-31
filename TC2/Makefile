#	Iagoh Ribeiro Lima - 2014100678
#		Computacao Grafica 2018/2


all: inicia

TC2Compile: TC2.cpp 
	g++ -c TC2.cpp -lmath

tinystr: tinystr.cpp
	g++ -c tinystr.cpp

tinyxml: tinyxml.cpp
	g++ -c tinyxml.cpp

tinyxmlerror: tinyxmlerror.cpp
	g++ -c tinyxmlerror.cpp

tinyxmlparser: tinyxmlparser.cpp
	g++ -c tinyxmlparser.cpp

inicia: TC2Compile tinystr tinyxml tinyxmlerror tinyxmlparser
	g++ -o trabalhocg TC2.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o -framework OpenGL -framework GLUT

clean:
	rm *.o 
	rm  trabalhocg
	clear
