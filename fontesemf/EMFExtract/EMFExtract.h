#pragma once

#include "MappedFile.h"
#include <string>
#include <map>

#ifdef EMF_EXTRACT_LIB
#	define DLL_PORT __declspec(dllexport)
#else
#	define DLL_PORT __declspec(dllimport)
#endif // EMF_EXTRACT_LIB

class DLL_PORT EMFExtract
{
private:
	std::string splFilename;
	std::string fontTempFolder;
	//std::map<int, char *> m_devmodes;
	int numPages;
	int numFonts;
	bool printServer; 

	bool SPLAnalyze(const MappedFile & mf);
	const char * EMFAnalyze(const char * ptr, const char * ptr_max);
	bool EMFCopy(const char * ptr, size_t size);
	bool FontCopy(const char * ptr, int size);
public:
	EMFExtract();
	virtual ~EMFExtract();

	void SetFontTempFolder(const std::string &tempFolder);
	bool Extract(const std::string & splFilename);
	bool LoadFonts();
	bool UnloadFonts();
	bool Cleanup();
	int GetNumPages() const;
	int GetNumFonts() const;
	//std::map<int, char *> GetDevmodes() const;
};

extern "C" {
	DLL_PORT void * EMFExtract_New();
	DLL_PORT int EMFExtract_Delete(void * _this);
	DLL_PORT int EMFExtract_Extract(void * _this, const char * splFilename);
	DLL_PORT int EMFExtract_LoadFonts(void * _this);
	DLL_PORT int EMFExtract_UnloadFonts(void * _this);
	DLL_PORT int EMFExtract_Cleanup(void * _this);
	DLL_PORT int EMFExtract_GetNumPages(void * _this);
	DLL_PORT int EMFExtract_GetNumFonts(void * _this);
};