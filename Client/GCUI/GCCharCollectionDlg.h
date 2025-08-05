#pragma once
class KGCCharCollection;
class KGCCharCollectionImage;
class KGCCharCollectionDlg : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCharCollectionDlg );
    KGCCharCollectionDlg( void );
    virtual ~KGCCharCollectionDlg( void );
    KGCCharCollectionDlg( const KGCCharCollectionDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    const KGCCharCollectionDlg& operator=( const KGCCharCollectionDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    void OnScrollPos();
    void SetAttrString();
    void CreateCharSlot();
    void UpdateSlotCollectionViewList();
    void UpdateCollectionViewItemList();
    void PopulateCharPlate( int iIndex, int iCharType );
    int                               m_iScrollpos;
private:
	enum {
        EM_STARTER_X         =  78,
        EM_STARTER_Y         = 161,
		EM_INVEN_ROW		 =   3,
		EM_INVEN_COLUMN		 =   2,
		EM_INVEN_WIDH_SIZE	 = 279,
		EM_INVEN_HEIGHT_SIZE = 162,
	};
    std::vector< KGCCharCollection* >               m_vecCharPlate;
    KD3DScrollbar*                                  m_pkScrollbar;
    KD3DStatic*                                     m_pkStatusList;
};
DEFINE_WND_FACTORY( KGCCharCollectionDlg );
DECLARE_WND_FACTORY( KGCCharCollectionDlg );
DECLARE_WND_FACTORY_NAME( KGCCharCollectionDlg );
