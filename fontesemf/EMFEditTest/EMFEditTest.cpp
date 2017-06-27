// EMFEditTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include "EMFEditTest.h"

using std::cout;
using std::endl;

EditEMFFunc* editEMF;

Test tests[] =
{
	// teste 1 0%
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test0.emf",
		  0,
		  0 },
	// teste 2 25%
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test25.emf",
		  25,
		  0 },
	  // teste 3 50%
	  // espera sucesso (0)
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test50.emf",
		  50,
		  0 },
	  // teste 4 75%
	  // espera sucesso (0)
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test75.emf",
		  75,
		  0 },
	  // teste 5 100%
	  // espera sucesso (0)
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test100.emf",
		  100,
		  0 },
};

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
	int res = 0;
	for (int i = 0, c = 0; c < sizeof(tests) ; c += sizeof(tests[i]), i++)
	{
		Test* t = &tests[i];
		int result = CallFunction(t->input, t->output, t->percentage);
		printf("\nTest: %d\ninput: %s\noutput: %s\npercentage: %d"
				"\nexpected result: %d\nexpected result text: %s"
				"\nresult: %d\nresult text: %s"
				"\nvalidated test: %s\n",
				(i+1), t->input, t->output, t->percentage,
				t->expectedResult, ResultText(t->expectedResult),
				result, ResultText(result),
				(result == t->expectedResult ? "yes" : "no"));
		res += (result != t->expectedResult) << i;
	}

	cout << endl;
	if (res)
	{
		cout << "Failed tests: ";
		for (int i = 0; res; res >>= 1, i++)
			if (res & 1)
				cout << (i+1) << ((res >> 1) ? ", " : "");
	}
	else
		cout << "All tests were successful";
	cout << endl;

	getchar();

	return 0;
}

int CallFunction(char* i, char* o, int p)
{
	return editEMF(i, o, p);
}

char* ResultText(int r) {
	switch (r)
	{
	case 0: return "Successful";
	case 1: return "Error: invalid input file";
	case 2: return "Error: the percentage value must be between 0 and 100";
	case 3: return "Error: invalid output file";
	case 4: return "Error: the input file must be different than the output file";
	case 5: return "Generic error";
	}
}

