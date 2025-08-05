#pragma once

class KGCAgitUserElement : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitUserElement );
    /// Default constructor
    KGCAgitUserElement( void );
    /// Default destructor
    virtual ~KGCAgitUserElement( void );
    /// Copy constructor
    KGCAgitUserElement( const KGCAgitUserElement& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitUserElement& operator=( const KGCAgitUserElement& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DStatic* m_pkStaticName;
    DWORD       m_dwUID;
	bool        m_bIsRBtnClick;
	bool        m_bIsLBtnClick;
	bool		m_bIsInNameBG;

public:
    void SetUserName( std::wstring& strName_, DWORD dwUID );
    const std::wstring& GetUserName( void );
    void Clear( void );

	bool IsEmpty( void );
	DWORD GetUserUID( void ) const { return m_dwUID; }
	bool IsRBtnClickOnList( void ) const { return m_bIsRBtnClick; }
	void SetRClick( bool IsClick );
	bool IsLBtnClickOnList( void ) const { return m_bIsLBtnClick; }
	void SetLClick( bool IsClick );
};

DEFINE_WND_FACTORY( KGCAgitUserElement );
DECLARE_WND_FACTORY( KGCAgitUserElement );
DECLARE_WND_FACTORY_NAME( KGCAgitUserElement );
