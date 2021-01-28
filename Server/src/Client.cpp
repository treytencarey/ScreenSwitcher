#include "Client.h"

Client::Client(int sock, int ID)
{
		this->sock = sock;
		this->ID = ID;
}

void playSound(string sound)
{
	printf("PLAYING SOUND: %s\n", sound.c_str());
	mciSendString(("play " + sound).c_str(), NULL, 0, NULL);
}
void Client::switchScenePlaySound(string name, bool selfCalled/*=false*/)
{
	if (Config::selfSounds == false && selfCalled)
		return;

	// PLAY SOUND
	if (currentScene != name)
	{
		Sleep(Config::transitionPoint);
		// If config has fromScene->toScene
		if (Config::sceneSounds.find(currentScene) != Config::sceneSounds.end())
		{
			if (Config::sceneSounds[currentScene].find(name) != Config::sceneSounds[currentScene].end())
			{
				playSound(Config::sceneSounds[currentScene][name]);
			}
			else if (Config::sceneSounds[currentScene].find("all") != Config::sceneSounds[currentScene].end())
			{
				playSound(Config::sceneSounds[currentScene]["all"]);
			}
		}
		// If config has all->toScene or just toScene
		else if (Config::sceneSounds.find("all") != Config::sceneSounds.end())
		{
			if (Config::sceneSounds["all"].find(name) != Config::sceneSounds["all"].end())
			{
				playSound(Config::sceneSounds["all"][name]);
			}
			else if (Config::sceneSounds["all"].find("all") != Config::sceneSounds["all"].end())
			{
				playSound(Config::sceneSounds["all"]["all"]);
			}
		}
	}
}

void Client::switchScene(string name)
{
	if (Config::keyMap.find(name) == Config::keyMap.end()) {
		cout << "NO EXISTING SCENE: " << name << endl;
		return;
	}
	cout << "Changing scene: " << name << endl;

	// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wScan = 0;
	input.ki.dwFlags = 0;

	for (int key : Config::keyMap[name][0])
	{
		input.ki.wVk = key;
		SendInput(1, &input, sizeof(INPUT));
	}

	Sleep(150);

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	for (int i = Config::keyMap[name][0].size() - 1; i >= 0; i--)
	{
		int key = Config::keyMap[name][0][i];

		input.ki.wVk = key;
		SendInput(1, &input, sizeof(INPUT));
	}

	this->switchScenePlaySound(name);

	this->currentScene = name;

	// Alt Down
	/*input.ki.wVk = hex1;
	SendInput(1, &input, sizeof(INPUT));
	// 2 Down
	input.ki.wVk = hex2;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(100);
	// 2 Up
	input.ki.dwFlags = hex2;
	SendInput(1, &input, sizeof(INPUT));
	// Alt Up
	input.ki.wVk = hex1;
	SendInput(1, &input, sizeof(INPUT));*/
}

void Client::getMessages()
{
	u_long iMode = 1;
	ioctlsocket(this->sock, FIONBIO, &iMode);

	char* msg = new char[255];

	int recvd = recv(this->sock, msg, 255, 0); //Server::recvAll(this->sock, &msg, 255, 0);
	if (recvd > 0)
	{
		switchScene(msg);
		for (Client* client : Server::clients)
		{
			if (client != this)
				client->sendMessage(msg);
		}
	}

	delete msg;
}

void Client::sendMessages()
{
	u_long iMode = 1;
	ioctlsocket(this->sock, FIONBIO, &iMode);

	for (string message : this->messages)
	{
		char* msg = new char[255];

		memcpy(msg, message.data(), 255);
		int sent = send(this->sock, msg, 255, 0);

		delete msg;
	}
	this->messages.clear();
}

void Client::sendMessage(string msg)
{
	this->messages.push_back(msg);
}