#pragma once

class KGCPopupAttributeSelect : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPopupAttributeSelect );
    /// Default constructor
    KGCPopupAttributeSelect( void );
    /// Default destructor
    virtual ~KGCPopupAttributeSelect( void );
    /// Copy constructor
    KGCPopupAttributeSelect( const KGCPopupAttributeSelect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPopupAttributeSelect& operator=( const KGCPopupAttributeSelect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
    KD3DWnd*    m_pkBtnUseScroll;
    KD3DWnd*    m_pkBtnUseScrollSingle;
    KD3DWnd*    m_pkBtnFreeSelect;

    KD3DSizingBox*  m_pkBackMenu2;
    KD3DSizingBox*  m_pkBackMenu3;
        

    KItem*      m_pkSelectInvenItem;
    GCItem*     m_pkSelectItemData;
    D3DXVECTOR2 m_dxvPos;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void  ShowPopupAttributeSelect( const GCITEMUID itemUID_, const D3DXVECTOR2 dxvPos_, bool bMigration_ );

private:
    void UpdateBackUI( bool bMigration );

    void OnClickUseScroll( void );
    void OnClickUseScrollSingle( void );
    void OnClickFreeSelect( void );

};

DEFINE_WND_FACTORY( KGCPopupAttributeSelect );
DECLARE_WND_FACTORY( KGCPopupAttributeSelect );
DECLARE_WND_FACTORY_NAME( KGCPopupAttributeSelect );
