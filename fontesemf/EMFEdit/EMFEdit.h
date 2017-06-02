#ifndef _EMFEDIT_H_
#define _EMFEDIT_H_

#include <windows.h>

void EditEMF(char* input, char* output, unsigned percent);

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

	char* input;
	char* output;

public:

	EMFEdit(char* input, char* output, unsigned percentage);

	char* getInput(void) { return input; }
	char* getOutput(void) { return output; }
	unsigned getPercentage(void) { return percentage; }

	void setInput(char* input) { this->input = input; }
	void setOutput(char* output) { this->output = output; }
	void setPercentage(unsigned percentage);

	BOOL EnumEMF(HDC hDC, HENHMETAFILE hemf, const RECT * lpRect);
};

#endif // !_EMFEDIT_H_
