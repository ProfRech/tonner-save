#include "printersettings.h" 
#include <windows.h>
#include "Winspool.h"
//extern void Trace( char * msg ); 

/*
typedef struct _PRINTER_INFO_8
{
	DEVMODE* pDevMode;

}PRINTER_INFO_8z;

*/

#include "XYTrace.h"
#define DEBUG_MSG(fmt, ...) \
			WriteTrace( __FUNCTION__ " - " fmt , ## __VA_ARGS__ );

PrinterSettings::PrinterSettings( string _printerName ) 
{


}



PRINTER_INFO_8* PrinterSettings::SetDocumentOrientation(HANDLE hPrinter, TOrientation& orientation, char* printerName , DEVMODE * dev )
{
    // retorno o devmode o dev8

	DWORD needed = 0 ; 
	BYTE * pBuffer =NULL; 
	GetPrinter( hPrinter, 8 , pBuffer , needed , &needed  ); 


	if( !needed ) 
	{
		return NULL; 
		
	
	}

	pBuffer = new BYTE[ needed] ; 	
	GetPrinter( hPrinter, 8 , pBuffer , needed , &needed  ); 

	return reinterpret_cast< PRINTER_INFO_8*>( pBuffer )  ; 
}


//-----------------------------------------------------------------------------------------------//
// DESCRICAO:
// se a orientação da impressão.. 
//
//
bool PrinterSettings::SetOrientation( string _printerName , TOrientation orientation , bool* alterStatus , bool ajust)
{
	DEBUG_MSG("start | printerName:%s orientation:%i", _printerName.c_str(), orientation);
	// criando HANDLE da impressora.. 

	HANDLE hPrinter ; 	
	DWORD sizePrinterInfo = 0; 
	PRINTER_INFO_2 *pi2;
	bool set = false; 
	
	PRINTER_DEFAULTSA *def ;
	BYTE * pBuffer ;

	bool ret = false; 
	def = new PRINTER_DEFAULTSA;
	def->pDatatype = NULL;
	def->pDevMode = NULL;
	def->DesiredAccess = PRINTER_ALL_ACCESS;

	
	//Trace( "PrinterSettings::SetOrientation 01 " ); 
	if( !OpenPrinterA( (char*)_printerName.c_str(), &hPrinter , def ))
	{
		int err = GetLastError();
		DEBUG_MSG("end | erro no OpenPrinter:%i", err);
		delete def; 
		return false;	
	}
	

	GetPrinter( hPrinter, 2, NULL, 0 , &sizePrinterInfo);
	

	if( sizePrinterInfo < 1)
	{
		int err = GetLastError();
		DEBUG_MSG("end | sizePrinterInfo < 1:%i", err);
		ClosePrinter( hPrinter);
		delete def; 	
		return false;	
	}
	

	if( (pBuffer = new BYTE[ sizePrinterInfo ] )== NULL ) 
	{
		ClosePrinter( hPrinter);
		delete def; 	
		return false;			
	}
	

	if( !GetPrinter( hPrinter, 2, pBuffer, sizePrinterInfo, &sizePrinterInfo ) )
	{
		int err = GetLastError();
		DEBUG_MSG("end | erro do GetPrinter:%i", err);

		delete [] pBuffer ; 
		ClosePrinter( hPrinter);
		delete def; 	
		return false;			
	}
	

	pi2 = ( PRINTER_INFO_2*) pBuffer ;
	DWORD pi2Orientation = DMORIENT_PORTRAIT; 
	
	
	//capturando a orientação antes de mudar o devmode.
	if( pi2->pDevMode ) 
		pi2Orientation = pi2->pDevMode->dmOrientation;

	DEBUG_MSG("orientacao:%i", pi2Orientation);

	PRINTER_INFO_8 *ji8 = NULL ; 
	ji8 = SetDocumentOrientation( hPrinter, orientation,(char*) _printerName.c_str() ,  NULL );		
	
	if( ajust ) 
	{
		pi2->pDevMode = ji8->pDevMode; 
	}

	//Trace( "PrinterSettings::SetOrientation 02 " ); 

	set = false; 
	
	if( pi2->pDevMode ) 
	{
		if( orientation == ePortrait ) 
		{
			if( pi2->pDevMode->dmOrientation == DMORIENT_PORTRAIT  && pi2Orientation == DMORIENT_PORTRAIT   &&  ji8->pDevMode->dmOrientation == DMORIENT_PORTRAIT ) 
			{
				
				set = false; 
			}
			else
			{
				set = true; 
				ji8->pDevMode->dmOrientation = DMORIENT_PORTRAIT ;	
				pi2->pDevMode->dmOrientation = DMORIENT_PORTRAIT ;	
			}	
		}
		else 
		{
			if( pi2->pDevMode->dmOrientation == DMORIENT_LANDSCAPE && pi2Orientation == DMORIENT_LANDSCAPE && ji8->pDevMode->dmOrientation == DMORIENT_LANDSCAPE	) 
				set = false; 
			else
			{
				set = true; 
				ji8->pDevMode->dmOrientation = DMORIENT_LANDSCAPE;	
				pi2->pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
			}		
		}
		
		
		ret = true; 
		

		//set = true;
		//if( set ) 
		{
			DEBUG_MSG("setando as propriedades da impressora");

			//Trace( "PrinterSettings::Setando a orientacao.. " ); 			
			//Trace( orientation == ePortrait ? "Portrait" :"Landscape" ); 



			ret = SetPrinter( hPrinter, 2, ( BYTE*) pi2, 0 )== TRUE; 		
			if( !ret ) 
			{
				int err = GetLastError();
				DEBUG_MSG("end | erro do SetPrinter2:%i", err);
			}

			if( ajust ) 
			{
				if (SetPrinter( hPrinter , 8 , (BYTE*) ji8, 0 ) != TRUE)
				{
					int err = GetLastError();
					DEBUG_MSG("end | erro do SetPrinter8:%i", err);
				}
			}


			*alterStatus = ret; 
		}
		//else
			//Trace( "PrinterSettings::Não é necessario setar a impressora.." ); 
	}
	
	//Trace( "PrinterSettings::SetOrientation 032 " ); 

	delete [] ji8;

	delete [] pBuffer ; 	
	ClosePrinter( hPrinter);
	delete def; 	

	DEBUG_MSG("end | ret %i", ret);
	return ret;  
}
