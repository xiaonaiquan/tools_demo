#include "TimeInterval.h"

CTimeInterval::CTimeInterval()
{
    m_timevalMin.tv_sec = 0;
    m_timevalMin.tv_usec = 0;

    m_timevalMax = m_timevalMin;

    m_timevalCurrent = m_timevalMin;
}

CTimeInterval::CTimeInterval(const __time_t timeSecMin, const __suseconds_t timeUSecMin,
                             const __time_t timeSecMax /*= 0*/, const __suseconds_t timeUSecMax /*= 0*/)
{
    m_timevalMin.tv_sec = timeSecMin;
    m_timevalMin.tv_usec = timeUSecMin;

    m_timevalMax.tv_sec = timeSecMax;
    m_timevalMax.tv_usec = timeUSecMax;

    m_timevalCurrent = m_timevalMin;
}

CTimeInterval::~CTimeInterval()
{
}

void CTimeInterval::Reset()
{
    m_timevalCurrent = m_timevalMin;
}

CTimeInterval CTimeInterval::operator++()
{
    struct timeval timeVal;
    timeVal.tv_sec = m_timevalCurrent.tv_sec * 2 + (m_timevalCurrent.tv_usec * 2) / (1000 * 1000);
    timeVal.tv_usec = (m_timevalCurrent.tv_usec * 2) % (1000 * 1000);

    // 如果时小于或等于最大时间间隔数值的话，则设置为计算出的数值，否则将设置为最大时间间隔数值
    if (timeVal.tv_sec < m_timevalMax.tv_sec ||
        (timeVal.tv_sec == m_timevalMax.tv_sec && timeVal.tv_sec <= m_timevalMax.tv_sec))
    {
        m_timevalCurrent = timeVal;
    }
    else
    {
        m_timevalCurrent = m_timevalMax;
    }
    return *this;
}
CTimeInterval CTimeInterval::operator++(int)
{
    CTimeInterval timeInterval = *this;
    operator++();
    return timeInterval;
}

CTimeInterval::operator struct timeval()
{
    return m_timevalCurrent;
}

struct timeval CTimeInterval::GetCurrent() const
{
    return m_timevalCurrent;
}

void CTimeInterval::SetMin(const __time_t timeSecMin, const __suseconds_t timeUSecMin)
{
    m_timevalMin.tv_sec = timeSecMin;
    m_timevalMin.tv_usec = timeUSecMin;
    CTimeInterval::Reset();
}

void CTimeInterval::SetMax(const __time_t timeSecMax, const __suseconds_t timeUSecMax)
{
    m_timevalMax.tv_sec = timeSecMax;
    m_timevalMax.tv_usec = timeUSecMax;
}
