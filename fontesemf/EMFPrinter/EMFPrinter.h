#pragma once

#include <string>
#include <windows.h>
#include <iostream>

#ifdef EMFPRINTER_EXPORTS
#	define DLLAPI __declspec(dllexport) 
#else
#	define DLLAPI __declspec(dllimport) 
#endif

extern "C" {
	DLLAPI void* GetEMFPrinterObject();
	DLLAPI bool PrintEMF( void *objEMFPrinter, char* fileName, char* printerName, char* docName, char* userName, int copies, int pagesPerSheet, int border);
	DLLAPI bool DeleteEMFObject( void *objEMFPrinter );
	DLLAPI bool StartPrintEMFJob(void *objEMFPrinter, void** objEMFExtract, void** objPrinter, void** objHDCPrinter, int* jobId, char* fileName, char* printerName, char* docName, char* userName);
	DLLAPI bool ProcessPrintEMFJob( void *objEMFPrinter, void* objEMFExtract, void* objPrinter, void* objHDCPrinter, int jobId, char* fileName, char* docName);
}


class DLLAPI EMFPrinter
{
public:
	enum MultiupType {
		NUP_1 = 1,
		NUP_2 = 2,
		NUP_4 = 4,
		NUP_6 = 6,
		NUP_9 = 9,
		NUP_16 = 16
	};
	EMFPrinter();
	virtual ~EMFPrinter();

	void SetFontTempFolder(const std::string &tempFolder);
	// orientation = DMORIENT_LANDSCAPE || DMORIENT_PORTRAIT
	bool Print(const std::string &filename, const std::string &printerName, const std::string &docName, const std::string &userName, int copies, int orientation);
	bool Print(const std::string &filename, const std::string &printerName, const std::string &docName, const std::string &userName, int copies, int orientation, bool rotate, bool schedule, char * scheduleTime);
	void SetPagesPerSheet(MultiupType multiup);
	void SetPrinterBorder(bool enableBorder);

	DWORD StartPrintJob(const std::string &filename, const std::string &printerName, const std::string &docName, const std::string &userName, void** objEMFExtract, void** objPrinter, void** objHDCPrinter, bool convertMono = false, bool convertDuplex = false);
	bool ProcessPrintJob(void* objEMFExtract, void* objPrinter, void* objHDCPrinter, int jobId, char * filename, const std::string &docName);

private:
	MultiupType m_multiup;
	bool m_enableBorder;
	std::string fontTempFolder;
	int GetTimeInMinutes ( char *hora );

	bool SetUserAndScheduleJob(HANDLE hp, DWORD jobId, const std::string & userName, bool schedule, char * scheduleTime);
};

#pragma warning(push)
#pragma warning(disable:4200) // desabilitar warning sobre zero sized array
#pragma pack(1)
struct EMFData {
	unsigned __int32 type;
	unsigned __int32 size;
};

struct DevMode {
	wchar_t dmDeviceName[32];
	unsigned __int16 dmSpecVersion;
	unsigned __int16 dmDriverVersion;
	unsigned __int16 dmSize;
	unsigned __int16 dmDriverExtra;
	union {
		unsigned __int32 dmFields;
		struct {
			unsigned __int32 :1;
			unsigned __int32 nup:1;
			unsigned __int32 :1;
			unsigned __int32 scale:1;
			unsigned __int32 paperwidth:1;
			unsigned __int32 paperlength:1;
			unsigned __int32 papersize:1;
			unsigned __int32 orientation:1;
			unsigned __int32 collate:1;
			unsigned __int32 ttoption:1;
			unsigned __int32 yresolution:1;
			unsigned __int32 duplex:1;
			unsigned __int32 color:1;
			unsigned __int32 printquality:1;
			unsigned __int32 defaultsource:1;
			unsigned __int32 copies:1;
			unsigned __int32 icmmethod:1;
			unsigned __int32 :6;
			unsigned __int32 formname:1;
			unsigned __int32 :5;
			unsigned __int32 dithertype:1;
			unsigned __int32 mediatype:1;
			unsigned __int32 icmintent:1;
		} dmFieldsSet;
	};
	union {
		struct {
			__int16 dmOrientation;
			__int16 dmPaperSize;
			__int16 dmPaperLength;
			__int16 dmPaperWidth;
			__int16 dmScale;
			__int16 dmCopies;
			__int16 dmDefaultSource;
			__int16 dmPrintQuality;
		};
		struct {
			struct dmPosition {
				unsigned __int16 x;
				unsigned __int16 y;
			};
			unsigned __int32 dmDisplayOrientation;
			unsigned __int32 dmDisplayFixedOutput;
		};
	};
	__int16 dmColor;
	__int16 dmDuplex;
	__int16 dmYResolution;
	__int16 dmTTOption;
	__int16 dmCollate;
	wchar_t dmFormName[32];
	unsigned __int16 dmLogPixels;
	unsigned __int32 dmBitsPerPel;
	unsigned __int32 dmPelsWidth;
	unsigned __int32 dmPelsHeight;
	union {
		unsigned __int32 dmDisplayFlags;
		unsigned __int32 dmNup;
	};
	unsigned __int32 dmDisplayFrequency;
	unsigned __int32 dmICMMethod;
	unsigned __int32 dmICMIntent;
	unsigned __int32 dmMediaType;
	unsigned __int32 dmDitherType;
	unsigned __int32 reserved1;
	unsigned __int32 reserved2;
	unsigned __int32 dmPanningWidth;
	unsigned __int32 dmPanningHeight;
	char dmDriverExtraData[];
};
#pragma pack()
#pragma warning(pop)
