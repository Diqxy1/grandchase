#pragma once

#define MAX_ITEM_LINE 6

class KGCItemImgWnd;
class KGCCharacterUI;
class KGCDescBox;
class KLuaManager;

class KGCGPCapsuleItemList :	public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGPCapsuleItemList );
	/// Default constructor
	KGCGPCapsuleItemList( void );
	/// Default destructor
	virtual ~KGCGPCapsuleItemList( void );
	/// Copy constructor
	KGCGPCapsuleItemList( const KGCGPCapsuleItemList& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGPCapsuleItemList& operator=( const KGCGPCapsuleItemList& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum ITEM_GRADE
	{
		IG_BASE,            // 기본 아이템
		IG_RARE,            // 레어 아이템
		IG_EVENT,           // 이벤트 아이템
	};
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

protected:
	int m_iCharType;
	int m_iItemKind;
	int m_iCapsuleIndex;
	int m_iPrice;
	int m_iCurPage;	
	int m_iRandomItem;
	int m_iObtainItemID;

	std::vector<std::pair<KDropItemInfo,int>> m_vecItemList;	

	KD3DWnd*	m_pkOpenCapsuleBtn;
	KD3DWnd*	m_pkLine;
	KD3DWnd*	m_pkTitle;
	KD3DWnd*	m_pkTitleLevel;
	KD3DWnd*	m_pkTitleItemName;
	KD3DWnd*	m_pkPrevPage;
	KD3DWnd*	m_pkNextPage;
	KSafeArray<KD3DWnd*,MAX_ITEM_LINE> m_pkObtainedItem;

	KSafeArray<KD3DStatic*,MAX_ITEM_LINE> m_pkItemFactorText;
	KSafeArray<KD3DStatic*,MAX_ITEM_LINE> m_pkLevelText;
	KSafeArray<KD3DStatic*,MAX_ITEM_LINE> m_pkItemNameText;
	KD3DStatic* m_pkCapsuleDesc0;
	KD3DStatic* m_pkSlash;
	KD3DStatic* m_pkCurPage;
	KD3DStatic* m_pkTotalPage;

	KGCItemInfoBox* m_pkItemInfoBox;

	GCDeviceTexture* m_pTextureItem;

public:
	int m_iParticleTime;

public:
	void UpdatePage();
	void OnOpenCapsule();
	void MovePage( int iPage_ );
	void DrawItemList( int iPage_, int iCurPageItemListNum_ );
	void SetCapsuleItemList( std::vector<std::pair<KDropItemInfo,int>>& vecItemList_, int iCharType_, int iItemKind_, int iCapsuleIndex_, int iPrice_);
	int GetMaxPage();
	void DrawItemInfoBox( int iLineIndex_ );
	void SetObtainItemID( int iItemID_ ){ m_iObtainItemID = iItemID_; }
};


DEFINE_WND_FACTORY( KGCGPCapsuleItemList );
DECLARE_WND_FACTORY( KGCGPCapsuleItemList );
DECLARE_WND_FACTORY_NAME( KGCGPCapsuleItemList );
