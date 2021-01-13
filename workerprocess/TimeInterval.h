#pragma once

#include <stdlib.h>

class CTimeInterval
{
  public:
    CTimeInterval();
    CTimeInterval(const __time_t timeSecMin, const __suseconds_t timeUSecMin, const __time_t timeSecMax = 0,
                  const __suseconds_t timeUSecMax = 0);
    ~CTimeInterval();

    CTimeInterval operator++();
    CTimeInterval operator++(int);

    operator struct timeval();

    void Reset();

    struct timeval GetCurrent() const;

    void SetMin(const __time_t timeSecMin, const __suseconds_t timeUSecMin);
    void SetMax(const __time_t timeSecMax, const __suseconds_t timeUSecMax);

  private:
    struct timeval m_timevalMin;
    struct timeval m_timevalMax;

    struct timeval m_timevalCurrent;
};
