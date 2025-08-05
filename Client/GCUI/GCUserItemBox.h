#ifndef _GCUSERITEMBOX_H_
#define _GCUSERITEMBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"
#include "GCUserItemWndInterface.h"

class KGCUserItemWnd;
class KD3DScrollbar;
struct KInventoryItem;
class KGCShopTab;

class KGCUserItemBox : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCUserItemBox );
	/// Default constructor
	KGCUserItemBox( void );
	/// Default destructor
	virtual ~KGCUserItemBox( void );
	/// Copy constructor
	KGCUserItemBox( const KGCUserItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCUserItemBox& operator=( const KGCUserItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum
	{
		NUM_ITEM_VIEW_WND = 2,
    };

    struct SSortbyUID : public std::binary_function<const KItem&, const KItem&, bool> {
        SSortbyUID() {}
        bool operator() ( const KItem& lparam, const KItem& rparam ) {
            return lparam.m_ItemUID > rparam.m_ItemUID;
        }
    };

public:	
	virtual void ActionPerformed( const KActionEvent& event );
	void	RefreshScrollMax();
	void	OnScrollPos(int iPos);
	void	RefreshItemList( const std::vector<KItem>& vecItemList_ , bool bShowBtn = true );
	void	RefreshItemList( const std::vector<GCITEMUID>& vecItemList_ , bool bShowBtn = true );
	void	RefreshItemWnd();
    void	RefreshItemWndCustom( int iScrollPos_ );
	bool 	GetCurrentHoveredItem( GCItem*& pItem, KItem*& pkInventoryItem, D3DXVECTOR2 &vecPos );
	void	UpdateData();
    void    SetWndType( int iType_ );
    void    AddItemList( GCITEMUID itemUID_ );
    void    AddItemList( const KItem& kItem_ );
    void    RemoveItemList( GCITEMUID itemUID_ );
    void    ClearItemList();
    void    SetScrollPixel( float fScroll_ ) { m_fScollGap = fScroll_; };
    const std::vector<KItem>& GetItemList() const { return m_vecItemList; }
    std::vector<GCITEMUID> GetItemUIDList() {
        std::vector<GCITEMUID> vecItemUIDList;
        for( DWORD i = 0 ; i < m_vecItemList.size() ; ++i ) {
            vecItemUIDList.push_back( m_vecItemList[i].m_ItemUID );
        }
        return vecItemUIDList;
    };
    int    GetListSize() { return static_cast<int>(m_vecItemList.size()); }

    int GetRows() const { return m_iRows; }
    int GetCols() const { return m_iCols; }
    float GetXGap() const { return m_fXGap; }
    float GetYGap() const { return m_fYGap; }
    float GetXOffset() const { return m_fXOffset; }
    float GetYOffset() const { return m_fYOffset; }

    void SetRows(int val);
    void SetCols(int val);
    void SetXGap(float val);
    void SetYGap(float val);
    void SetXOffset(float val);
    void SetYOffset(float val);
    bool GetAnimScroll() const { return m_bAnimScroll; }
    void SetAnimScroll(bool val) { m_bAnimScroll = val; }
    bool GetSorting() const { return m_bSorting; }
    void SetSorting(bool val) { m_bSorting = val; }

    int GetTotalPage() const { return m_iTotalPage; }
    int GetCurrentPage() const { return m_iCurrentPage; }
    void SetCurrentPage(int val) { m_iCurrentPage = val; }

    void ShowItemEffect( bool _bShow );

	UINT GetEnabledStateWndNum();

    bool MakeItemOnFirstSlot( IN GCITEMID dwItemID );

    void CreateChildWndList();
    void CalcuateWndPosition();

protected:
	virtual void OnCreate( void );
	virtual void OnDestroy( void );
	virtual void OnInitialize( void );
	virtual void FrameMoveInEnabledState( void );

    void RefreshWndPosition();

    bool OnClickItem( const KActionEvent& kEvent_ );
    bool OnRClickItem( const KActionEvent& kEvent_ );

    bool OnEnterCursor( const KActionEvent& kEvent_ );
    bool OnLeaveCursor( const KActionEvent& kEvent_ );
    void SetTotalPage(int val) { m_iTotalPage = val; }

    std::string GetChildFile();
    std::string GetChildPath();
    void SetChildFile(std::string val);
    void SetChildPath(std::string val);

    KGCUserItemWndInterface* CreateOneChildWnd();



protected:
	KSafeArray<KGCUserItemWndInterface*,NUM_ITEM_VIEW_WND> m_apItemWnd;
    std::vector<KGCUserItemWndInterface*>    m_vecItemWnd;
    KSafeArray<D3DXVECTOR2,NUM_ITEM_VIEW_WND> m_aOriginPos;
    std::vector<D3DXVECTOR2>        m_vecOriginPos;
    typedef boost::unordered_map<KD3DWnd::uid_type,GCITEMUID> wnditeminfo_type;
    boost::unordered_map<KD3DWnd::uid_type,GCITEMUID>   m_mapWndItemInfo;
    std::vector<KItem>      m_vecItemList;

    KD3DScrollbar*		m_pkScrollbar;
    KD3DWnd*            m_pkClipRange;
	int					m_iScrollPos;
    int                 m_iScrollTarget;
    float               m_fScrolled;
    bool                m_bAnimScroll;
    bool                m_bSorting;

    GCITEMUID           m_HoveredItemUID;
    KItem               m_kHoveredInvenItem;
    GCItem*             m_kHoveredItemInfo;
    D3DXVECTOR2         m_vHoveredItemPos;

    std::string m_strChildFile;
    std::string m_strChildPath;

    int m_iTotalPage;
    int m_iCurrentPage;
    int m_iRows;
    int m_iCols;
    float m_fXGap;
    float m_fYGap;
    float m_fXOffset;
    float m_fYOffset;
    int   m_iScrollFrame;
    float m_fScollGap;
    float m_fLineHeight;

	bool m_bShowBtnState;

    std::vector< CParticleEventSeqPTR >   m_vecHotEquipParticle;    

#if defined( USE_ONGAME_INVENTORY ) 
private:
    std::vector< std::pair<bool, bool > > m_vecItemState;

private:
    void SetItemState( void );
    void SortItemListByItemState( void );        
    void MakeItemListAndStateList( IN const std::vector< KItem >& vecItem, bool bHot_, bool bNew_ );
#endif

};

DEFINE_WND_FACTORY( KGCUserItemBox );
DECLARE_WND_FACTORY( KGCUserItemBox );
DECLARE_WND_FACTORY_NAME( KGCUserItemBox );

#endif // _GCUSERITEMBOX_H_
