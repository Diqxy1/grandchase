#pragma once

class KGCAgitObjectSlot;

class KGCAgitObjectPlaceCollectUI : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{

public:
    enum { NUM_SLOT = 8 };
    enum BTN_TYPE { BTN_FIRST = 0, BTN_PREV, BTN_END, BTN_NEXT, BTN_CLOSE, NUM_BTN };
    enum TAB_TYPE { TAB_OBJ_DEFAULT = 0, TAB_OBJ_ACTIVE, TAB_CHAR_DEFAULT, TAB_CHAR_ACTIVE, NUM_TAB };

public:
    DECLARE_CLASSNAME( KGCAgitObjectPlaceCollectUI );
    /// Default constructor
    KGCAgitObjectPlaceCollectUI( void );
    /// Default destructor
    virtual ~KGCAgitObjectPlaceCollectUI( void );
    /// Copy constructor
    KGCAgitObjectPlaceCollectUI( const KGCAgitObjectPlaceCollectUI& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitObjectPlaceCollectUI& operator=( const KGCAgitObjectPlaceCollectUI& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KSafeArray< KGCAgitObjectSlot*, NUM_SLOT > m_apkSlot;
    KSafeArray< KD3DWnd*, NUM_BTN >            m_apkBtn;
    KSafeArray< KD3DWnd*, NUM_TAB >            m_apkTab;
    KD3DWnd*    m_pkRollback;
    KD3DStatic* m_pkStaticPage;

    int m_iCurPage;
    int m_iTotalPage;
    int m_iCurSlot;
    std::vector< std::pair< GCITEMID, int > >           m_vec2DObject;      // 배치 가능한 오브젝트들 vector< pair<ID, 개수> >
    std::vector< std::pair< int, KAgitCharacterPos > >  m_vecCharObject;    // 배치 가능한 캐릭터 vector< pair<캐릭터 타입, 배치 정보> > 

    TAB_TYPE    m_eCurTab;

public:
    void InitUI( void );
    void UpdateData( void );
    void SetObjectInfo( std::map< GCITEMID, int >& mapObj_ );
    void SetCharacterInfo( std::map<int, KAgitCharacterPos>& mapChar_ );
    void InitPage( void );
    void SetSlot( int iPage_ );
    void SetSlot( void );
    void SetPageStatic( void );
    void ClearSlot( void );
    
    void OnClickSlot( int iIndex_ );
    void OnClickTab( TAB_TYPE tabType_ );
    void OnClickPageBtn( BTN_TYPE btnType_ );
    void OnClickRollback();
    void OnClose( void );
    void RefreshCharTab( bool bIsTrue );
};

DEFINE_WND_FACTORY( KGCAgitObjectPlaceCollectUI );
DECLARE_WND_FACTORY( KGCAgitObjectPlaceCollectUI );
DECLARE_WND_FACTORY_NAME( KGCAgitObjectPlaceCollectUI );
