#include "includes.h"
#include "Config.h"
#include "Server.h"

int main()
{
	Config::Parse({});
	Server* server = new Server(2322);

	return 0;
}