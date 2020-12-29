#include "includes.h"

int main()
{
	Config::Parse({});
	Server* server = new Server(2322);

	return 0;
}