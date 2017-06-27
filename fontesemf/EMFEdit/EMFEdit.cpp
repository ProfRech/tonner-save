// EMFEdit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EMFEdit.h"
#include "EMF_NT.h"
#include <Windows.h>
#include <Objbase.h>

int EditEMF(char* input, char* output, unsigned __int8 percent)
{

	//TODO validar input, output e percent

	HENHMETAFILE emf = GetEnhMetaFileA(input);

	ENHMETAHEADER emfHeader;
	int  length = GetEnhMetaFileHeader(emf, 0, NULL);
	GetEnhMetaFileHeader(emf, length, &emfHeader);

	UINT bSize = emfHeader.nBytes;
	LPBYTE metabits = (LPBYTE)malloc(bSize);

	UINT bufferSize = GetEnhMetaFileBits(emf, bSize, metabits);

	LPBYTE res = (LPBYTE)EMFAnalyze((char*)metabits, (char*)(metabits + bSize), percent);

	HENHMETAFILE newemf = SetEnhMetaFileBits(bufferSize, metabits);
	CopyEnhMetaFileA(newemf, output);

	free(metabits);

	return 0;
}

char * EMFAnalyze(char * ptr, char * ptr_max, unsigned __int8 percent)
{
	char * ptr_start = ptr;
	EMFData * emf_data = NULL;
	EMRCreateBrushIndirect * emr_createBrushIndirect = NULL;
	EMRSetTextColor * emr_setTextColor = NULL;
	EMRComment * emr_comment = NULL;
	EMRSetBkColor * emr_setBkColor = NULL;
	EMRExtCreatePen * emr_extCreatePen = NULL;

	for (; ptr < ptr_max; ptr += emf_data->size)
	{
		emf_data = reinterpret_cast<EMFData *>(ptr);
		switch (emf_data->type)
		{
		case 0:
			ptr++;
			break;
		case _EMR_HEADER:
		case _EMR_POLYBEZIER:
		case _EMR_POLYGON:
		case _EMR_POLYLINE:
		case _EMR_POLYBEZIERTO:
		case _EMR_POLYLINETO:
		case _EMR_POLYPOLYLINE:
		case _EMR_POLYPOLYGON:
		case _EMR_SETWINDOWEXTEX:
		case _EMR_SETWINDOWORGEX:
		case _EMR_SETVIEWPORTEXTEX:
		case _EMR_SETVIEWPORTORGEX:
		case _EMR_SETBRUSHORGEX:
			break;
		case _EMR_EOF:
			return ptr + emf_data->size;
			break;
		case _EMR_SETPIXELV:
		case _EMR_SETMAPPERFLAGS:
		case _EMR_SETMAPMODE:
		case _EMR_SETBKMODE:
		case _EMR_SETPOLYFILLMODE:
		case _EMR_SETROP2:
		case _EMR_SETSTRETCHBLTMODE:
		case _EMR_SETTEXTALIGN:
		case _EMR_SETCOLORADJUSTMENT:
			break;
		case _EMR_SETTEXTCOLOR:
			emr_setTextColor = reinterpret_cast<EMRSetTextColor *>(ptr);
			emr_setTextColor->color.red = (emr_setTextColor->color.red * (100-percent) + 0xff * percent) / 100;
			emr_setTextColor->color.green = (emr_setTextColor->color.green * (100 - percent) + 0xff * percent) / 100;
			emr_setTextColor->color.blue = (emr_setTextColor->color.blue * (100 - percent) + 0xff * percent) / 100;
			break;
		case _EMR_SETBKCOLOR:
			emr_setBkColor = reinterpret_cast<EMRSetBkColor *>(ptr);
			emr_setBkColor->color.red = (emr_setBkColor->color.red * (100 - percent) + 0xff * percent) / 100;
			emr_setBkColor->color.green = (emr_setBkColor->color.green * (100 - percent) + 0xff * percent) / 100;
			emr_setBkColor->color.blue = (emr_setBkColor->color.blue * (100 - percent) + 0xff * percent) / 100;
			break;
		case _EMR_OFFSETCLIPRGN:
		case _EMR_MOVETOEX:
		case _EMR_SETMETARGN:
		case _EMR_EXCLUDECLIPRECT:
		case _EMR_INTERSECTCLIPRECT:
		case _EMR_SCALEVIEWPORTEXTEX:
		case _EMR_SCALEWINDOWEXTEX:
		case _EMR_SAVEDC:
		case _EMR_RESTOREDC:
		case _EMR_SETWORLDTRANSFORM:
		case _EMR_MODIFYWORLDTRANSFORM:
		case _EMR_SELECTOBJECT:
		case _EMR_CREATEPEN:
			break;
		case _EMR_CREATEBRUSHINDIRECT:
			emr_createBrushIndirect = reinterpret_cast<EMRCreateBrushIndirect *>(ptr);
			emr_createBrushIndirect->logBrush.color.red = (emr_createBrushIndirect->logBrush.color.red * (100 - percent) + 0xff * percent) / 100;
			emr_createBrushIndirect->logBrush.color.green = (emr_createBrushIndirect->logBrush.color.green * (100 - percent) + 0xff * percent) / 100;
			emr_createBrushIndirect->logBrush.color.blue = (emr_createBrushIndirect->logBrush.color.blue * (100 - percent) + 0xff * percent) / 100;
			break;
		case _EMR_DELETEOBJECT:
		case _EMR_ANGLEARC:
		case _EMR_ELLIPSE:
		case _EMR_RECTANGLE:
		case _EMR_ROUNDRECT:
		case _EMR_ARC:
		case _EMR_CHORD:
		case _EMR_PIE:
		case _EMR_SELECTPALETTE:
		case _EMR_CREATEPALETTE:
		case _EMR_SETPALETTEENTRIES:
		case _EMR_RESIZEPALETTE:
		case _EMR_REALIZEPALETTE:
		case _EMR_EXTFLOODFILL:
		case _EMR_LINETO:
		case _EMR_ARCTO:
		case _EMR_POLYDRAW:
		case _EMR_SETARCDIRECTION:
		case _EMR_SETMITERLIMIT:
		case _EMR_BEGINPATH:
		case _EMR_ENDPATH:
		case _EMR_CLOSEFIGURE:
		case _EMR_FILLPATH:
		case _EMR_STROKEANDFILLPATH:
		case _EMR_STROKEPATH:
		case _EMR_FLATTENPATH:
		case _EMR_WIDENPATH:
		case _EMR_SELECTCLIPPATH:
		case _EMR_ABORTPATH:
		case _EMR_RESERVED_69:
			break;
		case _EMR_COMMENT:
			emr_comment = reinterpret_cast<EMRComment *>(ptr);
			break;
		case _EMR_FILLRGN:
		case _EMR_FRAMERGN:
		case _EMR_INVERTRGN:
		case _EMR_PAINTRGN:
		case _EMR_EXTSELECTCLIPRGN:
		case _EMR_BITBLT:
		case _EMR_STRETCHBLT:
		case _EMR_MASKBLT:
		case _EMR_PLGBLT:
		case _EMR_SETDIBITSTODEVICE:
		case _EMR_STRETCHDIBITS:
		case _EMR_EXTCREATEFONTINDIRECTW:
		case _EMR_EXTTEXTOUTA:
		case _EMR_EXTTEXTOUTW:
		case _EMR_POLYBEZIER16:
		case _EMR_POLYGON16:
		case _EMR_POLYLINE16:
		case _EMR_POLYBEZIERTO16:
		case _EMR_POLYLINETO16:
		case _EMR_POLYPOLYLINE16:
		case _EMR_POLYPOLYGON16:
		case _EMR_POLYDRAW16:
		case _EMR_CREATEMONOBRUSH:
		case _EMR_CREATEDIBPATTERNBRUSHPT:
			break;
		case _EMR_EXTCREATEPEN:
			emr_extCreatePen = reinterpret_cast<EMRExtCreatePen *>(ptr);
			emr_extCreatePen->elp.color.red = (emr_extCreatePen->elp.color.red * (100 - percent) + 0xff * percent) / 100;
			emr_extCreatePen->elp.color.green = (emr_extCreatePen->elp.color.green * (100 - percent) + 0xff * percent) / 100;
			emr_extCreatePen->elp.color.blue = (emr_extCreatePen->elp.color.blue * (100 - percent) + 0xff * percent) / 100;
			break;
		case _EMR_POLYTEXTOUTA:
		case _EMR_POLYTEXTOUTW:
		case _EMR_SETICMMODE:
		case _EMR_CREATECOLORSPACE:
		case _EMR_SETCOLORSPACE:
		case _EMR_DELETECOLORSPACE:
		case _EMR_GLSRECORD:
		case _EMR_GLSBOUNDEDRECORD:
		case _EMR_PIXELFORMAT:
		case _EMR_DRAWESCAPE:
		case _EMR_EXTESCAPE:
		case _EMR_RESERVED_107:
		case _EMR_SMALLTEXTOUT:
		case _EMR_FORCEUFIMAPPING:
		case _EMR_NAMEDESCAPE:
		case _EMR_COLORCORRECTPALETTE:
		case _EMR_SETICMPROFILEA:
		case _EMR_SETICMPROFILEW:
		case _EMR_ALPHABLEND:
		case _EMR_SETLAYOUT:
		case _EMR_TRANSPARENTBLT:
		case _EMR_RESERVED_117:
		case _EMR_GRADIENTFILL:
		case _EMR_SETLINKEDUFIS:
		case _EMR_SETTEXTJUSTIFICATION:
		case _EMR_COLORMATCHTOTARGETW:
		case _EMR_CREATECOLORSPACEW:
			break;
		default:
			return false;
		}
	}
	//return ptr_max; // não deveria chegar aqui
	return ptr;
}
