#pragma once

class KGCItemImgWnd;
class KGCUserItemWndInterface : public KD3DWnd    // extends
{
public:
    DECLARE_CLASSNAME( KGCUserItemWndInterface );
    /// Default constructor
    KGCUserItemWndInterface( void );
    /// Default destructor
    virtual ~KGCUserItemWndInterface( void );
    /// Copy constructor
    KGCUserItemWndInterface( const KGCUserItemWndInterface& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCUserItemWndInterface& operator=( const KGCUserItemWndInterface& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual GCITEMID GetItemID() { return 0; };
    void SetItem();
    void SetItem( KItem* pInventoryItem );
    void SetItem( GCItem* pItem );
    GCItem* GetItemData() { return m_ItemInfo; };
    KItem& GetInvenItem() { return m_kInvenItem; };

    virtual bool IsCursorRollover() { return false; };
	virtual void OffItemBtn() {}
protected:
    virtual void OnSetItem() {};
    virtual void OnSetItem( KItem* pInventoryItem ) {};
    void SetItemImg( KItem* pInventoryItem );
    void SetItemImg( GCItem* pItem );
    virtual void OnCreate( void );

protected:
    KGCItemImgWnd*      m_pkItemImg;
    GCItem*             m_ItemInfo;
    KItem               m_kInvenItem;

    GCItem              m_kItem;

protected:
    bool        m_bHotItem;
    bool        m_bNewItem;

public:
    bool IsNewItem( void ) { return m_bNewItem; }
    bool IsHotItem( void ) { return m_bHotItem; }

#if defined( USE_ONGAME_INVENTORY )
protected:
    KD3DWnd*    m_pkIconHotItem;
    KD3DWnd*    m_pkIconNewItem;

public:
    void OnSetItemState( bool bHot_ = false, bool bNew_ = false );

#endif
};

DEFINE_WND_FACTORY( KGCUserItemWndInterface );
DECLARE_WND_FACTORY( KGCUserItemWndInterface );
DECLARE_WND_FACTORY_NAME( KGCUserItemWndInterface );
