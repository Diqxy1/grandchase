#pragma once

#define MAX_VIRTUALKEY_SIZE 68

class KGCVirtualKey;
class KGCVirtualKeyManager : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVirtualKeyManager );
    /// Default constructor
    KGCVirtualKeyManager( void );
    /// Default destructor
    virtual ~KGCVirtualKeyManager( void );
    /// Copy constructor
    KGCVirtualKeyManager( const KGCVirtualKeyManager& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVirtualKeyManager& operator=( const KGCVirtualKeyManager& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        EVK_TEXT        = 1,
        EVK_BACKSPACE   = 2,
        EVK_ENTER       = 3,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnInitialize();
    virtual void FrameMoveInEnabledState( void );

    void    AddKeyData();
    void    ShuffleKeyData();
    void    UpperLowerChange();
    std::wstring GetClickKeyData() { return m_strClickKeyData; }

private:
    std::vector< std::wstring >    m_vecNumberKeyData;
    std::vector< std::wstring >    m_vecAlphabetKeyData;
    std::vector< std::wstring >    m_vecSymbolMarkKeyData;

    KSafeArray<KGCVirtualKey*, MAX_VIRTUALKEY_SIZE> m_apkVirtualKey;
    KGCVirtualKey*  m_apkVirtualKey_Enter;
    KGCVirtualKey*  m_apkVirtualKey_BackSpace;
    KGCVirtualKey*  m_apkVirtualKey_UpperLower;

    bool    m_bUpperCase;
    std::wstring    m_strClickKeyData;

};

DEFINE_WND_FACTORY( KGCVirtualKeyManager );
DECLARE_WND_FACTORY( KGCVirtualKeyManager );
DECLARE_WND_FACTORY_NAME( KGCVirtualKeyManager );
