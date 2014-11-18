#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <memory>
#include <vector>
#include "GLHeaders.h"

class Game;
class State;

class StateManager
{
private:
	// Vector of all states, states are rendered when not paused.
	// Last state in vector is current state (like a stack)
	std::vector< std::unique_ptr<State> > m_States; 
	Game* m_pGame;

public:

	StateManager();
	~StateManager();

	bool Init(Game* game);
	void Shutdown();

	void HandleEvents(SDL_Event &e);
	void Update(double dt);
	void Render(double alpha);

	bool Push(State* state, bool overlay = false);
	void Pop();
	State* Peek();
};

#endif