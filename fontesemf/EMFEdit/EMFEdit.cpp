// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include <assert.h>

void EditEMF(char * input, char * output, unsigned percent)
{
	//TODO criar objeto do tipo EMFEdit e chamar o método EnumEMF
}

inline void MaptoWhitish(COLORREF & cr, unsigned percent)
{
	if ((cr & 0xFF000000) != PALETTEINDEX(0)) // not paletteindex
	{
		BYTE gray = (GetRValue(cr) * 77 + GetGValue(cr) * 150 + GetBValue(cr) * 29 + 128) / 256;

		cr = (cr & 0xFF000000) | RGB(gray, gray, gray);
	}
}

int EMFEdit::ProcessRecord(HDC hDC, HANDLETABLE * pHTable, const ENHMETARECORD * pEMFR, int nObj)
{
	int rslt;

	switch (pEMFR->iType)
	{
	case EMR_CREATEBRUSHINDIRECT:
	{
		EMRCREATEBRUSHINDIRECT cbi;

		cbi = *(const EMRCREATEBRUSHINDIRECT *)pEMFR;
		MaptoWhitish(cbi.lb.lbColor, this->percentage);

		rslt = PlayEnhMetaFileRecord(hDC, pHTable, (const ENHMETARECORD *)& cbi, nObj);
	}
	break;

	case EMR_CREATEPEN:
	{
		EMRCREATEPEN cp;

		cp = *(const EMRCREATEPEN *)pEMFR;
		MaptoWhitish(cp.lopn.lopnColor, this->percentage);

		rslt = PlayEnhMetaFileRecord(hDC, pHTable, (const ENHMETARECORD *)& cp, nObj);
	}
	break;

	case EMR_SETTEXTCOLOR:
	case EMR_SETBKCOLOR:
	{
		EMRSETTEXTCOLOR stc;

		stc = *(const EMRSETTEXTCOLOR *)pEMFR;
		MaptoWhitish(stc.crColor, this->percentage);

		rslt = PlayEnhMetaFileRecord(hDC, pHTable, (const ENHMETARECORD *)& stc, nObj);
	}
	break;

	case EMR_RESERVED_105:
	case EMR_RESERVED_106:
	case EMR_RESERVED_107:
	case EMR_RESERVED_108:
	case EMR_RESERVED_109:
	case EMR_RESERVED_110:
	case EMR_RESERVED_119:
	case EMR_RESERVED_120:
		rslt = PlayEnhMetaFileRecord(hDC, pHTable, pEMFR, nObj);
		break;

	default:
		rslt = PlayEnhMetaFileRecord(hDC, pHTable, pEMFR, nObj);
	}

	return rslt;
}

int EMFEdit::EMFProc(HDC hDC, HANDLETABLE * pHTable, const ENHMETARECORD * pEMFR, int nObj, LPARAM lpData)
{
	EMFEdit * pObj = (EMFEdit *)lpData;

	if (IsBadWritePtr(pObj, sizeof(EMFEdit)))
	{
		assert(false);
		return 0;
	}

	return pObj->ProcessRecord(hDC, pHTable, pEMFR, nObj);
}

EMFEdit::EMFEdit(char * input, char * output, unsigned percentage) : input(input), output(output)
{
	setPercentage(percentage);
}

void EMFEdit::setPercentage(unsigned percentage)
{
	if (percentage < 100)
		this->percentage = percentage;
	else
		this->percentage = 0; //TODO throw an exception
}

BOOL EMFEdit::EnumEMF(HDC hDC, HENHMETAFILE hemf, const RECT * lpRect)
{
	return ::EnumEnhMetaFile(hDC, hemf, EMFProc, this, lpRect);
}
