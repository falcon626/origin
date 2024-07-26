#include "Timer.h"

void Timer::Start() noexcept
{
    if (!m_isRunning)
	{
        auto now         = std::chrono::steady_clock::now();
        if (m_startTime != std::chrono::steady_clock::time_point()) m_startTime += (now - m_stopTime);
        else m_startTime = now;
        m_isRunning      = true;
    }
}

void Timer::Stop() noexcept
{
    if (m_isRunning)
	{
        m_stopTime  = std::chrono::steady_clock::now();
        m_isRunning = false;
    }
}

void Timer::Restart() noexcept
{
    Stop();
    m_startTime = std::chrono::steady_clock::now();
    m_isRunning = true;
}



void Timer::Resume() noexcept
{
    if (!m_isRunning) 
	{
        m_startTime += (std::chrono::steady_clock::now() - m_stopTime);
        m_isRunning  = true;
    }
}



void Timer::Reset() noexcept
{
	m_startTime = std::chrono::steady_clock::time_point();
	m_stopTime = std::chrono::steady_clock::time_point();
	m_isRunning = false;
}

uint32_t const Timer::ElapsedSeconds() const noexcept
{
#if TimerSwitcher
	auto endTime = m_isRunning ? std::chrono::steady_clock::now() : m_stopTime;
	return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(endTime - m_startTime).count());
#else
    ++m_frameCounter;
    if (m_frameCounter >= Fps)
    {
        m_frameCounter = NULL;
        ++m_second;
    }
    return m_second;
#endif // TimerSwitcher
}