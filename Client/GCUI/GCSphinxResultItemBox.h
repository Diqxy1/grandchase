#pragma once

class KGCSphinxResultItemBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxResultItemBox );
    /// Default constructor
    KGCSphinxResultItemBox( void );
    /// Default destructor
    virtual ~KGCSphinxResultItemBox( void );
    /// Copy constructor
    KGCSphinxResultItemBox( const KGCSphinxResultItemBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxResultItemBox& operator=( const KGCSphinxResultItemBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnDestroy();
    virtual void PostDraw();

    void SetItem(GCITEMID itemID, int num);
    GCITEMID GetItemID(){return m_ItemID;}
    void ClearImage(); 
    void SetItemImagePos();
private:
    KGCUINumber*    m_pkItemNum;
    KGC2DObject*    m_pkItemImage;

    GCITEMID        m_ItemID;
};

DEFINE_WND_FACTORY( KGCSphinxResultItemBox );
DECLARE_WND_FACTORY( KGCSphinxResultItemBox );
DECLARE_WND_FACTORY_NAME( KGCSphinxResultItemBox );
