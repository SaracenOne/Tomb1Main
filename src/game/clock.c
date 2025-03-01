#include "game/clock.h"

#include "specific/s_clock.h"

#include <stdio.h>
#include <time.h>

bool Clock_Init()
{
    return S_Clock_Init();
}

int32_t Clock_GetMS()
{
    return S_Clock_GetMS();
}

int32_t Clock_Sync()
{
    return S_Clock_Sync();
}

int32_t Clock_SyncTicks(int32_t target)
{
    return S_Clock_SyncTicks(target);
}

void Clock_GetDateTime(char *date_time)
{
    time_t lt = time(0);
    struct tm *tptr = localtime(&lt);

    sprintf(
        date_time, "%04d%02d%02d_%02d%02d%02d", tptr->tm_year + 1900,
        tptr->tm_mon + 1, tptr->tm_mday, tptr->tm_hour, tptr->tm_min,
        tptr->tm_sec);
}
