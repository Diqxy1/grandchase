// mix All References@!
// remake by kuna

// nessary Includes & libs 
#include "PSPTypes.h"
#pragma comment(lib, "imgdecoder.lib")


/* part 01 :: imgdecoder */
DWORD _stdcall ImgNewDecoder(LPVOID /*out*/*ppDecoder);
DWORD _stdcall ImgDeleteDecoder(LPVOID /*in*/pDecoder);
DWORD _stdcall ImgNewDIBFromFile(LPVOID /*in*/pDecoder, const char* /*in*/pFileName, LPVOID /*out*/*pImg);
DWORD _stdcall ImgDeleteDIBSection(LPVOID /*in*/pImg);
DWORD _stdcall ImgGetHandle(LPVOID /*in*/pImg, HBITMAP /*out*/*pBitmap, LPVOID /*out*/*ppDIBBits);


/* part 00 :: function */
void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits)
{
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// pre-multiply rgb channels with alpha channel
	for (int y=0; y<bmpInfo.bmHeight; ++y)
	{
		BYTE *pPixel= pBitmapBits + bmpInfo.bmWidth * 4 * y;

		for (int x=0; x<bmpInfo.bmWidth ; ++x)
		{
			pPixel[0]= pPixel[0]*pPixel[3]/255;
			pPixel[1]= pPixel[1]*pPixel[3]/255;
			pPixel[2]= pPixel[2]*pPixel[3]/255;

			pPixel+= 4;
		}
	}
}


/* part 02 :: PSPLoader (DIB) */
LRESULT LoadPSPFileToDIB(/*[in]*/const char* pFileName, /*[out]*/HBITMAP &hBitmap, /*[out]*/VOID **ppvBits= NULL);


/* part 03 :: Packager - dis part is made by k. */
CBitmap m_bmpDialog;
LPVOID m_pImg;
LPVOID m_pImgDecoder;

void UpdateDialog(HWND wnd, CBitmap &bmp, BYTE SourceConstantAlpha=255)
{
	// make sure the window has the WS_EX_LAYERED style
	SetWindowLong(wnd, GWL_EXSTYLE, GetWindowLong(wnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//wnd->ModifyStyleEx(0, WS_EX_LAYERED);


	// ok... into the per-pixel-alpha bendling....

	// Create/setup the DC's

	CDC dcScreen;
	CDC dcMemory;

	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);

	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmp);


	// get the bitmap dimensions
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg;
	GetWindowRect(wnd, rectDlg);
	//wnd->GetWindowRect(rectDlg);

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(bmpInfo.bmWidth, bmpInfo.bmHeight);


	// Perform the alpha blend

	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0); // start point of the copy from dcMemory to dcScreen

	// perform the alpha blend
	BOOL bRet= ::UpdateLayeredWindow(wnd, dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

	ASSERT(bRet); // ops... something bad just occured....

	// clean up
	dcMemory.SelectObject(pOldBitmap);
}

void WindowAlphaBlendFromPNG(const char* pFileName, HWND wnd)
{
	CWaitCursor wait;

	HBITMAP hBitmap;
	LPBYTE pBitmapBits;
	LPVOID pImg= NULL;

	// Initalize m_pImgDecoder
	::ImgNewDecoder(&m_pImgDecoder);

	// first try to load with paintlib....
	if (!::ImgNewDIBFromFile(m_pImgDecoder, pFileName, &pImg))
	{
		::ImgGetHandle(pImg, &hBitmap, (LPVOID *)&pBitmapBits);

		// get the bitmap info
		BITMAP bmpInfo;
		::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

		if (bmpInfo.bmBitsPixel != 32)
		{
			::ImgDeleteDIBSection(pImg);
			::AfxMessageBox((LPCTSTR)"Error while Loading Image");
			return;
		}
	}
	else // ops... paitlib fail to load file... maybe is an PSP file?
	{
		if (ERR_NOERROR != ::LoadPSPFileToDIB(pFileName, hBitmap, (LPVOID *)&pBitmapBits))
		{
			::AfxMessageBox((LPCTSTR)"Error while Loading Image");
			return;
		}
	}

	//::ImgDeleteDIBSection(m_pImg); < I dont know why.. but it doesnt work. maybe object was perfectly removed?
	m_pImg= pImg;


	m_bmpDialog.DeleteObject();
	m_bmpDialog.Attach(hBitmap);

	// before using the UpdateLayeredWindow we have to 
	// pre-multiply the RGB channels with the alpha channel
	::PreMultiplyRGBChannels(m_bmpDialog, pBitmapBits);

	// now update dialog....
	UpdateDialog(wnd, m_bmpDialog);
}
