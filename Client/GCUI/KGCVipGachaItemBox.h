#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../UIFX/D3DWnd.h"
#include "../uifx/ActionListener.h"

struct GCItem;
class KD3DStatic;
class KD3DSizingBox;
class KGCVipGachaItemBox : public KD3DWnd,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCVipGachaItemBox );
	/// Default constructor
	KGCVipGachaItemBox( void );
	/// Default destructor
	virtual ~KGCVipGachaItemBox( void );
	/// Copy constructor
	KGCVipGachaItemBox( const KGCVipGachaItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCVipGachaItemBox& operator=( const KGCVipGachaItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void	OnClickBuyBtn( void );
    void    BuyItemProcedure( void );
    void    SetMouseOver( bool bMouseOver ) { m_bMouseOver = bMouseOver; }
    void    SetItemInfo( int iItemID, int iCoinNum );
    int     GetItemID( void ) { return m_iItemID; }
    bool    IsEquiped( void ) { return m_bIsEquip; }
    void    SetEquiped( bool equip );
    void    SetIndex( int index ) { m_iIndex = index; }

protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );

    void IsExistItem( void );

protected:
 	KD3DWnd*        m_pkBuyBtn;
    KD3DWnd*        m_pkAlreadyGet;
    KD3DWnd*        m_pkGachaCoin;
    KD3DSizingBox*  m_pkEquipedBack;
    KD3DSizingBox*  m_pkMouseOver;
 	KD3DStatic*     m_pkNeedBadgeNum;

    GCDeviceTexture*m_pTextureItem;

    bool        m_bMouseOver;
    bool        m_bExistItem;
    bool        m_bIsEquip;

    int         m_iItemID;
    int         m_iCoinNum;
    int         m_iIndex;

    GCItem*     m_ItemInfo;
};

DEFINE_WND_FACTORY( KGCVipGachaItemBox );
DECLARE_WND_FACTORY( KGCVipGachaItemBox );
DECLARE_WND_FACTORY_NAME( KGCVipGachaItemBox );

