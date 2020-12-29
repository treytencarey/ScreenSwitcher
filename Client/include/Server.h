#pragma once

#include "includes.h"

class Server {
public:
	Server(string ip, int port);

	void sendStr(string str);
	vector<string> getMessages();

	int sendAll(int sockfd, const void* buf, size_t len, int flags);

private:
	bool online = false;
	int sock;
};