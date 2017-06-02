#include "EMFExtract.h"
#include "EMF_NT.h"
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <stdio.h>
#include "FontsManager.h"
#include "FontProperties.h"

using namespace std;

#define DEBUG_MSG(fmt, ...)
/*#define DEBUG_MSG(fmt, ...) \
	do { \
		{\
		FILE * f = fopen(  "C:\\emfextracttrace.txt" , "a+t"); \
			if (f) \
			{ \
				SYSTEMTIME sysTime;\
				GetLocalTime( &sysTime );\
				DWORD ss =0;\
				ProcessIdToSessionId(GetCurrentProcessId(), &ss);\
				fprintf(f, "%02d:%02d:%02d.%03d " __FUNCTION__ ": %i(%i:%i:%i) - " fmt "\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, __LINE__, GetCurrentProcessId(), GetCurrentThreadId(),ss, ## __VA_ARGS__); \
				fclose(f); \
			} \
		}\
	} while (0)*/

std::string fileBasename(const std::string &filename)
{
	int t = filename.rfind('\\');
	if (t != std::string::npos)
		return filename.substr(t + 1);
	else
		return filename;
}

EMFExtract::EMFExtract() : numPages(0), numFonts(0), printServer(false)
{
}

EMFExtract::~EMFExtract()
{
	/*for (int i = 0; i < this->numPages; i++)
	{
		if ((m_devmodes.count(this->numPages) > 0) &&
			m_devmodes[this->numPages])
		{
			delete [] m_devmodes[this->numPages];
			m_devmodes[this->numPages] = NULL;
		}
	}
	m_devmodes.clear();*/
}

void EMFExtract::SetFontTempFolder(const std::string &tempFolder)
{
	this->fontTempFolder = tempFolder;
}

bool EMFExtract::Extract(const std::string & splFilename)
{
	this->splFilename = splFilename;
	try
	{
		this->numPages = 0;
		this->numFonts = 0;
		MappedFile mf(this->splFilename);
		this->SPLAnalyze(mf);
	}
	catch (const char * msg)
	{
		DEBUG_MSG("exception: %s", msg);
		return false;
	}
	catch (...)
	{
		DEBUG_MSG("exception desconhecido");
		return false;
	}

	return true;
}


std::string LoadPrintServerTempFolder()
{

	string workFolder = "c:\temp"; 

	return workFolder ; 


}
bool EMFExtract::LoadFonts()
{
	DEBUG_MSG("start");
	bool r = true;

	FontsManager objFontsManager;
	objFontsManager.LoadLocalFonts();

	
	for (int i = 0; i < this->numFonts; i++)
	{
		std::stringstream ss;
		ss << this->fontTempFolder << '\\' << fileBasename(this->splFilename) << '.' << std::setfill('0') << std::setw(5) << i << ".ttf";
		string fontFileName = ss.str();

		FontProperties objFontProperties;
		objFontProperties.Load(fontFileName);
		DEBUG_MSG("verificando: %s | family:%s", fontFileName.c_str(), objFontProperties.GetFamily().c_str());

		if (objFontsManager.InstalledFont(objFontProperties.GetFamily()) == false)
		{
			r = AddFontResourceA(fontFileName.c_str());//, FR_PRIVATE, 0) && r;
			DEBUG_MSG("loading result: %i", r);
		}
		else
		{
			DEBUG_MSG("fonte ja instalada!");
		}
	}

	PostMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	return r;
}

bool EMFExtract::UnloadFonts()
{
	DEBUG_MSG("start");
	bool r = true;

	for (int i = 0; i < this->numFonts; i++)
	{
		std::stringstream ss;
		ss << this->fontTempFolder << '\\' << fileBasename(this->splFilename) << '.' << std::setfill('0') << std::setw(5) << i << ".ttf";
		string fontFileName = ss.str();
		r = RemoveFontResourceA(fontFileName.c_str());//, FR_PRIVATE, 0) && r;
		DEBUG_MSG("unloading: %s | result: %i", fontFileName.c_str(), r);
	}
	PostMessageA(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
	return r;
}

bool EMFExtract::Cleanup()
{
	int i;
	/*for (i = 0; i < this->numFonts; i++)
	{
		std::stringstream ss;
		ss << this->fontTempFolder << '\\' << fileBasename(this->splFilename) << '.' << std::setfill('0') << std::setw(5) << i << ".ttf" << '\0';
		RemoveFontResourceA(ss.str().c_str());
		DeleteFileA(ss.str().c_str());
	}
	SendMessageA(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);*/
	for (i = 0; i < this->numPages; i++)
	{
		std::stringstream ss;		
		ss << this->splFilename << '.' << std::setfill('0') << std::setw(5) << i << ".emf" << '\0';		
		DeleteFileA(ss.str().c_str());
		
	}

	// limpando as fontes
	//if( printServer == false ) xxx
	{
		UnloadFonts();
		for( i= 0 ; i < this->numFonts; i++ ) 
		{
			std::stringstream ssFont;
			ssFont<< this->fontTempFolder << "\\"<< fileBasename(this->splFilename) << '.' << std::setfill('0') << std::setw(5) << i << ".ttf";
			DeleteFileA(ssFont.str().c_str());
		}
	}

	this->numFonts = 0;
	this->numPages = 0;
	return true;
}

int EMFExtract::GetNumPages() const
{
	return this->numPages;
}

int EMFExtract::GetNumFonts() const
{
	return this->numFonts;
}

bool EMFExtract::SPLAnalyze(const MappedFile & mf)
{
	const char * ptr = mf.contents;
	const char * ptr_end = ptr + mf.length;

	const EMFSpool * emf_spool = reinterpret_cast<const EMFSpool *>(ptr);
	DEBUG_MSG("EMFSpool->dwVersion: 0x%08x", emf_spool->dwVersion);
	DEBUG_MSG("EMFSpool->cjSize: %i", emf_spool->cjSize);
	DEBUG_MSG("EMFSpool->dpszDocName: %i", emf_spool->dpszDocName);
	DEBUG_MSG("EMFSpool->dpszOutput: %i", emf_spool->dpszOutput);
	if (emf_spool->dpszDocName)
		DEBUG_MSG("EMFSpool->docName: %S", reinterpret_cast<const wchar_t *>(reinterpret_cast<const char *>(emf_spool) + emf_spool->dpszDocName));
	if (emf_spool->dpszOutput)
		DEBUG_MSG("EMFSpool->output: %S", reinterpret_cast<const wchar_t *>(reinterpret_cast<const char *>(emf_spool) + emf_spool->dpszOutput));

	const EMFData * emf_data = NULL;
	ptr += emf_spool->cjSize;

	while (ptr < ptr_end)
	{
		emf_data = reinterpret_cast<const EMFData *>(ptr);
		switch (emf_data->type)
		{
		case EMRI_METAFILE:
			{
				const char * ptr2 = this->EMFAnalyze(ptr, ptr_end);
				if (ptr2 == NULL)
				{
					DEBUG_MSG("EMFAnalyze retornou nulo");
					return false;
				}

				if ((ptr2 - ptr) > 0)
					this->EMFCopy(ptr, ptr2 - ptr);
				//printf("2: %i\n", ptr2 - ptr);
				ptr = ptr2;
			}
			break;
		case EMRI_DEVMODE:
			{
				/*char * devmode = new char[sizeof(EMFData) + emf_data->size];
				memcpy(devmode, ptr, sizeof(EMFData) + emf_data->size);
				m_devmodes[this->numPages] = devmode;*/
				ptr += sizeof(EMFData) + emf_data->size;
			}
			break;
		case EMRI_METAFILE_DATA:
			ptr += sizeof(EMFData);
			this->EMFAnalyze(ptr, ptr + emf_data->size);
			if (emf_data->size > 0)
				this->EMFCopy(ptr, emf_data->size);
			//printf("1: %i\n", emf_data->size);
			ptr += emf_data->size;
			break;
		case EMRI_METAFILE_EXT:
			//DEBUG_MSG("jump2 %i -> 0x%08x", emf_data->size, static_cast<const EMFPageOffset *>(emf_data)->offsetLow);
			ptr += sizeof(EMFData) + emf_data->size;
			break;
		case EMRI_BW_METAFILE_EXT:
			//DEBUG_MSG("jump3 %i at 0x%08x", emf_data->size, ptr - this->commons->mappedSPL->contents);
			ptr += sizeof(EMFData) + emf_data->size;
			break;
		case EMRI_ENGINE_FONT_EXT:
			ptr += sizeof(EMFData) + emf_data->size;
			break;
		case EMRI_DELTA_FONT:
		case EMRI_DELTA_FONT_EXT:
		case EMRI_SUBSET_FONT:
		case EMRI_SUBSET_FONT_EXT:
		case EMRI_TYPE1_FONT:
		case EMRI_TYPE1_FONT_EXT:
		case EMRI_EMBED_FONT_EXT:
			ptr += sizeof(EMFData) + emf_data->size;
			break;
		case EMRI_PS_JOB_DATA:
		case EMRI_PRESTARTPAGE:
		case EMRI_DESIGNVECTOR:
		case EMRI_DESIGNVECTOR_EXT:
		case EMRI_FORM_METAFILE:
		case EMRI_BW_FORM_METAFILE:
			ptr += sizeof(EMFData) + emf_data->size;
			break;
		case 0:
			ptr++;
			break;
		default:
			ptr++;
			DEBUG_MSG("type unknown: 0x%08x at: 0x%08x", emf_data->type, ptr - mf.contents);
			return false;
		}
	}

	return true;
}

const char * EMFExtract::EMFAnalyze(const char * ptr, const char * ptr_max)
{
	const char * ptr_start = ptr;
	const EMFData * emf_data = NULL;
	const EMRComment * emr_comment = NULL;

	for (; ptr < ptr_max; ptr += emf_data->size)
	{
		emf_data = reinterpret_cast<const EMFData *>(ptr);
		DEBUG_MSG("%s at 0x%08x size 0x%x", obj_str[emf_data->type], ptr - ptr_start, emf_data->size);
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
			DEBUG_MSG("page %i", this->numPages);
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
		case _EMR_SETTEXTCOLOR:
		case _EMR_SETBKCOLOR:
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
		case _EMR_CREATEBRUSHINDIRECT:
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
			DEBUG_MSG("FontCopy");
			emr_comment = reinterpret_cast<const EMRComment *>(ptr);
			this->FontCopy(ptr + sizeof(*emr_comment), emr_comment->size);
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
		case _EMR_EXTCREATEPEN:
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
			DEBUG_MSG("type obj unknown: 0x%08x at: 0x%08x", emf_data->type, ptr - ptr_start);
			return false;
		}
	}
	//return ptr_max; // não deveria chegar aqui
	return ptr;
}

bool EMFExtract::EMFCopy(const char * ptr, size_t size)
{
	// copiar EMF (formato do nome: arquivo.spl.00001.emf)
	std::stringstream ss;
	ss << this->splFilename << '.' << std::setfill('0') << std::setw(5) << this->numPages << ".emf"; 
	FILE * f = NULL;
	if (fopen_s(&f, ss.str().c_str(), "wb") == 0)
	{
		fwrite(ptr, size, 1, f);
		fclose(f);
		this->numPages++;
		return true;
	}
	else
	{
		return false;
	}
}

bool EMFExtract::FontCopy(const char * ptr_start, int size)
{

	if ((size > 64) &&
		(strncmp(ptr_start + 4, "FNOT", 4) == 0))
	{
		if( fontTempFolder.length() < 1  ) 
		{
			fontTempFolder = LoadPrintServerTempFolder();
			printServer = true; 
		}

		int fontsize = 0;

		char * ptr_font_start = NULL;
		const char * ptr_end = ptr_start + size;
		char * d = (char*)ptr_start;

		for (; d < ptr_end; d += 4)
		{
			if (d[0] == 0x00 &&
				d[1] == 0x01 &&
				d[2] == 0x00 &&
				d[3] == 0x00)
			{
				fontsize = size - 0x0C - (d - ptr_start);
				ptr_font_start = d;
				break;
			}
		}

		// copiar fonte (formato do nome: arquivo.spl.00001.ttf)
		std::stringstream ss;
		ss << this->fontTempFolder << '\\' << fileBasename(this->splFilename) << '.' << std::setfill('0') << std::setw(5) << this->numFonts << ".ttf"; 
		FILE * f = NULL;
		if (fopen_s(&f, ss.str().c_str(), "wb") == 0)
		{
			fwrite(ptr_font_start, fontsize, 1, f);
			fclose(f);
			this->numFonts++;
			return true;
		}
	}

	return false;
}

/*std::map<int, char *> EMFExtract::GetDevmodes() const
{
	return m_devmodes;
}*/

void * EMFExtract_New()
{
	return new EMFExtract();
}

int EMFExtract_Delete(void * _this)
{
	delete static_cast<EMFExtract *>(_this);
	return true;
}

int EMFExtract_Extract(void * _this, const char * splFilename)
{
	return static_cast<EMFExtract *>(_this)->Extract(splFilename);
}

int EMFExtract_LoadFonts(void * _this)
{
	return static_cast<EMFExtract *>(_this)->LoadFonts();
}

int EMFExtract_UnloadFonts(void * _this)
{
	return static_cast<EMFExtract *>(_this)->UnloadFonts();
}

int EMFExtract_Cleanup(void * _this)
{
	return static_cast<EMFExtract *>(_this)->Cleanup();
}

int EMFExtract_GetNumPages(void * _this)
{
	return static_cast<EMFExtract *>(_this)->GetNumPages();
}

int EMFExtract_GetNumFonts(void * _this)
{
	return static_cast<EMFExtract *>(_this)->GetNumFonts();
}
