#include "includes.h"

void playSound(string sound)
{
	printf("PLAYING SOUND: %s\n", sound.c_str());
	mciSendString(("play " + sound).c_str(), NULL, 0, NULL);
}
string currentScene = "";
void switchScene(string name, bool selfCalled=false)
{
	if (Config::selfSounds == false && selfCalled) {
		currentScene = name;
		return;
	}

	cout << "Changing scene: " << name << endl;
	// PLAY SOUND
	if (currentScene != name && (!selfCalled || Config::selfSounds))
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
	
	currentScene = name;
}

map<string, int> triggers;
int sensitivity = 3; // Must trigger at least this many times (GetAsyncKeyState bug that pressing then releasing 1 then pressing ctrl would still trigger but only once?)
Server* server;
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
				if (!GetAsyncKeyState(key))
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