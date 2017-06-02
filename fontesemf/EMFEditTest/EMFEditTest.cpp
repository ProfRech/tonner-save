// EMFEditTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

typedef void (EditEMF)(char* input, char* output, unsigned percentage);

char* input = "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\1.spl.00000.emf";
char* output = "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\new.emf";
unsigned percentage = 75;

int main(void)
{
	HMODULE hLib = LoadLibraryA("EMFPrinter.dll");
	if (hLib == NULL)
	{
		return 1;
	}

	EditEMF* editEMF = reinterpret_cast<EditEMF*>(GetProcAddress(hLib, "EditEMF"));

	editEMF(input, output, percentage);

	return 0;
}

