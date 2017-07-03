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
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test0.emf",
		  0 ,
		  SUCCESSFUL },
	// teste 2 25%
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test25.emf",
		  25,
		  SUCCESSFUL },
	  // teste 3 50%
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test50.emf",
		  50,
		  SUCCESSFUL },
	  // teste 4 75%
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test75.emf",
		  75,
		  SUCCESSFUL },
	  // teste 5 100%
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test100.emf",
		  100,
		  SUCCESSFUL },
	  // teste 6
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT2.emf",
		  0,
		  SUCCESSFUL },
	  // teste 7 salvar em arquivo existente
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT2.emf",
		  0,
		  SUCCESSFUL },
	  // teste 8 salvar no mesmo arquivo
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT2.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT2.emf",
		  75,
		  SUCCESSFUL },
	  // teste 9 porcentagem acima de 100
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT3.emf",
		  101,
		  INVALID_PERCENTAGE },
	  // teste 10 input invalido
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT9999.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT4.emf",
		  50,
		  INVALID_INPUT_FILE },
	  // teste 11 input invalido
	  Test{ "",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\TestT4.emf",
		  50,
		  INVALID_INPUT_FILE },
	  // teste 12 output invalido
	  Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "",
		  50,
		  INVALID_OUTPUT_FILE },
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
	case SUCCESSFUL: return "Successful";
	case INVALID_INPUT_FILE: return "Error: invalid input file";
	case INVALID_PERCENTAGE: return "Error: the percentage value must be between 0 and 100";
	case INVALID_OUTPUT_FILE: return "Error: invalid output file";
	}
}

