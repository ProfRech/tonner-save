#ifndef _EMFEDIT_H_
#define _EMFEDIT_H_

#include <windows.h>

#ifdef EMFPRINTER_EXPORTS
#	define DLLAPI __declspec(dllexport) 
#else
#	define DLLAPI __declspec(dllimport) 
#endif

extern "C" {
	DLLAPI void EditEMF(char* input, char* output, unsigned percent);
}

inline void MaptoWhitish(COLORREF & cr, unsigned percent);

class EMFEdit
{
	// virtual function to process every EMF record, return 0 to terminate
	virtual int ProcessRecord(HDC hDC, HANDLETABLE * pHTable, const ENHMETARECORD * pEMFR, int nObj);

	// static callback function, dispatch to virtual function ProcessRecord
	static int CALLBACK EMFProc(HDC hDC, HANDLETABLE * pHTable,
		const ENHMETARECORD * pEMFR, int nObj, LPARAM lpData);

private:

	unsigned percentage;

public:

	EMFEdit(unsigned percentage);

	unsigned getPercentage(void) { return percentage; }

	void setPercentage(unsigned percentage);

	BOOL EnumEMF(HDC hDC, HENHMETAFILE hemf, const RECT * lpRect);
};

#endif // !_EMFEDIT_H_
