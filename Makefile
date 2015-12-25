#led-blaster Makefile
led-blaster-pre : led-blaster-pre.cpp led-blaster-pre.hpp fade.cpp fade.hpp led.hpp led.cpp currentBrightnessFileRW.cpp currentBrightnessFileRW.hpp init.cpp init.hpp modes.cpp modes.hpp config.h
	echo compiling led-blaster
	g++ -std=c++0x -g -o led-blaster-pre led-blaster-pre.cpp fade.cpp currentBrightnessFileRW.cpp init.cpp modes.cpp led.cpp -lpigpio -lrt -lpthread 
