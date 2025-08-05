#pragma once

#include "GCUI/GCItemImgWnd.h" 


class KGCMagicBoxItemWnd : public KD3DWnd,    // extends
                           public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMagicBoxItemWnd );
    /// Default constructor
    KGCMagicBoxItemWnd( void );
    /// Default destructor
    virtual ~KGCMagicBoxItemWnd( void );
    /// Copy constructor
    KGCMagicBoxItemWnd( const KGCMagicBoxItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMagicBoxItemWnd& operator=( const KGCMagicBoxItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    void SetImage( DWORD dwGoodsID_ = 0);
    void SetCount( int nCount_ = -1 );
    void RenderIcon( bool _bRender = false );
    void SetBlank();
    GCITEMID GetItemID( );

    KGCItemImgWnd*      m_pkItemImg;        //item_img
    KD3DWnd*      m_pkMouseOverImg;   //mouse_over_img
    KD3DSizingBox*      m_pkSizingBg;       //sizing_bg
    KD3DWnd*            m_pkBtnClose;       //icon

    KGCUINumber*        m_pkStaticCount;    //static_item_count

    GCITEMID            m_ItemID;


};

DEFINE_WND_FACTORY( KGCMagicBoxItemWnd );
DECLARE_WND_FACTORY( KGCMagicBoxItemWnd );
DECLARE_WND_FACTORY_NAME( KGCMagicBoxItemWnd );
