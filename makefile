# Makefile for the socket programming example
#

server_objects =  Server/server.o
client_objects = Client/client.o
lib_objects = Bin/Socket.o Bin/ServerSocket.o Bin/ClientSocket.o Bin/ConcurrentOutput.o Bin/EventDispatcher.o

all : server client



# Server Objects -------------------

server_: $(server_objects)
	g++ -std=gnu++11 -pthread -o server $(server_objects)

client_: $(client_objects)
	g++ -std=gnu++11 -pthread -o client $(client_objects)

server: server.cpp
	g++ -std=gnu++11 -c server.cpp -pthread



# Client Objects -------------------

client_: $(client_objects)
	g++ -std=gnu++11 -pthread -o client $(client_objects)
	
client: client.cpp
	g++ -std=gnu++11 -c client.cpp -pthread



# Lib Objects -------------------

lib: $(lib_objects)

Bin/Socket.o: Lib/Socket.cpp
	g++ -std=gnu++11 -Wall -c -o Bin/Socket.o Lib/Socket.cpp

Bin/ServerSocket.o: Lib/ServerSocket.cpp
	g++ -std=gnu++11 -Wall -c -o Bin/ServerSocket.o Lib/ServerSocket.cpp
	
Bin/ClientSocket.o: Lib/ClientSocket.cpp
	g++ -std=gnu++11 -Wall -c -o Bin/ClientSocket.o Lib/ClientSocket.cpp

Bin/ConcurrentOutput.o: Lib/ConcurrentOutput.cpp
	g++ -std=gnu++11 -Wall -c -o Bin/ConcurrentOutput.o Lib/ConcurrentOutput.cpp

Bin/EventDispatcher.o: Lib/EventDispatcher.cpp
	g++ -std=gnu++11 -Wall -c -o Bin/EventDispatcher.o Lib/EventDispatcher.cpp



# Clean rules ---------------------

clean:
	rm -fv server client
	rm -fv Bin/*.o