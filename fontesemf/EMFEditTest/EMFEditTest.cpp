// EMFEditTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include "EMFEditTest.h"

EditEMFFunc* editEMF;

Test tests[] =
{
	// teste normal
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25,
		  0 },
	// teste salvar sobre arquivo já existente
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25,
		  0 },
	// teste processar arquivo já processado
	// espera sucesso (0)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test3.emf",
		  25,
		  0 },
	// teste salvar no mesmo arquivo
	// espera erro (4)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25,
		  4 },
	// teste porcentagem menor que 0
	// espera erro (2)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf", 
		  -1,
		  2 },
	// teste porcentagem maior que 100
	// espera erro (2)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  101, 
		  2 },
	// teste arquivo de entrada inexistente
	// espera erro (1)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test4.emf",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf", 
		  25, 
		  1 },
	// teste caminho inválido para arquivo de entrada
	// espera erro (1)
	Test{ "",
		  "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
		  25, 
		  1 },
	// teste caminho inválido para arquivo de saída
	// espera erro (3)
	Test{ "C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
		  "",
		  25,
		  3 }
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

	for (int i = 0, c = 0; c < sizeof(tests) ; i++, c += sizeof(tests[i]))
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

	}
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

