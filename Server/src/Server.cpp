#include "Server.h"
#include "Client.h"
#include "Config.h"

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
int sensitivity = 1; // Must trigger at least this many times (GetKeyState bug that pressing then releasing 1 then pressing ctrl would still trigger but only once?)
void checkSwitchScene()
{
	map<string, vector<vector<int>>>::iterator it;
	for (it = Config::keyMap.begin(); it != Config::keyMap.end(); it++)
	{
		for (vector<int> keys : it->second)
		{
			bool ok = true;
			for (int key : keys)
			{
				if (!(GetKeyState(key) & 0x8000))
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				triggers[it->first]++;
				break;
			}
			else
				triggers[it->first] = 0;
		}
		if (triggers[it->first] == sensitivity)
		{
			cout << "Changing scene: " << it->first << endl;

			if (Server::clients.size())
				Server::clients[0]->switchScenePlaySound(it->first, true);
			for (int ID = Server::clients.size() - 1; ID >= 0; ID--) {
				Client* client = Server::clients[ID];
				
				client->sendMessage(it->first);
				client->currentScene = it->first;
				client->sendMessages();
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
			Client* client = Server::clients[ID];
			
			client->getMessages();
			client->sendMessages();
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