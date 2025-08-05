#pragma once

class KGCVirtualDepotChar : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVirtualDepotChar );
    /// Default constructor
    KGCVirtualDepotChar( void );
    /// Default destructor
    virtual ~KGCVirtualDepotChar( void );
    /// Copy constructor
    KGCVirtualDepotChar( const KGCVirtualDepotChar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVirtualDepotChar& operator=( const KGCVirtualDepotChar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetChar(int iChar);
    int GetCurrentCharType(void) { return m_iCharType; }
private:
    std::map<int,KD3DWnd*> m_mapCharInfoVD;
    int m_iCharType;
};

DEFINE_WND_FACTORY( KGCVirtualDepotChar );
DECLARE_WND_FACTORY( KGCVirtualDepotChar );
DECLARE_WND_FACTORY_NAME( KGCVirtualDepotChar );
