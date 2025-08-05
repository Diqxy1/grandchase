#ifndef _GCGambleBox_H_
#define _GCGambleBox_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCGambleBox : public KD3DDialog,          // extends
	public KActionListener   // implements
{
    enum {
        MAX_I_GOT_WND = 9,
        MAX_WANNA_DO_WND = 6,
    };
public:
	DECLARE_CLASSNAME( KGCGambleBox );
	KGCGambleBox(void);
	~KGCGambleBox(void);	

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
	virtual void OnCreate( void );
	virtual void OnUpdate( void );

	virtual void FrameMoveInEnabledState( void );

    void OnDestroy();
	void InsertGambledItem( std::vector<KItem>& vecItem );

	std::vector<GCITEMUID>		m_vecGot;
	std::vector<GCItem*>	m_vecWanna;

	KGCItemImgWnd*			m_itemwndGot[ MAX_I_GOT_WND ];
	KGCItemImgWnd*			m_itemwndwannado[ MAX_WANNA_DO_WND ];

	int						m_iCurrentPage;
	int						m_iTotalPage;
	float					m_fCostRate;

	void RemoveItem( GCITEMUID uid );
	void DeleteItem( DWORD goodsID );
protected:

	void SortGot();
	void SellEtcItem();
	KD3DWnd*	            m_pkBtnBuy;
	KD3DWnd*	            m_pkBtnExit;

    int                     m_iItemID;
	DWORD					m_dwTotalPrice;


};


DEFINE_WND_FACTORY( KGCGambleBox );
DECLARE_WND_FACTORY( KGCGambleBox );
DECLARE_WND_FACTORY_NAME( KGCGambleBox );

#endif // _GCGambleBox_H_
