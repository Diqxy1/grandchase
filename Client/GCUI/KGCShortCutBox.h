#ifndef _KGCSHORTCUTBOX_H_
#define _KGCSHORTCUTBOX_H_

//#include "../uifx/D3DWnd.h"
#include "../GCShortCutSlot.h"
//#include "KGCItemSlot.h"
//#include "KGCEmoticonSlot.h"

class KGCItemSlot;
class KGCEmoticonSlot;
class KGCMacroCommunitySlot;

enum SCB_MODE_STATE
{
	SCB_MODE_ALL = 0,
	SCB_MODE_ITEM,
	SCB_MODE_EMOT,
    SCB_MODE_STATE_NUM,
};

enum SCB_SHOW_STATE
{
    SFS_HIDE = 0,
    SFS_FADE_IN,
    SFS_FADE_OUT,
    SFS_SHOW,

};

class KGCShortCutBox : public KD3DWnd,
					   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCShortCutBox );

	KGCShortCutBox();
	virtual ~KGCShortCutBox(void);
	/// Copy constructor
	KGCShortCutBox( const KGCShortCutBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCShortCutBox& operator=( const KGCShortCutBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:    
	void OnInitialize();
	void ActionPerformed( const KActionEvent& event );

	void SetItemPos( float Pos );
	void SetItemAlpha( int Var );

	void SetShortCutMode( SCB_MODE_STATE state );
    void SetShowState( SCB_SHOW_STATE state ) 
    { 
        m_eShowState = state; 
        if (m_eShowState == SFS_SHOW || m_eShowState == SFS_FADE_IN)
        {
            ToggleRender( true );
        }
    }
    SCB_SHOW_STATE GetShowState() { return m_eShowState;}
    bool GetShowInfo( void ) 
    {
        if (m_eShowState == SFS_SHOW || m_eShowState == SFS_FADE_IN)
        {
            return true;
        }

        return false;
    }

	void ResetEmotPos();
    void ResetMcommuPos();
    void ResetBoxPos()
    {
        SetWindowPos( m_vOriginalPos );
    }

    void ToggleShowState();

protected:
	virtual void OnCreate( void );
	virtual void OnDestroy( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostDraw( void );

protected:
	//	Item관련 정보를 얻어오는 함수랑
	void SetItemID( int index );
	void SetItemQuantity( int index );
	void SetItemCoolTime( int index );
	//	Emoticon 관련 정보를 얻어오는 함수가 다르다
	void SetEmotID( int index );
	void SetEmotQuantity( int index );
	void SetEmotCoolTime( int index );

protected:
	SCB_MODE_STATE m_eModeState;
    SCB_SHOW_STATE m_eShowState;
	KSafeArray<int,ITEM_SLOT_SIZE+EMOT_SLOT_SIZE> m_iID;
	KSafeArray<int,ITEM_SLOT_SIZE+EMOT_SLOT_SIZE> m_iQuantity;
	KSafeArray<int,ITEM_SLOT_SIZE+EMOT_SLOT_SIZE> m_iCooltime;

protected:
	KSafeArray<KGCItemSlot*,ITEM_SLOT_SIZE> m_pkItem_Slot;			//	ITEM_SLOT
	KSafeArray<KGCEmoticonSlot*,EMOT_SLOT_SIZE> m_pkEmot_Slot;			//	EMOTICON_SLOT
    KSafeArray<KGCMacroCommunitySlot*,MCOMMU_SLOT_SIZE> m_pkMcommu_Slot;	//	MACRO_COMMUNITY_SLOT

	KD3DWnd*			m_pkWaveKey;							// 물결무늬 버튼

    KSafeArray<D3DXVECTOR2,ITEM_SLOT_SIZE> m_vFixedItemSlotPos;	//	ITEM_SLOT 고정 좌표
	KSafeArray<D3DXVECTOR2,EMOT_SLOT_SIZE> m_vFixedEmotSlotPos;	//	EMOTICON_SLOT 고정 좌표
    KSafeArray<D3DXVECTOR2,MCOMMU_SLOT_SIZE> m_vFixedMcommuSlotPos;	//	MCOMMU_SLOT 고정 좌표

    D3DXVECTOR2         m_vOriginalPos;    

public:
    void SetInputCommunityKey( int iKeyIndex, int iCoolTime );
    void MacroCommunityFrameMove();
};

DEFINE_WND_FACTORY( KGCShortCutBox );
DECLARE_WND_FACTORY( KGCShortCutBox );
DECLARE_WND_FACTORY_NAME( KGCShortCutBox );

#endif
