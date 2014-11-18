#include "TestState.h"
#include "GLHeaders.h"
#include "iostream"

TestState::TestState(Game* game, char* str) : State(game), str(str)
{
}

TestState::~TestState()
{}

bool TestState::Init()
{
	return true;
}

void TestState::Shutdown()
{}

void TestState::HandleEvents(SDL_Event &e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		std::cout << str << std::endl;
		break;
	case SDL_MOUSEBUTTONUP:
	{
		if (strcmp(str, "This is the second state") == 0)
			m_pGame->PopState();
		else
		{
			TestState* second = new TestState(m_pGame, "This is the second state");
			m_pGame->PushState(second);
		}
		break;
	}
	default:
		break;
	}
}

void TestState::Update(double dt)
{

}

void TestState::Render(double alpha)
{

}