#include "State.h"

State::State(Game* game) : m_pGame(game), m_Paused(false)
{}

State::~State()
{
	m_pGame = nullptr;
	m_Paused = false;
}

void State::Pause()
{
	m_Paused = true;
}

void State::Resume()
{
	m_Paused = false;
}