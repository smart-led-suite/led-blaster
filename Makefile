#led-blaster Makefile
#led-blaster-pre : led-blaster-pre.cpp led-blaster-pre.hpp fade.cpp fade.hpp fifo.hpp fifo.cpp led.hpp led.cpp file.cpp file.hpp config.h
#	echo compiling led-blaster
#	g++ -std=c++0x -g -o led-blaster-pre led-blaster-pre.cpp fade.cpp file.cpp fifo.cpp led.cpp -lpigpio -lrt -lpthread

	# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++0x -Wall -Wextra -pthread -lpigpio -lrt
# Where to find user code.
USER_DIR = /opt/led-blaster

led-blaster : led.o file.o led-blaster-pre.o fade.o fifo.o
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@

file.o :  $(USER_DIR)/file.cpp $(USER_DIR)/file.hpp  $(USER_DIR)/led-blaster-pre.hpp
			$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/file.cpp

led.o :  $(USER_DIR)/led.cpp $(USER_DIR)/led.hpp  $(USER_DIR)/led-blaster-pre.hpp
			$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/led.cpp

	#init.o :  $(USER_DIR)/init.cpp $(USER_DIR)/init.hpp  $(USER_DIR)/led-blaster-pre.hpp
	#				$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/init.cpp

led-blaster-pre.o :  $(USER_DIR)/led-blaster-pre.cpp $(USER_DIR)/led-blaster-pre.hpp
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/led-blaster-pre.cpp

fade.o :  $(USER_DIR)/fade.cpp $(USER_DIR)/fade.hpp  $(USER_DIR)/led-blaster-pre.hpp
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/fade.cpp

fifo.o :  $(USER_DIR)/fifo.cpp $(USER_DIR)/fifo.hpp  $(USER_DIR)/led-blaster-pre.hpp
						$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  $(USER_DIR)/fifo.cpp
