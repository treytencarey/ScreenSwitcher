#include "Server.h"

Server::Server(string ip, int port)
{
	sockaddr_in ser;
	sockaddr addr;

	WSADATA WSdata;
	if (WSAStartup(MAKEWORD(1, 1), &WSdata) != 0) {
		exit(255);
	}

	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr = inet_addr(ip.c_str());

	memcpy(&addr, &ser, 16);

	int res = 0;
	res = WSAStartup(MAKEWORD(1, 1), &WSdata);      //Start Winsock

	if (res != 0)
		exit(0);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);       //Create the socket

	fd_set fd;

	FD_ZERO(&fd);
	FD_SET(sock, &fd);

	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	res = connect(sock, &addr, sizeof(addr));

	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	/* Wait for write bit to be set */
	if (select(FD_SETSIZE, 0, &fd, 0, &timeout) <= 0)
	{
		this->online = true;
		cout << "Connection Failed. Try again." << endl;
		system("pause");
		exit(0);
	}
	cout << "Connection success!" << endl;
}

vector<string> Server::getMessages()
{
	u_long iMode = 1;
	ioctlsocket(this->sock, FIONBIO, &iMode);

	char* msg = new char[255];

	vector<string> messages;

	int recvd;
	while ((recvd = recv(this->sock, msg, 255, 0)) == 255)
	{
		messages.push_back(msg);
	}

	delete msg;
	return messages;
}

void Server::sendStr(string str)
{
	u_long iMode = 1;
	ioctlsocket(this->sock, FIONBIO, &iMode);

	str += "\0";
	char msg[255];
	strcpy(msg, str.c_str());

	int ao = send(this->sock, msg, 255, 0);
}

int Server::sendAll(int sockfd, const void* buf, size_t len, int flags) {
	if (!Server::online)
		return len;
	int result;
	char* pbuf = (char*)buf;
	while (len > 0) {
		result = send(sockfd, pbuf, len, flags);
		if (result <= 0) break;
		pbuf += result;
		len -= result;
	}
	return result;
}