#pragma once

class KGCSphinxInfoView : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSphinxInfoView );
    /// Default constructor
    KGCSphinxInfoView( void );
    /// Default destructor
    virtual ~KGCSphinxInfoView( void );
    /// Copy constructor
    KGCSphinxInfoView( const KGCSphinxInfoView& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSphinxInfoView& operator=( const KGCSphinxInfoView& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void SetTotalAnswer(int iTotalAnswer);
    void SetDiffculty( int iDifficult );
    void UpdateCoin( );

    enum SPHINX_INFO_STATIC_ENUM{
        SISE_COIN               = 0,
        SISE_COIN_NUM           = 1,
        SISE_DIFFICULTY         = 2,
        SISE_DIFFICULTY_NUM     = 3,
        SISE_TOTAL_ANSWER       = 4,
        SISE_TOTAL_ANSWER_NUM   = 5,
        SISE_MAX                = 6,
    };

    void OnChangeStateResultItemView();
    void OnBuyBtn();
private:
    KD3DWnd         *m_pkBuyBtn;
    KD3DWnd         *m_pkRewardItemViewBtn;

    std::vector<KD3DStatic*> m_vecStaticSphinxInfo;
};

DEFINE_WND_FACTORY( KGCSphinxInfoView );
DECLARE_WND_FACTORY( KGCSphinxInfoView );
DECLARE_WND_FACTORY_NAME( KGCSphinxInfoView );
