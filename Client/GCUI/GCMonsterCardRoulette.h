#pragma once

class KGCMonsterCardContents;

class KGCMonsterCardRoulette : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMonsterCardRoulette );
    /// Default constructor
    KGCMonsterCardRoulette( void );
    /// Default destructor
    virtual ~KGCMonsterCardRoulette( void );
    /// Copy constructor
    KGCMonsterCardRoulette( const KGCMonsterCardRoulette& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterCardRoulette& operator=( const KGCMonsterCardRoulette& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum EM_CARD_COMPOSE_STATE
	{
		EM_CARD_COMPOSE = 0,	//	합성중
		EM_CARD_WAIT	= 1,	//	결과 대기중
		EM_CARD_RESULT  = 2,	//	결과 출력중
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	void Init();
	void SendComposeReq();
	void SetResult( GCITEMID _itemID );
	void SetRewardList ( std::vector<GCITEMID>& _vectorReward ) { m_vecRewardList = _vectorReward; } 
	void OnOK();

    KGCAttributeTable GetAttributebyMonsterCardID(DWORD dMonsterCardID);
    int  GetValuebyMonsterCardID(DWORD dMonsterCardID);
    D3DXVECTOR4  GetTexCoordbyMonsterCardID(DWORD dMonsterCardID);
    int GetMonsterCardGrade(DWORD dwItemID);
public:
	int	m_nCardChangeFrame;
	int	m_nCardMaxFrame;
    bool m_bCriticalRate;
    int m_iOrderComma;
	EM_CARD_COMPOSE_STATE	m_emState;

	GCITEMID				m_itemID;

	std::vector<GCITEMID>	m_vecRewardList;

public:
	KGCMonsterCardContents*		m_pkCardImageOn;
    KGCMonsterCardContents*         m_pkMonsterCardContentsBigDummy;
    KGCMonsterCardContents*         m_pkMonsterCardContentsDummy;

	KD3DSizingBox*			m_pkCardImageOnBack;

	KGCMonsterCardContents*	m_pkItemSlot;
	KD3DWnd*				m_pkOKBtn;
    KD3DWnd*				m_pkTitleFusing;
    KD3DWnd*				m_pkTitleGetCard;

};

DEFINE_WND_FACTORY( KGCMonsterCardRoulette );
DECLARE_WND_FACTORY( KGCMonsterCardRoulette );
DECLARE_WND_FACTORY_NAME( KGCMonsterCardRoulette );
