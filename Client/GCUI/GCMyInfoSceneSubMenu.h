#pragma once

class KGCMyInfoSceneSubMenuFrame;

class KGCMyInfoSceneSubMenu : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMyInfoSceneSubMenu );
    /// Default constructor
    KGCMyInfoSceneSubMenu( void );
    /// Default destructor
    virtual ~KGCMyInfoSceneSubMenu( void );
    /// Copy constructor
    KGCMyInfoSceneSubMenu( const KGCMyInfoSceneSubMenu& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMyInfoSceneSubMenu& operator=( const KGCMyInfoSceneSubMenu& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void InitSubMenu();
    void ButtonRender( bool bRender, bool bPet, bool bSell = true );
private:
	KD3DWnd*	m_pBtnOpen;
	KD3DWnd*	m_pBtnClose;
	KGCMyInfoSceneSubMenuFrame* m_pCloseFrame;

	void ClickCloseBtn(); 
	void ClickOpenBtn(); 
};

DEFINE_WND_FACTORY( KGCMyInfoSceneSubMenu );
DECLARE_WND_FACTORY( KGCMyInfoSceneSubMenu );
DECLARE_WND_FACTORY_NAME( KGCMyInfoSceneSubMenu );
