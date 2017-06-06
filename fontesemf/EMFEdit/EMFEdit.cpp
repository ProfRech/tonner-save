// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

//TODO fazer esse método funcionar
void EditEMF(WCHAR* input, WCHAR* output, unsigned percent)
{
	Metafile in(input);

	puts("1");

	int paletteSize = in.GetPaletteSize();

	ColorPalette* palette = (ColorPalette*)malloc(paletteSize);

	in.GetPalette(palette, paletteSize);

	puts("2");

	for (int i = 0; i < paletteSize; i++)
	{
		ARGB color = palette->Entries[i];
		BYTE red = (GetRValue(color) * percent + 0xff * (100-percent))/100;
		BYTE green = (GetGValue(color) * percent + 0xff * (100 - percent)) / 100;
		BYTE blue = (GetBValue(color) * percent + 0xff * (100 - percent)) / 100;
		palette->Entries[i] = (blue)|(green<<8)|(red<<16)|(0xff<<24);
	}

	in.SetPalette(palette);

	free(palette);

	puts("3");

	CLSID  encoderClsid;
	auto result = GetEncoderClsid(L"image/emf", &encoderClsid);// ERROR
	
	puts("4");

	int encSize = in.GetEncoderParameterListSize(&encoderClsid);

	EncoderParameters* encPar = (EncoderParameters*)malloc(encSize);

	in.GetEncoderParameterList(&encoderClsid, encSize, encPar);

	puts("5");

	in.Save(output, &encoderClsid, encPar);

	free(encPar);

	/* Não funciona
	RECT size = { 0, 0, in->GetWidth(), in->GetHeight() };

	HDC metaDC = CreateEnhMetaFileA(0 , output, &size, "TEST");

	Metafile* out = new Metafile(metaDC, out);

	

	/* Não funciona
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
	/**/
	getchar();
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
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
