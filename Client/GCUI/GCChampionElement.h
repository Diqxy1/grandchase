#pragma once

class KGCChampionElement : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChampionElement );
    /// Default constructor
    KGCChampionElement( void );
    /// Default destructor
    virtual ~KGCChampionElement( void );
    /// Copy constructor
    KGCChampionElement( const KGCChampionElement& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChampionElement& operator=( const KGCChampionElement& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetChampionText( int iIndex );
    void ResetChampionText();

private:
    KD3DStatic* m_pkText;
};

DEFINE_WND_FACTORY( KGCChampionElement );
DECLARE_WND_FACTORY( KGCChampionElement );
DECLARE_WND_FACTORY_NAME( KGCChampionElement );
