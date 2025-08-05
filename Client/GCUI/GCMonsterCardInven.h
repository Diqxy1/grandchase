#pragma once

class KGCUINumber;

class KGCMonsterCardInven : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMonsterCardInven );
    /// Default constructor
    KGCMonsterCardInven( void );
    /// Default destructor
    virtual ~KGCMonsterCardInven( void );
    /// Copy constructor
    KGCMonsterCardInven( const KGCMonsterCardInven& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterCardInven& operator=( const KGCMonsterCardInven& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
	void SetDuration( int nDuration );
	void SetItemInfo( GCITEMID _itemID );
	void ShowDuration( bool _bShow );
	void OnClickSlot();
	void SetShowItem( bool _bShow );

public:
	KGCItemImgWnd*		m_pkItemImg;
	KGCUINumber*		m_pkDuration;

public:
	GCITEMID			m_ItemID;
	int					m_nDuration;
	int					m_nSlotIndex;

};

DEFINE_WND_FACTORY( KGCMonsterCardInven );
DECLARE_WND_FACTORY( KGCMonsterCardInven );
DECLARE_WND_FACTORY_NAME( KGCMonsterCardInven );
