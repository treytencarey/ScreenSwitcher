#pragma once

#include "includes.h"

class Client;

class Server
{
public:
	Server(int port);

	static void Connection(int threadID);

	static int recvAll(int sockfd, const void* buf, size_t len, int flags);

	static vector<Client*> clients;
};