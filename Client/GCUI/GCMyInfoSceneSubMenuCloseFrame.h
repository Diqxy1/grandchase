#pragma once

class KGCMyInfoSceneSubMenuFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMyInfoSceneSubMenuFrame );
    /// Default constructor
    KGCMyInfoSceneSubMenuFrame( void );
    /// Default destructor
    virtual ~KGCMyInfoSceneSubMenuFrame( void );
    /// Copy constructor
    KGCMyInfoSceneSubMenuFrame( const KGCMyInfoSceneSubMenuFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMyInfoSceneSubMenuFrame& operator=( const KGCMyInfoSceneSubMenuFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

	void OnClickSellBtn( const KActionEvent& event); 
	void OnClickInitAttributeBtn( const KActionEvent& event );
	void OnClickSocketBtn( const KActionEvent& event );
	void OnClickSellCloseBtn( const KActionEvent& event );

	void InitSubMenu();
    void ButtonRender( bool bRender, bool bPet, bool bSell = true );
private:
	KD3DWnd*	m_pBtnSell;
	KD3DWnd*	m_pBtnInitAttribute;
	KD3DWnd*	m_pBtnSocket;
	KD3DWnd*	m_pBtnSeal;

	bool		m_bSellMode;
};

DEFINE_WND_FACTORY( KGCMyInfoSceneSubMenuFrame );
DECLARE_WND_FACTORY( KGCMyInfoSceneSubMenuFrame );
DECLARE_WND_FACTORY_NAME( KGCMyInfoSceneSubMenuFrame );
