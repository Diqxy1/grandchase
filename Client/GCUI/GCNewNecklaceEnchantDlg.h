#pragma once

class KGCNewNecklaceEnchantDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCNewNecklaceEnchantDlg );
    /// Default constructor
    KGCNewNecklaceEnchantDlg( void );
    /// Default destructor
    virtual ~KGCNewNecklaceEnchantDlg( void );
    /// Copy constructor
    KGCNewNecklaceEnchantDlg( const KGCNewNecklaceEnchantDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNewNecklaceEnchantDlg& operator=( const KGCNewNecklaceEnchantDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum
	{
		EM_MATERIAL_GEM = 0,
		EM_MATERIAL_CHOCOLATE	,
		EM_MATERIAL_HIGH_GEM,
		EM_MATERIAL_SIZE,
	};

	enum 
	{
		EM_SUCCESS_TYPE = 0,
		EM_FAIL_NONE_TYPE,
		EM_FAIL_DOWN_TYPE,
		EM_FAIL_BREAK_TYPE,
	};

	enum 
	{
		EM_NORMAL_STATE = 0,
		EM_ENCHANT_STATE,
		EM_RESULT_STATE,
	};

	enum 
	{
		EM_NORMAL_ELENA = 0,
		EM_ENCHANT_ELENA = 1,
		EM_SUCCESS_ELENA = 2,
		EM_FAIL_ELENA = 3,
		EM_SIZE_ELENA = 4,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMUID _itemUID );    
	void Init();
	
	void UpdateEnchantDlg();
	void UpdateItemID();
	void UpdateCheckBtn();	
	void UpdateTextControl();	
	void UpdateItemImgae();

	void UpdateMaterialText();
	void UpdateFailText();

	void OnOK();
	void OnClose();
	void OnClickCheckBtn( int nCheck );

	
	int GetEnchantCost();
	void SetItemInfo( GCITEMUID _itemUID );
	void SetSecondaryRatio( float _fRate ) { m_fRatio = _fRate; }
	void LockButton( bool _bLock );
	void SetResult( int nResult );
	void RenderResultParticle( int nResultType );

	void ToggleElenaImage( int nElenaType );

	void SetEnchantGemCount( const std::map<DWORD, DWORD>& _mapEnchantGemCount ) { m_mapEnchantGemCount = _mapEnchantGemCount; }

	void OnClickAuto(void);
		
public:
	GCITEMID	m_ItemID;
	GCITEMUID	m_ItemUID;
	int			m_nMaterialType;
	int			m_nStrongNum;

	DWORD		m_dwState;
	int			m_iParticleTime;
	int			m_nResult;

	float		m_fRatio;

	std::map<DWORD, DWORD> m_mapEnchantGemCount;


public:
	KSafeArray<KGCCheckBox*,EM_MATERIAL_SIZE>	m_pkBtnCheck;
	KSafeArray<KD3DStatic*,EM_MATERIAL_SIZE>	m_pkStaticCheck;
	KSafeArray<KD3DWnd*,EM_SIZE_ELENA>			m_pkElenaImage;


	KD3DWnd*	    m_pkBtnOK;
	KD3DWnd*		m_pkBtnCancel;

	KGCItemImgWnd*	m_pkItemImage;

	KD3DStatic*		m_pkStaticDesc;
	KD3DStatic*		m_pkStaticDescTitle;
	KD3DStatic*		m_pkStaticItemName;
	KD3DStatic*		m_pkStaticCostMaterialTitle;
	KD3DStatic*		m_pkStaticHaveMaterialTitle;
	KD3DStatic*		m_pkStaticCostMaterial;
	KD3DStatic*		m_pkStaticHaveMaterial;

	KGCUINumber*	m_pkStrongNum;

    KD3DWnd*        m_pkTitleNecklace;
    KD3DWnd*        m_pkTitleAnkle;
	KD3DWnd*        m_pkAutoText;

	bool            m_bAutoMode = false;
	KD3DCheckBox* m_pkCheckBoxAutoClear; // 자동해체 체크 
};

DEFINE_WND_FACTORY( KGCNewNecklaceEnchantDlg );
DECLARE_WND_FACTORY( KGCNewNecklaceEnchantDlg );
DECLARE_WND_FACTORY_NAME( KGCNewNecklaceEnchantDlg );

extern int g_iEnchantGemCountResult;
