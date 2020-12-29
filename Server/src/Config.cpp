#include "Config.h"

map<string, vector<int>> Config::keyMap;
map<string, string> Config::sceneSounds;
bool Config::selfSounds = false;

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
		if (line.find("PLAYSOUND") == 0)
		{
			line = line.substr(line.find("=") + 1, line.length());
			while (line.find("[") != std::string::npos)
			{
				int begScene = line.find("[");
				int endScene = line.find("]");
				if (begScene != std::string::npos && endScene != std::string::npos && endScene > begScene)
				{
					std::string scene = line.substr(begScene + 1, endScene - begScene - 1);
					line = line.substr(endScene + 1, line.length());
					std::string sound = "";
					int endSound = line.find("[");
					if (endSound == std::string::npos)
						sound = line;
					else
						sound = line.substr(0, endSound);
					Config::sceneSounds[scene] = sound;
				}
				else
					break;
			}

			continue;
		}
		if (line.find("SELFSOUNDS") == 0)
		{
			Config::selfSounds = stoi(line.substr(line.find("=") + 1, line.length()));
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

		keyMap[key] = keys;
	}

	return foundParams;
}