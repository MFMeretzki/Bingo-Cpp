#include "Server.h"


int main(int argc, char **argv)
{
	Server *server = new Server();
	server->Start();
	
	return 0;
}