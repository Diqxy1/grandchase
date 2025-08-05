#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class KGCMaterialItemInfo : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMaterialItemInfo );
    /// Default constructor
    KGCMaterialItemInfo( void );
    /// Default destructor
    virtual ~KGCMaterialItemInfo( void );
    /// Copy constructor
    KGCMaterialItemInfo( const KGCMaterialItemInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMaterialItemInfo& operator=( const KGCMaterialItemInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event_ );

private:

    KD3DWnd*        m_pkBack;
    KD3DStatic*     m_pkStaticCount;
    KD3DWnd*        m_pkItemBtn;
    unsigned short  m_usIndex;
    GCITEMID        m_giItemID;
    bool            m_bEnable;

public:
    void SetItemID(GCITEMID giItemID){m_giItemID = giItemID;}
    GCITEMID GetItemID(){return m_giItemID;}

    void SetIndex(unsigned short usIndex){m_usIndex = usIndex;}
    bool IsEnable( void ) { return m_bEnable; }
    void SetHeightDirect( DWORD dwHeight );
};

DEFINE_WND_FACTORY( KGCMaterialItemInfo );
DECLARE_WND_FACTORY( KGCMaterialItemInfo );
DECLARE_WND_FACTORY_NAME( KGCMaterialItemInfo );


