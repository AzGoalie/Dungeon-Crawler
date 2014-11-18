#ifndef TIMER_H
#define TIMER_H

#include "GLHeaders.h"

class Timer
{
private:
	Uint64 m_StartTicks;
	Uint64 m_PausedTicks;
	Uint64 m_Frequency;

	bool m_Started;
	bool m_Paused;

public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	void Pause();
	void Unpause();
	double Reset();

	double GetTicks();

	bool IsStarted();
	bool IsPaused();
};

#endif