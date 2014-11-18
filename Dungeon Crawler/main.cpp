#include "Game.h"
#include "TestState.h"

int main(int argc, char** argv)
{
	Game* game = new Game;
	TestState* first = new TestState(game, "This is the first state");
	
	game->PushState(first);

	if (!game->Init("Dungeon Crawler", 800, 600, WINDOWED))
		return 1;

	game->Run();
	game->Shutdown();

	delete game;

	return 0;
}