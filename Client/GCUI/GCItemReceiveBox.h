#ifndef _GCITEMRECEIVEBOX_H_
#define _GCITEMRECEIVEBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include <vector>
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/ActionListener.h"
//#include "../GCItem.h"

class KD3DStatic;
class GCDeviceTexture;
class KGCItemInfoBox;
class KGCItemImgWnd;
class KGCUserItemWnd;
class KGCDungeonDetailWnd;

#define NUM_OF_ITEMBOX_LIST         8


class FindItem 
{ 
public:

	void SetItemIndex(int nId)
	{
		m_iAddItemID = nId;	// FindID 함수에 쓰기 위해서
	};
    void SetItemUID(GCITEMUID nId)
    {
        m_i64AddItemUID = nId;	// FindID 함수에 쓰기 위해서
    };
    void SetItemID(DWORD nId)
    {
        m_dwAddItemUID = nId;	// FindID 함수에 쓰기 위해서
    };

    bool operator()(const std::pair< int, int >& data) { return data.first == m_iAddItemID; } 
    bool operator()(const std::pair< GCITEMUID, int >& data) { return data.first == m_i64AddItemUID; } 
    bool operator()(const std::pair< GCITEMID, int >& data) { return data.first == m_dwAddItemUID; } 

private:
	int         m_iAddItemID;
    GCITEMUID   m_i64AddItemUID;
    GCITEMID    m_dwAddItemUID;
}; 


class KGCItemReceiveBox : public KD3DDialog,           // extends
	public KActionListener    // implements
{
public:
	/// Default constructor
	KGCItemReceiveBox( void );
	/// Default destructor
	virtual ~KGCItemReceiveBox( void ){}
	/// Copy constructor
	KGCItemReceiveBox( const KGCItemReceiveBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCItemReceiveBox& operator=( const KGCItemReceiveBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void AddItem( GCITEMUID ItemUID_, int iDuration = 0 ) = 0;
	virtual void ClearItemList(void) = 0;	
	std::vector< std::pair< GCITEMUID, int > > GetItemVector(){ return m_vecItem; };
    virtual bool IsEmptyReceiveItem(void) { return true; };
    void SetEnableSell( bool bEnableSell_ ) { m_bEnableSell = bEnableSell_; }

protected:	
	virtual void RefreshPage(void) = 0;
	std::vector< std::pair< GCITEMUID, int > >  m_vecItem;       // <ItemID, NumOfItem>	
    bool                                        m_bEnableSell;
};



class KGCItemReceiveBoxNew : public KGCItemReceiveBox
{
public:
	DECLARE_CLASSNAME( KGCItemReceiveBoxNew );
	/// Default constructor
	KGCItemReceiveBoxNew( void );
	/// Default destructor
	virtual ~KGCItemReceiveBoxNew( void ){}
	/// Copy constructor
	KGCItemReceiveBoxNew( const KGCItemReceiveBoxNew& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCItemReceiveBoxNew& operator=( const KGCItemReceiveBoxNew& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:	    
	void AddItem( GCITEMUID ItemUID_, int iDuration = 0 );
	void ClearItemList(void);	
	virtual void ActionPerformed( const KActionEvent& event );
    virtual bool IsEmptyReceiveItem(void);
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );	
		
	virtual void RefreshPage(void);

	void SelectEtcItem( bool bSelect );

protected:	

	KD3DWnd*									m_pkOk;
	KD3DWnd*									m_pkSellEtcItem;
	KD3DScrollbar*								m_pkScroll;
	KD3DWnd*                                    m_pkDungeonDetailBtn;
    KD3DWnd*                                    m_pkDungeonDetailCloseBtn;
    std::vector< KGCUserItemWnd* >				m_vecItemWnd;
    KGCDungeonDetailWnd*                        m_pkDungeonDetailWnd;
    
};

DEFINE_WND_FACTORY( KGCItemReceiveBoxNew );
DECLARE_WND_FACTORY( KGCItemReceiveBoxNew );
DECLARE_WND_FACTORY_NAME( KGCItemReceiveBoxNew );








//////////////////////////////////////////////////////////////////////////
///////////////////KGCChoiceDrop////////////////////////////////
//////////////////////////////////////////////////////////////////////////


class KGCChoiceDrop : public KD3DDialog,           // extends
    public KActionListener    // implements
{
protected:	
    std::vector< KDropItemInfo >  m_vecItem;       // <ItemID, NumOfItem>	

public:
    DECLARE_CLASSNAME( KGCChoiceDrop );
    /// Default constructor
    KGCChoiceDrop( void );
    /// Default destructor
    virtual ~KGCChoiceDrop( void ){}
    /// Copy constructor
    KGCChoiceDrop( const KGCChoiceDrop& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChoiceDrop& operator=( const KGCChoiceDrop& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:	
    void AddItem( KDropItemInfo kDropItemInfo );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition( const KDialogInfo& kInfo_ );
    void ReceiveItemBoxList(std::set<GCITEMID>& _itemBoxList);
    bool IsChoiceDropItem( int _itemID );
    GCITEMID& GetBoxID();
    void SetBoxID( GCITEMID& _itemID );
    void SetLimitSelCount(USHORT _limitCount);
    std::vector<KDropItemInfo> GetItemVector(){ return m_vecItem; };

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );	
    virtual void RefreshPage(void);

    void ClearItemList(void);	
    void OnGet();
    void OnClose();
    void OnGetAll();
    void ResizeWindow( int _row, bool _scroll );
    void OnMouseEnterConfirm(void);
    void OnMouseLeaveConfirm(void);
    void InitVariable( void );
protected:	

    KD3DWnd*									m_pkGet;
    KD3DWnd*									m_pkGetLock;
    KD3DWnd*									m_pkGetAllLock;
   
    KD3DWnd*									m_pkGetAll;
    KD3DWnd*									m_pkClose;
    KD3DWnd*									m_pkCancel;
    KD3DWnd*									m_pkimgItemBackground;

    KD3DWnd*									m_pkImgTooltipConfirmBG; 
    KD3DWnd*									m_pkImgTooltipConfirmBG1; 


    KD3DStatic*									m_pkStaticTolltipConfirm;   
    

    KD3DWnd*									m_pkOptionRowCols;

    KD3DScrollbar*								m_pkScroll;

    KD3DStatic* m_pkStaticGuide;

    std::vector< KGCUserItemWnd* >				m_vecItemWnd; 


    KD3DSizingBox*                      m_pkSboxMainBackground;   
    KD3DSizingBox*                      m_pkSboxInnerBak;
    KD3DSizingBox*                      m_pkSboxTitleBackground  ;


    int m_iCols;
    int m_iRows;

    D3DXVECTOR2 m_vtGetPos;
    D3DXVECTOR2 m_vtCancelPos;
    D3DXVECTOR2 m_vtClosePos;
    D3DXVECTOR2 m_vtImgTooltipConfirmBGPos;
    D3DXVECTOR2 m_vtStaticTolltipConfirmPos;

    DWORD m_dwScrollHeight;
    POINT m_ptSboxMainBackgroundWH;
    POINT m_ptSboxInnerBakWH;
    POINT m_ptimgItemBackgroundWH;



    int m_nSelectItemCount;
    int m_nLimitItemCount;


    GCITEMID m_ItemID;
    std::set<GCITEMID> m_setItemBoxList;


    //셀렉트된 상태를 저장하는 변수
    std::vector<bool>  m_vecItemSelected;       // [index] = selected
    int m_nLastSelection;


};

DEFINE_WND_FACTORY( KGCChoiceDrop );
DECLARE_WND_FACTORY( KGCChoiceDrop );
DECLARE_WND_FACTORY_NAME( KGCChoiceDrop );


#endif // _GCITEMRECEIVEBOX_H_
