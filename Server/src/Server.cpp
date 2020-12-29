#include "Server.h"

vector<Client*> Server::clients;

Server::Server(int port)
{
	/// Start the socket server
	int server_skt, client_skt;

	WSADATA wsaData;
	WSAStartup(0x101, &wsaData);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;//IPv4
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(server_addr.sin_zero, 0, 8);

	if ((server_skt = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		// printf("%s", strerror(errno));
		exit(14);
	}

	if (bind(server_skt, (struct sockaddr*) & server_addr, sizeof(struct sockaddr)) == -1) {
		exit(13);
	}

	int queueSize = 1;
	if (listen(server_skt, queueSize) == -1) {
		exit(12);
	}

	printf("Waiting for a connection.\n");
	int ID = 0;
	bool running = true;
	while (running) {
		int sin_size = sizeof(struct sockaddr_in);
		socklen_t temp = sin_size;

		sockaddr clients_addr;
		if ((client_skt = accept(server_skt, &clients_addr, &temp)) != -1) {
			Client* newClient = new Client(client_skt, ID);
			if (ID == 0)
				std::thread(Server::Connection, ID).detach();
			printf("Client %d connected!\n", ID);
			Server::clients.push_back(newClient);
			ID++;
		}
	}
}

map<string, int> triggers;
int sensitivity = 3; // Must trigger at least this many times (GetAsyncKeyState bug that pressing then releasing 1 then pressing ctrl would still trigger but only once?)
void checkSwitchScene()
{
	map<string, vector<int> >::iterator it;
	for (it = Config::keyMap.begin(); it != Config::keyMap.end(); it++)
	{
		bool ok = true;
		for (int key : it->second)
		{
			if (!GetAsyncKeyState(key))
			{
				ok = false;
				triggers[it->first] = 0;
				break;
			}
		}
		if (ok)
			triggers[it->first]++;
		if (triggers[it->first] == sensitivity)
		{
			cout << "Changing scene: " << it->first << endl;

			for (Client* client : Server::clients)
			{
				client->sendMessage(it->first);
				client->sendMessages();
			}

			// Only the first client needs to trigger a sound to play
			if (Config::selfSounds && Server::clients.size() > 0)
			{
				if (Config::sceneSounds.find("!" + Server::clients[0]->currentScene) != Config::sceneSounds.end())
					Sleep(3000);
				Server::clients[0]->switchScenePlaySound(it->first);
			}

			for (Client* client : Server::clients)
			{
				client->currentScene = it->first;
			}
		}
	}
}

void Server::Connection(int threadID)
{
	bool running = true;
	while (running)
	{
		Sleep(10);
		checkSwitchScene();
		for (int ID = Server::clients.size() - 1; ID >= 0; ID--) {
			Server::clients[ID]->getMessages();
			Server::clients[ID]->sendMessages();
		}
	}
}

int Server::recvAll(int sockfd, const void* buf, size_t len, int flags) {
	u_long iMode = 1;
	ioctlsocket(sockfd, FIONBIO, &iMode);

	int result;
	int recvd = 0;
	char* pbuf = (char*)buf;
	while (len > 0) {
		result = recv(sockfd, pbuf, len, flags);

		if (result <= 0) break;

		recvd += result;
		pbuf += result;
		len -= result;
	}

	return recvd;
}