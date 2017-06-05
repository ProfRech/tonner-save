// EMFEditTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

typedef void (EditEMFFunc)(char* input, char* output, unsigned percentage);

char* input = "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\1.spl.00000.emf";
char* output = "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\new.emf";
unsigned percentage = 75;

int main(void)
{
	HMODULE hLib = LoadLibraryA("EMFEdit.dll");
	if (hLib == NULL)
	{
		puts("Biblioteca nao encontrada.");
		getchar();
		return 1;
	}

	EditEMFFunc* editEMF = reinterpret_cast<EditEMFFunc*>(GetProcAddress(hLib, "EditEMF"));

	editEMF(input, output, percentage);

	//getchar();

	return 0;
}

