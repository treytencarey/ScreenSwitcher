#include "Config.h"

map<string, vector<vector<int>>> Config::keyMap;
map<string, map<string, string>> Config::sceneSounds; // EXAMPLE: if the config has PLAYSOUND=[Scene1]sound1[Scene1->Scene2]sound2, then this is ["all"]["Scene1"]=sound and ["Scene1"]["Scene2"]=sound2.
bool Config::selfSounds = false;
int Config::transitionPoint = 0;

map<string, string> Config::Parse(vector<string> params)
{
	map<string, string> foundParams;

	// Parse Config.ini file
	string line;
	ifstream infile("config.ini");
	while (std::getline(infile, line))
	{
		if (line.find("//") == 0 || line.find("=") == std::string::npos)
			continue;
		else if (line.find("PLAYSOUND") == 0)
		{
			line = line.substr(line.find("=") + 1, line.length());
			if (line.find("[") == std::string::npos)
				Config::sceneSounds["all"]["all"] = line;
			while (line.find("[") != std::string::npos)
			{
				int begScene = line.find("[");
				int endScene = line.find("]");
				if (begScene != std::string::npos && endScene != std::string::npos && endScene > begScene)
				{
					string toScene = line.substr(begScene + 1, endScene - begScene - 1);
					line = line.substr(endScene + 1, line.length());
					string sound = "";
					int endSound = line.find("[");
					if (endSound == std::string::npos)
						sound = line;
					else
						sound = line.substr(0, endSound);

					string fromScene = "all";
					if (toScene.find("->") != std::string::npos)
					{
						fromScene = toScene.substr(0, toScene.find("->"));
						toScene = toScene.substr(toScene.find("->") + 2, toScene.length());
					}

					Config::sceneSounds[fromScene][toScene] = sound;
				}
				else
					break;
			}

			continue;
		}
		else if (line.find("SELFSOUNDS") == 0)
		{
			Config::selfSounds = stoi(line.substr(line.find("=") + 1, line.length()));
			continue;
		}
		else if (line.find("TRANSITIONPOINT") == 0)
		{
			Config::transitionPoint = stoi(line.substr(line.find("=") + 1, line.length()));
			continue;
		}
		bool continuing = false;
		for (string param : params)
		{
			if (line.find(param) == 0)
			{
				foundParams[param] = line.substr(line.find("=") + 1, line.length());

				continuing = true;
				break;
			}
		}
		if (continuing)
			continue;

		string key = line.substr(0, line.find("="));
		string keysStr = line.substr(line.find("=") + 1, line.length());

		vector<int> keys;
		while (keysStr.find("+") != std::string::npos)
		{
			string keyToHex = keysStr.substr(0, keysStr.find("+"));
			int hexKey;
			stringstream strstr;
			strstr << keyToHex;
			strstr >> hex >> hexKey;
			keys.push_back(hexKey);

			keysStr = keysStr.substr(keysStr.find("+") + 1, keysStr.length());
		}
		string keyToHex = keysStr;
		int hexKey;
		stringstream strstr;
		strstr << keyToHex;
		strstr >> hex >> hexKey;
		keys.push_back(hexKey);

		keyMap[key].push_back(keys);
	}

	return foundParams;
}