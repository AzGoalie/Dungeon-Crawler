#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "State.h"

// Simple test for changing states

// hiting any key will print the current state
// clicking the mouse will change the state
class TestState : public State
{
private:
	char* str;

public:
	TestState(Game* game, char* str);
	~TestState();

	bool Init();
	void Shutdown();

	void HandleEvents(SDL_Event &e);
	void Update(double dt);
	void Render(double alpha);
};

#endif