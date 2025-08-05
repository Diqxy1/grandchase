#pragma once
#include <string>
#include "../kom/kom.h"

struct KTexCoord
{
    int iPosX;
    int iPosY;
    int iWidth;
    int iHeight;
};

class KProgressCtrl : public CStatic
{
public:
    CBitmap*        m_pkBarBitMap;
    CBitmap*        m_pkMask;
    HANDLE          m_hBarBitMap;
    KTexCoord       m_kCoordLeft;
    KTexCoord       m_kCoordBar;
    KTexCoord       m_kCoordRight;
    RECT            m_kRect;
    int             m_iPos;
    float           m_fPercent;
    int             m_iWidth;
    int             m_iHeight;
public:
    KProgressCtrl(void);
    ~KProgressCtrl(void);

    void InitializeCtrl( Komfile *massfile, std::string strName_, RECT rt_ );
    void SetProgress( float fPos_ );
    void SetRect( RECT kRect_ );
    void DrawProgressBar( CDC* pkMemDC_ );
    int GetBitmapWidth (CBitmap* pBitMap);
    int GetBitmapHeight (CBitmap* pBitMap);
};

extern void LoadBitmapFromMassFile( Komfile * massfile,std::string strFileName_, CBitmap** ppBitmap_ );


