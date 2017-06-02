#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "FontProperties.h"
#include "FontsManager.h"

#define DEBUG_MSG
using namespace std;

typedef void* (GetEMFPrinterObjectFunction) (); 
typedef bool (PrintEMFFunction) (void *objEMFPrinter, char* fileName, char* printerName, char* docName, char* userName, int copies, int pagesPerSheet, int border); 
typedef bool (DeleteEMFObjectFunction) (void * objEMFPrinter); 
typedef bool (StartPrintEMFJobFunction) (void *objEMFPrinter, void** objEMFExtract, void** objPrinter, void** objHDCPrinter, int* jobId, char* fileName, char* printerName, char* docName, char* userName, bool convertMono, bool convertDuplex);
typedef bool (ProcessPrintEMFJobFunction)( void *objEMFPrinter, void* objEMFExtract, void* objPrinter, void* objHDCPrinter, int jobId, char * fileName, char* docName);


int main(int argc, char* argv[])
{
	
	/*FontsManager objFontsManager;
	objFontsManager.LoadLocalFonts();

	FontProperties objFontProperties;
	objFontProperties.Load("C:\\00004.SPL.00000.ttf");
	printf("fontFamily:%s | instaled:%i\n\n", objFontProperties.GetFamily().c_str(), objFontsManager.InstalledFont(objFontProperties.GetFamily()));
	objFontProperties.Load("C:\\00004.SPL.00001.ttf");
	printf("fontFamily:%s | instaled:%i\n\n", objFontProperties.GetFamily().c_str(), objFontsManager.InstalledFont(objFontProperties.GetFamily()));
	objFontProperties.Load("C:\\00004.SPL.00002.ttf");
	printf("fontFamily:%s | instaled:%i\n\n", objFontProperties.GetFamily().c_str(), objFontsManager.InstalledFont(objFontProperties.GetFamily()));
	objFontProperties.Load("C:\\00004.SPL.00003.ttf");
	printf("fontFamily:%s | instaled:%i\n\n", objFontProperties.GetFamily().c_str(), objFontsManager.InstalledFont(objFontProperties.GetFamily()));
	objFontProperties.Load("C:\\00004.SPL.00004.ttf");
	printf("fontFamily:%s | instaled:%i\n\n", objFontProperties.GetFamily().c_str(), objFontsManager.InstalledFont(objFontProperties.GetFamily()));
	objFontProperties.Load("C:\\00004.SPL.00005.ttf");
	printf("fontFamily:%s | instaled:%i\n\n", objFontProperties.GetFamily().c_str(), objFontsManager.InstalledFont(objFontProperties.GetFamily()));
	
	return 0;

	/*int r = AddFontResource(TEXT("C:\\WINDOWS\\FONTS\\00011.SPL.00000.ttf"));
	PostMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);*/

	/*RemoveFontResource(TEXT("C:\\00027.SPL.00000.ttf"));
	int ret = RemoveFontResource(TEXT("C:\\windows\\fonts\\00027.SPL.00000.ttf"));
	PostMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);

	/*int r = AddFontResource(TEXT("C:\\WINDOWS\\FONTS\\00010.SPL.00000.ttf"));
	PostMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);*/

	//int r = AddFontResourceEx(TEXT("C:\\00008.SPL.00000.ttf"), FR_PRIVATE , 0 );

	/*int ret = RemoveFontResource(TEXT("C:\\windows\\fonts\\00048.SPL.00000.ttf"));
	PostMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	BOOL a = DeleteFileA("C:\\windows\\fonts\\00048.SPL.00000.ttf");
	return 0 ;*/

	HMODULE hLib = LoadLibraryA("EMFPrinter.dll");
	if (hLib == NULL)
	{
		return 0;
	}

	GetEMFPrinterObjectFunction * getEMFPrinterObject = reinterpret_cast<GetEMFPrinterObjectFunction*>(GetProcAddress(hLib, "GetEMFPrinterObject"));
	PrintEMFFunction * printEMF = reinterpret_cast<PrintEMFFunction*>(GetProcAddress(hLib, "PrintEMF"));
	DeleteEMFObjectFunction * deleteEMFObject = reinterpret_cast<DeleteEMFObjectFunction*>(GetProcAddress(hLib, "DeleteEMFObject"));
	StartPrintEMFJobFunction * startPrintJob = reinterpret_cast<StartPrintEMFJobFunction*>(GetProcAddress(hLib, "StartPrintEMFJob"));
	ProcessPrintEMFJobFunction * processPrintJob = reinterpret_cast<ProcessPrintEMFJobFunction*>(GetProcAddress(hLib, "ProcessPrintEMFJob"));

	void * objEMFPrinter = getEMFPrinterObject();

	//printEMF(objEMFPrinter, "F:\\shared\\dcs\\DPS-0049-1-162845735-00032-cp#0001#1#-marciom.spl", "hp LaserJet 1320 PCL 5", "Teste de EMFPrinter", "Khristian", 1, 1, 0);
	int jobId = 0;
	char fileName[ MAX_PATH ] = {0};
	char printerName[ MAX_PATH ] = {0};
	char docName[ MAX_PATH ] = {0};
	char userName[ MAX_PATH ] = {0};

	strcpy(fileName, argv[1]);
	//strcpy(printerName, "Samsung SCX-6x55 Series PCL6");
	strcpy(printerName, argv[2]);
	strcpy(docName, "Teste");
	strcpy(userName, "Khristian");

	void* objEMFExtract = NULL;
	void* objPrinter = NULL;
	void* objHDCPrinter = NULL;
	startPrintJob(objEMFPrinter, &objEMFExtract, &objPrinter, &objHDCPrinter, &jobId, fileName, printerName, docName, userName, false, false);
	processPrintJob(objEMFPrinter, objEMFExtract, objPrinter, objHDCPrinter, jobId, fileName, docName);

	deleteEMFObject(objEMFPrinter);


	return 0;
}

