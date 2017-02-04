#MAKEFILE FOR led-blaster, a c++ daemon to fade leds with a raspberry pi

	# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++0x -Wall -Wextra -pthread  -lrt
# Where to find user code.
USER_DIR = ./

led-blaster : led.o led-blaster.o fifo.o
	$(CXX) $(CXXFLAGS) -lpthread -lpigpio $^ -o $@

local-led-blaster: led.o led-blaster.o fifo.o
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@

led.o :  $(USER_DIR)/led.cpp $(USER_DIR)/led.hpp  $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h $(USER_DIR)/desktop-config.h
			$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/led.cpp


led-blaster.o :  $(USER_DIR)/led-blaster.cpp $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h $(USER_DIR)/desktop-config.h
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/led-blaster.cpp

fifo.o :  $(USER_DIR)/fifo.cpp $(USER_DIR)/fifo.hpp  $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h $(USER_DIR)/desktop-config.h
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/fifo.cpp
