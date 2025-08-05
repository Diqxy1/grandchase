#ifndef _GCUSEREQUIPBOX_H_
#define _GCUSEREQUIPBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"

class KGCUserEquipWnd;
class KGCUserCoreEquipWnd;
class KD3DScrollbar;
class KGCPetWnd;
class KGCPetInfoBox;
struct KInventoryItem;
class KGCLookSavePanel;
class KGCPetItembox;

class KGCZeroMaskDlg;

class KGCUserEquipBox : public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCUserEquipBox );
    /// Default constructor
    KGCUserEquipBox( void );
    /// Default destructor
    virtual ~KGCUserEquipBox( void );
    /// Copy constructor
    KGCUserEquipBox( const KGCUserEquipBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCUserEquipBox& operator=( const KGCUserEquipBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_PET_WND = 2,
        NUM_ITEM_WND = 8,
        NUM_CATEGORY = 12,
        NUM_LOOK_CATEGORY = 15,

		NUM_1STEPSKILL = 3,
		NUM_2STEPSKILL = 3,
		NUM_3STEPSKILL = 3,

		NUM_POTION_WND = 5,
		NUM_CLOSED_WND = 10,      
		NUM_SHORTCUT_WND = 15,
        NUM_LOOK_WND = 15,
        NUM_LOOK_WND_EX = 19,   // 쩐?쨔??쨍 ??쨉챨 째쨀?챠
    };

public:
    void OnClickBtnMask();
    void SetItemCategory(USER_EQUIP_CATEGORY eCategory );
	bool GetCurrentHoveredItem( GCItem*& pItem, KItem*& pkInventoryItem, D3DXVECTOR2 &vecPos );
	void LockEquipment( bool bLock );

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void Clear();
    void ResetWindow();
    void ResetScroll();

    template< class T >
    bool InitSlotInfo( GCItem*& pItem, KItem*& pkInventoryItem, D3DXVECTOR2 &vecPos, const T& pWnd )
    {
        POINT   pt = g_pkInput->GetMousePos();

        int iStart = 0;
        if( m_ItemCategory == UEC_SHORTCUT_SLOT || m_ItemCategory == UEC_EQUIP2 )
            iStart = m_iScrollTop*5;
        else
            iStart = m_iScrollTop*4;

        int iMaxWnd = NUM_ITEM_WND;
        if( m_ItemCategory == UEC_EQUIP2 )
            iMaxWnd = NUM_CLOSED_WND;
        else if( m_ItemCategory == UEC_LOOK )
        {
            iMaxWnd = m_nLookWnd;
        }


        for( int i=iStart ; i<iStart+iMaxWnd ; ++i )
        {
            if( i >= (int)pWnd.size() )
                continue;

            if ( !pWnd[i]->IsRenderOn() )
                continue;

            if ( pWnd[i]->CheckPosInWindowBound( D3DXVECTOR2( (float)pt.x, (float)pt.y ) ) )
            {
                if( pWnd[i]->GetItemID() == -1 )
                    return false;

                pItem = pWnd[i]->GetItemInfo();
                pkInventoryItem = pWnd[i]->GetItemData();

                vecPos = pWnd[i]->GetCurrentWindowPos();
                vecPos.x += ( pWnd[i]->GetWidth() );
                vecPos.y += ( pWnd[i]->GetHeight() / 2.0f );
				vecPos.x *= g_pGCDeviceManager2->GetWindowScaleX();
				vecPos.y *= g_pGCDeviceManager2->GetWindowScaleY();

				if( i%4 == 3 )
				{
					vecPos.x -= ( static_cast<float>( pWnd[i]->GetWidth() ) * m_fWindowScaleX );
					vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth();
				}
                return true;
            }
        }
        return false;
    }

protected:
    KSafeArray<KGCUserCoreEquipWnd*,NUM_ITEM_WND> m_apItemWnd;
    KSafeArray<KGCUserCoreEquipWnd*,NUM_CLOSED_WND> m_apAccessoryWnd;

	KSafeArray<KD3DWnd*,NUM_CLOSED_WND> m_apCloseAcceWnd;


    // 펫 스킬 쉽게 사자
#if defined ( USE_SIMPLE_PET_BUY )
    KSafeArray<KGCPetItembox*,NUM_PET_WND> m_apPetItemWnd;
#else
    KSafeArray<KGCUserEquipWnd*,NUM_PET_WND> m_apPetItemWnd;
#endif
    
    KSafeArray<KGCUserEquipWnd*,NUM_CATEGORY> m_apSkillWnd;
    KSafeArray<KGCUserEquipWnd*,NUM_SHORTCUT_WND> m_apShortCutWnd;
    KSafeArray<KGCUserEquipWnd*,NUM_LOOK_WND_EX> m_apLookWnd;
    KSafeArray<KD3DWnd*,NUM_ITEM_WND> m_apWnd;
	KSafeArray<KD3DWnd*,NUM_CLOSED_WND> m_pkClosedShortCutWnd;
	KSafeArray<KD3DWnd*,NUM_ITEM_WND> m_pkClosedNomalItemWnd;

	KSafeArray<KD3DWnd*,NUM_ITEM_WND> m_pkClosedapItemWnd;

	KD3DWnd*				m_pkItemButton;
	KD3DWnd*				m_pkEmoticonButton;
    KD3DScrollbar*          m_pkScrollbar;
    KD3DSizingBox*          m_pkScrollbarBack;
    USER_EQUIP_CATEGORY     m_ItemCategory;
    KGCLookSavePanel*       m_pkLookSavePanel;
	KD3DWnd*				m_pkBtnLookChang;
    KD3DWnd*                m_pkBtnCoordiCompose;
    KD3DWnd*                m_pkBtnCoordiUpgrade;
    KD3DWnd*				m_pkBtnMaskSelect;

    //Zero Mask
    KGCZeroMaskDlg*         m_pkZeroMaskDlg;

    int m_iScrollTop;
	bool m_bEquipLock;

private:
    int m_nLookWnd;     //  코디 개편에 따른 코디 슬롯 갯수

public:
    KGCPetWnd*              m_pkPetWnd;
    KGCPetInfoBox*          m_pkPetInfoBox;

public:
	USER_EQUIP_CATEGORY GetCurrentCategory(void){ return m_ItemCategory; }
	void SetScrollPos(int Pos);
	int GetCurItemSlot();
    void RenderAccessoryWnd( bool bRender );
    void RenderLookWnd( bool bRender );
	void OnClickBtnChange();
    void OnClickBtnCompose();
    void OnClickBtnUpgrade();
        
	void UnEquipPetItemForOtherChar( void );
public:
    void SetItemLookCategory( USER_EQUIP_CATEGORY eCategory, KUserItemVector* vecLookInfo );
    void SetItemEquipCategory( USER_EQUIP_CATEGORY eCategory, KUserItemVector* vecItemInfo );
    void SetItemShotcutCategory( USER_EQUIP_CATEGORY eCategory );
    void SetItemPetCategory( USER_EQUIP_CATEGORY eCategory, SCharInfo* pCharInfo );
};

DEFINE_WND_FACTORY( KGCUserEquipBox );
DECLARE_WND_FACTORY( KGCUserEquipBox );
DECLARE_WND_FACTORY_NAME( KGCUserEquipBox );

#endif // _GCUSEREQUIPBOX_H_
