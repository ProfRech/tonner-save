#ifndef _EMFEDIT_H_
#define _EMFEDIT_H_

#include <windows.h>

#ifdef EMFPRINTER_EXPORTS
#	define DLLAPI __declspec(dllexport) 
#else
#	define DLLAPI __declspec(dllimport) 
#endif

extern "C" {
	DLLAPI int EditEMF(char* input, char* output, unsigned __int8 percent);
}

char * EMFAnalyze(char * ptr, char * ptr_max, unsigned __int8 percent);

#endif // !_EMFEDIT_H_
