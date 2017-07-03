#ifndef _EMFEDIT_H_
#define _EMFEDIT_H_

#include <windows.h>

#ifdef EMFPRINTER_EXPORTS
#	define DLLAPI __declspec(dllexport) 
#else
#	define DLLAPI __declspec(dllimport) 
#endif

#define SUCCESSFUL 0
#define INVALID_PERCENTAGE 1
#define INVALID_INPUT_FILE 2
#define INVALID_OUTPUT_FILE 3

extern "C" {
	DLLAPI int EditEMF(char* input, char* output, unsigned __int8 percent);
}

char * EMFAnalyze(char * ptr, char * ptr_max, unsigned __int8 percent);

#endif // !_EMFEDIT_H_
