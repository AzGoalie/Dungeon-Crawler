#ifndef GAME_H
#define GAME_H

#include "GLHeaders.h"
#include "Timer.h"
#include "StateManager.h"

enum WindowType {
	WINDOWED,
	FULLSCREEN,
	BORDERLESS
};

class Game
{
private:
	SDL_Window* m_pWindow;
	SDL_GLContext m_Context;
	
	StateManager m_StateManager;

	// Screen size
	int m_Width;
	int m_Height;

	// Fixed timestep variables
	Timer m_Timer;
	double m_Delta;		// Time in seconds for each update (tick)
	double m_GameTime;	// Current time in seconds
	const double DEFAULT_DETLA = 0.01;
	const double MAX_FRAME_TIME = 0.25;	// Maximum update time

	bool m_Running;

	void HandleEvents();
	void Update(double dt);
	void Render(double alpha);

public:
	Game();
	~Game();

	bool Init(const char* title, int width, int height, WindowType type = WINDOWED);
	void Run();
	void Shutdown();

	void PushState(State* state, bool overlay = false);
	void PopState();

	double GetTimeInSec();
	double GetDelta();
	void SetDelta(double dt);

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
};

#endif