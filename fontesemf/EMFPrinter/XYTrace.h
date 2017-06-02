#ifndef XY_TRACE_H
#define XY_TRACE_H

#ifndef VLR_TRACE_LEVEL
#define VLR_TRACE_LEVEL "PrintAgentTrace"
#endif

#ifndef VLR_TRACE_LEVEL_PS
#define VLR_TRACE_LEVEL_PS "PrintServerTrace"
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

enum TraceLevel
{
	TraceNone = 0, // no trace
	TraceError = 10, // only trace error
	TraceInfo = 20, // some extra info
	TraceDebug = 30, // debugging info
	TraceDetail = 40 // detailed debugging info
};

extern void SetTraceFilePrefix(LPCTSTR strFilePrefix);
extern void SetTraceLevel(const int nLevel);
extern void WriteTrace( char* strFormat, ...);
extern void ErrMessage( DWORD nErr , char _msg[] );
extern int StartTrace();
extern void CloseTrace();
extern char errorMessage[ MAX_PATH ];



#endif // XY_TRACE_H
