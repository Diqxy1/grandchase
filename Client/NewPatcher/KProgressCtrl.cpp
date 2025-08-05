#include "stdafx.h"
#include ".\kprogressctrl.h"
#include "resource.h"
#include "GCPatcherDlg.h"
#ifdef GRID_DOWN
#include "GCGridPatcherDlg.h"
#endif




KProgressCtrl::KProgressCtrl(void)
{
    m_pkBarBitMap   = NULL;
    m_hBarBitMap    = NULL;
    m_pkMask        = NULL;
    memset( &m_kCoordLeft,      0, sizeof(KTexCoord) );
    memset( &m_kCoordBar,       0, sizeof(KTexCoord) );
    memset( &m_kCoordRight,     0, sizeof(KTexCoord) );
    memset( &m_kRect,           0, sizeof(RECT) );
    m_iPos          = 0;
    m_fPercent      = 0;
}

KProgressCtrl::~KProgressCtrl(void)
{
    if( m_pkBarBitMap != NULL )
        m_pkBarBitMap->DeleteObject();
    if( m_pkMask != NULL )
        m_pkMask->DeleteObject();
}
void KProgressCtrl::InitializeCtrl(Komfile *massfile, std::string strName_, RECT rt_ )
{
    std::string strBar = strName_ + std::string( ".bmp" );
    std::string strMask = strName_ + std::string( "mask.bmp" );
    LoadBitmapFromMassFile(massfile, strBar, &m_pkBarBitMap );
    LoadBitmapFromMassFile(massfile, strMask, &m_pkMask );
    BITMAP bmp;
    m_pkBarBitMap->GetObject( sizeof( BITMAP ), &bmp );

    m_kCoordLeft.iPosX      = 0;
    m_kCoordLeft.iPosY      = 0;
    m_kCoordLeft.iHeight    = bmp.bmHeight;
    m_kCoordLeft.iWidth     = bmp.bmWidth/3;

    m_kCoordBar.iPosX       = m_kCoordLeft.iPosX + m_kCoordLeft.iWidth;
    m_kCoordBar.iPosY       = 0;
    m_kCoordBar.iHeight     = bmp.bmHeight;
    m_kCoordBar.iWidth      = bmp.bmWidth/3;

    m_kCoordRight.iPosX     = m_kCoordBar.iPosX + m_kCoordBar.iWidth;
    m_kCoordRight.iPosY     = 0;
    m_kCoordRight.iHeight   = bmp.bmHeight;
    m_kCoordRight.iWidth    = bmp.bmWidth/3;

    SetRect( rt_ );
}
void KProgressCtrl::SetProgress( float fPos_ )
{
    m_fPercent = fPos_;
    m_iPos = (( m_kRect.right - m_kRect.left - m_kCoordRight.iWidth - m_kCoordLeft.iWidth )*m_fPercent)/100;
}
void KProgressCtrl::SetRect( RECT kRect_ )
{
    m_kRect     = kRect_;
    m_iWidth    = m_kRect.right - m_kRect.left;
    m_iHeight   = m_kRect.bottom - m_kRect.top;
}
void KProgressCtrl::DrawProgressBar( CDC* pkMemDC_ )
{
	CDC dcBmp, dcMask;
	CBitmap hBitmap;
	dcBmp.CreateCompatibleDC(pkMemDC_);
	CBitmap *pOldBmp =dcBmp.SelectObject( m_pkBarBitMap );
	dcMask.CreateCompatibleDC( pkMemDC_ );
	CBitmap *pOldMask = dcMask.SelectObject( m_pkMask );
	

	pkMemDC_->StretchBlt(m_kRect.left, m_kRect.top, m_kCoordLeft.iWidth, m_kCoordLeft.iHeight, &dcBmp, 
		m_kCoordLeft.iPosX, m_kCoordLeft.iPosY, m_kCoordLeft.iWidth, m_kCoordLeft.iHeight, SRCINVERT );
	pkMemDC_->StretchBlt(m_kRect.left, m_kRect.top, m_kCoordLeft.iWidth, m_kCoordLeft.iHeight, &dcMask, 
		m_kCoordLeft.iPosX, m_kCoordLeft.iPosY, m_kCoordLeft.iWidth, m_kCoordLeft.iHeight, SRCAND);
	pkMemDC_->StretchBlt(m_kRect.left, m_kRect.top, m_kCoordLeft.iWidth, m_kCoordLeft.iHeight, &dcBmp, 
		m_kCoordLeft.iPosX, m_kCoordLeft.iPosY, m_kCoordLeft.iWidth, m_kCoordLeft.iHeight, SRCINVERT);

	pkMemDC_->StretchBlt(m_kRect.left+m_kCoordLeft.iWidth, m_kRect.top, m_iPos, m_kCoordBar.iHeight, &dcBmp, 
		m_kCoordBar.iPosX, m_kCoordBar.iPosY, m_kCoordBar.iWidth, m_kCoordBar.iHeight, SRCINVERT);
	pkMemDC_->StretchBlt(m_kRect.left+m_kCoordLeft.iWidth, m_kRect.top, m_iPos, m_kCoordBar.iHeight, &dcMask, 
		m_kCoordBar.iPosX, m_kCoordBar.iPosY, m_kCoordBar.iWidth, m_kCoordBar.iHeight, SRCAND);
	pkMemDC_->StretchBlt(m_kRect.left+m_kCoordLeft.iWidth, m_kRect.top, m_iPos, m_kCoordBar.iHeight, &dcBmp, 
		m_kCoordBar.iPosX, m_kCoordBar.iPosY, m_kCoordBar.iWidth, m_kCoordBar.iHeight, SRCINVERT);

	pkMemDC_->StretchBlt(m_kRect.left+ m_kCoordLeft.iWidth + m_iPos, m_kRect.top, m_kCoordRight.iWidth, m_kCoordRight.iHeight, &dcBmp, 
		m_kCoordRight.iPosX, m_kCoordRight.iPosY, m_kCoordRight.iWidth, m_kCoordRight.iHeight, SRCINVERT);    
	pkMemDC_->StretchBlt(m_kRect.left+ m_kCoordLeft.iWidth + m_iPos, m_kRect.top, m_kCoordRight.iWidth, m_kCoordRight.iHeight, &dcMask, 
		m_kCoordRight.iPosX, m_kCoordRight.iPosY, m_kCoordRight.iWidth, m_kCoordRight.iHeight, SRCAND);    
	pkMemDC_->StretchBlt(m_kRect.left+ m_kCoordLeft.iWidth + m_iPos, m_kRect.top, m_kCoordRight.iWidth, m_kCoordRight.iHeight, &dcBmp, 
		m_kCoordRight.iPosX, m_kCoordRight.iPosY, m_kCoordRight.iWidth, m_kCoordRight.iHeight, SRCINVERT); 

	dcBmp.SelectObject(pOldBmp);		
	dcMask.SelectObject(pOldMask);	
}

int KProgressCtrl::GetBitmapWidth (CBitmap* pBitMap)
{ 
    BITMAP bm;
    pBitMap->GetObject(sizeof(BITMAP),(PSTR)&bm); 
    return bm.bmWidth;
}
int KProgressCtrl::GetBitmapHeight (CBitmap* pBitMap)
{ 
    BITMAP bm; 
    pBitMap->GetObject(sizeof(BITMAP),(PSTR)&bm); 
    return bm.bmHeight;
}
