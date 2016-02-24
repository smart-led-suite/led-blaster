#led-blaster Makefile
led-blaster-pre : led-blaster-pre.cpp led-blaster-pre.hpp fade.cpp fade.hpp fifo.hpp fifo.cpp led.hpp led.cpp file.cpp file.hpp init.cpp init.hpp modes.cpp modes.hpp config.h
	echo compiling led-blaster
	g++ -std=c++0x -g -o led-blaster-pre led-blaster-pre.cpp fade.cpp file.cpp init.cpp modes.cpp fifo.cpp led.cpp -lpigpio -lrt -lpthread
