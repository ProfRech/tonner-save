#include <iostream>
#include "EMFExtract.h"
#include "EMFPrinter.h"
#include "WinGDI.h"

int main(int argc, char * argv[])
{
/*
	EMFExtract ext; 
	ext.SetFontTempFolder( "C:\\WINDOWS\\Temp\\NDDigital\\DPS\\TempFonts");
	ext.Extract( "C:/Dados_ids/Marisol/DPS-0014-2-171106298-00050-cp#0001#0#-te200623.spl" ); 

	ext.Cleanup();
	*/
	
	/*std::string file_name = "C:/Dados_ids/Marisol/DPS-0014-2-171106298-00050-cp#0001#0#-te200623.spl" ;
	EMFPrinter emf_printer;
	emf_printer.SetFontTempFolder("D:\\Windows\\Temp\\NDDigital\\DPS\\TempFonts");
	//emf_printer.SetPagesPerSheet(EMFPrinter::NUP_2);
	emf_printer.SetPrinterBorder(true);
	//emf_printer.Print("c:\\dps.spl", "Lexmark Universal PS3", "just a test", "nousername", 1, DMORIENT_PORTRAIT);
	emf_printer.Print( file_name, "[IMP004] Lexmark T644 XL", "DPS#word.", "nousername", 1, DMORIENT_PORTRAIT);
	//emf_printer.Print("c:\\dps.spl", "TOSHIBA e-STUDIO205Series PSL3", "just a test", "nousername", 1, DMORIENT_PORTRAIT);
	
	return 0;*/

	if (argc >= 2)
	{
		EMFExtract emf_extract;
		emf_extract.Extract(argv[1]);
		emf_extract.LoadFonts(); 
	}
	else
	{
		EMFExtract emf_extract;
		std::string filename;
		do
		{
			std::cout << ">>> ";
			std::getline(std::cin, filename);
			if ((filename == "exit") ||
				(filename == "quit"))
			{
				break;
			}

			if (filename == "load")
			{
				emf_extract.LoadFonts();
				continue;
			}
			else if (filename == "unload")
			{
				emf_extract.UnloadFonts();
				continue;
			}
			else if (filename == "cleanup")
			{
				emf_extract.Cleanup();
				continue;
			}

			if (filename.at(0) == '"')
			{
				filename.erase(0, 1);
			}
			if (filename.at(filename.length() - 1) == '"')
			{
				filename.erase(filename.length() - 1);
			}

			emf_extract.Extract(filename);
		} while (1);
	}
	return 0;
}
