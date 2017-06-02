
#include "XYTrace.h"

// private helper class
class XYTraceHelper
{
	
	// friend functions of this class
	friend void SetTraceFilePrefix(char * strFilePrefix);
	friend void SetTraceLevel(const int nLevel);
	friend void WriteTrace( char* strFormat, ...);
	
	// internal data members
	//HANDLE m_hFile;
	FILE *m_hFile;
	int m_nLevel;
	long m_nThreadId;
	char* m_pTraceFilePrefix;
	SYSTEMTIME m_timeStart;
	CRITICAL_SECTION cs;
	
	// close the current trace file
	void CloseTraceFile()
	{
		if( m_hFile ) 
		{
			fclose( m_hFile  );
			//::CloseHandle(m_hFile);			
			m_hFile = NULL;
		}
	}
	
	// open a new trace file
	FILE *OpenTraceFile()
	{
		// construct the new trace file path
		char strFilePath[1001] = {0};
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		sprintf
		(
			strFilePath,
			("%s_%d%d%d.txt"),m_pTraceFilePrefix?m_pTraceFilePrefix:("Trace"),
			sysTime.wYear, sysTime.wMonth, sysTime.wDay 

			/*
			_T("%s_%04d%02d%02d_%02d%02d%02d_%X.txt"),
			m_pTraceFilePrefix?m_pTraceFilePrefix:_T("Trace"),
			sysTime.wYear,
			sysTime.wMonth,
			sysTime.wDay,
			sysTime.wHour,	
			sysTime.wMinute,
			sysTime.wSecond,
			::GetCurrentProcessId()
			*/
		);
		
		// create the new trace file
		m_hFile = fopen( (char*)strFilePath , "a+t");
/*		m_hFile = CreateFile
		(
			strFilePath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
*/		// if successful, save the start time variable
		if(m_hFile) 
			m_timeStart = sysTime;
		// return the file handle
		return m_hFile;
	}
	// set lock to gain exclusive access to trace
	// functions
	void Lock()
	{
	
		EnterCriticalSection( &cs ) ;
	/*
		long nThreadId = ::GetCurrentThreadId();
		while(m_nThreadId!=nThreadId)
		{
			// keep trying until successfully completed the operation
			::InterlockedCompareExchange((void**)&m_nThreadId, (void*)nThreadId, 0);
			if(m_nThreadId==nThreadId) break;
			::Sleep(25);
		}
		*/
	}
	// release lock so that other threads can access 
	// trace functions
	void Unlock()
	{
		LeaveCriticalSection(&cs ) ;
		// only the thread that set the lock can release it
		//::InterlockedCompareExchange((void**)&m_nThreadId, 0, (void*)::GetCurrentThreadId());
	}
	// set the current trace level
	void SetTraceLevel(const int nLevel) { m_nLevel = nLevel>0?nLevel:0; }
	// set the trace file name prefix
	void SetTraceFilePrefix(char *strFilePrefix)
	{
		// close existing trace file first
		CloseTraceFile();
		// copy the file name prefix
		size_t nSize = 0; 
		if( strFilePrefix ) 
			nSize = strlen ( strFilePrefix);
		
		delete []m_pTraceFilePrefix;
		m_pTraceFilePrefix = new char[nSize+1];
		strcpy(m_pTraceFilePrefix, nSize>0?strFilePrefix:(""));
	}
public:
	// constructor and destructor
	XYTraceHelper()
	{
		m_hFile = NULL;
		m_nLevel = 0;
		m_nThreadId = 0;
		m_pTraceFilePrefix = NULL;
		InitializeCriticalSection( &cs );
	}
	virtual ~XYTraceHelper()
	{
		CloseTraceFile();		
		delete []m_pTraceFilePrefix;
		DeleteCriticalSection( &cs );
	}
};

// the one and only instance of XYTraceHelper
XYTraceHelper * theHelper;

int StartTrace ()
{
	theHelper = new XYTraceHelper();


	// Procura no registro a chave WorkFolder, setar o caminho do arquivo de trace
	HKEY reg;
	char tracePrefix[MAX_PATH + 1] = "";
	DWORD dw = MAX_PATH;

	LONG regReturn = RegOpenKeyExA( HKEY_LOCAL_MACHINE, "SOFTWARE\\NDDigital\\DPS\\PrintAgent", 0, KEY_READ, &reg );

	if( regReturn == ERROR_SUCCESS )
	{
		if( RegQueryValueExA( reg, "WorkFolder", NULL, NULL, (BYTE*)tracePrefix, &dw) == ERROR_SUCCESS )
			strcat( tracePrefix, "\\NDDigital\\DPS" );
		else
		{
			char bufferdir[ MAX_PATH ] =""; 
			GetWindowsDirectoryA( bufferdir , MAX_PATH -1 ); 
			strcpy( tracePrefix, bufferdir ); 
			strcat( tracePrefix , "\\TEMP\\NDDigital\\DPS" ); 		
		}

		RegCloseKey ( reg );
	}
	else
	{
		if (RegOpenKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\NDDigital\\DPS\\PrintServer", &reg) == ERROR_SUCCESS)
		{
			if( RegQueryValueExA( reg, "WorkFolder", NULL, NULL, (BYTE*)tracePrefix, &dw) == ERROR_SUCCESS )
				strcat( tracePrefix, "\\NDDigital\\DPS" );
			else
			{
				char bufferdir[ MAX_PATH ] =""; 
				GetWindowsDirectoryA( bufferdir , MAX_PATH -1 ); 
				strcpy( tracePrefix, bufferdir ); 
				strcat( tracePrefix , "\\TEMP\\NDDigital\\DPS" ); 		
			}

			RegCloseKey ( reg );

			
		}	
	}

/*	
	GetSystemDirectory ( tracePrefix, sizeof(tracePrefix) );
	
	if ( tracePrefix[strlen(tracePrefix) - 1] != '\\' )
		strcat ( tracePrefix, "\\TPar\\TRACE" );
	else
		strcat ( tracePrefix, "TPar\\TRACE" );


	char	fileName[ MAX_PATH ] ="",
			content[ MAX_PATH *2 ],
			*p; 
	FILE* fp; 
*/	

		
	DWORD folderAttrib = GetFileAttributesA((char*)tracePrefix);
	if( (folderAttrib == INVALID_FILE_ATTRIBUTES) || (!(folderAttrib & FILE_ATTRIBUTE_DIRECTORY)) )
		CreateDirectoryA ( tracePrefix, NULL);

	// Prefixo do nome do arquivo de trace
	strcat( tracePrefix, "\\DPS_Trace");
	
	SetTraceFilePrefix( (char*)tracePrefix );


	// Procura no registro a chave TRACELEVEL para setar o nível do trace
	int traceLevel = 0;
	char valLevel[10] = "";
	
	dw = 10;

	regReturn = RegOpenKeyExA( HKEY_LOCAL_MACHINE, "SOFTWARE\\NDDigital\\DPS\\PrintAgent", 0, KEY_READ, &reg );

	if( regReturn == ERROR_SUCCESS )
	{
		if( RegQueryValueExA( reg, VLR_TRACE_LEVEL, NULL, NULL, (BYTE*)valLevel, &dw) == ERROR_SUCCESS )
			traceLevel = atoi( valLevel );

		RegCloseKey ( reg );
	}
	else
	{
		if (RegOpenKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\NDDigital\\DPS\\PrintServer", &reg) == ERROR_SUCCESS)
		{
			
		if( RegQueryValueExA( reg, VLR_TRACE_LEVEL_PS, NULL, NULL, (BYTE*)valLevel, &dw) == ERROR_SUCCESS )
			traceLevel = atoi( valLevel );

			RegCloseKey ( reg );		
		}	
	}
	
	SetTraceLevel ( traceLevel );
	return traceLevel;
}

void CloseTrace()
{
	delete theHelper;
}

void SetTraceFilePrefix(char* strFilePrefix)
{
	// set lock
	theHelper->Lock();
	// set trace file name prefix
	theHelper->SetTraceFilePrefix(strFilePrefix);
	// release lock
	theHelper->Unlock();
}

void SetTraceLevel(const int nLevel)
{
	// set lock
	theHelper->Lock();
	// set trace level
	theHelper->SetTraceLevel(nLevel);
	// release lock
	theHelper->Unlock();
}



void WriteTrace( char* strFormat, ...)
{
	// if the specified trace level is greater than
	// the current trace level, return immediately
	if( theHelper->m_nLevel==0 ) return;

	// set lock
	theHelper->Lock();
	try
	{
		// get local time
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		// get trace file handle	
		FILE *hFile = theHelper->m_hFile;
		// open the trace file if not already open
		if( hFile == NULL ) 
			hFile = theHelper->OpenTraceFile();
		// if it is already a new day, close the old
		// trace file and open a new one
		else if
		(
			sysTime.wYear!=theHelper->m_timeStart.wYear||
			sysTime.wMonth!=theHelper->m_timeStart.wMonth||
			sysTime.wDay!=theHelper->m_timeStart.wDay)
		{
			theHelper->CloseTraceFile();
			theHelper->OpenTraceFile();
		}
		// write the trace message
		if(hFile)
		{
			// declare buffer (default max buffer size = 32k)
			const int nMaxSize = 1024 ;
			char pBuffer[nMaxSize+51] ={0};
			DWORD sessionId = 0; 
			ProcessIdToSessionId( GetCurrentProcessId() , &sessionId  ); 

			// print time stamp and thread id to buffer
			int nPos = sprintf
			(			
				pBuffer,
				("%02d:%02d:%02d:%03d ss:%04d th:%05d: "), 
				sysTime.wHour,
				sysTime.wMinute,
				sysTime.wSecond,
				sysTime.wMilliseconds,
				sessionId,
				GetCurrentThreadId()
				
			);
			// print the trace message to buffer
			va_list args;
			va_start(args, strFormat);
			nPos += _vsnprintf( pBuffer+nPos, nMaxSize, strFormat, args);
			va_end(args);
			// print the end of the line to buffer
			sprintf(pBuffer + nPos,("\r\n"));
			// write the buffer to the trace file
			fputs( pBuffer , hFile );	
			fflush( hFile );
		}
	}
	catch(...)
	{
		// add code to handle exception (if needed)
	}
	// release lock
	theHelper->Unlock();
}

char errorMessage[ MAX_PATH ];

void ErrMessage( DWORD nErr , char _msg[] )
{
	char szErrMsg[ MAX_PATH ];

	if ( FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, 0, nErr, 0, szErrMsg, sizeof szErrMsg / sizeof *szErrMsg, 0 ) )
		strcpy( _msg , szErrMsg ) ;
	else 
		strcpy( _msg, "Erro desconhecido");	
}


class start_trace
{
public:
	start_trace(){ StartTrace(); }	

} obj_start_trace;