#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DSizingBox;

class KGCTitleSlotRow : public KD3DWnd,          // extends
                        public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCTitleSlotRow );
    /// Default constructor
    KGCTitleSlotRow( void );
    /// Default destructor
    virtual ~KGCTitleSlotRow( void );
    /// Copy constructor
    KGCTitleSlotRow( const KGCTitleSlotRow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCTitleSlotRow& operator=( const KGCTitleSlotRow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum { NUM_OF_TITLE_COL = 3, REFRESH_TITLE_LIST = 10000 };
    enum TITLE_SLOT_BTN { TSB_GET = 0, TSB_ON, TSB_OFF, TSB_CONDITION, TSB_NONE };

    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void PostDraw( void );
    virtual void FrameMoveInEnabledState( void );

    bool IsTitleSlotIndex( int iIndex );

    void SetClicked( int iIndex, bool bOn );
    void SetUseBtnType( int iIndex, TITLE_SLOT_BTN enType );
    void SetMissionID( int iIndex, int iMissionID ) { m_iMissionID[iIndex] = iMissionID; }
    int GetMissionID( int iIndex ) { return m_iMissionID[iIndex]; }

    void ReSet(void);
    void DrawTitle(void);
    void SetItemInfo(int iIndex, int iItemID, GCDeviceTexture *pTextureItem);
	void FrameMove_Alpha(void);

private:
    KSafeArray<KD3DSizingBox*,NUM_OF_TITLE_COL> m_pkBGSlot;
    KSafeArray<KD3DSizingBox*,NUM_OF_TITLE_COL> m_pkBGClicked;

    KSafeArray<KD3DWnd*,NUM_OF_TITLE_COL> m_pkOn;
    KSafeArray<KD3DWnd*,NUM_OF_TITLE_COL> m_pkOff;
    KSafeArray<KD3DWnd*,NUM_OF_TITLE_COL> m_pkGet;
    KSafeArray<KD3DWnd*,NUM_OF_TITLE_COL> m_pkCondition;

    KSafeArray<int,NUM_OF_TITLE_COL> m_iMissionID;
    KSafeArray<int,NUM_OF_TITLE_COL> m_iItemID;
    KSafeArray<KD3DWnd*, NUM_OF_TITLE_COL> m_pkTimed;
    KSafeArray<GCDeviceTexture*,NUM_OF_TITLE_COL> m_pTextureItem;
};

DEFINE_WND_FACTORY( KGCTitleSlotRow );
DECLARE_WND_FACTORY( KGCTitleSlotRow );
DECLARE_WND_FACTORY_NAME( KGCTitleSlotRow );
