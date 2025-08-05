#ifndef _KGC_MATERIAL_ITEM_INFO_H_
#define _KGC_MATERIAL_ITEM_INFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMaterialItemInfo : 
	public KD3DWnd,
	public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCMaterialItemInfo );
    /// Default constructor
    KGCMaterialItemInfo( void );
    /// Default destructor
    virtual ~KGCMaterialItemInfo( void );
    /// Copy constructor
    KGCMaterialItemInfo( const KGCMaterialItemInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMaterialItemInfo& operator=( const KGCMaterialItemInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate();
	virtual void ActionPerformed( const KActionEvent& );
	virtual void FrameMoveInEnabledState();

public:
	void SetItem(GCITEMID dwItemID, DWORD dwImgColor = 0xffffffff, DWORD dwCostColor = 0xffffffff);
	void SetItemUID( GCITEMUID uItemUID ) { m_uItemUID = uItemUID; }
	void ItemSetCount(int iCount);
	void ItemSetLevel(int nLevel){ m_nLevel = nLevel; }
	void EnableSelection(bool bEnable);
	GCITEMID GetItemID() { return m_dwItemID; }
	GCITEMUID GetItemUID() { return m_uItemUID; }

private:
	void OnClickItemSelect( const KActionEvent& event );

private:
	KGCItemImgWnd*	m_pkItemImg;
	KD3DStatic*		m_pkItemCount;
	KD3DWnd*		m_pkItemSelBtn;

	GCITEMID		m_dwItemID;
	GCITEMUID		m_uItemUID;
	int				m_nLevel;
	int				m_nCount;
	int				m_nInvCount;
	bool			m_bEnableSelection;
	DWORD			m_dwImageColor;
	DWORD			m_dwCostColor;
};

DEFINE_WND_FACTORY( KGCMaterialItemInfo );
DECLARE_WND_FACTORY( KGCMaterialItemInfo );
DECLARE_WND_FACTORY_NAME( KGCMaterialItemInfo );

#endif // _KGC_MATERIAL_ITEM_INFO_H_