#include "Game.h"
#include "Timer.h"

#include <iostream>

Game::Game()
{
	// Do nothing except null pointer and booleans, everything should be initialized in Init
	m_Running = false;
	m_pWindow = nullptr;
	m_Context = nullptr;
	m_Delta = DEFAULT_DETLA;
	m_GameTime = 0.0;
}

Game::~Game()
{
	// Do nothing, everything should be called in Shutdown()
}

bool Game::Init(const char* title, int width, int height, WindowType type)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)   // If not 0 then an error occured
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}
	SDL_Log("SDL initialized");

	// Create our window
	int flags = SDL_WINDOW_OPENGL;
	if (type == FULLSCREEN)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (type == BORDERLESS) 
	{
		flags |= SDL_WINDOW_BORDERLESS;
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			return false;
		}
		width = dm.w;
		height = dm.h;
	}

	m_pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (!m_pWindow)
	{
		SDL_Log("Failed to create the window: %s", SDL_GetError());
		return false;
	}
	SDL_Log("Window created", SDL_GetError());

	// Setup OpenGL settings
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create our OpenGL Context
	m_Context = SDL_GL_CreateContext(m_pWindow);
	if (!m_Context)
	{
		SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
		return false;
	}
	SDL_Log("OpenGL context created");

	// Setup GLEW
	GLenum err;
	glewExperimental = true;
	if ((err = glewInit()) != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(err));
		return false;
	}
	SDL_Log("GLEW Initialized");

	//TODO: this icon is only for testing
#ifdef _DEBUG
	SDL_Surface *icon = SDL_LoadBMP("data/icons/dev_icon.bmp");
	if (!icon)
	{
		SDL_Log("Error loading the window icon");
		return false;
	}
	SDL_SetWindowIcon(m_pWindow, icon);
#endif

	// Init State Manager
	m_StateManager.Init(this);

	SDL_Log("Initialization done");
	m_Running = true;
	return true;
}

void Game::HandleEvents()
{
	SDL_Event e;
	if (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			m_Running = false;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE)
				m_Running = false;
			break;
		default:
			break;
		}
		m_StateManager.HandleEvents(e);
	}
}

void Game::Update(double dt)
{
	m_StateManager.Update(dt);
}

void Game::Render(double alpha)
{
	glClear(GL_COLOR_BUFFER_BIT);

	m_StateManager.Render(alpha);

	SDL_GL_SwapWindow(m_pWindow);
}

void Game::Shutdown()
{
	SDL_Log("Shutting down...");

	m_StateManager.Shutdown();

	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Game::Run()
{
	m_Timer.Start();

	double currentTime = m_Timer.GetTicks();
	double accumulator = 0.0;
	bool canRender = false;

	while (m_Running)
	{
		canRender = false;

		// Find the time inbetween frames 
		double newTime = m_Timer.GetTicks();
		double frameTime = newTime - currentTime;
		if (frameTime > MAX_FRAME_TIME)
			frameTime = MAX_FRAME_TIME;
		currentTime = newTime;

		accumulator += frameTime;

		// Fixed timestep
		while (accumulator >= m_Delta)
		{
			canRender = true;
			// Update game state
			HandleEvents();
			Update(m_Delta);

			// Update times
			accumulator -= m_Delta;
			m_GameTime += m_Delta;
		}

		if (canRender)
		{
			// Find interpolation value
			const double alpha = accumulator / m_Delta;
			Render(alpha);
		}
	}
}

double Game::GetTimeInSec()
{
	return m_GameTime;
}

double Game::GetDelta()
{
	return m_Delta;
}

void Game::SetDelta(double dt)
{
	m_Delta = dt;
}

void Game::PushState(State* state, bool overlay)
{
	m_StateManager.Push(state, overlay);
}

void Game::PopState()
{
	m_StateManager.Pop();
}