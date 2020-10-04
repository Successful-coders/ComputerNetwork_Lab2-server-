#pragma comment (lib, "Ws2_32.lib")
#include "winsock2.h"

using namespace std;

class Server {
public:
	Server(unsigned short port);
	~Server();

	void start();
	void run();
	void close();

private:
	// Порт, на котором будет расположен сервер.
	unsigned short port;

	// Максимальное количество одновременных TCP соединений.
	unsigned short const MAX_CON_NUM = 10;

	// Служебная структура для хранения информации
	// о реализации Windows socket.
	WSADATA wsaData;

	SOCKET serverSocket;

	// Структура, хранящая информацию об адресе сокета сервера.
	// Привязана к адресам в интернете (первый аргумент: AF_INET).
	SOCKADDR_IN serverSIN;

	SOCKET clientSocket;

	// Структура, хранящая информацию об адресе сокета клиента.
	// Так же привязана к адресам в интернете.
	SOCKADDR_IN clientSIN;

	unsigned int const BUF_SIZE = 256;

	// Буфер для взаимодействия с клиентами.
	char* buf;
};