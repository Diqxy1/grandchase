#pragma once

class KGCAgitUserListCombo;

class KGCAgitInfoBar : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitInfoBar );
    /// Default constructor
    KGCAgitInfoBar( void );
    /// Default destructor
    virtual ~KGCAgitInfoBar( void );
    /// Copy constructor
    KGCAgitInfoBar( const KGCAgitInfoBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitInfoBar& operator=( const KGCAgitInfoBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum { NUM_OPEN_TYPE = 3 };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void SetAgitInfo( void );
    void SetOpenType( int iType_ );
    void SetAgitName( void );
    void SetNumToday( void );
    void SetNumTotal( void );

	bool CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos );
	void SetPopupBoxRender( bool bSet );
    
private:
    KSafeArray< KD3DWnd*, NUM_OPEN_TYPE >   m_apkOpenType;
    KGCAgitUserListCombo*                   m_pkUserCombo;

    KD3DStatic*                 m_pkStaticAgitName;
    KD3DStatic*                 m_pkStaticToday;
    KD3DStatic*                 m_pkStaticNumToday;
    KD3DStatic*                 m_pkStaticTotal;
    KD3DStatic*                 m_pkStaticNumTotal;
};

DEFINE_WND_FACTORY( KGCAgitInfoBar );
DECLARE_WND_FACTORY( KGCAgitInfoBar );
DECLARE_WND_FACTORY_NAME( KGCAgitInfoBar );
