#include "server.h"

int main()
{
	Server *server = new Server();
	server->start();

	system("pause");
	return 0;
}