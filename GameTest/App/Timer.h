#pragma once
#include <chrono>

class CCustomTimer
{
public:

	CCustomTimer() = default;
	CCustomTimer(const CCustomTimer&) = delete;
	CCustomTimer& operator=(const CCustomTimer&) = delete;

	void Start();
	void StartCountDown(int interval);
	bool isOutOfTime();
	void End();
	void Update();
	std::chrono::steady_clock::time_point GetCurrentTime();
	std::chrono::steady_clock::time_point GetStartTime() { return m_startTime; }
	std::chrono::steady_clock::time_point GetEndTime() { return m_endTime; }
	std::chrono::duration<double> GetElapsedTime();

private:

	std::chrono::steady_clock::time_point m_startTime;
	std::chrono::steady_clock::time_point m_endTime;

};

