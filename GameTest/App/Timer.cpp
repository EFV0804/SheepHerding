#include "stdafx.h"
#include "Timer.h"

CCustomTimer::CCustomTimer(bool isCountDown, std::chrono::milliseconds interval) :
	m_isCountDown(isCountDown),
	m_refInterval(interval),
	m_interval(interval)
{

};

bool CCustomTimer::isRunning() const
{
	return m_isRunning;
}

void CCustomTimer::Start()
{
	if (!isRunning() && !m_isCountDown) {
		m_startTime = GetCurrentTime();
	}
	else {
		m_startTime = GetCurrentTime();
		m_endTime = GetCurrentTime() + m_interval;
		//m_isCountDown = true;
		//m_interval = std::chrono::milliseconds(interval);
	}
	m_isRunning = true;
}

void CCustomTimer::Stop()
{
	if (isRunning() && !m_isCountDown) {
		
		m_elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_startTime);
		m_startTime = {};
	}
	else {
		auto now = GetCurrentTime();
		m_elapsedTime = m_refInterval - GetRemainingTime();
		m_interval = GetRemainingTime();
		m_endTime = {};
	}
	m_isRunning = false;
}

void CCustomTimer::Reset()
{
	m_startTime = {};
	m_endTime = {};
	m_elapsedTime = {};
	m_interval = m_refInterval;
	m_isRunning = true;
}

std::chrono::steady_clock::time_point CCustomTimer::GetCurrentTime()
{
	return std::chrono::steady_clock::now();
}

std::chrono::milliseconds CCustomTimer::GetElapsedTime()
{
	std::chrono::milliseconds temp = m_elapsedTime;
	if (!m_isCountDown) {
	
		temp += std::chrono::duration_cast<std::chrono::milliseconds>(GetCurrentTime() - m_startTime);
	}
	else {
		temp += m_refInterval - GetRemainingTime();
	}
	return temp;
}

std::chrono::milliseconds CCustomTimer::GetRemainingTime() {

	if (isRunning()) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - GetCurrentTime());
	}
	else {
		return m_interval;
	}
}
