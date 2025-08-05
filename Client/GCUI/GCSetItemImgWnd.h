#ifndef _GCSETITEMIMGWND_H_
#define _GCSETITEMIMGWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"

class GCDeviceTexture;

class KGCSetItemImgWnd : public KD3DWnd          // extends
{
public:
    DECLARE_CLASSNAME( KGCSetItemImgWnd );
    /// Default constructor
    KGCSetItemImgWnd( void );
    /// Default destructor
    virtual ~KGCSetItemImgWnd( void );
    /// Copy constructor
    KGCSetItemImgWnd( const KGCSetItemImgWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSetItemImgWnd& operator=( const KGCSetItemImgWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    void SetItemInfo( int iSetNumber_, DWORD dwGoodsID_ , float fSizeX = 155.f, float fSizeY = 287.f);
    void ShowItem( bool bShow_ );
protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    D3DXVECTOR4 GetTextureCoords( int iSetNum );
protected:
    GCDeviceTexture*        m_pTexture;
    bool                    m_bShow;
    int                     m_iSetNumber;
private:
    float m_fSizeX;
    float m_fSizeY;
};

DEFINE_WND_FACTORY( KGCSetItemImgWnd );
DECLARE_WND_FACTORY( KGCSetItemImgWnd );
DECLARE_WND_FACTORY_NAME( KGCSetItemImgWnd );

#endif//_GCSETITEMIMGWND_H_