#pragma once

class KGCVirtualKey : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVirtualKey );
    /// Default constructor
    KGCVirtualKey( void );
    /// Default destructor
    virtual ~KGCVirtualKey( void );
    /// Copy constructor
    KGCVirtualKey( const KGCVirtualKey& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVirtualKey& operator=( const KGCVirtualKey& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnInitialize();
    virtual void FrameMoveInEnabledState( void );

    void SetKeyData( const std::wstring& strKeyData_ );
    std::wstring GetKeyData();
    bool IsUpperCase() { return m_bUpperCase; }

private:
    bool    m_bUpperCase;
    KD3DStatic* m_pkShowData;
};

DEFINE_WND_FACTORY( KGCVirtualKey );
DECLARE_WND_FACTORY( KGCVirtualKey );
DECLARE_WND_FACTORY_NAME( KGCVirtualKey );
