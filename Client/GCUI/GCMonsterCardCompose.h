#pragma once



typedef std::pair<GCITEMID,int>		PAIR_MONSTER_CARD;

class KGCMonsterCardRoulette;
class KGCMonsterCardInven;
class KGCCheckBox;
class KGCItemImgWnd;
class KGCMonsterCardContents;
class KGCItemImgUIWnd;

class KGCMonsterCardCompose : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMonsterCardCompose );
    /// Default constructor
    KGCMonsterCardCompose( void );
    /// Default destructor
    virtual ~KGCMonsterCardCompose( void );
    /// Copy constructor
    KGCMonsterCardCompose( const KGCMonsterCardCompose& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterCardCompose& operator=( const KGCMonsterCardCompose& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum
	{
		FLAG_GRADE_NORMAL    = 0x0001,
		FLAG_GRADE_REAR      = 0x0002,
		FLAG_GRADE_EPIC      = 0x0004,
		FLAG_GRADE_REGEOND   = 0x0008,
		FLAG_GRADE_ALL       = FLAG_GRADE_NORMAL + FLAG_GRADE_REAR + FLAG_GRADE_EPIC + FLAG_GRADE_REGEOND,
	};

	enum
	{
		EM_GRADE_NORMAL     = 0,
		EM_GRADE_REAR       = 1,
		EM_GRADE_EPIC       = 2,
		EM_GRADE_REGEOND    = 3,
		EM_GRADE_SIZE       = 4,
	};

	enum
	{
#if defined( NATION_KOREA )
        EM_MIN_CARD_MIX	= 2,
#else 
		EM_MIN_CARD_MIX	= 2,
#endif
		EM_INVEN_SLOT_SIZE	= 7,
		EM_MATERIAL_SLOT_SIZE	= 7,
	};

	enum
	{
		SEL_CARD_STATE	= 0,
		MOVE_CARD_STATE = 1,
		MIX_CARD_STATE	= 2,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );

public:
	void OnOK();
	void OnClose();
	void OnCheckBtn1();
	void OnCheckBtn2();
	void OnCheckBtn3();
	void OnCheckBtn4();
	void ClickCheckBtn( int iCheck );

	void OnPrevMove();
	void OnNextMove();
	void OnClickOkCard();
	void OnClickInvenSlot( int nSlot );
	void OnClickMaterialSlot( int nSlot );

	void UpdateData();
	void UpdateItemList();
	void UpdateInventory();
	void UpdateMaterial();
	void UpdateCheckBtn();

	bool IsGradeCheck( char cItemGrade );

	
	DWORD GetGradeFlag( int emGrade );
	int GetReMainCardNum( int nIndex );

	void SetResult( GCITEMID _itemID );
	void SetFailResult();

	void ReqMonsterCardMix();
	void ButtonLock( bool _bLock );

	void InitMeterialPos();
	void ResetMeterialPos();
	void OpenRoulette();

    void UpdateSendReq() { m_bSendScriptReq = true; }
    bool GetSendReq() { return m_bSendScriptReq; }

    void SetPrevIndex( void );  // 마지막에 사용한 페이지로 세팅 

   
public:
	bool		m_bMoveCard;
	DWORD       m_dwGradeFlag;
	int			m_nIndex;
    int         m_nPrevIndex;

	DWORD		m_dwState;

	std::vector<PAIR_MONSTER_CARD>			m_vecMonsterCardList;
	std::vector<GCITEMID>					m_vecMaterialList;

	D3DXVECTOR2			m_vtFixedWindowPos[EM_MATERIAL_SLOT_SIZE];

public:
	KD3DWnd*    m_pkBtnOK;
	KD3DWnd*    m_pkBtnCancel;

    KD3DWnd*    m_pkRectInven;
    KD3DWnd*    m_pkRectMaterial;


	KD3DWnd* m_pkBtnPrevPage;
	KD3DWnd* m_pkBtnNextPage;

	KD3DStatic*	m_pkMainDescStatic;
	KD3DStatic*	m_pkInvenCardDescStatic;
	KD3DStatic*	m_pkPageNum;
	
	KSafeArray<KGCCheckBox*,EM_GRADE_SIZE>          m_pkGradeCheckBtn;
	KSafeArray<KD3DStatic*,EM_GRADE_SIZE>           m_pkGradeStatic;

	KSafeArray<KGCMonsterCardContents*,EM_INVEN_SLOT_SIZE>		m_pkInvenSlot;
	KSafeArray<KGCMonsterCardContents*,EM_MATERIAL_SLOT_SIZE>    m_pkMaterialSlot;


	KGCMonsterCardContents*			m_pkCardImageOn;
    KGCMonsterCardContents*         m_pkMonsterCardContentsBigDummy;
    KGCMonsterCardContents*         m_pkMonsterCardContentsDummy;

	KD3DSizingBox*			m_pkCardImageOnBack;
	
	KGCMonsterCardRoulette*	m_pkMonsterCardRoulette;

    bool    m_bSendScriptReq;
    int     iCurrentSelectedMonsterCard;

};

DEFINE_WND_FACTORY( KGCMonsterCardCompose );
DECLARE_WND_FACTORY( KGCMonsterCardCompose );
DECLARE_WND_FACTORY_NAME( KGCMonsterCardCompose );
