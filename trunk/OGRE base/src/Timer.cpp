// Timer.cpp: implementation of the CTimer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <windows.h>					// standard Windows app include
#include <mmsystem.h>
#include "Timer.h"

#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimer::CTimer()
{

_timeNow=GetTickCount() * 0.001f;
_timeLastAsk=_timeNow;
_mostRecentDt=0.0f;

}

CTimer::~CTimer()
{

}

void
CTimer::Destroy()
{
if(_timer)
	{
	delete _timer;
	_timer=0;
	}
}

CTimer *
CTimer::_timer=0;

CTimer *
CTimer::Instance()
{
if (0==_timer)
{
	_timer=new CTimer();
	atexit(Destroy);
}
return _timer;
}

double
CTimer::Getdt()
{
double dt;
_timeNow=GetTickCount() * 0.001f;
//dt is time now minus time of last request
dt=_timeNow-_timeLastAsk;
_mostRecentDt= dt;
//update timelastask for next cycle
_timeLastAsk=_timeNow;
return dt;
}

double
CTimer::GetTime()
{
double timenow=GetTickCount() * 0.001f;

return timenow;
}

double
CTimer::GetLastdt()
{
	return _mostRecentDt;
}