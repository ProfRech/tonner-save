#include "EMFPrinter.h"
#include "EMFExtract.h"
#include "PrinterSettings.h"
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <time.h>
//#include <gdiplus.h>

#include <windows.h>
#include <stdio.h>
//using namespace Gdiplus;

/*#define DEBUG_MSG(fmt, ...) \
	do { \
		{\
		FILE * f = fopen(  "C:\\emfprintertrace.txt" , "a+t"); \
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

#include "XYTrace.h"
#define DEBUG_MSG(fmt, ...) \
			WriteTrace( __FUNCTION__ " - " fmt , ## __VA_ARGS__ );


EMFPrinter::EMFPrinter() : m_multiup(NUP_1), m_enableBorder(false)
{
	StartTrace();
	DEBUG_MSG("inicializando EMFPrinter.dll compilada em %s %s", __DATE__, __TIME__ );
}

EMFPrinter::~EMFPrinter()
{
	CloseTrace();
}

void EMFPrinter::SetPagesPerSheet(MultiupType multiup)
{
	m_multiup = multiup;
}

void EMFPrinter::SetPrinterBorder(bool enableBorder)
{
	m_enableBorder = enableBorder;
}

int CALLBACK EnumEnhProc(
  HDC hDC,                      // handle to DC
  HANDLETABLE *lpHTable,        // metafile handle table
  CONST ENHMETARECORD *lpEMFR,  // metafile record
  int nObj,                     // count of objects
  LPARAM lpData                 // optional data
)
{  
	//printf( "\n%i", lpEMFR->iType ); 
	//if( lpEMFR->iType!= 70 && EMR_MODIFYWORLDTRANSFORM != lpEMFR->iType && EMR_SETLAYOUT != lpEMFR->iType && 
	//	EMR_SETVIEWPORTORGEX !=  lpEMFR->iType )

		PlayEnhMetaFileRecord( hDC, lpHTable , lpEMFR , nObj ); 
	//else
	//	printf( "\nGDI OBJECT"); 

	return 1; 
}

#if 0
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
	   printf("%S\n", pImageCodecInfo[j].MimeType);
   }

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}
#endif

void ShowDeviceValues( HDC hdc )
{
DWORD vHORZSIZE		= GetDeviceCaps( hdc,     4   );  /* Horizontal size in millimeters           */
DWORD  vVERTSIZE	= GetDeviceCaps( hdc,         6  );/* Vertical size in millimeters             */
DWORD  vHORZRES     = GetDeviceCaps( hdc,       8 );    /* Horizontal width in pixels               */
DWORD  vVERTRES     = GetDeviceCaps( hdc,       10);    /* Vertical height in pixels                */
DWORD  vBITSPIXEL   = GetDeviceCaps( hdc,       12);    /* Number of bits per pixel                 */
DWORD  vPLANES      = GetDeviceCaps( hdc,       14);    /* Number of planes                         */
DWORD  vNUMBRUSHES  = GetDeviceCaps( hdc,       16);    /* Number of brushes the device has         */
DWORD  vNUMPENS     = GetDeviceCaps( hdc,       18);    /* Number of pens the device has            */
DWORD  vNUMMARKERS  = GetDeviceCaps( hdc,       20);    /* Number of markers the device has         */
DWORD  vNUMFONTS    = GetDeviceCaps( hdc,       22);    /* Number of fonts the device has           */
DWORD  vNUMCOLORS   = GetDeviceCaps( hdc,       24);    /* Number of colors the device supports     */
DWORD  vPDEVICESIZE = GetDeviceCaps( hdc,       26);    /* Size required for device descriptor      */
DWORD  vCURVECAPS   = GetDeviceCaps( hdc,       28);    /* Curve capabilities                       */
DWORD  vLINECAPS    = GetDeviceCaps( hdc,       30);    /* Line capabilities                        */
DWORD  vPOLYGONALCAPS = GetDeviceCaps( hdc,     32);    /* Polygonal capabilities                   */
DWORD  vTEXTCAPS     = GetDeviceCaps( hdc,      34);    /* Text capabilities                        */
DWORD  vCLIPCAPS   = GetDeviceCaps( hdc,        36);    /* Clipping capabilities                    */
DWORD  vRASTERCAPS = GetDeviceCaps( hdc,        38);    /* Bitblt capabilities                      */
DWORD  vASPECTX    = GetDeviceCaps( hdc,        40);    /* Length of the X leg                      */
DWORD  vASPECTY    = GetDeviceCaps( hdc,        42);    /* Length of the Y leg                      */
DWORD  vASPECTXY   = GetDeviceCaps( hdc,        44);    /* Length of the hypotenuse                 */

DWORD  vLOGPIXELSX = GetDeviceCaps( hdc,        88);    /* Logical pixels/inch in X                 */
DWORD  vLOGPIXELSY = GetDeviceCaps( hdc,        90);    /* Logical pixels/inch in Y                 */

DWORD  vSIZEPALETTE = GetDeviceCaps( hdc,      104);    /* Number of entries in physical palette    */
DWORD  vNUMRESERVED = GetDeviceCaps( hdc,      106);    /* Number of reserved entries in palette    */
DWORD  vCOLORRES    = GetDeviceCaps( hdc,      108);    /* Actual color resolution                  */



}

typedef struct _UpdateRect {
        double  top;
        double  bottom;
        double  left;
        double  right;
}  UpdateRect;

// The update factors for the different nup options. These factors when multiplied
// with the horizontal and vertical resolutions give the coordinates for the rectangle
// where the EMF page is to be played.

UpdateRect URect21[] = {{0, 0.5, 0, 1},
                        {0.5, 1, 0, 1}};


UpdateRect URect21RR[] = {{0.5, 1, 0, 1},
                         {0, 0.5, 0, 1}};  // ORIGINAL (funciona retrato / nao funciona paisagem)
						 

UpdateRect URect21RP[] = {{0, 0.5, 0, 1}, // (nao funciona retrato / funciona paisagem)
						 {0.5, 1, 0, 1}};
						 

UpdateRect URect22[] = {{0, 1, 0, 0.5},
                        {0, 1, 0.5, 1}};

UpdateRect URect4[] = {{0, 0.5, 0, 0.5},
                       {0, 0.5, 0.5, 1},
                       {0.5, 1, 0, 0.5},
                       {0.5, 1, 0.5, 1}};

UpdateRect URect4R[] = {{0.5, 1, 0, 0.5},
                       {0, 0.5, 0, 0.5},
                       {0.5, 1, 0.5, 1},
                       {0, 0.5, 0.5, 1}};

UpdateRect URect61[] = {{0, 1.0/3.0, 0, 0.5},
                        {0, 1.0/3.0, 0.5, 1},
                        {1.0/3.0, 2.0/3.0, 0, 0.5},
                        {1.0/3.0, 2.0/3.0, 0.5, 1},
                        {2.0/3.0, 1, 0, 0.5},
                        {2.0/3.0, 1, 0.5, 1}};

UpdateRect URect61R[] = {{2.0/3.0, 1, 0, 0.5},
                         {1.0/3.0, 2.0/3.0, 0, 0.5},
                         {0, 1.0/3.0, 0, 0.5},
                         {2.0/3.0, 1, 0.5, 1},
                         {1.0/3.0, 2.0/3.0, 0.5, 1},
                         {0, 1.0/3.0, 0.5, 1}};

UpdateRect URect61P[] = {{0, 1.0/3.0, 0, 0.5},
                         {0, 1.0/3.0, 0.5, 1},
                         {1.0/3.0, 2.0/3.0, 0, 0.5},
                         {1.0/3.0, 2.0/3.0, 0.5, 1},
                         {2.0/3.0, 1, 0, 0.5},
                         {2.0/3.0, 1, 0.5, 1}};

UpdateRect URect62[]  = {{0, 0.5, 0, 1.0/3.0},
                         {0, 0.5, 1.0/3.0, 2.0/3.0},
                         {0, 0.5, 2.0/3.0, 1},
                         {0.5, 1, 0, 1.0/3.0},
                         {0.5, 1, 1.0/3.0, 2.0/3.0},
                         {0.5, 1, 2.0/3.0, 1}};

UpdateRect URect62R[] = {{0.5, 1, 0, 1.0/3.0},
                         {0, 0.5, 0, 1.0/3.0},
                         {0.5, 1, 1.0/3.0, 2.0/3.0},
                         {0, 0.5, 1.0/3.0, 2.0/3.0},
                         {0.5, 1, 2.0/3.0, 1},
                         {0, 0.5, 2.0/3.0, 1}};

UpdateRect URect9[] = {{0, 1.0/3.0, 0, 1.0/3.0},
                       {0, 1.0/3.0, 1.0/3.0, 2.0/3.0},
                       {0, 1.0/3.0, 2.0/3.0, 1},
                       {1.0/3.0, 2.0/3.0, 0, 1.0/3.0},
                       {1.0/3.0, 2.0/3.0, 1.0/3.0, 2.0/3.0},
                       {1.0/3.0, 2.0/3.0, 2.0/3.0, 1},
                       {2.0/3.0, 1, 0, 1.0/3.0},
                       {2.0/3.0, 1, 1.0/3.0, 2.0/3.0},
                       {2.0/3.0, 1, 2.0/3.0, 1}};

UpdateRect URect9P[] = {{2.0/3.0, 1, 0, 1.0/3.0},
						{1.0/3.0, 2.0/3.0, 0, 1.0/3.0},
						{0, 1.0/3.0, 0, 1.0/3.0},
						{2.0/3.0, 1, 1.0/3.0, 2.0/3.0},
						{1.0/3.0, 2.0/3.0, 1.0/3.0, 2.0/3.0},
						{0, 1.0/3.0, 1.0/3.0, 2.0/3.0},
						{2.0/3.0, 1, 2.0/3.0, 1},
						{1.0/3.0, 2.0/3.0, 2.0/3.0, 1},
						{0, 1.0/3.0, 2.0/3.0, 1}};

UpdateRect URect16[] = {{0, 0.25, 0, 0.25},
                        {0, 0.25, 0.25, 0.5},
                        {0, 0.25, 0.5, 0.75},
                        {0, 0.25, 0.75, 1},
                        {0.25, 0.5, 0, 0.25},
                        {0.25, 0.5, 0.25, 0.5},
                        {0.25, 0.5, 0.5, 0.75},
                        {0.25, 0.5, 0.75, 1},
                        {0.5, 0.75, 0, 0.25},
                        {0.5, 0.75, 0.25, 0.5},
                        {0.5, 0.75, 0.5, 0.75},
                        {0.5, 0.75, 0.75, 1},
                        {0.75, 1, 0, 0.25},
                        {0.75, 1, 0.25, 0.5},
                        {0.75, 1, 0.5, 0.75},
                        {0.75, 1, 0.75, 1}};

UpdateRect URect16P[] = {{0.75, 1, 0, 0.25},
						 {0.5, 0.75, 0, 0.25},
						 {0.25, 0.5, 0, 0.25},
						 {0, 0.25, 0, 0.25},
						 {0.75, 1, 0.25, 0.5},
						 {0.5, 0.75, 0.25, 0.5},
						 {0.25, 0.5, 0.25, 0.5},
						 {0, 0.25, 0.25, 0.5},
						 {0.75, 1, 0.5, 0.75},
						 {0.5, 0.75, 0.5, 0.75},
						 {0.25, 0.5, 0.5, 0.75},
						 {0, 0.25, 0.5, 0.75},
						 {0.75, 1, 0.75, 1},
						 {0.5, 0.75, 0.75, 1},
						 {0.25, 0.5, 0.75, 1},
						 {0, 0.25, 0.75, 1}};


void
GetPageCoordinatesForNUp(
    HDC    hPrinterDC,
    RECT   *rectDocument,
    RECT   *rectBorder,
    DWORD  dwTotalNumberOfPages,
    UINT   uCurrentPageNumber,
    DWORD  dwNupBorderFlags,
    LPBOOL pbRotate,
	DWORD  dwAngle)

/*++
Function Description: GetPageCoordinatesForNUp computes the rectangle on the Page where the
                      EMF file is to be played. It also determines if the picture is to
                      rotated.

Parameters:  hPrinterDC           - Printer Device Context
             *rectDocument        - pointer to RECT where the coordinates to play the
                                     page will be returned.
             *rectBorder          - pointer to RECT where the page borders are to drawn.
             dwTotalNumberOfPages - Total number of pages on 1 side.
             uCurrentPageNumber   - 1 based page number on the side.
             dwNupBorderFlags     - flags to draw border along logical pages.
             pbRotate             - pointer to BOOL which indicates if the picture must be
                                    rotated.
			 dwAngle			  - 1 paisagem / 0 retrato

Return Values:  NONE.
--*/

{
	DEBUG_MSG("angle => %d", dwAngle);

	UpdateRect  *URect;
	LONG        lXPrintPage,lYPrintPage,lXPhyPage,lYPhyPage,lXFrame,lYFrame,ltemp,ldX,ldY;
	LONG        lXNewPhyPage,lYNewPhyPage,lXOffset,lYOffset,lNumRowCol,lRowIndex,lColIndex;
	double      dXleft,dXright,dYtop,dYbottom;
	LONG        xResolution = GetDeviceCaps(hPrinterDC, LOGPIXELSX);
	LONG        yResolution = GetDeviceCaps(hPrinterDC, LOGPIXELSY);

	// Get the 0-based array index for the current page

	uCurrentPageNumber = uCurrentPageNumber - 1;

	if (dwTotalNumberOfPages==1 || xResolution==yResolution) 
	{
		xResolution = yResolution = 1;
	}

	rectDocument->top = rectDocument->bottom = lYPrintPage = (GetDeviceCaps(hPrinterDC, DESKTOPVERTRES)-1) * xResolution;
	rectDocument->left = rectDocument->right = lXPrintPage = (GetDeviceCaps(hPrinterDC, DESKTOPHORZRES)-1) * yResolution;

	lXPhyPage = GetDeviceCaps(hPrinterDC, PHYSICALWIDTH)  * yResolution;
	lYPhyPage = GetDeviceCaps(hPrinterDC, PHYSICALHEIGHT) * xResolution;

	*pbRotate = FALSE;

	// Select the array containing the update factors

	DEBUG_MSG("lXPrintPage %d | lYPrintPage %d", lXPrintPage, lYPrintPage);

	switch (dwTotalNumberOfPages)
	{
	case 1:
		rectDocument->top = rectDocument->left = 0;
		rectDocument->right += 1;
		rectDocument->bottom += 1;
		return;

	case 2:
		if (lXPrintPage > lYPrintPage)
		{  // cut vertically
			URect = URect22;
			lXFrame = (LONG) (lXPrintPage / 2.0);
			lYFrame = lYPrintPage;
		}
		else
		{                          // cut horizontally
			URect = URect21;
			lYFrame = (LONG) (lYPrintPage / 2.0);
			lXFrame = lXPrintPage;
		}
		break;

	case 4:
		if (dwAngle == 1)
			URect = URect4R; // paisagem
		else
			URect = URect4; // retrato

		lXFrame = (LONG) (lXPrintPage / 2.0);
		lYFrame = (LONG) (lYPrintPage / 2.0);
		break;

	case 6:
		if (lXPrintPage > lYPrintPage)
		{  // cut vertically twice
			URect = URect62;
			lXFrame = (LONG) (lXPrintPage / 3.0);
			lYFrame = (LONG) (lYPrintPage / 2.0);
		}
		else
		{                          // cut horizontally twice
			URect = URect61;
			lYFrame = (LONG) (lYPrintPage / 3.0);
			lXFrame = (LONG) (lXPrintPage / 2.0);
		}
		break;

	case 9:
		if (dwAngle == 1)
			URect = URect9P; // paisagem
		else
			URect = URect9; // retrato

		lXFrame = (LONG) (lXPrintPage / 3.0);
		lYFrame = (LONG) (lYPrintPage / 3.0);
		break;

	case 16:
		if (dwAngle == 1)
			URect = URect16P; // paisagem
		else
			URect = URect16; // retrato
		
		lXFrame = (LONG) (lXPrintPage / 4.0);
		lYFrame = (LONG) (lYPrintPage / 4.0);
		break;

	default: // Should Not Occur.
		return;
	}

	// Set the flag if the picture has to be rotated
	DEBUG_MSG("pbRotate => lXPhyPage %d | lYPhyPage %d | lXFrame %d | lYFrame %d");
	*pbRotate = !((lXPhyPage >= lYPhyPage) && (lXFrame >= lYFrame)) &&
	!((lXPhyPage < lYPhyPage) && (lXFrame < lYFrame));


	// If the picture is to be rotated, modify the rectangle selected.

	if ((dwTotalNumberOfPages == 2) || (dwTotalNumberOfPages == 6))
	{
		DEBUG_MSG("verificando pbRotate.." );
		if (*pbRotate)
		{
			DEBUG_MSG("rotate.. %d", dwTotalNumberOfPages );

			switch (dwTotalNumberOfPages)
			{
			case 2:
				if (lXPrintPage <= lYPrintPage)
				{
					DEBUG_MSG("dwAngle.. %d", dwAngle );

					if ( dwAngle == 1 )
						URect = URect21RP; // paisagem
					else
						URect = URect21RR; // retrato
				} // URect22 = URect22R
				break;

			case 6:
				if (lXPrintPage <= lYPrintPage)
				{
					if ( dwAngle == 1 )
						URect = URect61P; // paisagem
					else
						URect = URect61R; // retrato
				}
				else
				{
					URect = URect62R;
				}
				break;
			}
		}
	}
	else
	{
		if (*pbRotate)
		{
			// get the number of rows/columns. switch is faster than sqrt.
			switch (dwTotalNumberOfPages)
			{
			case 4:
				lNumRowCol = 2;
				break;
			case 9:
				lNumRowCol = 3;
				break;
			case 16:
				lNumRowCol = 4;
			break;
			}

			lRowIndex  = (LONG) (uCurrentPageNumber / lNumRowCol);
			lColIndex  = (LONG) (uCurrentPageNumber % lNumRowCol);

			uCurrentPageNumber = (lNumRowCol - 1 - lColIndex) * lNumRowCol + lRowIndex;
		}
	}

	// Update the Page Coordinates.

	rectDocument->top    = (LONG) (rectDocument->top    * URect[uCurrentPageNumber].top);
	rectDocument->bottom = (LONG) (rectDocument->bottom * URect[uCurrentPageNumber].bottom);
	rectDocument->left   = (LONG) (rectDocument->left   * URect[uCurrentPageNumber].left);
	rectDocument->right  = (LONG) (rectDocument->right  * URect[uCurrentPageNumber].right);

	// If the page border has to drawn, return the corresponding coordinates in rectBorder.

	if (dwNupBorderFlags == 1)
	{
		rectBorder->top    = rectDocument->top/xResolution;
		rectBorder->bottom = rectDocument->bottom/xResolution - 1;
		rectBorder->left   = rectDocument->left/yResolution;
		rectBorder->right  = rectDocument->right/yResolution - 1;
	}

	if (*pbRotate)
	{
		ltemp = lXFrame; lXFrame = lYFrame; lYFrame = ltemp;
	}

	// Get the new size of the rectangle to keep the X/Y ratio constant.
	if ( ((LONG) (lYFrame*((lXPhyPage*1.0)/lYPhyPage))) >= lXFrame)
	{
		ldX = 0;
		ldY = lYFrame - ((LONG) (lXFrame*((lYPhyPage*1.0)/lXPhyPage)));
	}
	else
	{
		ldY = 0;
		ldX = lXFrame - ((LONG) (lYFrame*((lXPhyPage*1.0)/lYPhyPage)));
	}

	// Adjust the position of the rectangle.

	if (*pbRotate)
	{
		if (ldX)
		{
			rectDocument->bottom -= (LONG) (ldX / 2.0);
			rectDocument->top    += (LONG) (ldX / 2.0);
		}
		else
		{
			rectDocument->right   -= (LONG) (ldY / 2.0);
			rectDocument->left    += (LONG) (ldY / 2.0);
		}
	}
	else
	{
		if (ldX)
		{
			rectDocument->left    += (LONG) (ldX / 2.0);
			rectDocument->right   -= (LONG) (ldX / 2.0);
		}
		else
		{
			rectDocument->top     += (LONG) (ldY / 2.0);
			rectDocument->bottom  -= (LONG) (ldY / 2.0);
		}
	}

	// Adjust to get the Printable Area on the rectangle

	lXOffset = GetDeviceCaps(hPrinterDC, PHYSICALOFFSETX) * yResolution;
	lYOffset = GetDeviceCaps(hPrinterDC, PHYSICALOFFSETY) * xResolution;

	dXleft = ( lXOffset * 1.0) / lXPhyPage;
	dYtop  = ( lYOffset * 1.0) / lYPhyPage;
	dXright =  ((lXPhyPage - (lXOffset + lXPrintPage)) * 1.0) / lXPhyPage;
	dYbottom = ((lYPhyPage - (lYOffset + lYPrintPage)) * 1.0) / lYPhyPage;

	lXNewPhyPage = rectDocument->right  - rectDocument->left;
	lYNewPhyPage = rectDocument->bottom - rectDocument->top;

	if (*pbRotate)
	{
		ltemp = lXNewPhyPage; lXNewPhyPage = lYNewPhyPage; lYNewPhyPage = ltemp;

		rectDocument->left   += (LONG) (dYtop    * lYNewPhyPage);
		rectDocument->right  -= (LONG) (dYbottom * lYNewPhyPage);
		rectDocument->top    += (LONG) (dXright  * lXNewPhyPage);
		rectDocument->bottom -= (LONG) (dXleft   * lXNewPhyPage);
	}
	else
	{
		rectDocument->left   += (LONG) (dXleft   * lXNewPhyPage);
		rectDocument->right  -= (LONG) (dXright  * lXNewPhyPage);
		rectDocument->top    += (LONG) (dYtop    * lYNewPhyPage);
		rectDocument->bottom -= (LONG) (dYbottom * lYNewPhyPage);
	}

	if (xResolution!=yResolution) 
	{
		rectDocument->left   = rectDocument->left   / yResolution;
		rectDocument->right  = rectDocument->right  / yResolution; 
		rectDocument->top    = rectDocument->top    / xResolution;
		rectDocument->bottom = rectDocument->bottom / xResolution; 
	}
	return;
}

int PlayEMFPage(
    HDC          hPrinterDC,
	HENHMETAFILE hEMF,
    DWORD        dwNumberOfPagesPerSide,
    DWORD        dwPageNumber,
    DWORD        dwPageIndex,
    DWORD        dwNupBorderFlags,
    DWORD        dwAngle)
    
/*++
Function Description: PlayEMFPage plays the EMF in the appropriate rectangle. It performs
                      the required scaling, rotation and translation.

Parameters:   hPrinterDC             -- handle to the printer device context
              hEMF                   -- handle to the contents of the page in the spool file
              dwNumberOfPagesPerSide -- number of pages to be printed per side
              dwPageNumber           -- page number in the document
              dwPageIndex            -- page number in the side. (1 based)
              dwNupBorderFlags       -- border printing options for nup
              dwAngle                -- angle for rotation (if neccesary)

Return Values:  TRUE if successful
                FALSE otherwise
--*/
{
	BOOL         bReturn = FALSE, bRotate;
	RECT         rectDocument, rectPrinter, rectBorder = {-1, -1, -1, -1};
	RECT         *prectClip = NULL;
	XFORM        TransXForm = {1, 0, 0, 1, 0, 0}, RotateXForm = {0, -1, 1, 0, 0, 0};

	SetGraphicsMode(hPrinterDC, GM_ADVANCED);
	ModifyWorldTransform(hPrinterDC, NULL, MWT_IDENTITY);

	// Compute the rectangle for one page.
	GetPageCoordinatesForNUp(hPrinterDC,
		&rectDocument,
		&rectBorder,
		dwNumberOfPagesPerSide,
		dwPageIndex,
		dwNupBorderFlags,
		&bRotate,
		dwAngle);
	/*printf("%i %i %i %i | %i %i %i %i | %i\n", rectDocument.top, rectDocument.left, rectDocument.bottom, rectDocument.right,
	rectBorder.top, rectBorder.left, rectBorder.bottom, rectBorder.right, bRotate);*/

	// If swap flag is set, reverse rotate flag
	//
	if (dwAngle & 1)
		bRotate = !bRotate;

	/*if (dwAngle & EMF_DEGREE_SWAP)
	{
		RotateXForm.eM12 = 1;
		RotateXForm.eM21 = -1;
	}   // EMF_DEGREE_90 case is the initialization
	*/

	if (bRotate)
	{
		rectPrinter.top = 0;
		rectPrinter.bottom = rectDocument.right - rectDocument.left;
		rectPrinter.left = 0;
		rectPrinter.right = rectDocument.bottom - rectDocument.top;

		// Set the translation matrix
		if (dwAngle == 270)
		{
			TransXForm.eDx = float(rectDocument.right);
			TransXForm.eDy = float(rectDocument.top);
		}
		else
		{
			// EMF_DEGREE_90
			TransXForm.eDx = float(rectDocument.left);
			TransXForm.eDy = float(rectDocument.bottom);
		}

		// Set the transformation matrix
		if (!SetWorldTransform(hPrinterDC, &RotateXForm) ||
			!ModifyWorldTransform(hPrinterDC, &TransXForm, MWT_RIGHTMULTIPLY))
		{
			//printf("Setting transformation matrix failed: %i\n", GetLastError());
			goto CleanUp;
		}
	}

	// Add clipping for Nup
	if (dwNumberOfPagesPerSide != 1)
	{
		prectClip = &rectDocument;
	}

	// Print the page.
	if (bRotate)
	{
		PlayEnhMetaFile(hPrinterDC, hEMF, &rectPrinter);
		/*printf("%i %i %i %i\t", rectPrinter.left, rectPrinter.top, rectPrinter.right, rectPrinter.bottom);
		printf("%i %i %i %i\n", rectBorder.left, rectBorder.top, rectBorder.right, rectBorder.bottom);
		if (dwNupBorderFlags & 1)
		{
			SelectObject(hPrinterDC, GetStockObject(BLACK_PEN));
			//Rectangle(hPrinterDC, rectPrinter.left, rectPrinter.top, rectPrinter.right, rectPrinter.bottom);
			Rectangle(hPrinterDC, 0, 0, 1000, 1000);
		}*/
	}
	else
	{
		PlayEnhMetaFile(hPrinterDC, hEMF, &rectDocument);
	}

	bReturn = TRUE;

CleanUp:

	if (!ModifyWorldTransform(hPrinterDC, NULL, MWT_IDENTITY))
	{
		//printf("Setting Identity Transformation failed: %i\n", GetLastError());
		bReturn = FALSE;
	}

	if (dwNupBorderFlags & 1)
	{
		SelectObject(hPrinterDC, GetStockObject(NULL_BRUSH));
		SelectObject(hPrinterDC, GetStockObject(BLACK_PEN));
		Rectangle(hPrinterDC, rectBorder.left, rectBorder.top + 4, rectBorder.right, rectBorder.bottom);
	}

	return bReturn;
}

void EMFPrinter::SetFontTempFolder(const std::string &tempFolder)
{
	this->fontTempFolder = tempFolder;
}

bool EMFPrinter::Print(const std::string &filename, const std::string &printerName, const std::string &docName, const std::string &userName, int copies, int orientation)
{
	return this->Print(filename, printerName, docName, userName, copies, orientation, false, false, NULL);
}

bool EMFPrinter::Print(const std::string &filename, const std::string &printerName, const std::string &docName, const std::string &userName, int copies, int orientation, bool rotate, bool schedule, char * scheduleTime)
{
	bool ret = true;

	DEBUG_MSG("Iniciando Impressão arquivo=\"%s\" Impressora=\"%s\" User=\"%s\" nUP=%i orientation=%i", filename.c_str(),printerName.c_str() ,userName.c_str() , m_multiup, orientation );

	if (m_multiup != NUP_1)
	{
		bool status = false;
		bool ret = PrinterSettings::SetOrientation(printerName, (TOrientation) orientation, &status, true);
		DEBUG_MSG("pos setorientation | status => %i | ret %i", status, ret );
	}

	HANDLE hp = NULL;
	PRINTER_DEFAULTSA def;
	def.pDatatype = NULL;
	def.pDevMode = NULL;
	def.DesiredAccess = PRINTER_ALL_ACCESS;

	if (OpenPrinterA(const_cast<char *>(printerName.c_str()), &hp, &def) == 0)
	{
		int err = GetLastError();
		DEBUG_MSG("can't open printer %s erro %i", printerName.c_str(), err);
		return false;
	}

	// aplicativo do delphi (QRP) ou pdf, usar método normal
	bool clean_mode = true;
	const char * sharp = strchr(docName.c_str(), '#');
	if (sharp)
	{
		sharp++;
		if ((strlen(sharp) > 1) && !strstr(sharp, ".pdf") && !strstr(sharp, ".PDF"))
		{
			clean_mode = false;
		}
	}

	EMFExtract emf_extract;
	emf_extract.SetFontTempFolder(this->fontTempFolder);
	if (emf_extract.Extract(filename))
	{
		DOCINFOA doc_info;
		HENHMETAFILE hmf;
		RECT rect;
		emf_extract.LoadFonts();

		for (int c = 0; c < copies; c++)
		{
			doc_info.lpszDocName = docName.c_str();
			doc_info.cbSize = sizeof(DOCINFO);
			doc_info.lpszOutput = NULL;
			doc_info.lpszDatatype = NULL;
			doc_info.fwType = 0;

			HDC hdcPrinter = CreateDCA("WINSPOOL", printerName.c_str(), NULL, NULL);

			DWORD jobId = StartDocA(hdcPrinter, &doc_info);
			if (jobId >= 1)
			{
				this->SetUserAndScheduleJob(hp, jobId, userName, schedule, scheduleTime);
				int n_pages = emf_extract.GetNumPages();
				if (n_pages > 1)
				{
					int p = n_pages - 1;
					std::stringstream ss;
					ss << filename << '.' << std::setfill('0') << std::setw(5) << p << ".emf";
					hmf = GetEnhMetaFileA( ss.str().c_str());
					ENHMETAHEADER header; 
					GetEnhMetaFileHeader(hmf, sizeof(ENHMETAHEADER), &header);

					// tratar folha branca atoa
					if ((header.rclBounds.left == 0) &&
						(header.rclBounds.top == 0) &&
						(header.rclBounds.bottom == -1) &&
						(header.rclBounds.right == -1) &&
						(header.nHandles == 1))
					{
						DEBUG_MSG("a ultima pagina era NULA");
						n_pages--;
					}
					DeleteEnhMetaFile(hmf);
				}


				DEBUG_MSG("extraiu emfs.. %i pages", n_pages);

				// renderizar os emfs
				for (int p = 0; p < n_pages; p++)
				{
					DEBUG_MSG("processando pagina %i", p);
					std::stringstream ss;
					ss << filename << '.' << std::setfill('0') << std::setw(5) << p << ".emf";
					hmf = GetEnhMetaFileA(ss.str().c_str());

					DWORD phyOffX = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETX);
					DWORD phyOffY = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETY);
					DWORD physiWd = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH);
					DWORD physiHg = GetDeviceCaps(hdcPrinter, PHYSICALHEIGHT);
					DEBUG_MSG("phyOff:x %i y %i", phyOffX, phyOffY);
					DEBUG_MSG("physi:wd %i hg %i", physiWd, physiHg);

					//ShowDeviceValues( hdcPrinter ); 

					DWORD vHORZRES = GetDeviceCaps( hdcPrinter, HORZRES );    /* Horizontal width in pixels               */
					DWORD vVERTRES = GetDeviceCaps( hdcPrinter, VERTRES );    /* Vertical height in pixels                */
					DEBUG_MSG("vRES:%i %i", vHORZRES, vVERTRES);

					if (clean_mode)
					{
						rect.top = rect.left = 0;
						rect.right = vHORZRES ;
						rect.bottom = vVERTRES;
					}
					else
					{
						rect.left = 0;
						//rect.left -= phyOffX;
						rect.top = 0;
						//rect.top -= phyOffY;
						rect.right = physiWd - phyOffX;
						rect.bottom = physiHg - phyOffY;
					}


					DEBUG_MSG("m_multiup %i | rotate %i", m_multiup, rotate);
					if (m_multiup != NUP_1 || 
						rotate == true)
					{
						DEBUG_MSG("vai rodar o emf");
						if (!SetGraphicsMode(hdcPrinter, GM_ADVANCED))
						{
							DEBUG_MSG("SetGraphicsMode error:%i", GetLastError());
						}

						if (!ModifyWorldTransform(hdcPrinter, NULL, MWT_IDENTITY))
						{
							DEBUG_MSG("ModifyWorldTransform error:%i", GetLastError());
						}

						ENHMETAHEADER header; 
						GetEnhMetaFileHeader(hmf, sizeof(ENHMETAHEADER), &header);					
						if (header.szlMillimeters.cx > header.szlMillimeters.cy)
						{		
							DEBUG_MSG("Rotacionar o EMF cx = %i cy=%i", header.szlMillimeters.cx , header.szlMillimeters.cy );
							//XFORM TransXForm = {1, 0, 0, 1, 0, 0}, RotateXForm = {0, -1, 1, 0, 0, 0};
							XFORM RotateXForm = {1, 0, 0, 1, 0, 0};
							RotateXForm.eM11 = 0;
							RotateXForm.eM12 = -1;
							RotateXForm.eM21 = 1;
							RotateXForm.eM22 = 0;
							RotateXForm.eDx = 0.0;// - physiHg;
							RotateXForm.eDy = 0.0 + physiHg - (phyOffY * 2);

							DWORD t = physiHg;
							physiHg = physiWd;
							physiWd = t;
							t = phyOffX;
							phyOffX = phyOffY;
							phyOffY = t;

							// Set the transformation matrix
							if (!SetWorldTransform( hdcPrinter, &RotateXForm ))
							{
								DEBUG_MSG("SetWorldTransform error:%i", GetLastError());
							}
						}

						//printf("%i %i %i %i\n", rect.top, rect.left, rect.bottom, rect.right);

						DEBUG_MSG("Impressão com Multiup =%i",m_multiup );
						if (p % m_multiup == 0)						
							StartPage(hdcPrinter);

						if (header.szlMillimeters.cx > header.szlMillimeters.cy)
						{
							DEBUG_MSG("Multiup de pagina em paisagem" );
							PlayEMFPage(hdcPrinter, hmf, m_multiup, n_pages, (p % m_multiup) + 1, m_enableBorder, 1);
						}
						else
						{
							DEBUG_MSG("Multiup de pagina em retrato" );
							PlayEMFPage(hdcPrinter, hmf, m_multiup, n_pages, (p % m_multiup) + 1, m_enableBorder, 0);
						}
						if ((p + 1) % m_multiup == 0)
							EndPage(hdcPrinter);

						DEBUG_MSG("processou ok");
					}
					else
					{
						DEBUG_MSG("nao precisa rotacionar..");
						StartPage(hdcPrinter);
						PlayEnhMetaFile(hdcPrinter, hmf, &rect);
						EndPage(hdcPrinter);
						DEBUG_MSG("enviou ok");
					}

					//rect.left -= phyOffX;
					//rect.left = phyOffX;

					//rect.top -= phyOffY;
					//rect.right = physiWd - phyOffX;
					//rect.right = vHORZRES + (phyOffX/2 ); //ok para pdf_word com borda
					//rect.bottom = physiHg - phyOffY;					
					//rect.bottom = vVERTRES - phyOffY; //ok para pdf_word com borda

					/*PlayEnhMetaFile(hdcPrinter, hmf, &rect);
					//EnumEnhMetaFile(hdcPrinter, hmf, EnumEnhProc, NULL, &rect);
					DeleteEnhMetaFile(hmf);*/


					/*StartPage( hdcPrinter );
					//PlayEnhMetaFile( hdcPrinter, hmf, &rect );
					EnumEnhMetaFile( hdcPrinter, hmf, EnumEnhProc, NULL , &rect ); 
					DeleteEnhMetaFile(hmf);
					EndPage( hdcPrinter );*/

					DeleteEnhMetaFile(hmf);
				}
			}
			else
			{
				ret = false;
			}

			EndDoc(hdcPrinter);
			DeleteDC(hdcPrinter);
		}
	}
	else
	{
		DEBUG_MSG("não conseguiu extrair emf");
		ret = false;
	}
	emf_extract.Cleanup();

	ClosePrinter(hp);
	//GdiplusShutdown(gdiplusToken);

	return ret;
}

bool EMFPrinter::SetUserAndScheduleJob(HANDLE hp, DWORD jobId, const std::string & userName, bool schedule, char * scheduleTime)
{
	DEBUG_MSG("start");

	// setar nome do usuário
	DWORD sz = 0;
	GetJobA(hp, jobId, 2, NULL, 0, &sz);
	if (sz <= 0)
	{
		DEBUG_MSG("end | erro");
		return false;
	}

	BYTE * buf = new BYTE[sz + 1];
	//if (GetJobA(hp, jobId, 1, buf, sz, &sz))
	if (GetJobA(hp, jobId, 2, buf, sz, &sz))
	{
		//JOB_INFO_1A * jinfo = reinterpret_cast<JOB_INFO_1A *>(buf);
		JOB_INFO_2A * jinfo = reinterpret_cast<JOB_INFO_2A *>(buf);
		jinfo->pUserName = const_cast<char *>(userName.c_str());
		if (schedule)
		{
			int start = this->GetTimeInMinutes( scheduleTime );
			jinfo->StartTime = start;
			jinfo->UntilTime = start + 20;
		}

		if (!SetJobA(hp, jobId, 2, buf, 0))
		{
			int err = GetLastError();
			DEBUG_MSG("SetJob error: %i", err);
		}					
	}

	delete [] buf;

	DEBUG_MSG("end | ok");
	return true;
}

bool SetPrinterProperties( HANDLE hPrinter, int &dmDuplex, int &dmColor )
{
	DEBUG_MSG("start");
	PRINTER_INFO_8 * pi8;

	DWORD dwBytesNeeded = 0; 
	::GetPrinter( hPrinter,  8 , NULL, dwBytesNeeded, &dwBytesNeeded ); 

	if( dwBytesNeeded < 1 )
	{
		return false; 			
	}

	BYTE * pBuffer = new BYTE[ dwBytesNeeded + 1 ];
	
	if( !::GetPrinter( hPrinter,  8 , pBuffer, dwBytesNeeded, &dwBytesNeeded ) )
	{
		delete [] pBuffer;
		return false;
	}

	bool setPrinter = false;

	pi8 = reinterpret_cast< PRINTER_INFO_8*>( pBuffer );

	if (pi8->pDevMode == NULL)
	{

		DEBUG_MSG("pDevMode == NULL");
		return false;
	}


	if (pi8->pDevMode->dmColor != dmColor)
	{
		setPrinter = true;
		int dmColorAux = pi8->pDevMode->dmColor;

		pi8->pDevMode->dmColor = dmColor;

		dmColor = dmColorAux;
	}

	if (pi8->pDevMode->dmDuplex != dmDuplex)
	{
		setPrinter = true;

		int dmDuplexAux = pi8->pDevMode->dmDuplex;

		pi8->pDevMode->dmDuplex = dmDuplex;

		dmDuplex = dmDuplexAux;
	}

	if (setPrinter == true)
	{
		BOOL ret = SetPrinter( hPrinter, 8, (LPBYTE) pi8, 0);
		DEBUG_MSG("ret:%i", ret);
	}

	delete [] pBuffer;

	DEBUG_MSG("end");
	return true;
}

DWORD EMFPrinter::StartPrintJob(const std::string &filename, const std::string &printerName, const std::string &docName, const std::string &userName, void** objEMFExtract, void** objPrinter, void** objHDCPrinter, bool convertMono, bool convertDuplex)
{
	DWORD ret = 0;
	DEBUG_MSG("start | fileName:\"%s\" printerName=\"%s\" User=\"%s\"", filename.c_str(),printerName.c_str() ,userName.c_str());

	HANDLE hp = NULL;
	PRINTER_DEFAULTSA def;
	def.pDatatype = NULL;
	def.pDevMode = NULL;
	def.DesiredAccess = PRINTER_ALL_ACCESS;

	if (OpenPrinterA(const_cast<char *>(printerName.c_str()), &hp, &def) == 0)
	{
		int err = GetLastError();
		DEBUG_MSG("can't open printer %s erro %i", printerName.c_str(), err);
		return false;
	}

	EMFExtract * emf_extract = new EMFExtract();
	emf_extract->SetFontTempFolder(this->fontTempFolder);
	if (emf_extract->Extract(filename))
	{
		DEBUG_MSG("extraiu OK");
		bool setPrinter = false;

		int dmDuplex = convertDuplex == true ? 2 : 1;
		int dmColor = convertMono == true ? 1 : 2;

		if (convertDuplex == true || convertMono == true)
		{
			setPrinter = true;
			DEBUG_MSG("setando impressora..");
			SetPrinterProperties(hp, dmDuplex, dmColor);
		}

		DOCINFOA doc_info;
		HENHMETAFILE hmf;
		emf_extract->LoadFonts();

		doc_info.lpszDocName = docName.c_str();
		doc_info.cbSize = sizeof(DOCINFO);
		doc_info.lpszOutput = NULL;
		doc_info.lpszDatatype = NULL;
		doc_info.fwType = 0;

		HDC hdcPrinter = CreateDCA("WINSPOOL", printerName.c_str(), NULL, NULL);

		ret = StartDocA(hdcPrinter, &doc_info);
		this->SetUserAndScheduleJob(hp, ret, userName, false, NULL);

		*objEMFExtract = emf_extract;
		*objPrinter = hp;
		*objHDCPrinter = hdcPrinter;

		if (setPrinter == true)
		{
			SetPrinterProperties(hp, dmDuplex, dmColor);
		}
	}

	return ret;
}

#define SPACE_STR_SIZE 1024
bool insertDumbText(HDC hdc)
{
	char space_str[SPACE_STR_SIZE + 1];
	memset(space_str, 0x20, sizeof(space_str));

	DEBUG_MSG("hdc:%#p", hdc);
	// adicionar comment ao EMF apenas para termos espaço para modificá-lo depois
	BOOL r = GdiComment(hdc, sizeof(space_str) - 1, reinterpret_cast<const BYTE *>(space_str));
	DEBUG_MSG("GdiComment: %i", r);
	
	return true;
}


bool EMFPrinter::ProcessPrintJob(void* objEMFExtract, void* objPrinter, void* objHDCPrinter, int jobId, char* filename, const std::string &docName)
{
	DEBUG_MSG("start | fileName:%s", filename);
	HENHMETAFILE hmf;
	RECT rect;

	EMFExtract* emf_extract = (EMFExtract*)objEMFExtract;
	HANDLE hp = (HANDLE)objPrinter;
	HDC hdcPrinter = (HDC)objHDCPrinter;

	// aplicativo do delphi (QRP) ou pdf, usar método normal
	bool clean_mode = true;
	const char * sharp = strchr(docName.c_str(), '#');
	if (sharp)
	{
		sharp++;
		if ((strlen(sharp) > 1) && !strstr(sharp, ".pdf") && !strstr(sharp, ".PDF"))
		{
			DEBUG_MSG("clean_mode");
			clean_mode = false;
		}
	}

	int n_pages = emf_extract->GetNumPages();
	if (n_pages > 1)
	{
		int p = n_pages - 1;
		std::stringstream ss;
		ss << filename << '.' << std::setfill('0') << std::setw(5) << p << ".emf";
		hmf = GetEnhMetaFileA( ss.str().c_str());
		ENHMETAHEADER header; 
		GetEnhMetaFileHeader(hmf, sizeof(ENHMETAHEADER), &header);

		// tratar folha branca atoa
		if ((header.rclBounds.left == 0) &&
			(header.rclBounds.top == 0) &&
			(header.rclBounds.bottom == -1) &&
			(header.rclBounds.right == -1) &&
			(header.nHandles == 1))
		{
			DEBUG_MSG("a ultima pagina era NULA");
			n_pages--;
		}
		DeleteEnhMetaFile(hmf);
	}


	DEBUG_MSG("extraiu emfs.. %i pages", n_pages);

	// renderizar os emfs
	for (int p = 0; p < n_pages; p++)
	{
		DEBUG_MSG("processando pagina %i", p);
		std::stringstream ss;
		ss << filename << '.' << std::setfill('0') << std::setw(5) << p << ".emf";
		hmf = GetEnhMetaFileA(ss.str().c_str());

		DWORD phyOffX = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETX);
		DWORD phyOffY = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETY);
		DWORD physiWd = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH);
		DWORD physiHg = GetDeviceCaps(hdcPrinter, PHYSICALHEIGHT);
		DEBUG_MSG("phyOff:x %i y %i", phyOffX, phyOffY);
		DEBUG_MSG("physi:wd %i hg %i", physiWd, physiHg);

		//ShowDeviceValues( hdcPrinter ); 

		DWORD vHORZRES = GetDeviceCaps( hdcPrinter, HORZRES );    /* Horizontal width in pixels               */
		DWORD vVERTRES = GetDeviceCaps( hdcPrinter, VERTRES );    /* Vertical height in pixels                */
		DEBUG_MSG("vRES:%i %i", vHORZRES, vVERTRES);

		if (clean_mode)
		{
			rect.top = rect.left = 0;
			rect.right = vHORZRES ;
			rect.bottom = vVERTRES;
		}
		else
		{
			rect.left = 0;
			rect.left -= phyOffX;
			rect.top = 0;
			rect.top -= phyOffY;
			rect.right = physiWd - phyOffX;
			rect.bottom = physiHg - phyOffY;
		}

		DEBUG_MSG("nao precisa rotacionar..");
		StartPage(hdcPrinter);
		PlayEnhMetaFile(hdcPrinter, hmf, &rect);
		insertDumbText(hdcPrinter);
		EndPage(hdcPrinter);
		DEBUG_MSG("enviou ok");

		DeleteEnhMetaFile(hmf);
	}


	EndDoc(hdcPrinter);
	DeleteDC(hdcPrinter);

	emf_extract->Cleanup();
	delete emf_extract;
	emf_extract = NULL;

	ClosePrinter(hp);

	return true;
}

int EMFPrinter::GetTimeInMinutes ( char *hora )
{
	DEBUG_MSG("start | hora:%s", hora);

	time_t rawtime;
	struct tm * ptm;
	time ( &rawtime );
	ptm = gmtime ( &rawtime );
	int gmtHour = ptm->tm_hour;
	ptm = localtime ( &rawtime );
	int localHour = ptm->tm_hour;


	char h[20]={0};
	strncpy( h , hora,20 );
	int iHora   = atoi( strtok( h , ":" ) );
	int iMinuto = atoi( strtok( NULL , ":" ) );
	
	iHora += gmtHour - localHour;

	DEBUG_MSG("gmt:%i | local:%i | hora ajustada:%i", gmtHour, localHour, iHora);
	
	int iStart = iHora * 60 + iMinuto;

	DEBUG_MSG("end | returning %i minutes", iStart);
	return iStart;

}

void* GetEMFPrinterObject()
{
	EMFPrinter * emfPrinter = new EMFPrinter();
	return (void*)emfPrinter;
}

bool PrintEMF( void *objEMFPrinter, char* fileName, char* printerName, char* docName, char* userName, int copies, int pagesPerSheet, int border )
{
	EMFPrinter::MultiupType ppSheet = EMFPrinter::NUP_1;
	switch (pagesPerSheet)
	{
	default:
	case 1: ppSheet = EMFPrinter::NUP_1; break;
	case 2: ppSheet = EMFPrinter::NUP_2; break;
	case 4: ppSheet = EMFPrinter::NUP_4; break;
	case 6: ppSheet = EMFPrinter::NUP_6; break;
	case 9: ppSheet = EMFPrinter::NUP_9; break;
	case 16: ppSheet = EMFPrinter::NUP_16; break;
	}

	EMFPrinter * emfPrinter = (EMFPrinter*) objEMFPrinter;
	emfPrinter->SetPagesPerSheet(ppSheet);
	emfPrinter->SetPrinterBorder(border == 1);

	int orientation = 0;

	EMFExtract emf_extract;
	//emf_extract.SetFontTempFolder(this->fontTempFolder);
	if (emf_extract.Extract(fileName))
	{
		int n_pages = emf_extract.GetNumPages();
		if (n_pages > 0)
		{
			int p = 0;
			std::stringstream ss;
			ss << fileName << '.' << std::setfill('0') << std::setw(5) << p << ".emf";
			HENHMETAFILE hmf = GetEnhMetaFileA( ss.str().c_str());
			ENHMETAHEADER header; 
			GetEnhMetaFileHeader(hmf, sizeof(ENHMETAHEADER), &header);

			if (header.szlMillimeters.cx > header.szlMillimeters.cy)
				orientation = 1; // paisagem
			else
				orientation = 0; // retrato

			DeleteEnhMetaFile(hmf);
		}

		emf_extract.Cleanup();
	}

	return emfPrinter->Print(fileName, printerName, docName, userName, copies, orientation, true, false, NULL);
}

bool DeleteEMFObject( void *objEMFPrinter )
{
	EMFPrinter * emfPrinter = (EMFPrinter*) objEMFPrinter; 
	delete emfPrinter;
	return true;
}

bool StartPrintEMFJob(void *objEMFPrinter, void** objEMFExtract, void** objPrinter, void** objHDCPrinter, int* jobId, char* fileName, char* printerName, char* docName, char* userName, bool convertMono, bool convertDuplex)
{
	EMFPrinter * emfPrinter = (EMFPrinter*) objEMFPrinter;

	int orientation = 0;

	EMFExtract emf_extract;
	//emf_extract.SetFontTempFolder(this->fontTempFolder);
	if (emf_extract.Extract(fileName))
	{
		int n_pages = emf_extract.GetNumPages();
		if (n_pages > 0)
		{
			int p = 0;
			std::stringstream ss;
			ss << fileName << '.' << std::setfill('0') << std::setw(5) << p << ".emf";
			HENHMETAFILE hmf = GetEnhMetaFileA( ss.str().c_str());
			ENHMETAHEADER header; 
			GetEnhMetaFileHeader(hmf, sizeof(ENHMETAHEADER), &header);

			if (header.szlMillimeters.cx > header.szlMillimeters.cy)
				orientation = 1; // paisagem
			else
				orientation = 0; // retrato

			DeleteEnhMetaFile(hmf);
		}

		emf_extract.Cleanup();
	}

	*jobId = emfPrinter->StartPrintJob(fileName, printerName, docName, userName, objEMFExtract, objPrinter, objHDCPrinter, convertMono, convertDuplex );

	return *jobId > 0;
}

bool ProcessPrintEMFJob( void *objEMFPrinter, void* objEMFExtract, void* objPrinter, void* objHDCPrinter, int jobId, char * fileName, char* docName)
{
	EMFPrinter * emfPrinter = (EMFPrinter*) objEMFPrinter;
	return emfPrinter->ProcessPrintJob(objEMFExtract, objPrinter, objHDCPrinter, jobId, fileName, docName);
}