#include "Game.h"
#include <iostream>
using namespace std;

int main() {

	Gamebase* game = new Game();

	//TODO: startup
	if (game->Start()) {
		game->Loop();
	}

	game->Stop();

	delete game;
	cin.get();
	return 0;
}