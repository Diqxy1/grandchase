#pragma once

class KGCSuperGachaSelectWnd;

class KGCSuperGachaDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSuperGachaDlg );
    /// Default constructor
    KGCSuperGachaDlg( void );
    /// Default destructor
    virtual ~KGCSuperGachaDlg( void );
    /// Copy constructor
    KGCSuperGachaDlg( const KGCSuperGachaDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSuperGachaDlg& operator=( const KGCSuperGachaDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*    m_pkBtnOK;
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkHotMark;

    KGCSuperGachaSelectWnd*     m_pkSelectWnd;

private:
    int     m_iCharType;
    int     m_iPromotionLevel;
    int     m_iExtraCharNum;
    int     m_iVersion;

    std::map< int, std::map< int, std::set< GCITEMID > > >      m_mapRewardInfo;    // < 버전, < 캐릭터타입, 장비리스트> > - 장비 리스트 ( 장비 + 악세 )
    std::map< int, std::map< int, std::vector< GCITEMID > > >   m_mapRewardSetInfo; // < 버전, < 캐릭터타입, 장비리스트> > - 최종보상 리스트( 무기 + 펫 )
    std::map< int, std::map< int, int > >                       m_mapNeedKeyInfo;   // < 버전, < 레벨, 레벨별 필요 해제서 개수 > > - 장비가챠용 
    std::map< int, bool >                                       m_mapAccEnable;     // < 버전, m_bAccessoriesEnable > - 장비가챠용      
    std::map< int, GCITEMID >                                   m_mapSealInfo;      // < 캐릭터 타입, 해당 캐릭터 구슬 ID >

private:
    enum 
    {
        VIEWPORT_OFFSET_X = 206,
        VIEWPORT_OFFSET_Y = 179,
        VIEWPORT_WIDTH    = 265,
        VIEWPORT_HEIGHT   = 298,
    };

    D3DVIEWPORT9        m_viewport;
    D3DXMATRIX          m_matProjection;

    PLAYER              m_Player;
    GCObject*           m_pmeshStage;
    GCDeviceMotionFRM*  m_pShopMotion;

    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;

    KPet*               m_pkPet;

    int     m_iMotionCount;
    int     m_cFrame;
    float   m_fCharPosX;

private:
    POINT   m_ptStartMouse;
    float   m_fStartAngle;
    float   m_fStartStageAngle;
    float   m_fAngleMove;
    float   m_fStageAngleMove;
    float	m_fZoom;
    bool	m_bAngleChanged;

public:
    void OnClickClose( void );
    void OnClickOK( void );

    void SetSealInfo( void );
    GCITEMID GetSealID( void );
    int      GetCharType( const GCITEMID SealID_ );

    void Send_RewardListReq( void );                        // 장비보상정보 요청,세팅
    void SetRewardList( const KGachaRewardList& list_ );
    void Send_RewardSetListReq( void );                     // 최종보상정보 요청, 세팅 
    void SetRewardSetList( const KGachaRewardList& list_ );

    void SetSelectVersion( const int iVer_ );

    bool GetRewardMap( OUT std::map< int, std::set< GCITEMID > >& mapReward_ );
    bool GetRewardMap( const int iVer_, const int iCharType_, OUT std::map< int, std::set< GCITEMID > >& mapReward_ );
    bool GetRewardSetMap( OUT std::map< int, std::vector< GCITEMID > >& mapRewardSet_ );
    bool GetRewardSetMap( const int iVer_, const int iCharType_, OUT std::map< int, std::vector< GCITEMID > >& mapRewardSet_ );
    bool GetNeedKeyInfo( const int iVer_, OUT std::map<int, int>& mapKeyInfo_ );
    bool GetAccEnable( const int iVer_ );

public:
    void SetPlayer( void );
    void SetPet( void );
    void UpdateMeshObject( void );
    void DeleteMeshObject( void );
    void EquipItem( void );

public:
    void ChangeCharWndZoom( bool bZoom );
    void ProcessCharWndAngle( void );
    void ReloadMotion();

public:
	enum {
		GACHA_TYPE_EQUIP = 0,
		GACHA_TYPE_COORD = 11,
        GACHA_TYPE_DIAMOND = 22
	};
};

DEFINE_WND_FACTORY( KGCSuperGachaDlg );
DECLARE_WND_FACTORY( KGCSuperGachaDlg );
DECLARE_WND_FACTORY_NAME( KGCSuperGachaDlg );
