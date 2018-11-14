# Makefile for the socket programming example
#

server_objects =  Server/server.o
client_objects = Client/client.o
lib_objects = Lib/Socket.o Lib/ServerSocket.o Lib/ClientSocket.o


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

Socket.o: Lib/Socket.cpp
	g++ -std=gnu++11 -Wall -c -o Lib/Socket.o Lib/Socket.cpp

ServerSocket: Lib/ServerSocket.cpp
	g++ -std=gnu++11 -Wall -c -o Lib/ServerSocket.o Lib/ServerSocket.cpp
	
ClientSocket: Lib/ClientSocket.cpp
	g++ -std=gnu++11 -Wall -c -o Lib/ClientSocket.o Lib/ClientSocket.cpp



clean:
	rm -fv *.o server client
	rm -fv Lib/*.o
	rm -fv Server/*.o
	rm -fv Client/*.o