// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include <Windows.h>
#include <Objbase.h>
#import "C:\Users\Vitor\git\tonner-save\fontesemf\EMFEditCS\bin\Debug\EMFEditCS.tlb" no_namespace

int EditEMF(char* input, char* output, int percent)
{
	CoInitialize(NULL);

	IImageEditorPtr obj;

	obj.CreateInstance(__uuidof(ImageEditor));

	int r = obj->Edit(input, output, percent);

	CoUninitialize();

	return r;
}
