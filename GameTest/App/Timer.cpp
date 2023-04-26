#include "stdafx.h"
#include "Timer.h"


void CCustomTimer::Start()
{
	m_startTime = std::chrono::steady_clock::now();
}

void CCustomTimer::StartCountDown(int interval)
{
	m_startTime = std::chrono::steady_clock::now() + std::chrono::seconds(interval);
	//endtime
}

bool CCustomTimer::isOutOfTime()
{
	return m_startTime < GetCurrentTime();
}

void CCustomTimer::End()
{
}

void CCustomTimer::Update()
{

}

std::chrono::steady_clock::time_point CCustomTimer::GetCurrentTime()
{
	return std::chrono::steady_clock::now();
}

std::chrono::duration<double> CCustomTimer::GetElapsedTime()
{
	return std::chrono::duration<double>(GetCurrentTime() - m_startTime);
}
