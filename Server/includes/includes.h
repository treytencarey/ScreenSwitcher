#pragma once

#define _WIN32_WINNT 0x0601
#define WINVER 0x0601

#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <vector>
#include <sys/types.h>
#include <thread>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

#include "Server.h"
#include "Client.h"
#include "Config.h"