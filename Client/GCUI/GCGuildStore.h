#pragma once
#include "GCItemImgWnd.h"
#include "GCGuildItemBox.h"
#include "../MyD3D.h"
#include "../Procedure.h"

class KGCGuildItemBox;

class KGCGuildStore : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGuildStore );
    /// Default constructor
    KGCGuildStore( void );
    /// Default destructor
    virtual ~KGCGuildStore( void );
    /// Copy constructor
    KGCGuildStore( const KGCGuildStore& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildStore& operator=( const KGCGuildStore& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        EMAX_TAB_NUM = 2,
        EMAX_ITEM_BOX = 6,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState();
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    

    void OnClose();
    void SendGuildStoreCatalogReq();

    void SetItemTab();
    void SetItemList(std::map<int, std::vector<KManufactureItem>>& mapItemList_);

    void SetTabToggle(int iIndex_);

private:
    int                 m_iCurrentTabIndex;

    KD3DWnd*            m_pkBtnClose;

    KD3DWnd*            m_pkBtnPage[EMAX_TAB_NUM];
    KD3DWnd*            m_pkBtnPageover[EMAX_TAB_NUM];

    KD3DStatic*         m_pkStaticComment;

    KGCGuildItemBox*    m_pkItemBox[EMAX_ITEM_BOX];

    std::map<int, std::vector<KManufactureItem>>        m_mapItemList;

};

DEFINE_WND_FACTORY( KGCGuildStore );
DECLARE_WND_FACTORY( KGCGuildStore );
DECLARE_WND_FACTORY_NAME( KGCGuildStore );
