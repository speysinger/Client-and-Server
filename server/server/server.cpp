#include <WS2tcpip.h>
#include <iostream>
#include <string>

#include "server.h"

#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")

Server::Server() {
	DB = new DbFacade();
}

bool Server::validateUser(char *login, char *password) {
	return false;
}

bool Server::processLoginCommand(char *str) {
	return false;
}

bool Server::processLogoutCommand(char *str) {
	return false;
}

bool Server::processCalcCommand(char *str) {
	return false;
}

bool Server::checkCommand(char *str, Command *command) {

	const int commandSize = command->getSize();
	char *commandWord = new char[commandSize];
	commandWord[commandSize] = '\0';

	strncpy(commandWord, str, commandSize);

	if (strcmp(commandWord, command->getWord()) == 0)
		return true;
	return false;
}

char* Server::deleteSubString(char *str, int numberCharactersToRemove) {
	str = str + numberCharactersToRemove;
	char *temp;
	temp = str;
	return temp;
}

///Обработка команды в строке - поиск, распознавание, удаление
Server::ServerCommands Server::processCommand(char *str) {
	Command *loginCommand = new LoginCommand;
	Command *logoutCommand = new LogoutCommand;
	Command *calcCommand = new CalcCommand;

	//template or lambda? (Command, condition)
	if (checkCommand(str, loginCommand)) {
		str = deleteSubString(str, loginCommand->getSize());
		if (str[0] != ' ')
			return ServerCommands::WRONG;
		return ServerCommands::LOGIN;
	}
	else if (checkCommand(str, logoutCommand)) {
		str = deleteSubString(str, logoutCommand->getSize());
		if (str[0] != '\0')
			return ServerCommands::WRONG;
		return ServerCommands::LOGOUT;
	}
	else if (checkCommand(str, calcCommand)) {
		str = deleteSubString(str, logoutCommand->getSize());
		if (str[0] != ' ') {
			return ServerCommands::WRONG;
		}
		return ServerCommands::CALC;
	}
	else {
		return ServerCommands::WRONG;
	}
}


void Server::start()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return;
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
	char clientPort[NI_MAXSERV];

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
	bool isUserValidated = false;
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
		DB->createUser("test1", "test2", "test3");
		command = processCommand(buf);

		switch (command) {
		case ServerCommands::LOGIN:
			break;
		case ServerCommands::LOGOUT:
			if (!isUserValidated)
				std::cerr << "You must login" << std::endl;
			//logout
			break;
		case ServerCommands::CALC:
			if (!isUserValidated)
				std::cerr << "You must login" << std::endl;
			//get substr from str
			break;
		case ServerCommands::WRONG:
			char message[] = "wrong command\n";
			send(clientSocket, message, strlen(message), 0);
			break;
		}

		//std::cout << std::string(buf, 0, bytesReceived) << std::endl;
		//send(clientSocket, buf, bytesReceived + 1, 0);

	}

	closesocket(clientSocket);

	WSACleanup();
	return;
}