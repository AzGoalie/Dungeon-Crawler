#include "StateManager.h"
#include "State.h"
#include "Game.h"

StateManager::StateManager()
{
	m_pGame = nullptr;
}

StateManager::~StateManager()
{
	m_pGame = nullptr;
}

bool StateManager::Init(Game* game)
{
	if (!game)
		return false;

	m_pGame = game;
	return true;
}

void StateManager::Shutdown()
{
	//Call each states shutdow, then delete the stack
	for (int i = 0; i < m_States.size(); i++)
		m_States[i]->Shutdown();
	m_States.clear();
}

void StateManager::HandleEvents(SDL_Event &e)
{
	// Only handle the current state's input
	if (!m_States.empty())
		m_States.back()->HandleEvents(e);
}

void StateManager::Update(double dt)
{
	// Only update the current state
	if (!m_States.empty())
		m_States.back()->Update(dt);
}

void StateManager::Render(double alpha)
{
	//Go through states and render non-paused (overlays)
	for (int i = 0; i < m_States.size(); i++)
	{
		if (!m_States[i]->IsPaused())
			m_States[i]->Render(alpha);
	}
}

bool StateManager::Push(State* state, bool overlay)
{
	if (!state)
		return false;

	if (!overlay)
		if (!m_States.empty())
			m_States.back()->Pause();

	m_States.push_back(std::unique_ptr<State>(state));
	return true;
}

void StateManager::Pop()
{
	if (!m_States.empty())
	{
		m_States.pop_back();
		if (!m_States.empty())
			m_States.back()->Resume();
	}
}

State* StateManager::Peek()
{
	if (m_States.empty())
		return nullptr;

	return m_States.back().get();
}