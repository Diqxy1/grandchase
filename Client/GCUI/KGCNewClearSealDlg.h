#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KD3DComboBox;
class KGCGachaBuyItemBox;
class KGCMultiMenu;

class KGCNewClearSealDlg :	public KD3DDialog,
									public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCNewClearSealDlg );
	KGCNewClearSealDlg(void);
	virtual ~KGCNewClearSealDlg(void);

	KGCNewClearSealDlg( const KGCNewClearSealDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCNewClearSealDlg& operator=( const KGCNewClearSealDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostDraw( void );
	virtual void PostChildDraw( void );
    virtual void OnDestroy( void );

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

private:
	enum
	{
		ETYPE_EQUIP_ITEM			= 0,
		ETYPE_ACCESSORY_ITEM	= 1,
		EREWORD_ITEM_NUM     = 6,
		EREWORD_ACC_NUM		= 5,
		EBUY_ITEM_NUM           = 3,
		ENEED_SPELLITEM_NUM	= 2,
	};

private:
	KD3DComboBox*                          m_pkJobCombo;
	KD3DWnd*                                  m_pkBtnClose;
	KD3DWnd*                                  m_pkBtnTakeEquip;
	KD3DWnd*                                  m_pkBtnTakeAcc;
	KD3DWnd*                                  m_pkBtnClearSealEquip;
	KD3DWnd*                                  m_pkBtnClearSealAcc;
	KD3DWnd*                                  m_pkBackground;
	KD3DWnd*                                  m_pkWndTakeEquipOver;
	KD3DWnd*                                  m_pkWndTakeAccOver;
	KD3DWnd*									m_pkBtnBuy;
	KD3DWnd*									m_pkBtnSelectItemLevel;
	KD3DWnd*                                  m_pkBtnPetView;
	KD3DStatic*									m_pkStaticMsg;

	KSafeArray<KGCItemImgWnd*, EREWORD_ITEM_NUM>	m_pkImgRewordEquip;
	KSafeArray<KD3DWnd*, EREWORD_ITEM_NUM>			m_pkWndRewordEquipOver;
	KSafeArray<KGCItemImgWnd*, EREWORD_ACC_NUM>	m_pkImgRewordAcc;
	KSafeArray<KD3DWnd*, EREWORD_ACC_NUM>			m_pkWndRewordAccOver;
	KGCItemImgWnd*                          m_pkImgBeadItem;
	KGCItemImgWnd*                          m_pkImgSealItem;
	KGCItemImgWnd*                          m_pkImgTakeEquip;
	KGCItemImgWnd*                          m_pkImgTakeAcc;

	KD3DStatic*                                 m_pkStaticBeadItem;
	KD3DStatic*                                 m_pkStaticSealItem;
	KD3DStatic*                                 m_pkStaticClearSealDesc;

	//Equipment Level Stuff
	KD3DStatic*									m_pkStaticSelectedItemLevel;
	KGCMultiMenu*								m_pkPopupMenuItemLevel;
	std::map<int, std::pair<int,int>>			m_mapMenuItemLevelInfo;
	int												m_iCurItemLevel;
	int												m_iNeedSpellItemNum;

private:
	int                                         m_iCurCharType;
	int                                         m_iCurCharJob;
	int                                         m_iCurBuyItem;
	int                                         m_iCurBeadItemNum;
	int                                         m_iCurSpellItemNum;
	int                                         m_iParticleLife;
	int		                                    m_iFrameCount_ItemImage;
	int                                         m_iGachaKeyBuyCount;
	DWORD                                  m_dwGachaItemType;

	bool                                      m_bGetAll;
	bool                                      m_bGetEquipAll;
	bool                                      m_bGetAccessoriesAll;
	bool                                      m_bSuccess;
	bool                                      m_bPaticleInit;
	bool									  m_bRewardEquipItem;
	bool									  IsProccessedGacha;

	std::wstring							  m_wstrMsg;

	CParticleEventSeqPTR					m_pEffTextAlert;
	KGC2DObject*							m_pkGetItemImage;

	std::map<int, DWORD>							m_mapSealBead;
	std::map<DWORD, std::vector<DWORD> >	m_mapBuyItemList;
	std::vector< DWORD >							m_vecCurBuyItemList;
	std::map< DWORD, std::map<int, std::set<GCITEMID>> >			m_mapStoredRewardInfo;
	std::map< DWORD, std::map<int, std::vector<GCITEMID>> >		m_mapStoredFinalRewardInfo; //최종 보상아이템 리스트
	std::vector< DWORD >						m_vecRewardEuipItems;
	std::vector< DWORD >						m_vecRewardAccItems;
	std::vector< DWORD >						m_vecFinalRewardEuipItems;
	std::map< std::pair<GCITEMID, int>, std::set<DWORD> > m_mapObtainedBeadList;
	std::set<std::pair<DWORD, DWORD> >		m_setObtainedBeadList;
	std::vector< DWORD >						m_vecGachaKeyItemList;

private:// Preview 관련
	enum
	{
		VIEWPORT_OFFSET_X   = 107,
		VIEWPORT_OFFSET_Y   = 245,
		VIEWPORT_WIDTH      = 157,
		VIEWPORT_HEIGHT     = 228,
	};
	PLAYER              m_Player;
	GCObject*           m_pMeshFootStage;
	D3DVIEWPORT9        m_viewport;
	D3DXMATRIX          m_matProject;
	D3DXMATRIX			m_matScreenProj;
	GCDeviceMotionFRM*  m_pShopMotion;
	GCDeviceTexture*    m_pkBackTexture;

	KSafeArray<GCDeviceMotionFRM*,GC_CHAR_NUM * NUM_JOB_LEVEL> m_pWaitMotion;
	KSafeArray<GCDeviceMotionFRM*,GC_CHAR_NUM * NUM_JOB_LEVEL> m_pActionMotion;

	int					m_iMotionCount;
	int                 m_cFrame;
	float				m_fCharX;

	POINT				m_ptStartMouse;
	float               m_fStartAngle;
	float               m_fStartStageAngle;
	float               m_fAngleMove;
	float               m_fStageAngleMove;
	float				m_fZoom;
	bool				m_bAngleChanged;

	int					m_iExtraCharNum;
	int					m_iCurrentCharacter;
	bool              m_bShowDefault;

	bool							m_bAutoMode;
	KD3DCheckBox*				m_pkCheckBoxAutoClear; // 자동해체 체크 
	KGCGachaBuyItemBox*		m_pkGachaBuyItemBox;
private:
	int m_iGachaVer;

public:
	void Init_ItemImage();
	void InitRewardBtn();
	void InitUseSealBtn();
	void InitPreview();
	void InitializeData();
	void InitPromotion( int iCharType );
	void UpdateCharacter(int iIndex);
	void UpdatePromotion(int iIndex);
	void UpdateOwnItemList();
	void UpdateBuyItemList();
	void UpdateRewardList(GCITEMID ItemID_);
	void UpdateFinalRewardList(GCITEMID ItemID_);

	void SetMapGachaRewardList( GCITEMID ItemID_, std::map<int, std::set<GCITEMID>>& mapRewardItemList_);
	void SetMapGachaFinalRewardList( GCITEMID ItemID_, std::map<int, std::vector<GCITEMID>>& mapRewardItemList_ );
	void SetRewardItemList( std::map< int, std::set<GCITEMID> >& mapRewardItemList_ );
	void SetFinalRewardtItemList( std::map<int, std::vector<GCITEMID>>& mapRewardItemList_ );
	void SetSuccessGachaAction( bool bSuccess_ ) { m_bSuccess = bSuccess_; }
	void SetParticleLife( int iLife_ ) { m_iParticleLife = iLife_; }
	void SetStrMessage( std::wstring wstr_ ) { m_wstrMsg = wstr_; } 
	void SetObtainedBeadList( std::set<std::pair<DWORD,DWORD>>& setObtainedBeadList_ );
	void AddObtainedBeadList( std::pair<DWORD,DWORD>& prItemInfo_ );
	void SetGachaKeyBuyCount(const int iBuyCount) {m_iGachaKeyBuyCount = iBuyCount; }
	int  GetGachaKeyBuyCount(){return m_iGachaKeyBuyCount;}
	void UpdateMeshObject( std::vector<GCItem*> *pvecItem = NULL );
	void ResetPlayer( void );
	void SetCoordiView( int iCharType_ );
	bool IsOpenUI();

public:
	bool LoadScriptBuyItemList();

private:
	void LoadGachaKeyList();
	void SelCharToCharType(int iCharType);
	void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID dwItemID_, int iLevel );
	void ClearPaticle();
	//void GachaStartTextAlert();
	//void GachaStartBtnAlertAndStopTextAlert();
	void FrameMove_ItemImage();
	void ShowGachaEffect();
	void DeleteMeshObject( void );
	void RenderPreview( void );
	void FrameMove_Preview( void );
	void ChangeCharWndZoom( bool bZoom );
	void ProcessCharWndAngle( void );
	void SetChar( int iCharType );
	void InitCharacterInfo();

public:
	bool IsGachaKeyItem(DWORD dwItemID);
	void SetBeadSpellItemNum( int iBeadNum_, int iSpellNum_ );
	void RollGacha();

public:
	void OnExit();
	void OnClickBuy();
	void OnClickPetView();
	void OnClickClearSealButton( DWORD dwType );
	void OnEnterCursor();
	void StartAuto();
	void OnStopAuto( void );
	void OnItemEquipPreview();
	void OnRecvGachaAck();
	void OnRecvFinalReward();
	void OnRecvFinalTotalReward();
	void OnClickAuto( void );
	int  GetCurrentGachaVer( void ) { return m_iGachaVer; }

	//Item Level Gacha Stuff
	void SetMapGachaItemLevelList( GCITEMID ItemID_, std::map<int, int>& mapItemLevelList_ );
	void SetItemLevelList( std::map< int,int >& mapItemLevelList_);
	void InitSetLevel();
	void SetObtainedBeadListForGachaLevel( std::map< std::pair< GCITEMID, int >, std::set< DWORD > >& mapObtainedBeadList_ );
	void AddObtainedBeadListForGachaLevel( std::pair< GCITEMID, int >& prItemInfo_, DWORD dwType_ );
	void UpdateItemLevel( int iCurSelection );
	bool IsObtainedItem( DWORD dwType_ );
};

DEFINE_WND_FACTORY( KGCNewClearSealDlg );
DECLARE_WND_FACTORY( KGCNewClearSealDlg );
DECLARE_WND_FACTORY_NAME( KGCNewClearSealDlg );
