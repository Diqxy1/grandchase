#pragma once

class KGCSphinxChallenge : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxChallenge );
    /// Default constructor
    KGCSphinxChallenge( void );
    /// Default destructor
    virtual ~KGCSphinxChallenge( void );
    /// Copy constructor
    KGCSphinxChallenge( const KGCSphinxChallenge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxChallenge& operator=( const KGCSphinxChallenge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void OnClickBtnOXQuizStart();
private:
    KD3DStatic  *m_pkStaticSphinxMent;
    KD3DWnd     *m_pkBtnChallenge;
};

DEFINE_WND_FACTORY( KGCSphinxChallenge );
DECLARE_WND_FACTORY( KGCSphinxChallenge );
DECLARE_WND_FACTORY_NAME( KGCSphinxChallenge );
