#pragma once

class KGCAgitEnterPass : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
	enum { 
		MAX_AGIT_PASSWORD = 6,
	};

    DECLARE_CLASSNAME( KGCAgitEnterPass );
    /// Default constructor
    KGCAgitEnterPass( void );
    /// Default destructor
    virtual ~KGCAgitEnterPass( void );
    /// Copy constructor
    KGCAgitEnterPass( const KGCAgitEnterPass& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitEnterPass& operator=( const KGCAgitEnterPass& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
	KD3DStatic*		m_pkStaticDesc;
	KD3DWnd*        m_pkBtnOK;
	KD3DWnd*        m_pkBtnCancel;
	KD3DEdit*		m_pkEditPass;

public:
	DWORD			m_dwAgitID;
	std::wstring	m_strPass;

public:
	void SetAgitID( DWORD dwAgitID ) { m_dwAgitID = dwAgitID; }
	void InitData();
	void VisibleUI( DWORD dwAgitID );

public:
	void OnClose( void );
	void OnEnterAgit();
	void OnClickEditPass();
};

DEFINE_WND_FACTORY( KGCAgitEnterPass );
DECLARE_WND_FACTORY( KGCAgitEnterPass );
DECLARE_WND_FACTORY_NAME( KGCAgitEnterPass );
