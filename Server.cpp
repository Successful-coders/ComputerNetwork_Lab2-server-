#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include "stdio.h"
#include "string"
#include "sstream"

Server::Server(unsigned short port) {
	this->port = port;

	int res = WSAStartup(0x0101, &wsaData);
	if (res == SOCKET_ERROR) {
		printf("WSAStartuo() failed: %ld\n", GetLastError());
		Server::close();
	}

	// Присваеваем сокету дескриптор.
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		printf("Failed to create server socket: %ld\n", GetLastError());
		Server::close();
	}
	// Заполняем структуру с информацией
	// о сокете сервера.
	serverSIN.sin_family = AF_INET;			// AF_INET определяет взаимодействие через интернет.
	serverSIN.sin_port = htons(port);		// Записываем порт.
	serverSIN.sin_addr.s_addr = INADDR_ANY; // Привязываем сокет ко всем локальным IP.

	buf = (char*)malloc(BUF_SIZE);
}

Server::~Server() {
	free(buf);
}

// Привязываем сервер к IP-адресу.
void Server::start() {
	int res = bind(serverSocket, (LPSOCKADDR)&serverSIN, sizeof(serverSIN));
	if (res == SOCKET_ERROR) {
		printf("Failed to bind server: %ld\n", GetLastError());
		Server::close();
	}
	printf("Server binded.\n");
}

// Запускаем работу сервера.
void Server::run() {
	int res;
	int a = 0, b = 0;
	int clientSINSize = sizeof(clientSIN);
	while (true) {
		// Инициализируем слушающий сокет.
		res = listen(serverSocket, MAX_CON_NUM);
		if (res == SOCKET_ERROR) {
			printf("Failed to start listen: %ld\n", GetLastError());
			continue;
		}

		// Ожидаем запрос к серверу и записываем дескриптор сокета клиента.
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientSIN, &clientSINSize);
		if (clientSocket == INVALID_SOCKET) {
			printf("Failed to accept client: %ld\n", GetLastError());
			continue;
		}
		printf("Accept connection from %s, port %d\n", inet_ntoa(clientSIN.sin_addr), htons(clientSIN.sin_port));

		// Получаем данные от клиента.
		res = recv(clientSocket, buf, BUF_SIZE, 0);

		if (res == SOCKET_ERROR) {
			// ошибка получения данных
			printf("recv failed: %ld\n", GetLastError());
			continue;
		}
		else if (res == 0) {
			// соединение закрыто клиентом
			printf("connection closed...\n");
			closesocket(clientSocket);
			continue;
		}
		else if (res > 0) {
			// Мы знаем фактический размер полученных данных,
			// поэтому ставим метку конца строки в буфере запроса.
			buf[res] = '\0';
			printf("Received string: %s\n", buf);
		}

		// Извлекаем числа из строки.
		istringstream stringStream(buf);
		stringStream >> a;
		stringStream >> b;
		sprintf_s(buf, BUF_SIZE, "%d", a + b);

		res = send(clientSocket, buf, strlen(buf), MSG_DONTROUTE);

		if (res == SOCKET_ERROR) {
			printf("send failed: %ld\n", WSAGetLastError());
		}

		closesocket(clientSocket);
	}
}

void Server::close() {
	closesocket(serverSocket);
	closesocket(clientSocket);
	WSACleanup();
	printf("Server closed.\n");
}