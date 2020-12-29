#pragma once

#include "includes.h"

class Client
{
public:
	Client(int sock, int ID);

	void getMessages();
	void sendMessages();
	void sendMessage(string msg);

	void switchScene(string name);
	void switchScenePlaySound(string name);

	string currentScene = "";
private:
	int sock, ID;
	vector<string> messages;
};