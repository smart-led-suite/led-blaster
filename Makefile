#led-blaster Makefile
led-blaster-pre : led-blaster-pre.cpp led-blaster-pre.hpp fadeModes.cpp fadeModes.hpp currentBrightnessFileRW.cpp currentBrightnessFileRW.hpp init.cpp init.hpp modes.cpp modes.hpp config.h
	echo compiling led-blaster
	g++ -g -o led-blaster-pre led-blaster-pre.cpp fadeModes.cpp currentBrightnessFileRW.cpp init.cpp modes.cpp  -lpigpio -lrt -lpthread 

