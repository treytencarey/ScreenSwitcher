#pragma once

#include "includes.h"

class Config
{
public:
	// Feed Parse a vector of params to exclude them from the keyMap, but return them as a map instead.
	static map<string, string> Parse(vector<string> params);

	static map<string, vector<vector<int>>> keyMap;
	static map<string, map<string, string>> sceneSounds;
	static bool selfSounds;
	static int transitionPoint;
	static int pingTime; // Time until a ping is sent out
	static int pongTime; // Time until a pong is required after a ping
};