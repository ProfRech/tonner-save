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
	// teste 1 normal
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25,
		  0 },
	// teste 2 salvar sobre arquivo já existente
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25,
		  0 },
	// teste 3 processar arquivo já processado
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test3.emf",
		  25,
		  0 },
	// teste 4 salvar no mesmo arquivo
	// espera erro (4)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25,
		  4 },
	// teste 5 porcentagem menor que 0
	// espera erro (2)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf", 
		  -1,
		  2 },
	// teste 6 porcentagem maior que 100
	// espera erro (2)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  101, 
		  2 },
	// teste 7 arquivo de entrada inexistente
	// espera erro (1)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test0.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf", 
		  25, 
		  1 },
	// teste 8 caminho inválido para arquivo de entrada
	// espera erro (1)
	Test{ "",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25, 
		  1 },
	// teste 9 caminho inválido para arquivo de saída
	// espera erro (3)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "",
		  25,
		  3 },
	// teste 10 porcentagem 0 - arquivo gerado equivalente ao original
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test4.emf",
		  0,
		  0 },
	// teste 11 porcentagem 100 - arquivo gerado totalmente em branco
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test5.emf",
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

