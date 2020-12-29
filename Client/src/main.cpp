#include "includes.h"

string currentScene = "";
void switchScene(string name, bool selfCalled=false)
{
	cout << "Changing scene: " << name << endl;
	// PLAY SOUND
	if (currentScene != name && (!selfCalled || Config::selfSounds))
	{
		if (Config::sceneSounds.find("!" + currentScene) != Config::sceneSounds.end())
		{
			// cout << "PLAYING DISCONNECT SOUND: " << Config::sceneSounds["!" + currentScene] << endl;
			Sleep(3000);
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
	
	currentScene = name;
}

map<string, int> triggers;
int sensitivity = 3; // Must trigger at least this many times (GetAsyncKeyState bug that pressing then releasing 1 then pressing ctrl would still trigger but only once?)
Server* server;
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
			server->sendStr(it->first);
			switchScene(it->first, true);
		}
	}
}

int main()
{
	map<string, string> params = Config::Parse({ "IPV4", "PORT" });

	if (params.find("IPV4") == params.end() || params.find("PORT") == params.end())
		return -1;
	std::string ip = params["IPV4"];
	int port = stoi(params["PORT"]);

	server = new Server(ip, port);

	while (true) {
		Sleep(10);

		vector<string> messages = server->getMessages();
		for (string message : messages)
		{
			switchScene(message);
		}

		checkSwitchScene();
	}

	return 0;
}