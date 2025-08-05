#pragma once

class KGCAgitCharEquipSlot;
class KGCAgitCharItemSlot;
class KGCAgitCharActionSlot;

class KGCAgitCharSetting : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { SLOT_HELMET = 0, 
           SLOT_JACKET = 1, 
           SLOT_PANTS = 2, 
           SLOT_GLOVES = 3, 
           SLOT_SHOES = 4, 
           SLOT_MANTLE = 5, 
           SLOT_A_UPPER_HELMET = 6, 
           SLOT_A_DOWN_HELMET = 7, 
           SLOT_A_JACKET = 8, 
           SLOT_A_PANTS = 9, 
           SLOT_A_ARM = 10, 
           SLOT_A_SHOES = 11 
         };

    enum { COLS = 3, ROWS = 4, NUM_ITEM_SLOT = 12 };
    enum { NUM_CHAR_MENT = 3, NUM_ACTION_SLOT = 4 };


private:
    enum
    {
        VIEWPORT_OFFSET_X   = 125,
        VIEWPORT_OFFSET_Y   = 155,
        VIEWPORT_WIDTH      = 160,
        VIEWPORT_HEIGHT     = 220,
    };

public:
    DECLARE_CLASSNAME( KGCAgitCharSetting );
    /// Default constructor
    KGCAgitCharSetting( void );
    /// Default destructor
    virtual ~KGCAgitCharSetting( void );
    /// Copy constructor
    KGCAgitCharSetting( const KGCAgitCharSetting& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitCharSetting& operator=( const KGCAgitCharSetting& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnOK;
    KD3DWnd*    m_pkBtnCancle;
    KD3DWnd*    m_pkWndCharView;

    KD3DStatic* m_pkStaticActionSetting;
    KD3DStatic* m_pkStaticMentSetting;
    KD3DStatic* m_pkStaticMent0;
    KD3DStatic* m_pkStaticMent1;
    KD3DStatic* m_pkStaticMent2;
    KSafeArray< KD3DEdit*, NUM_CHAR_MENT > m_apkEditMent;
    
    std::vector< KItem* >  m_vecInvenItemList;
    KSafeArray< KGCAgitCharEquipSlot*, NUM_ITEM_SLOT >  m_apkEquipSlot;
    KSafeArray< KGCAgitCharItemSlot*, NUM_ITEM_SLOT >   m_apkItemSlot;

    int m_iItemScrollPage;
    int m_iItemScrollPos;
    int m_iBeforeItemScrollPos;
    KD3DScrollbar*  m_pkItemListScroll;

    // 미리보기 관련 
    PLAYER*             m_pPlayer;
    D3DVIEWPORT9        m_ViewPort;
    D3DXMATRIX          m_matProjection;
    GCDeviceMotionFRM*  m_pViewMotion;   

    int     m_iCharType;
    int     m_iMotionCount;
    int     m_iFrame;
    float   m_fCharPosX;
    float   m_fCharPosY;

    // 모션 관련
    std::vector< std::pair< int, int > >                  m_vecCharacterMotionList; // < index, stringID >
    KSafeArray< KGCAgitCharActionSlot*, NUM_ACTION_SLOT > m_apkActionSlot;
    std::string m_strCurMotion;

    int m_iActionScrollPage;
    int m_iActionScrollPos;
    int m_iBeforeActionScrollPos;
    int m_iCurSelActionIndex;
    KD3DScrollbar*  m_pkActionListScroll;

public:
    void InitUI( int iCharType_ );
    void OnOK( void );
    void Onclose( void );

    void OnClickItemSlot( const int iIndex_ );
    void OnClickEquipSlot( void );

private:
    void SetItemList( int iCharType_ );
    void ClearItemList( void );
    void SetItemSlotList( void );

    void ClearItemSlotList( void );
    void ClearEquipSlotList( void );
    void ClearAllSlotList( void );

    void InitEquipSlotPosition( void );
    void InitEquipSlotList( void );
    void InitItemListScroll( void );
    void OnItemListScroll( void );
    void UpdateItemList( const int iIndex_ );
    void UpdateEquipItem( void );
    int  GetEquipSlotIndex( DWORD dwSlotPosition_ );

    // 미리보기 관련 
    void SetViewCharacter( int iCharType_ );
    void UpdateMeshObject( void );
    void EquipItem( std::vector< GCITEMUID >& vecItemList_ );
    void UnequipAllItem( void );
    bool GetEquipItemList( OUT std::vector< GCITEMUID >& vecItemList_ );
    void SaveCoordiInfo( void );
    void SetViewMotion( void );

    // 모션 관련 
    void SetMotionListVector( void );
    void InitActionListScroll( void );
    void OnActionListScroll( void );
    void UpdateActionList( int iIndex_ );
    void OnClickActionSlot( int iIndex_ );
    void SaveActionIndex( void );
    
    // 대사 관련
    void InitCharacterMent( void );
    void InitCharacterMentEdit( int iMentType_ );
    
public:
    void InitMent( void );
    void SaveCharacterMent( void );
};

DEFINE_WND_FACTORY( KGCAgitCharSetting );
DECLARE_WND_FACTORY( KGCAgitCharSetting );
DECLARE_WND_FACTORY_NAME( KGCAgitCharSetting );
