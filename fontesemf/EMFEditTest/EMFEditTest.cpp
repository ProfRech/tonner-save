// EMFEditTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include "EMFEditTest.h"

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
		int result = CallFunction(inputs[i], outputs[i], percentages[i]);
		printf("\nTest: %d\ninput: %s\noutput: %s\npercentage: %d"
				"\nresult expected: %d\nresult expected text: %s"
				"\nresult: %d\nresult text: %s"
				"\nvalidated test: %s\n",
				(i+1), inputs[i], outputs[i], percentages[i],
				resultExpected[i], ResultText(resultExpected[i]),
				result, ResultText(result),
				(result == resultExpected[i] ? "yes" : "no"));

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

