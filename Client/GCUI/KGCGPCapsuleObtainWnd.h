#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../GCItem.h"

class KD3DStatic;
class KGCItemImgWnd;
class KGCCharacterUI;
class KGCDescBox;
class KLuaManager;

struct KInventoryItem;

class KGCGPCapsuleObtainWnd : public KD3DDialog,
	public KActionListener// extends
{
public:
	DECLARE_CLASSNAME( KGCGPCapsuleObtainWnd );
	/// Default constructor
	KGCGPCapsuleObtainWnd( void );
	/// Default destructor
	virtual ~KGCGPCapsuleObtainWnd( void );
	/// Copy constructor
	KGCGPCapsuleObtainWnd( const KGCItemInfoBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGPCapsuleObtainWnd& operator=( const KGCItemInfoBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	void SetItemForPet( GCItem* pkItem, char cPromotion );
	void SetItem( GCItem* pkItem, KItem* pkInventoryItem );
	void SetItem( GCItem* pkItem );
	int GetItemID(void) { return m_iItemID; }

	char GetPetEvolutionLevel( void ) { return m_cPromotion; }
    void SetTextColor( D3DCOLOR color );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

protected:
	virtual void OnCreate( void );
	//virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	//virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );
	

	D3DXVECTOR4 GetTextureCoords( int iSetNum );

	struct KItemText
	{
		std::wostringstream strmName;
		std::wostringstream strmAbillity;
		std::wostringstream strmDesc;
	};

	void GetItemText( GCItem*& pkItem, KItemText& ItemText, KItem* pkInventory = NULL );
	std::wstring MakeDiffString( int iDiff );
	std::wstring MakeLvString( int iLv );
	std::wstring MakeSocketItemString( int iAbility );
	void GetEnchantAbility( OUT GCItemAbility& kAbility, DWORD dwID, KItem* pkInventory );

	GCDeviceTexture* m_pTextureItem;

protected:
	KD3DWnd*	m_pkOKBtn;

	KD3DStatic*	m_pkItemName;
	KD3DStatic*	m_pkItemAbllity;
	KD3DStatic* m_pkItemDecs;

	int			m_iItemID;
	GCITEMUID		m_dwItemUID;
	int			m_iSetItemNumber;
	char        m_cPromotion;

public:
	bool        m_bUpdate;
};

DEFINE_WND_FACTORY( KGCGPCapsuleObtainWnd );
DECLARE_WND_FACTORY( KGCGPCapsuleObtainWnd );
DECLARE_WND_FACTORY_NAME( KGCGPCapsuleObtainWnd );
