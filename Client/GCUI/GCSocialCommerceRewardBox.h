#pragma once


class KGCSocialCommerceRewardBox : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSocialCommerceRewardBox );
    /// Default constructor
    KGCSocialCommerceRewardBox( void );
    /// Default destructor
    virtual ~KGCSocialCommerceRewardBox( void );
    /// Copy constructor
    KGCSocialCommerceRewardBox( const KGCSocialCommerceRewardBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSocialCommerceRewardBox& operator=( const KGCSocialCommerceRewardBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnClose;

    KGCItemImgWnd*              m_pkItemImgGoods;
    KGCItemImgWnd*              m_pkItemImgReward;

    KD3DStatic*                 m_pkStaticItemNameGoods;
    KD3DStatic*                 m_pkStaticItemNameReward;
    KD3DStatic*                 m_pkStaticTitleDesc;
    KD3DStatic*                 m_pkStaticDesc;



public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();
    void SetRewardInfo( std::pair<KSCSellInfo, VEC_REWARD>& prInfo );


};

DEFINE_WND_FACTORY( KGCSocialCommerceRewardBox );
DECLARE_WND_FACTORY( KGCSocialCommerceRewardBox );
DECLARE_WND_FACTORY_NAME( KGCSocialCommerceRewardBox );
