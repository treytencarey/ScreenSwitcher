#include "includes.h"
#include "Operations.h"

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

	if (!selfCalled &&
		Config::keyMap.find(name) != Config::keyMap.end() && Config::keyMap[name].size() && Config::keyMap[name][0].size() && Config::keyMap[name][0][0] != -1) // Ensure that we only simulate a press if the first config for a scene is not blank
	{
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
	}

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
int sensitivity = 1; // Must trigger at least this many times (GetKeyState bug that pressing then releasing 1 then pressing ctrl would still trigger but only once?)
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
				if (key != -1 && !(GetKeyState(key) & 0x8000))
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

	long long ping = Operations::getTimeMilli() + Config::pingTime + Config::pongTime;

	while (true) {
		Sleep(10);

		if (Operations::getTimeMilli() > ping)
		{
			printf("ERROR: LOST CONNECTION. Reconnecting...\n");
			server = new Server(ip, port);
			ping = Operations::getTimeMilli() + Config::pingTime + Config::pongTime;
		}

		vector<string> messages = server->getMessages();
		for (string message : messages)
		{
			if (message == "PING")
			{
				server->sendStr("PONG");
			}
			else
				switchScene(message);
			ping = Operations::getTimeMilli() + Config::pingTime + Config::pongTime;
		}

		checkSwitchScene();
	}

	return 0;
}