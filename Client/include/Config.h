#pragma once

#include "includes.h"

class Config
{
public:
	// Feed Parse a vector of params to exclude them from the keyMap, but return them as a map instead.
	static map<string, string> Parse(vector<string> params);

	static map<string, vector<int>> keyMap;
	static map<string, map<string, string>> sceneSounds;
	static bool selfSounds;
	static int transitionPoint;
};