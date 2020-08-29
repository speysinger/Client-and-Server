#include <iostream>
#include <WS2tcpip.h>
#include <string>

#include "Commands.h"

#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")

const int stringLength = 140;

enum ServerCommands { LOGIN, LOGOUT, CALC, WRONG };

bool validateUser(char *login, char *password) {
	return false;
}


bool checkCommand(char *str, Command *command) {

	const int commandSize = command->getSize();
	char *commandWord = new char[commandSize];
	commandWord[commandSize] = '\0';

	strncpy(commandWord, str, commandSize);

	if (strcmp(commandWord, command->getWord()) == 0)
		return true;
	return false;
}

ServerCommands processCommand(char *str) {
	Command *loginCommand = new LoginCommand;
	Command *logoutCommand = new LogoutCommand;
	Command *calcCommand = new CalcCommand;

	if (checkCommand(str, loginCommand)) {
		return ServerCommands::LOGIN;
	}
	else if (checkCommand(str, logoutCommand)) {
		return ServerCommands::LOGOUT;
	}
	else if (checkCommand(str, calcCommand)) {
		return ServerCommands::CALC;
	}
	else {
		return ServerCommands::WRONG;
	}
}


int main()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return 0;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return 0;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	std::cout << "Server started" << std::endl;

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char clientName[NI_MAXHOST];
	char clientPort[NI_MAXSERV];	// clientPort

	memset(clientName, 0, NI_MAXHOST);
	memset(clientPort, 0, NI_MAXSERV);


	if (getnameinfo((sockaddr*)&client, sizeof(client), clientName, NI_MAXHOST, clientPort, NI_MAXSERV, 0) == 0) {
		std::cout << clientName << " connected on port " << clientPort << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, clientName, NI_MAXHOST);
		std::cout << clientName << " connected on port " <<
			ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[stringLength];

	ServerCommands command;
	while (true) {
		memset(buf, 0, stringLength);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, stringLength, 0);
		if (bytesReceived == SOCKET_ERROR) {
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}

		if (bytesReceived == 0) {
			std::cout << clientName << " disconnected " << std::endl;
			break;
		}

		command = processCommand(buf);

		switch (command) {
		case ServerCommands::LOGIN:
			break;
		case ServerCommands::LOGOUT:
			break;
		case ServerCommands::CALC:
			break;
		case ServerCommands::WRONG:
			char message[] = "wrong command";
			send(clientSocket, message, strlen(message), 0);
		}

		std::cout << std::string(buf, 0, bytesReceived) << std::endl;

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);

	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
	return 0;
}