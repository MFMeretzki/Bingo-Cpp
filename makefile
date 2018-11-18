# Makefile for the socket programming example
#

VPATH = Bin Lib Server
CXXFLAGS = -std=gnu++11 -Wall
INC = -I/home/grunash/Projects/BingoCpp/Lib -I/home/grunash/Projects/BingoCpp/Server

server_objects = ClientConnection.o ClientData.o Card.o Encoder.o GameState.o WaitNewGameState.o GameStartedState.o GameLogic.o
client_objects = 
lib_objects = Socket.o ServerSocket.o ClientSocket.o ConcurrentOutput.o EventDispatcher.o
lib_headers = Socket.h ServerSocket.h ClientSocket.h ConcurrentOutput.h EventDispatcher.h



all : server client



# Server Objects -------------------



server_: $(server_objects)
	g++ -std=gnu++11 -pthread -o server $(server_objects)

server: $(server_objects)

Bin/ClientConnection.o: ClientConnection.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/ClientData.o: ClientData.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/Card.o: Card.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/Encoder.o: Encoder.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/GameState.o: GameState.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/WaitNewGameState.o: WaitNewGameState.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/GameStartedState.o: GameStartedState.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)

Bin/GameLogic.o: GameLogic.cpp
	g++ -c $(CXXFLAGS) $^ -o $@ $(INC)



# Lib Objects -------------------

lib: $(lib_objects)

Bin/Socket.o: Socket.cpp
	g++ -c $(CXXFLAGS) $^ -o $@

Bin/ServerSocket.o: ServerSocket.cpp
	g++ -c $(CXXFLAGS) $^ -o $@
	
Bin/ClientSocket.o: ClientSocket.cpp
	g++ -c $(CXXFLAGS) $^ -o $@
	
Bin/ConcurrentOutput.o: ConcurrentOutput.cpp
	g++ -c $(CXXFLAGS) $^ -o $@

Bin/EventDispatcher.o: EventDispatcher.cpp
	g++ -c $(CXXFLAGS) $^ -o $@



# Clean rules ---------------------

clean:
	rm -fv server client
	rm -fv Bin/*.o