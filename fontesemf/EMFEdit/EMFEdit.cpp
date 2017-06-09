// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include <Windows.h>
#include <Objbase.h>
#import "C:\Users\Vitor\git\tonner-save\fontesemf\EMFEditCS\bin\Debug\EMFEditCS.tlb" no_namespace

void EditEMF(char* input, char* output, int percent)
{
	CoInitialize(NULL);

	IImageEditorPtr obj;

	obj.CreateInstance(__uuidof(ImageEditor));

	obj->Edit(input, output, percent);

	CoUninitialize();
}
