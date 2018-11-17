# Makefile for the socket programming example
#

VPATH = Bin Lib Server
CXXFLAGS = -std=gnu++11 -Wall
INC = -I/home/grunash/Projects/BingoCpp/Lib

server_objects =  Server/server.o
client_objects = Client/client.o
lib_objects = Socket.o ServerSocket.o ClientSocket.o ConcurrentOutput.o EventDispatcher.o
lib_headers = Socket.h ServerSocket.h ClientSocket.h ConcurrentOutput.h EventDispatcher.h



all : server client



# Server Objects -------------------

server_: $(server_objects)
	g++ -std=gnu++11 -pthread -o server $(server_objects)

server: server.cpp
	g++ -std=gnu++11 -c server.cpp -pthread

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



# Client Objects -------------------

client_: $(client_objects)
	g++ -std=gnu++11 -pthread -o client $(client_objects)
	
client: client.cpp
	g++ -std=gnu++11 -c client.cpp -pthread



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