#ifndef _GCITEMIMGWND_H_
#define _GCITEMIMGWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"

class GCDeviceTexture;
class KGCUINumber;

class KGCItemImgWnd : public KD3DWnd          // extends
{
public:
    DECLARE_CLASSNAME( KGCItemImgWnd );
    /// Default constructor
    KGCItemImgWnd( void );
    /// Default destructor
    virtual ~KGCItemImgWnd( void );
    /// Copy constructor
    KGCItemImgWnd( const KGCItemImgWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemImgWnd& operator=( const KGCItemImgWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
    void SetItemInfo( DWORD dwGoodsID_ ,DWORD dwColor = -1, bool bSetItem = false, int iSetID = -1, GCITEMUID dwUID = 0, int nCount = -1, GCITEMID dwCoordiID = 0 );
    void SetCustomItemInfo(DWORD dwGoodsID_, DWORD dwColor = -1, bool bSetItem = false, int iSetID = -1, GCITEMUID dwUID = 0, int nCount = -1, GCITEMID dwCoordiID = 0);
	void SetPetItemInfo( DWORD dwGoodsID_, int iEvolutionLv, DWORD dwColor = -1 );
    void SetColor( IN const DWORD dwColor_ ) { m_dwColor = dwColor_; }
    DWORD GetColor() { return m_dwColor; }
    void ShowItem( bool bShow_ );	
    bool IsShowItem( void ) { return m_bShow; }
	DWORD GetItemID() { return m_dwItemID; }

	

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void BindToLua();
protected:
    GCDeviceTexture*        m_pTexture;
	KGCUINumber*			m_pkEquipEnchantItemLv;
    KGCUINumber*			m_pkCount;
    
    DWORD                   m_dwColor;
	DWORD					m_dwItemID;
    GCITEMID                m_DesignCoordiID; //  코디 외형 ID
	int						m_iSetID;
    bool                    m_bLikeBtn;
    bool                    m_bEmoticon;
	int						m_iStrong;
    bool                    m_bShow;

public:
    void SetLikeBtn(bool bLikeBtn_ ) { m_bLikeBtn = bLikeBtn_; }
    void SetItemInfoLua( DWORD dwGoodsID_, bool bSetItem, int iSetID );
    void SetItemTexture( GCDeviceTexture* pTex );
};

DEFINE_WND_FACTORY( KGCItemImgWnd );
DECLARE_WND_FACTORY( KGCItemImgWnd );
DECLARE_WND_FACTORY_NAME( KGCItemImgWnd );

#endif//_GCITEMIMGWND_H_