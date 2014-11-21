#include "Game.h"

#include "StateTest.h"
#include "ShaderTest.h"

int main(int argc, char** argv)
{
	Game* game = new Game;
	if (!game->Init("Dungeon Crawler", 800, 600, WINDOWED))
		return 1;

	//TestState* first = new TestState(game, "This is the first state");
	ShaderTest* shaderTest = new ShaderTest(game);
	shaderTest->Init();

	game->PushState(shaderTest);

	game->Run();
	game->Shutdown();

	delete game;

	return 0;
}