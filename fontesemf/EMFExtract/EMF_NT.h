#ifndef EMF_NT_H
#define EMF_NT_H

#include <stddef.h>

#pragma pack(1)

#pragma warning( push )
#pragma warning( disable : 4200 )

enum EMRIRecordType {
	EMRI_METAFILE         = 0x00000001,
	EMRI_ENGINE_FONT      = 0x00000002,
	EMRI_DEVMODE          = 0x00000003,
	EMRI_TYPE1_FONT       = 0x00000004,
	EMRI_PRESTARTPAGE     = 0x00000005,
	EMRI_DESIGNVECTOR     = 0x00000006,
	EMRI_SUBSET_FONT      = 0x00000007,
	EMRI_DELTA_FONT       = 0x00000008,
	EMRI_FORM_METAFILE    = 0x00000009,
	EMRI_BW_METAFILE      = 0x0000000A,
	EMRI_BW_FORM_METAFILE = 0x0000000B,
	EMRI_METAFILE_DATA    = 0x0000000C,
	EMRI_METAFILE_EXT     = 0x0000000D,
	EMRI_BW_METAFILE_EXT  = 0x0000000E,
	EMRI_ENGINE_FONT_EXT  = 0x0000000F,
	EMRI_TYPE1_FONT_EXT   = 0x00000010,
	EMRI_DESIGNVECTOR_EXT = 0x00000011,
	EMRI_SUBSET_FONT_EXT  = 0x00000012,
	EMRI_DELTA_FONT_EXT   = 0x00000013,
	EMRI_PS_JOB_DATA      = 0x00000014,
	EMRI_EMBED_FONT_EXT   = 0x00000015
};

enum EMRRecordType {
	_EMR_HEADER                  = 0x00000001,
	_EMR_POLYBEZIER              = 0x00000002,
	_EMR_POLYGON                 = 0x00000003,
	_EMR_POLYLINE                = 0x00000004,
	_EMR_POLYBEZIERTO            = 0x00000005,
	_EMR_POLYLINETO              = 0x00000006,
	_EMR_POLYPOLYLINE            = 0x00000007,
	_EMR_POLYPOLYGON             = 0x00000008,
	_EMR_SETWINDOWEXTEX          = 0x00000009,
	_EMR_SETWINDOWORGEX          = 0x0000000A,
	_EMR_SETVIEWPORTEXTEX        = 0x0000000B,
	_EMR_SETVIEWPORTORGEX        = 0x0000000C,
	_EMR_SETBRUSHORGEX           = 0x0000000D,
	_EMR_EOF                     = 0x0000000E,
	_EMR_SETPIXELV               = 0x0000000F,
	_EMR_SETMAPPERFLAGS          = 0x00000010,
	_EMR_SETMAPMODE              = 0x00000011,
	_EMR_SETBKMODE               = 0x00000012,
	_EMR_SETPOLYFILLMODE         = 0x00000013,
	_EMR_SETROP2                 = 0x00000014,
	_EMR_SETSTRETCHBLTMODE       = 0x00000015,
	_EMR_SETTEXTALIGN            = 0x00000016,
	_EMR_SETCOLORADJUSTMENT      = 0x00000017,
	_EMR_SETTEXTCOLOR            = 0x00000018,
	_EMR_SETBKCOLOR              = 0x00000019,
	_EMR_OFFSETCLIPRGN           = 0x0000001A,
	_EMR_MOVETOEX                = 0x0000001B,
	_EMR_SETMETARGN              = 0x0000001C,
	_EMR_EXCLUDECLIPRECT         = 0x0000001D,
	_EMR_INTERSECTCLIPRECT       = 0x0000001E,
	_EMR_SCALEVIEWPORTEXTEX      = 0x0000001F,
	_EMR_SCALEWINDOWEXTEX        = 0x00000020,
	_EMR_SAVEDC                  = 0x00000021,
	_EMR_RESTOREDC               = 0x00000022,
	_EMR_SETWORLDTRANSFORM       = 0x00000023,
	_EMR_MODIFYWORLDTRANSFORM    = 0x00000024,
	_EMR_SELECTOBJECT            = 0x00000025,
	_EMR_CREATEPEN               = 0x00000026,
	_EMR_CREATEBRUSHINDIRECT     = 0x00000027,
	_EMR_DELETEOBJECT            = 0x00000028,
	_EMR_ANGLEARC                = 0x00000029,
	_EMR_ELLIPSE                 = 0x0000002A,
	_EMR_RECTANGLE               = 0x0000002B,
	_EMR_ROUNDRECT               = 0x0000002C,
	_EMR_ARC                     = 0x0000002D,
	_EMR_CHORD                   = 0x0000002E,
	_EMR_PIE                     = 0x0000002F,
	_EMR_SELECTPALETTE           = 0x00000030,
	_EMR_CREATEPALETTE           = 0x00000031,
	_EMR_SETPALETTEENTRIES       = 0x00000032,
	_EMR_RESIZEPALETTE           = 0x00000033,
	_EMR_REALIZEPALETTE          = 0x00000034,
	_EMR_EXTFLOODFILL            = 0x00000035,
	_EMR_LINETO                  = 0x00000036,
	_EMR_ARCTO                   = 0x00000037,
	_EMR_POLYDRAW                = 0x00000038,
	_EMR_SETARCDIRECTION         = 0x00000039,
	_EMR_SETMITERLIMIT           = 0x0000003A,
	_EMR_BEGINPATH               = 0x0000003B,
	_EMR_ENDPATH                 = 0x0000003C,
	_EMR_CLOSEFIGURE             = 0x0000003D,
	_EMR_FILLPATH                = 0x0000003E,
	_EMR_STROKEANDFILLPATH       = 0x0000003F,
	_EMR_STROKEPATH              = 0x00000040,
	_EMR_FLATTENPATH             = 0x00000041,
	_EMR_WIDENPATH               = 0x00000042,
	_EMR_SELECTCLIPPATH          = 0x00000043,
	_EMR_ABORTPATH               = 0x00000044,
	_EMR_RESERVED_69             = 0x00000045,
	_EMR_COMMENT                 = 0x00000046,
	_EMR_FILLRGN                 = 0x00000047,
	_EMR_FRAMERGN                = 0x00000048,
	_EMR_INVERTRGN               = 0x00000049,
	_EMR_PAINTRGN                = 0x0000004A,
	_EMR_EXTSELECTCLIPRGN        = 0x0000004B,
	_EMR_BITBLT                  = 0x0000004C,
	_EMR_STRETCHBLT              = 0x0000004D,
	_EMR_MASKBLT                 = 0x0000004E,
	_EMR_PLGBLT                  = 0x0000004F,
	_EMR_SETDIBITSTODEVICE       = 0x00000050,
	_EMR_STRETCHDIBITS           = 0x00000051,
	_EMR_EXTCREATEFONTINDIRECTW  = 0x00000052,
	_EMR_EXTTEXTOUTA             = 0x00000053,
	_EMR_EXTTEXTOUTW             = 0x00000054,
	_EMR_POLYBEZIER16            = 0x00000055,
	_EMR_POLYGON16               = 0x00000056,
	_EMR_POLYLINE16              = 0x00000057,
	_EMR_POLYBEZIERTO16          = 0x00000058,
	_EMR_POLYLINETO16            = 0x00000059,
	_EMR_POLYPOLYLINE16          = 0x0000005A,
	_EMR_POLYPOLYGON16           = 0x0000005B,
	_EMR_POLYDRAW16              = 0x0000005C,
	_EMR_CREATEMONOBRUSH         = 0x0000005D,
	_EMR_CREATEDIBPATTERNBRUSHPT = 0x0000005E,
	_EMR_EXTCREATEPEN            = 0x0000005F,
	_EMR_POLYTEXTOUTA            = 0x00000060,
	_EMR_POLYTEXTOUTW            = 0x00000061,
// WINVER >= 0x0400
	_EMR_SETICMMODE              = 0x00000062,
	_EMR_CREATECOLORSPACE        = 0x00000063,
	_EMR_SETCOLORSPACE           = 0x00000064,
	_EMR_DELETECOLORSPACE        = 0x00000065,
	_EMR_GLSRECORD               = 0x00000066,
	_EMR_GLSBOUNDEDRECORD        = 0x00000067,
	_EMR_PIXELFORMAT             = 0x00000068,
// WINVER >= 0x0500
	_EMR_DRAWESCAPE              = 0x00000069,
	_EMR_EXTESCAPE               = 0x0000006A,
	_EMR_RESERVED_107            = 0x0000006B,
	_EMR_SMALLTEXTOUT            = 0x0000006C,
	_EMR_FORCEUFIMAPPING         = 0x0000006D,
	_EMR_NAMEDESCAPE             = 0x0000006E,
	_EMR_COLORCORRECTPALETTE     = 0x0000006F,
	_EMR_SETICMPROFILEA          = 0x00000070,
	_EMR_SETICMPROFILEW          = 0x00000071,
	_EMR_ALPHABLEND              = 0x00000072,
	_EMR_SETLAYOUT               = 0x00000073,
	_EMR_TRANSPARENTBLT          = 0x00000074,
	_EMR_RESERVED_117            = 0x00000075,
	_EMR_GRADIENTFILL            = 0x00000076,
	_EMR_SETLINKEDUFIS           = 0x00000077,
	_EMR_SETTEXTJUSTIFICATION    = 0x00000078,
	_EMR_COLORMATCHTOTARGETW     = 0x00000079,
	_EMR_CREATECOLORSPACEW       = 0x0000007A
};

enum StockObject {
	_WHITE_BRUSH         = 0x80000000,
	_LTGRAY_BRUSH        = 0x80000001,
	_GRAY_BRUSH          = 0x80000002,
	_DKGRAY_BRUSH        = 0x80000003,
	_BLACK_BRUSH         = 0x80000004,
	_NULL_BRUSH          = 0x80000005,
	_WHITE_PEN           = 0x80000006,
	_BLACK_PEN           = 0x80000007,
	_NULL_PEN            = 0x80000008,
	_OEM_FIXED_FONT      = 0x8000000A,
	_ANSI_FIXED_FONT     = 0x8000000B,
	_ANSI_VAR_FONT       = 0x8000000C,
	_SYSTEM_FONT         = 0x8000000D,
	_DEVICE_DEFAULT_FONT = 0x8000000E,
	_DEFAULT_PALETTE     = 0x8000000F,
	_SYSTEM_FIXED_FONT   = 0x80000010,
// WINVER >= 0x0400
	_DEFAULT_GUI_FONT    = 0x80000011,
// _WIN32_WINNT >= 0x0500
	_DC_BRUSH            = 0x80000012,
	_DC_PEN              = 0x80000013
};

enum MetafileVersion {
	META_FORMAT_WINDOWS   = 0x00000300,
	META_FORMAT_ENHANCED  = 0x00010000
};

enum ExtTextOutOptions {
	_ETO_OPAQUE            = 0x00000002,
	_ETO_CLIPPED           = 0x00000004,
	_ETO_GLYPH_INDEX       = 0x00000010,
	_ETO_RTLREADING        = 0x00000080,
	_ETO_NO_RECT           = 0x00000100,
	_ETO_SMALL_CHARS       = 0x00000200,
	_ETO_NUMERICSLOCAL     = 0x00000400,
	_ETO_NUMERICSLATIN     = 0x00000800,
	_ETO_IGNORELANGUAGE    = 0x00001000,
	_ETO_PDY               = 0x00002000,
	_ETO_REVERSE_INDEX_MAP = 0x00010000
};

struct RectL {
	unsigned __int32 left;
	unsigned __int32 top;
	unsigned __int32 right;
	unsigned __int32 bottom;
};

struct SizeL {
	unsigned __int32 cx;
	unsigned __int32 cy;
};

struct PointL {
	unsigned __int32 x;
	unsigned __int32 y;
};

struct ColorRef {
	unsigned __int8 red;
	unsigned __int8 green;
	unsigned __int8 blue;
	unsigned __int8 reserved;
};

struct EMFSpool {
	unsigned __int32 dwVersion;
	unsigned __int32 cjSize;
	unsigned __int32 dpszDocName;
	unsigned __int32 dpszOutput;
	wchar_t extraData[];
};

struct EMFData {
	unsigned __int32 type;
	unsigned __int32 size;
};

struct EMRHeader : EMFData {
	RectL bounds;
	RectL frame;
	union {
		unsigned __int32 signature; // 0x464d4520
		char signature_str[4]; // " EMF"
	};
	unsigned __int32 version;
	unsigned __int32 bytes;
	unsigned __int32 records;
	unsigned __int16 handles;
	unsigned __int16 reserved;
	unsigned __int32 nDescription;
	unsigned __int32 offDescription;
	unsigned __int32 nPalEntries;
	SizeL device;
	SizeL milimeters;
};

struct EMRHeaderExt1 : EMRHeader {
	unsigned __int32 cbPixelFormat;
	unsigned __int32 offPixelFormat;
	unsigned __int32 bOpenGl;
};

struct EMRHeaderExt2 : EMRHeaderExt1 {
	unsigned __int32 micrometersX;
	unsigned __int32 micrometersY;
};

struct EMFPageOffset : EMFData {
	unsigned __int32 offsetLow;
	unsigned __int32 offsetHigh;
};

struct EMRSelectObject : EMFData {
	unsigned __int32 ihObject; // StockObject
};

struct EMRSetColorSpace : EMFData {
	unsigned __int32 ihCS;
};

struct EMRText {
	PointL reference;
	unsigned __int32 chars;
	unsigned __int32 offString;
	unsigned __int32 options;
	RectL rectangle;
	unsigned __int32 offDx;
	__int8 var[];
};

struct EMRExtTextOutW : EMFData {
	RectL bounds;
	unsigned __int32 iGraphicsMode;
	unsigned __int32 exScale;
	unsigned __int32 eyScale;
	EMRText emrText;
};

struct EMRSetTextColor : EMFData {
	ColorRef color;
};

struct LogBrushEx {
	unsigned __int32 brushStyle;
	ColorRef color;
	unsigned __int32 brushHatch;
};

struct EMRCreateBrushIndirect : EMFData {
	unsigned __int32 ihBrush;
	LogBrushEx logBrush;
};

struct LogPenEx {
	unsigned __int32 penStyle;
	unsigned __int32 width;
	unsigned __int32 brushStyle;
	ColorRef color;
	unsigned __int32 brushHatch;
	unsigned __int32 numStylesEntries;
	__int8 styleEntry[];
};

struct EMRExtCreatePen : EMFData {
	unsigned __int32 ihPen;
	unsigned __int32 offBmi;
	unsigned __int32 cbBmi;
	unsigned __int32 offBits;
	unsigned __int32 cbBits;
	LogPenEx elp;
};

struct EMRSetBkColor : EMFData {
	ColorRef color;
};

struct EMRSmallTextOutBase : EMFData {
	unsigned __int32 x;
	unsigned __int32 y;
	unsigned __int32 cChars;
	unsigned __int32 fuOptions; // ExtTextOutOptions
	unsigned __int32 iGraphicsMode;
	unsigned __int32 exScale;
	unsigned __int32 eyScale;
};

struct EMRSmallTextOut : EMRSmallTextOutBase {
	union {
		char textString[];
		wchar_t wTextString[];
	};
};

struct EMRSmallTextOutBounds : EMRSmallTextOutBase {
	RectL bounds;
	union {
		char textString[];
		wchar_t wTextString[];
	};
};

struct EMRStretchDIBits : EMFData {
	RectL bounds;
	unsigned __int32 xDest;
	unsigned __int32 yDest;
	unsigned __int32 xSrc;
	unsigned __int32 ySrc;
	unsigned __int32 cxSrc;
	unsigned __int32 cySrc;
	unsigned __int32 offBmiSrc;
	unsigned __int32 cbBmiSrc;
	unsigned __int32 offBitsSrc;
	unsigned __int32 cbBitsSrc;
	unsigned __int32 usageSrc;
	unsigned __int32 bitBltRasterOperation;
	unsigned __int32 cxDest;
	unsigned __int32 cyDest;
	__int8 bmiSrc[];
};

struct EMRExtEscape : EMFData {
	unsigned __int32 iEscape;
	unsigned __int32 cjIn;
	unsigned __int16 dataSize; // passthrough
	__int8 data[];;
};

struct BitmapCoreHeader {
	unsigned __int32 headerSize;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 planes;
	unsigned __int16 bitCount;
};

struct BitmapInfoHeader {
	unsigned __int32 headerSize;
	__int32 width; // deve ser positivo
	__int32 height; // não pode ser zero, se for negativo, a imagem é invertida
	unsigned __int16 planes;
	unsigned __int16 bitCount;
	unsigned __int32 compression; // se for JPEG/PNG a imagem deve ser bottom-top
	unsigned __int32 imageSize;
	unsigned __int32 xPelsPerMeter;
	unsigned __int32 yPelsPerMeter;
	unsigned __int32 colorUsed;
	unsigned __int32 colorImportant;
};

struct EMRComment : EMFData {
	unsigned __int32 dataSize;
};

static const char * obj_str[] = {
	NULL,
	"EMR_HEADER",
	"EMR_POLYBEZIER",
	"EMR_POLYGON",
	"EMR_POLYLINE",
	"EMR_POLYBEZIERTO",
	"EMR_POLYLINETO",
	"EMR_POLYPOLYLINE",
	"EMR_POLYPOLYGON",
	"EMR_SETWINDOWEXTEX",
	"EMR_SETWINDOWORGEX",
	"EMR_SETVIEWPORTEXTEX",
	"EMR_SETVIEWPORTORGEX",
	"EMR_SETBRUSHORGEX",
	"EMR_EOF",
	"EMR_SETPIXELV",
	"EMR_SETMAPPERFLAGS",

	"EMR_SETMAPMODE",
	"EMR_SETBKMODE",
	"EMR_SETPOLYFILLMODE",
	"EMR_SETROP2",
	"EMR_SETSTRETCHBLTMODE",
	"EMR_SETTEXTALIGN",
	"EMR_SETCOLORADJUSTMENT",
	"EMR_SETTEXTCOLOR",
	"EMR_SETBKCOLOR",
	"EMR_OFFSETCLIPRGN",
	"EMR_MOVETOEX",
	"EMR_SETMETARGN",
	"EMR_EXCLUDECLIPRECT",
	"EMR_INTERSECTCLIPRECT",
	"EMR_SCALEVIEWPORTEXTEX",
	"EMR_SCALEWINDOWEXTEX",

	"EMR_SAVEDC",
	"EMR_RESTOREDC",
	"EMR_SETWORLDTRANSFORM",
	"EMR_MODIFYWORLDTRANSFORM",
	"EMR_SELECTOBJECT",
	"EMR_CREATEPEN",
	"EMR_CREATEBRUSHINDIRECT",
	"EMR_DELETEOBJECT",
	"EMR_ANGLEARC",
	"EMR_ELLIPSE",
	"EMR_RECTANGLE",
	"EMR_ROUNDRECT",
	"EMR_ARC",
	"EMR_CHORD",
	"EMR_PIE",
	"EMR_SELECTPALETTE",

	"EMR_CREATEPALETTE",
	"EMR_SETPALETTEENTRIES",
	"EMR_RESIZEPALETTE",
	"EMR_REALIZEPALETTE",
	"EMR_EXTFLOODFILL",
	"EMR_LINETO",
	"EMR_ARCTO",
	"EMR_POLYDRAW",
	"EMR_SETARCDIRECTION",
	"EMR_SETMITERLIMIT",
	"EMR_BEGINPATH",
	"EMR_ENDPATH",
	"EMR_CLOSEFIGURE",
	"EMR_FILLPATH",
	"EMR_STROKEANDFILLPATH",
	"EMR_STROKEPATH",

	"EMR_FLATTENPATH",
	"EMR_WIDENPATH",
	"EMR_SELECTCLIPPATH",
	"EMR_ABORTPATH",
	"EMR_RESERVED_69",
	"EMR_COMMENT",
	"EMR_FILLRGN",
	"EMR_FRAMERGN",
	"EMR_INVERTRGN",
	"EMR_PAINTRGN",
	"EMR_EXTSELECTCLIPRGN",
	"EMR_BITBLT",
	"EMR_STRETCHBLT",
	"EMR_MASKBLT",
	"EMR_PLGBLT",
	"EMR_SETDIBITSTODEVICE",

	"EMR_STRETCHDIBITS",
	"EMR_EXTCREATEFONTINDIRECTW",
	"EMR_EXTTEXTOUTA",
	"EMR_EXTTEXTOUTW",
	"EMR_POLYBEZIER16",
	"EMR_POLYGON16",
	"EMR_POLYLINE16",
	"EMR_POLYBEZIERTO16",
	"EMR_POLYLINETO16",
	"EMR_POLYPOLYLINE16",
	"EMR_POLYPOLYGON16",
	"EMR_POLYDRAW16",
	"EMR_CREATEMONOBRUSH",
	"EMR_CREATEDIBPATTERNBRUSHPT",
	"EMR_EXTCREATEPEN",
	"EMR_POLYTEXTOUTA",

	"EMR_POLYTEXTOUTW",
	"EMR_SETICMMODE",
	"EMR_CREATECOLORSPACE",
	"EMR_SETCOLORSPACE",
	"EMR_DELETECOLORSPACE",
	"EMR_GLSRECORD",
	"EMR_GLSBOUNDEDRECORD",
	"EMR_PIXELFORMAT",
	"EMR_DRAWESCAPE",
	"EMR_EXTESCAPE",
	"EMR_RESERVED_107",
	"EMR_SMALLTEXTOUT",
	"EMR_FORCEUFIMAPPING",
	"EMR_NAMEDESCAPE",
	"EMR_COLORCORRECTPALETTE",
	"EMR_SETICMPROFILEA",

	"EMR_SETICMPROFILEW",
	"EMR_ALPHABLEND",
	"EMR_SETLAYOUT",
	"EMR_TRANSPARENTBLT",
	"EMR_RESERVED_117",
	"EMR_GRADIENTFILL",
	"EMR_SETLINKEDUFIS",
	"EMR_SETTEXTJUSTIFICATION",
	"EMR_COLORMATCHTOTARGETW",
	"EMR_CREATECOLORSPACEW"
};

struct OEMDriverExtraData {
	unsigned __int32 dwSize;
	unsigned __int32 dwSignature;
	unsigned __int32 dwVersion;
	//char data[];
};

struct PSDriverExtraData {
	unsigned __int32 dwSignature; // PRIV
	unsigned __int32 dwFlags;
	unsigned __int32 wchEPSFile[20];
	unsigned __int16 caSize;
	unsigned __int16 caFlags;
	unsigned __int16 caIlluminantIndex;
	unsigned __int16 caRedGamma;
	unsigned __int16 caGreenGamma;
	unsigned __int16 caBlueGamma;
	unsigned __int16 caReferenceBlack;
	unsigned __int16 caReferenceWhite;
	unsigned __int16 caContrast;
	unsigned __int16 caBrightness;
	unsigned __int16 caColorfulness;
	unsigned __int16 caRegGreenTint;
	unsigned __int16 wCoreJTExpSize;
	unsigned __int16 wCoreFullSize;
	unsigned __int32 fxScrFreq;
	unsigned __int32 fxScrAngle;
	unsigned __int32 iDialect;
	unsigned __int32 iTTDLFmt;
	unsigned __int32 bReversePrint;
	unsigned __int32 iLayout;
	unsigned __int32 iPSLevel;
	unsigned __int32 dwReserved2;
	unsigned __int16 wOEMExtra;
	unsigned __int16 wVer;
	unsigned __int32 dwX;
	unsigned __int32 dwY;
	unsigned __int32 dwWidthOffset;
	unsigned __int32 dwHeightOffset;
	unsigned __int16 wFeedDirection;
	unsigned __int16 wCutSheet;
	unsigned __int32 dwReserved3[4];
	unsigned __int32 dwChecksum32;
	unsigned __int32 dwOptions;
	unsigned __int32 aOptions[128];
	unsigned __int32 dwNupDirection;
	unsigned __int32 dwNupBorderFlags;
	unsigned __int32 dwBookletFlags;
	unsigned __int32 dwPadding;
};

enum SpecVersion {
	__WIN32_WINNT_NT4    = 0x0400,
	__WIN32_WINNT_WIN2K  = 0x0500,
	__WIN32_WINNT_WINXP  = 0x0501,
	__WIN32_WINNT_WS03   = 0x0502,
	__WIN32_WINNT_VISTA  = 0x0600
};

struct PSDriverExtraData_0x0500 { // xerox phaser
	unsigned __int32 dwSignature; // PRIV
	unsigned __int32 flags;
	__int8 unknown[0x19c];
	unsigned __int32 iLayout;
	unsigned __int32 unknown2[9];
	unsigned __int16 paperSize;
};

struct UNIDDriverExtraData {
	unsigned __int32 dwSignature; // DINU
	unsigned __int16 wVer;
	unsigned __int16 wCoreJTExpSize ;
	unsigned __int16 wCoreFullSize;
	unsigned __int16 wOEMExtra;
	unsigned __int32 dwChecksum32;
	unsigned __int32 dwFlags;
	unsigned __int32 bReversePrint;
	unsigned __int32 iLayout;
	unsigned __int32 iQuality;
	unsigned __int32 wReserved[3];
	unsigned __int32 dwOptions;
	unsigned __int32 aOptions[128];
	unsigned __int32 dwNupDirection;
	unsigned __int32 dwNupBorderFlags;
	unsigned __int32 dwBookletFlags;
};

struct SHARPDriverExtraData {
	unsigned __int32 signature; // \0\0SC
	unsigned __int16 version; // PI
	unsigned __int16 oemExtra;
};

struct SHARPDriverExtraData_0x02d6 {
	SHARPDriverExtraData header;
	__int8 unknown[0xf2];
	unsigned __int16 horizontalNup;
	unsigned __int16 verticalNup;
};

struct SHARPDriverExtraData_0x031a {
	SHARPDriverExtraData header;
	__int8 unknown[0xfa];
	unsigned __int16 horizontalNup;
	unsigned __int16 verticalNup;
};

///// HP Structs /////
struct DriverExtraData_0x4008 {
	__int8 unknown[5];
	unsigned __int8 printQuality;
	__int8 unknown2[5];
	unsigned __int8 grayscale;
	__int8 unknown3[0x0e];
	unsigned __int16 zoomSmartPaperSize;
	unsigned __int16 paperSize;
	__int8 unknown4[0x02];
	struct {
		unsigned __int32 :1;
		unsigned __int32 zoomSmart:1;
		unsigned __int32 :1;
		unsigned __int32 tiling:1;
		unsigned __int32 multiup:1;
		unsigned __int32 :3;
		unsigned __int32 twoTiles:1; // mesmo que tiling porém usado apenas para dois tiles (1 x 2)
		unsigned __int32 :11;
		unsigned __int32 useWatermark:1;
		unsigned __int32 optimisedForFax:1;
		unsigned __int32 imageEnhancement:1;
	} flags;
	unsigned __int8 multiup;
	__int8 unknown5[2];
	struct {
		unsigned __int16 angle:9;
		unsigned __int16 justifyAcrossPage:1;
		unsigned __int16 firstPageOnly:1;
		unsigned __int16 horizontal:1; // em diagonal por default
		unsigned __int16 useAngle:1;
	} watermarkFlags;
	__int8 unknown6[3];
	unsigned __int32 tilingPagesUsed;
	struct {
		__int8 unknown;
		struct {
			wchar_t face[32];
			struct {
				unsigned __int16 bold:1;
				unsigned __int16 italic:1;
			} flags;
			ColorRef color; // definição por intensidade de luz (0, 0, 0 = black)
			__int8 size;
		} font;
		__int8 unknown2;
		wchar_t text[0x2c];
	} watermark;
	__int8 unknown7[0x1f7];
	unsigned __int16 tilingType;
	unsigned __int16 duplex;
};

struct DriverExtraDataHeader_0x4021 {
	unsigned __int32 unknown[2];
	struct {
		unsigned __int8 multiup:1;
		unsigned __int8 tiling:1;
	} flags;
	union {
		unsigned __int8 multiup;
		unsigned __int8 tiling;
	};
	__int8 data[];
};

struct DriverExtraData_0x4021 {
	__int8 unknown2[0x29];
	unsigned __int8 paperSize;
	unsigned __int32 paperSize2;
	__int8 unknown3[0x38];
	struct {
		unsigned __int32 :6;
		unsigned __int32 monoColorText:1;
		unsigned __int32 :13;
		unsigned __int32 sepia:1;
	} colorFlags;
	struct {
		unsigned __int32 unknown;
		struct {
			unsigned __int32 :8;
			unsigned __int32 active:1;
			unsigned __int32 :7;
			unsigned __int32 allPages:1; // se for 0, somente na primeira página
		} flags;
		unsigned __int32 unknown2;
		wchar_t text[32];
		wchar_t fontFace[32];
		ColorRef color; // definição por intensidade de luz (0, 0, 0 = black)
	} watermark;
	__int8 unknown4[0x658];
	unsigned __int32 xResolution;
	unsigned __int32 yResolution;
};

union HPUIDriverExtraData_0x0002 { // signature = 0x48505549 (IUPH)
	struct {
		OEMDriverExtraData oem;
		__int8 unknown[0x24];
		unsigned __int16 layout; // 1 = PORTRAIT, 2 = LANDSCAPE
		unsigned __int16 duplex;
		unsigned __int32 unknown2[2];
		unsigned __int32 colorSpace; // 2 = CMYK, 1 = Gray;
		unsigned __int32 unknown3[2];
		wchar_t formName[16];
		__int8 unknown4[0x3c];
		unsigned __int32 blackOnly;
		unsigned __int32 unknown1;
		struct {
			wchar_t text[0x100];
			wchar_t text2[0x100];
			struct {
				wchar_t face[0x100];
				unsigned __int32 unknown;
				unsigned __int32 size;
				unsigned __int32 unknown2;
				unsigned __int32 shading; // sombreamento
				struct {
					unsigned __int32 bold:1;
					unsigned __int32 italic:1;
				} flags;
				unsigned __int32 unknown3;
				unsigned __int32 color;
			} font;
			unsigned __int32 unknown[2];
			unsigned __int32 enabled;
			unsigned __int32 enabled2;
			unsigned __int32 unknown2[4];
			struct {
				unsigned __int32 firstPageOnly:1;
			} flags;
		} watermark;
		unsigned __int32 borderOnNup;
		unsigned __int32 nupOrder;
		unsigned __int32 nupRotate180;
		__int8 unknown5[0x11];
		unsigned __int32 multiup;
		__int8 unknown6[0x1c];
		unsigned __int32 paperSize;
		__int8 unknown7[4];
		wchar_t user[0x100];
		wchar_t unknown8[0x100];
	};
	__int8 raw[0x127c];
};

struct HPUIDriverExtraData_0x0008 { // signature = 0x48505549 (IUPH)
	OEMDriverExtraData oem;
	__int8 unknown[0x26];
	unsigned __int16 duplex;
	wchar_t formName[16];
	__int8 unknown3[0x44];
	struct {
		wchar_t text[0x100];
		wchar_t text2[0x100];
		struct {
			wchar_t face[0x100];
			unsigned __int32 unknown;
			unsigned __int32 size;
			unsigned __int32 unknown2;
			unsigned __int32 shading; // sombreamento
			struct {
				unsigned __int32 bold:1;
				unsigned __int32 italic:1;
			} flags;
			unsigned __int32 unknown3;
			unsigned __int32 color;
		} font;
		unsigned __int32 unknown[8];
		struct {
			unsigned __int32 firstPageOnly:1;
		} flags;
	} watermark;
	__int8 unknown4[0x34];
	unsigned __int32 paperSize;
};

struct HPUIDriverExtraData_0x0010 { // signature = 0x48505549 (IUPH)
	OEMDriverExtraData oem;
	__int8 unknown[0x26];
	unsigned __int16 duplex;
	__int8 unknown2[0x14];
	wchar_t formName[16];
	__int8 unknown3[0x84];
	struct {
		wchar_t text[0x100];
		wchar_t text2[0x100];
		struct {
			wchar_t face[0x100];
			unsigned __int32 unknown;
			unsigned __int32 size;
			unsigned __int32 unknown2;
			unsigned __int32 shading; // sombreamento
			struct {
				unsigned __int32 bold:1;
				unsigned __int32 italic:1;
			} flags;
			unsigned __int32 unknown3;
			unsigned __int32 color;
		} font;
		unsigned __int32 unknown[8];
		struct {
			unsigned __int32 firstPageOnly:1;
		} flags;
	} watermark;
	__int8 unknown4[0x34];
	unsigned __int32 paperSize;
};
///// HP Structs /////

#pragma warning( pop )
#pragma pack()

#endif // EMF_NT_H
