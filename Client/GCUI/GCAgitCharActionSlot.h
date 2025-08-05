#pragma once

class KGCAgitCharActionSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitCharActionSlot );
    /// Default constructor
    KGCAgitCharActionSlot( void );
    /// Default destructor
    virtual ~KGCAgitCharActionSlot( void );
    /// Copy constructor
    KGCAgitCharActionSlot( const KGCAgitCharActionSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitCharActionSlot& operator=( const KGCAgitCharActionSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DWnd*    m_pkBtnDefault;
    KD3DWnd*    m_pkBtnActive;
    KD3DStatic* m_pkStaticAction;

    int m_iActionIndex;
    int m_iStringIndex;

public:
    void SetSlot( int iActionIndex_, int iStringIndex_ );
    void Clear( void );

    void OnClickBtn( void );
    void SetActive( bool bActive_ );
    bool IsActiveSlot( void ) { return m_pkBtnActive->IsRenderOn(); }
    int  GetActionIndex( void ) { return m_iActionIndex; }
};

DEFINE_WND_FACTORY( KGCAgitCharActionSlot );
DECLARE_WND_FACTORY( KGCAgitCharActionSlot );
DECLARE_WND_FACTORY_NAME( KGCAgitCharActionSlot );
