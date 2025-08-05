#pragma once

class KGCSphinxResultItemBox;

class KGCSphinxResultItemView : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxResultItemView );
    /// Default constructor
    KGCSphinxResultItemView( void );
    /// Default destructor
    virtual ~KGCSphinxResultItemView( void );
    /// Copy constructor
    KGCSphinxResultItemView( const KGCSphinxResultItemView& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxResultItemView& operator=( const KGCSphinxResultItemView& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

    void SetRewardItem(KEVENT_SPHINX_ITEM_LIST_NOT mapRewardItem);
    void SetRewardItem();
    void ClearItemList();
    void OnScrollPos(int iPos);

    void UpdateItemList();
    void SetViewItem();

    void OnChangeStateChallenge();
    void PostChildDraw();
    void ItemInfoDraw();
private:
    KD3DStatic*     m_pkStatic;
    KD3DScrollbar*  m_pkScrollbar;
    KD3DWnd*        m_pkOKBtn;
    KD3DWnd*        m_pkClipRange;
    KGCSphinxResultItemBox* m_pkItemBox;

    int m_iTopIndex;
    int m_iScrollPos;

    int m_iViewFirstIndex;

    std::vector<std::pair<int,int>>      m_vecRewardItemList;
    std::vector<KGCSphinxResultItemBox*> m_vecResultItemImageList;
    KSafeArray<KD3DWnd*, 8>                m_arrBackBox;

};

DEFINE_WND_FACTORY( KGCSphinxResultItemView );
DECLARE_WND_FACTORY( KGCSphinxResultItemView );
DECLARE_WND_FACTORY_NAME( KGCSphinxResultItemView );
