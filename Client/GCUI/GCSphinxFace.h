#pragma once

class KGCSphinxFace : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxFace );
    /// Default constructor
    KGCSphinxFace( void );
    /// Default destructor
    virtual ~KGCSphinxFace( void );
    /// Copy constructor
    KGCSphinxFace( const KGCSphinxFace& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxFace& operator=( const KGCSphinxFace& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetSphinxFace( int iState );
    void PostDraw( );
    void RemoveFaceTexture();
    void ViewWinPaticle();
    void ViewLosePaticle();
private:
    KGC2DObject	*m_pkSphinxFace; //스핑크스 이미지

};

DEFINE_WND_FACTORY( KGCSphinxFace );
DECLARE_WND_FACTORY( KGCSphinxFace );
DECLARE_WND_FACTORY_NAME( KGCSphinxFace );
