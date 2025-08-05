#pragma once
class KGCVipCollection;
class KGCVipCollectionDlg : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCVipCollectionDlg );
    KGCVipCollectionDlg( void );
    virtual ~KGCVipCollectionDlg( void );
    KGCVipCollectionDlg( const KGCVipCollectionDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    const KGCVipCollectionDlg& operator=( const KGCVipCollectionDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    void OnScrollPos();
    void SetAttrString();
    void CreateVIPSlot();
    void UpdateSlotCollectionViewList();
    void UpdateCollectionViewItemList();
    void PopulateVIPPlate( int iIndex, int vipIndex, int iFontID );
    int m_iScrollpos;
private:
	enum {
        EM_STARTER_X         =  79,
        EM_STARTER_Y         = 165,
		EM_INVEN_ROW		 =   2,
		EM_INVEN_COLUMN		 =   4,
		EM_INVEN_WIDH_SIZE	 = 137,
		EM_INVEN_HEIGHT_SIZE = 236,
	};
    std::vector< KGCVipCollection* >        m_vecVipPlate;
    KD3DScrollbar*                          m_pkScrollbar;
    KD3DStatic*                             m_pkStatusList;
    KD3DStatic*                             m_pkStatusListComplete;
};
DEFINE_WND_FACTORY( KGCVipCollectionDlg );
DECLARE_WND_FACTORY( KGCVipCollectionDlg );
DECLARE_WND_FACTORY_NAME( KGCVipCollectionDlg );
