#include "Timer.h"

Timer::Timer() 
{
	m_StartTicks = 0;
	m_PausedTicks = 0;
	m_Frequency = SDL_GetPerformanceFrequency();

	m_Started = false;
	m_Paused = false;
}

Timer::~Timer()
{}

void Timer::Start()
{
	m_Started = true;
	m_Paused = false;

	m_StartTicks = SDL_GetPerformanceCounter();
	m_PausedTicks = 0;
}

void Timer::Stop()
{
	m_Started = false;
	m_Paused = false;

	m_StartTicks = 0;
	m_PausedTicks = 0;
}

void Timer::Pause()
{
	if (m_Started && !m_Paused)
	{
		m_Paused = true;

		m_PausedTicks = SDL_GetPerformanceCounter() - m_StartTicks;
		m_StartTicks = 0;
	}
}

void Timer::Unpause()
{
	if (m_Started && m_Paused)
	{
		m_Paused = false;

		m_StartTicks = SDL_GetPerformanceCounter() - m_PausedTicks;
		m_PausedTicks = 0;
	}
}

double Timer::GetTicks()
{
	Uint64 time = 0;

	if (m_Started)
	{
		if (m_Paused)
			time = m_PausedTicks;
		else
			time = SDL_GetPerformanceCounter() - m_StartTicks;
	}

	return time / (double)m_Frequency;
}

double Timer::Reset()
{
	double time = GetTicks();
	
	Stop();
	Start();

	return time;
}

bool Timer::IsStarted()
{
	return m_Started;
}

bool Timer::IsPaused()
{
	return m_Paused && m_Started;
}