#include "Server.h"

unsigned short const PORT = 2008;

void main() {
	Server* server = new Server(PORT);

	server->start();
	server->run();

	system("pause");
}