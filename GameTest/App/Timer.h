#pragma once
#include <chrono>

class CCustomTimer
{
public:

	CCustomTimer(bool isCountDown = false, std::chrono::duration<double> interval = {});
	CCustomTimer(const CCustomTimer&) = delete;
	CCustomTimer& operator=(const CCustomTimer&) = delete;

	bool isRunning() const;
	void Start();
	void Stop();
	void Reset();


	std::chrono::steady_clock::time_point GetCurrentTime();
	std::chrono::steady_clock::time_point GetStartTime() { return m_startTime; }
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> GetEndTime() { return m_endTime; }
	std::chrono::duration<double> GetElapsedTime();


private:
	bool m_isCountDown = false;
	std::chrono::duration<double> m_interval{};
	std::chrono::steady_clock::time_point m_startTime{};
	std::chrono::time_point<std::chrono::steady_clock,std::chrono::duration<double>> m_endTime{};
	std::chrono::duration<double> m_elapsedTime{};
};

