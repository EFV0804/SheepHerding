#pragma once
#include <chrono>

class CCustomTimer
{
public:

	CCustomTimer(bool isCountDown = false, std::chrono::duration<double> interval = {}) = default;
	CCustomTimer(const CCustomTimer&) = delete;
	CCustomTimer& operator=(const CCustomTimer&) = delete;

	bool isRunning() const;
	void Start(bool isCountDown = false, int interval = 0);
	void Stop();
	void Reset();


	std::chrono::steady_clock::time_point GetCurrentTime();
	std::chrono::steady_clock::time_point GetStartTime() { return m_startTime; }
	std::chrono::steady_clock::time_point GetEndTime() { return m_endTime; }
	std::chrono::duration<double> GetElapsedTime();


private:
	bool m_isCountDown = false;
	std::chrono::duration<double> m_interval{};
	std::chrono::steady_clock::time_point m_startTime{};
	std::chrono::steady_clock::time_point m_endTime{};
	std::chrono::duration<double> m_elapsedTime{};
};

