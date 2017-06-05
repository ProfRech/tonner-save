// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include <assert.h>
#include <stdio.h>

//TODO fazer esse método funcionar
void EditEMF(char * input, char * output, unsigned percent)
{
	HWND desktop = GetDesktopWindow();
	HDC dc = GetDC(desktop);

	RECT size = { 0, 0, 100000, 100000 };

	HENHMETAFILE emf = GetEnhMetaFileA(input);

	EMFEdit* emfEditor = new EMFEdit(percent);

	emfEditor->EnumEMF(dc, emf, &size);

	HDC metaDC = CreateEnhMetaFileA(dc, output, &size, "TEST");

	delete(emfEditor);

	HENHMETAFILE metafile = CloseEnhMetaFile(metaDC);
	DeleteEnhMetaFile(emf);
	DeleteEnhMetaFile(metafile);
	DeleteDC(metaDC);

	getchar();
}

inline void MaptoWhitish(COLORREF & cr, unsigned percent)
{
	if ((cr & 0xFF000000) != PALETTEINDEX(0)) // not paletteindex
	{
		static int teste = 0;
		printf("Passou pela %d vez, a cor eh: r = %d, g = %d, b = %d\n", ++teste, GetRValue(cr), GetGValue(cr), GetBValue(cr));

		BYTE black = 0x00;

		cr = (cr & 0xFF000000) | RGB(black, black, black);
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

EMFEdit::EMFEdit(unsigned percentage)
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
