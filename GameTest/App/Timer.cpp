#include "stdafx.h"
#include "Timer.h"

CCustomTimer::CCustomTimer(bool isCountDown = false, std::chrono::duration<double> interval = {}) {
	//TODO INITIALISE m_interval && isCountDown
}
bool CCustomTimer::isRunning() const
{
	return m_startTime != std::chrono::steady_clock::time_point{};
}

void CCustomTimer::Start(bool isCountDown, int interval)
{
	if (!isRunning() && !isCountDown) {
		m_startTime = std::chrono::steady_clock::now();
	}
	else {
		m_startTime = std::chrono::steady_clock::now();
		m_endTime = std::chrono::steady_clock::now() + m_interval;
		//m_isCountDown = true;
		//m_interval = std::chrono::seconds(interval);
	}
}

void CCustomTimer::Stop()
{
	if (isRunning() && !m_isCountDown) {
		m_elapsedTime = std::chrono::steady_clock::now() - m_startTime;
		m_startTime = {};
	}
	else {
		m_elapsedTime = m_endTime - std::chrono::steady_clock::now();
		m_interval = m_interval - m_elapsedTime;
		m_endTime = {};
	}
}

void CCustomTimer::Reset()
{
	m_startTime = {};
	m_endTime = {};
	m_elapsedTime = { };
}

std::chrono::steady_clock::time_point CCustomTimer::GetCurrentTime()
{
	return std::chrono::steady_clock::now();
}

std::chrono::duration<double> CCustomTimer::GetElapsedTime()
{
	std::chrono::duration<double> temp = m_elapsedTime;
	if (!m_isCountDown) {
	
		if (isRunning()) {
			temp += std::chrono::duration<double>(GetCurrentTime() - m_startTime);
		}
	}
	else {
		temp += std::chrono::duration<double>(m_endTime - GetCurrentTime());
	}
	return temp;
}
