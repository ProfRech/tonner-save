#ifndef PRINTER_SETTINGS_H_
#define PRINTER_SETTINGS_H_

#include <windows.h>
#include <Winspool.h>
#include <Wingdi.h>
//#include "CommonStructs.h"
#include <string>

using namespace std;


typedef enum _orientation
{
	
	ePortrait = 0,

	eLandscape = 1

} TOrientation;
 
class PrinterSettings
{

public:

	PrinterSettings( string _printerName ) ;
	static bool SetOrientation( string _printerName , TOrientation orientation , bool* alterStatus , bool ajust);
	//static DEVMODE FAR* SetDocumentOrientation(HANDLE hPrinter, TOrientation& orientation, char* printerName , DEVMODE * dev );
	static PRINTER_INFO_8 * SetDocumentOrientation( HANDLE hPrinter, TOrientation& orientation, char* printerName , DEVMODE * dev );
};

#endif