// EMFEditTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

typedef int (EditEMFFunc)(char* input, char* output, unsigned percentage);
EditEMFFunc* editEMF;

char* inputs[] =
{
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test4.emf",
	"",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
};
char* outputs[] =
{
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test3.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	""
};
int percentages[] =
{
	25,
	25,
	25,
	25,
	-1,
	101,
	25,
	25,
	25
};


char* CallFunction(char* i, char* o, int p);

int main(void)
{
	HMODULE hLib = LoadLibraryA("EMFEdit.dll");
	if (hLib == NULL)
	{
		puts("dll not found");
		getchar();
		return 1;
	}

	editEMF = reinterpret_cast<EditEMFFunc*>(GetProcAddress(hLib, "EditEMF"));

	for (int i = 0; i < 9; i++)
	{
		printf("\nTest: %d\ninput: %s\noutput: %s\npercentage: %d\nresult: %s\n",
				(i+1), inputs[i], outputs[i], percentages[i], CallFunction(inputs[i], outputs[i], percentages[i]));

	}
	getchar();

	return 0;
}

char* CallFunction(char* i, char* o, int p)
{

	int r = editEMF(i, o, p);

	switch (r)
	{
	case 0: return "Successful\n";
	case 1: return "Error: invalid input file\n";
	case 2: return "Error: the percentage value must be between 0 and 100\n";
	case 3: return "Error: invalid output file\n";
	case 4: return "Error: the input file must be different than the output file\n";
	case 5: return "Generic error\n";
	}
}

