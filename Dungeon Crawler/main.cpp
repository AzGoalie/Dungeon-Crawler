#include "Game.h"

#include "StateTest.h"
#include "ShaderTest.h"
#include "TextureTest.h"
#include "ResourceLoaderTest.h"

int main(int argc, char** argv)
{
	Game* game = new Game;
	if (!game->Init("Dungeon Crawler", 800, 600, WINDOWED))
		return 1;

	State* state;
	//state = new TestState(game, "This is the first state");
	//state = new ShaderTest(game);
	//state = new TextureTest(game);
    state = new ResourceLoaderTest(game);
    
	state->Init();
	game->PushState(state);

	game->Run();
	game->Shutdown();

	delete game;

	return 0;
}