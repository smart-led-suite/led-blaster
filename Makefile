#led-blaster Makefile
led-blaster-pre : led-blaster-pre.cpp led-blaster-pre.hpp fade.cpp fade.hpp led.hpp led.cpp file.cpp file.hpp init.cpp init.hpp modes.cpp modes.hpp config.h config.cpp config.hpp
	echo compiling led-blaster
	g++ -std=c++0x -g -o led-blaster-pre led-blaster-pre.cpp fade.cpp file.cpp config.cpp init.cpp modes.cpp led.cpp -lrt -lpthread -lconfig++
