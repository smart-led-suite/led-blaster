#led-blaster Makefile
#led-blaster : led-blaster.cpp led-blaster.hpp fade.cpp fade.hpp fifo.hpp fifo.cpp led.hpp led.cpp file.cpp file.hpp config.h
#	echo compiling led-blaster
#	g++ -std=c++0x -g -o led-blaster led-blaster.cpp fade.cpp file.cpp fifo.cpp led.cpp -lpigpio -lrt -lpthread

	# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++0x -Wall -Wextra -pthread  -lrt
# Where to find user code.
USER_DIR = ./

led-blaster : led.o led-blaster.o fifo.o
	$(CXX) $(CXXFLAGS) -lpthread -lpigpio $^ -o $@

local-led-blaster: led.o led-blaster.o fifo.o
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@

#file.o :  $(USER_DIR)/file.cpp $(USER_DIR)/file.hpp  $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h
#			$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/file.cpp

led.o :  $(USER_DIR)/led.cpp $(USER_DIR)/led.hpp  $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h
			$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/led.cpp

	#init.o :  $(USER_DIR)/init.cpp $(USER_DIR)/init.hpp  $(USER_DIR)/led-blaster.hpp
	#				$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/init.cpp

led-blaster.o :  $(USER_DIR)/led-blaster.cpp $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/led-blaster.cpp

fifo.o :  $(USER_DIR)/fifo.cpp $(USER_DIR)/fifo.hpp  $(USER_DIR)/led-blaster.hpp $(USER_DIR)/config.h
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/fifo.cpp
