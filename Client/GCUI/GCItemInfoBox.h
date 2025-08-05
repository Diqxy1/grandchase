#ifndef _GCITEMINFOBOX_H_
#define _GCITEMINFOBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../GCItem.h"

class KD3DStatic;
struct KInventoryItem;
class KGCItemInfoBox : public KD3DFrameWnd      // extends

{
public:
	DECLARE_CLASSNAME( KGCItemInfoBox );
	/// Default constructor
	KGCItemInfoBox( void );
	/// Default destructor
	virtual ~KGCItemInfoBox( void );
	/// Copy constructor
	KGCItemInfoBox( const KGCItemInfoBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCItemInfoBox& operator=( const KGCItemInfoBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	void SetItemForPet( GCItem* pkItem, char cPromotion );
	void SetItem( GCItem* pkItem, KItem* pkInventoryItem );
    void SetItem( GCItem* pkItem );
    int GetItemID(void) { return m_iItemID; }

	char GetPetEvolutionLevel( void ) { return m_cPromotion; }
	void SetTextColor( D3DCOLOR color );

protected:
	virtual void OnCreate( void );
	virtual void PostChildDraw();
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
	KD3DStatic*	m_pkItemName;
	KD3DStatic*	m_pkItemAbllity;
	KD3DStatic* m_pkItemDecs;

	int			m_iItemID;
	GCITEMUID		m_dwItemUID;
	int			m_iSetItemNumber;
    char        m_cPromotion;

    GCItem*     m_pkItem;

public:
    bool        m_bUpdate;
};

DEFINE_WND_FACTORY( KGCItemInfoBox );
DECLARE_WND_FACTORY( KGCItemInfoBox );
DECLARE_WND_FACTORY_NAME( KGCItemInfoBox );

#endif // _GCITEMINFOBOX_H_
