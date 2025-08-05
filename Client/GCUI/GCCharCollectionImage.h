#pragma once

class KGCCharCollectionImage : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharCollectionImage );
    /// Default constructor
    KGCCharCollectionImage( void );
    /// Default destructor
    virtual ~KGCCharCollectionImage( void );
    /// Copy constructor
    KGCCharCollectionImage( const KGCCharCollectionImage& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharCollectionImage& operator=( const KGCCharCollectionImage& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void OnCreate( void );
    void SetImageActive( int iCharPromotion, bool bIsActive );
    void SetImageTooltip( int iCharType, int iCharPromotion );

private:
	KSafeArray< KD3DWnd*, 4 > m_vecCharImage;
    KSafeArray< KD3DWnd*, 4 > m_vecCharSelect;

};

DEFINE_WND_FACTORY( KGCCharCollectionImage );
DECLARE_WND_FACTORY( KGCCharCollectionImage );
DECLARE_WND_FACTORY_NAME( KGCCharCollectionImage );
