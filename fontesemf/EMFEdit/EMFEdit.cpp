// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include <Windows.h>
#include <Objbase.h>
#include <stdio.h>

int EditEMF(char* input, char* output, unsigned __int8 percent)
{
	if (percent > 100) return INVALID_PERCENTAGE; //percent < 0 nunca vai ocorrer por ser unsigned

	if (strlen(input) == 0) return INVALID_INPUT_FILE;

	if (strlen(output) == 0) return INVALID_OUTPUT_FILE;

	HENHMETAFILE emf = GetEnhMetaFileA(input);

	if (emf == NULL) return INVALID_INPUT_FILE;

	ENHMETAHEADER emfHeader;
	int  length = GetEnhMetaFileHeader(emf, 0, NULL);
	GetEnhMetaFileHeader(emf, length, &emfHeader);

	UINT bSize = emfHeader.nBytes;
	LPBYTE metabits = (LPBYTE)malloc(bSize);

	UINT bufferSize = GetEnhMetaFileBits(emf, bSize, metabits);

	DeleteEnhMetaFile(emf);

	LPBYTE res = (LPBYTE)EMFAnalyze((char*)metabits, (char*)(metabits + bSize), percent);

	HENHMETAFILE newemf = SetEnhMetaFileBits(bufferSize, metabits);

	free(metabits);

	HENHMETAFILE outemf = CopyEnhMetaFileA(newemf, output);

	DeleteEnhMetaFile(newemf);
	
	if (outemf == NULL) return INVALID_OUTPUT_FILE;

	DeleteEnhMetaFile(outemf);

	return SUCCESSFUL;
}

void MaptoWhitish(COLORREF* cr, unsigned __int8 percent)
{
	if ((*cr & 0xFF000000) != PALETTEINDEX(0)) // not paletteindex
	{
		BYTE red = (GetRValue(*cr) * (100 - percent) + 0xff * percent) / 100;
		BYTE green = (GetGValue(*cr) * (100 - percent) + 0xff * percent) / 100;
		BYTE blue = (GetBValue(*cr) * (100 - percent) + 0xff * percent) / 100;

		*cr = (*cr & 0xFF000000) | RGB(red, green, blue);
	}
}

char * EMFAnalyze(char * ptr, char * ptr_max, unsigned __int8 percent)
{
	void MaptoWhitish(COLORREF* cr, unsigned __int8 percent);

	char * ptr_start = ptr;
	PEMR emf_data = NULL;

	PEMRSETTEXTCOLOR emr_setTextColor = NULL;
	PEMRSETBKCOLOR emr_setBkColor = NULL;
	PEMRCREATEPEN emr_createPen = NULL;
	PEMRCREATEBRUSHINDIRECT emr_createBrushIndirect = NULL;
	PEMREXTCREATEPEN emr_extCreatePen = NULL;
	PEMRSETPIXELV emr_setPixelV = NULL;
	PEMREXTFLOODFILL emr_extFloodFill = NULL;
	PEMRBITBLT emr_bitBlt = NULL;
	PEMRSTRETCHBLT emr_stretchBlt = NULL;
	PEMRMASKBLT emr_maskBlt = NULL;
	PEMRPLGBLT emr_plgBlt = NULL;
	PEMRALPHABLEND emr_alphaBlend = NULL;
	PEMRTRANSPARENTBLT emr_transparentBlt = NULL;
	PEMRSTRETCHDIBITS emr_stretchDiBits = NULL;
	PEMRSTRETCHDIBITS emr_stretchDibits = NULL;

	for (; ptr < ptr_max; ptr += emf_data->nSize)
	{
		emf_data = reinterpret_cast<PEMR>(ptr);
		switch (emf_data->iType)
		{
		case 0:
			ptr++;
			break;
		case EMR_EOF:
			return ptr + emf_data->nSize;
		case EMR_SETTEXTCOLOR:
			emr_setTextColor = reinterpret_cast<PEMRSETTEXTCOLOR>(ptr);
			MaptoWhitish(&(emr_setTextColor->crColor), percent);
			break;
		case EMR_SETBKCOLOR:
			emr_setBkColor = reinterpret_cast<PEMRSETBKCOLOR>(ptr);
			MaptoWhitish(&(emr_setBkColor->crColor), percent);
			break;
		case EMR_CREATEPEN:
			emr_createPen = reinterpret_cast<PEMRCREATEPEN>(ptr);
			MaptoWhitish(&(emr_createPen->lopn.lopnColor), percent);
			break;
		case EMR_CREATEBRUSHINDIRECT:
			emr_createBrushIndirect = reinterpret_cast<PEMRCREATEBRUSHINDIRECT>(ptr);
			MaptoWhitish(&(emr_createBrushIndirect->lb.lbColor), percent);
			break;
		case EMR_EXTCREATEPEN:
			emr_extCreatePen = reinterpret_cast<PEMREXTCREATEPEN>(ptr);
			MaptoWhitish(&(emr_extCreatePen->elp.elpColor), percent);
			break;
		case EMR_SETPIXELV:
			emr_setPixelV = reinterpret_cast<PEMRSETPIXELV>(ptr);
			MaptoWhitish(&(emr_setPixelV->crColor), percent);
			break;
		case EMR_EXTFLOODFILL:
			emr_extFloodFill = reinterpret_cast<PEMREXTFLOODFILL>(ptr);
			MaptoWhitish(&(emr_extFloodFill->crColor), percent);
			break;
		case EMR_BITBLT:
			emr_bitBlt = reinterpret_cast<PEMRBITBLT>(ptr);
			MaptoWhitish(&(emr_bitBlt->crBkColorSrc), percent);
			break;
		case EMR_STRETCHBLT:
			emr_stretchBlt = reinterpret_cast<PEMRSTRETCHBLT>(ptr);
			MaptoWhitish(&(emr_stretchBlt->crBkColorSrc), percent);
			break;
		case EMR_MASKBLT:
			emr_maskBlt = reinterpret_cast<PEMRMASKBLT>(ptr);
			MaptoWhitish(&(emr_maskBlt->crBkColorSrc), percent);
			break;
		case EMR_PLGBLT:
			emr_plgBlt = reinterpret_cast<PEMRPLGBLT>(ptr);
			MaptoWhitish(&(emr_plgBlt->crBkColorSrc), percent);
			break;
		case EMR_ALPHABLEND:
			emr_alphaBlend = reinterpret_cast<PEMRALPHABLEND>(ptr);
			MaptoWhitish(&(emr_alphaBlend->crBkColorSrc), percent);
			break;
		case EMR_TRANSPARENTBLT:
			emr_transparentBlt = reinterpret_cast<PEMRTRANSPARENTBLT>(ptr);
			MaptoWhitish(&(emr_transparentBlt->crBkColorSrc), percent);
			break;
		case EMR_STRETCHDIBITS:
			emr_stretchDibits = reinterpret_cast<PEMRSTRETCHDIBITS>(ptr);

			BITMAPINFO* hbm = reinterpret_cast<BITMAPINFO *>(ptr + emr_stretchDibits->offBmiSrc);

			UINT size = (emr_stretchDibits->cbBitsSrc) / sizeof(RGBQUAD);

			for (UINT i = 0; i < size; i++)
			{
				hbm->bmiColors[i].rgbRed = (hbm->bmiColors[i].rgbRed * (100 - percent) + 0xff * percent)/100;
				hbm->bmiColors[i].rgbGreen = (hbm->bmiColors[i].rgbGreen * (100 - percent) + 0xff * percent) / 100;
				hbm->bmiColors[i].rgbBlue = (hbm->bmiColors[i].rgbBlue * (100 - percent) + 0xff * percent) / 100;
				hbm->bmiColors[i].rgbReserved = (hbm->bmiColors[i].rgbReserved * (100 - percent) + 0xff * percent) / 100;
			}
			break;
		}
	}
	return nullptr;
}
