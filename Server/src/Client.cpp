#include "Client.h"

Client::Client(int sock, int ID)
{
		this->sock = sock;
		this->ID = ID;
}

void Client::switchScenePlaySound(string name)
{
	// PLAY SOUND
	if (currentScene != name)
	{
		if (Config::sceneSounds.find("!" + currentScene) != Config::sceneSounds.end())
		{
			// cout << "PLAYING DISCONNECT SOUND: " << Config::sceneSounds["!" + currentScene] << endl;
			mciSendString(("play " + Config::sceneSounds["!" + currentScene]).c_str(), NULL, 0, NULL);
		}
		else if (Config::sceneSounds.find(name) != Config::sceneSounds.end())
		{
			// cout << "PLAYING CONNECT SOUND: " << Config::sceneSounds[name] << endl;
			mciSendString(("play " + Config::sceneSounds[name]).c_str(), NULL, 0, NULL);
		}
		else if (Config::sceneSounds.find("all") != Config::sceneSounds.end())
		{
			// cout << "PLAYING ALL SOUND: " << Config::sceneSounds["all"] << endl;
			mciSendString(("play " + Config::sceneSounds["all"]).c_str(), NULL, 0, NULL);
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

	if (Config::sceneSounds.find("!" + currentScene) == Config::sceneSounds.end()) {
		this->switchScenePlaySound(name);
		Sleep(2000);
	}

	// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wScan = 0;
	input.ki.dwFlags = 0;

	for (int key : Config::keyMap[name])
	{
		input.ki.wVk = key;
		SendInput(1, &input, sizeof(INPUT));
	}

	Sleep(150);

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	for (int i = Config::keyMap[name].size() - 1; i >= 0; i--)
	{
		int key = Config::keyMap[name][i];

		input.ki.wVk = key;
		SendInput(1, &input, sizeof(INPUT));
	}

	if (Config::sceneSounds.find("!" + currentScene) != Config::sceneSounds.end()) {
		Sleep(3000);
		this->switchScenePlaySound(name);
	}

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