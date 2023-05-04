#pragma once
#include <chrono>

class CCustomTimer
{
public:

	CCustomTimer(bool isCountDown = false, std::chrono::milliseconds interval = {});
	CCustomTimer(const CCustomTimer&) = delete;
	CCustomTimer& operator=(const CCustomTimer&) = delete;

	bool isRunning() const;
	void Start();
	void Stop();
	void Reset();


	std::chrono::steady_clock::time_point GetCurrentTime();
	std::chrono::steady_clock::time_point GetStartTime() { return m_startTime; }
	std::chrono::steady_clock::time_point GetEndTime() { return m_endTime; }
	std::chrono::milliseconds GetElapsedTime();
	std::chrono::milliseconds GetRemainingTime();


private:
	bool m_isCountDown = false;
	bool m_isRunning = false;
	std::chrono::milliseconds m_refInterval{};
	std::chrono::milliseconds m_interval{};
	std::chrono::steady_clock::time_point m_startTime{};
	std::chrono::steady_clock::time_point m_endTime{};
	std::chrono::milliseconds m_elapsedTime{};
	//std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>, std::nano> m_endTime{};
};

