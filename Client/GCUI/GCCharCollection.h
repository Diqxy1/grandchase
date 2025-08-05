#pragma once

class KGCCharCollectionImage;

class KGCCharCollection : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharCollection );
    /// Default constructor
    KGCCharCollection( void );
    /// Default destructor
    virtual ~KGCCharCollection( void );
    /// Copy constructor
    KGCCharCollection( const KGCCharCollection& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharCollection& operator=( const KGCCharCollection& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    D3DCOLOR GetCharColor( int iCharType );
    void SetCharType( int iCharType, int iPromotion );
    void SetCharPromotion( int iCharType, int iPromotion );
    void SetCharPromotionTooltip( int iCharType );
    void ResetCharPlates();

private:
	KSafeArray< KGCCharCollectionImage*, GC_CHAR_NUM > m_vecCharContainer;
    KSafeArray< KD3DWnd*, 2 > m_vecNameBG;
    KD3DStatic* m_pkStaticCharName;
};

DEFINE_WND_FACTORY( KGCCharCollection );
DECLARE_WND_FACTORY( KGCCharCollection );
DECLARE_WND_FACTORY_NAME( KGCCharCollection );
