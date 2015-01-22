#include "Game.h"

#include "StateTest.h"
#include "ShaderTest.h"
#include "TextureTest.h"
#include "ResourceLoaderTest.h"

int main(int argc, char** argv)
{
    // Create a new game and initialize it
	Game* game = new Game;
	if (!game->Init("Dungeon Crawler", 800, 600, WINDOWED))
		return 1;

    // Create the inital state, initialize it, push it to the game
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