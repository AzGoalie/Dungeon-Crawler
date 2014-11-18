#ifndef STATE_H
#define STATE_H

#include "Game.h"
#include "GLHeaders.h"

class State
{
protected:
	Game* m_pGame;
	bool m_Paused;

public:
	State(Game* game);
	virtual ~State();

	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	virtual void HandleEvents(SDL_Event &e) = 0;
	virtual void Update(double dt) = 0;
	virtual void Render(double alpha) = 0;

	virtual void Pause();
	virtual void Resume();

	bool IsPaused() { return m_Paused; }
};

#endif