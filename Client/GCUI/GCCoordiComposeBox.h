#pragma once

class KGCCoordiComposeBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{

enum COMPOSE_TYPE
{
    NORMAL_COMPOSE = 0,
    RANDOM_COMPOSE = 1,
    COMPOSE_SIZE = 2,
};

public:
    DECLARE_CLASSNAME( KGCCoordiComposeBox );
    /// Default constructor
    KGCCoordiComposeBox( void );
    /// Default destructor
    virtual ~KGCCoordiComposeBox( void );
    /// Copy constructor
    KGCCoordiComposeBox( const KGCCoordiComposeBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiComposeBox& operator=( const KGCCoordiComposeBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void ToggleRender( bool bRender );

public:
    void Init();
    void Reset();
    void OnClickBtnClose();
    void OnClickBtnNormalCompose();
    void OnClickBtnRandomCompose();
    void OnClickBtnCompose();
    
    void UpdateResultCoordi();
    void UpdateCheckButton();
    void UpdateScrollNum();
    void UpdateGuideString();

    bool SetItemUID( GCITEMUID _itemUID );

private:
    KD3DWnd*    m_pkBtnClose;
    KGCCheckBox*    m_pkCheckBtnNormal;
    KGCCheckBox*    m_pkCheckBtnRandom;
    KD3DWnd*    m_pkBtnCompose;

    KGCUserEquipWnd*    m_pOuterCoordi;
    KGCUserEquipWnd*    m_pInnerCoordi;
    KGCUserEquipWnd*    m_pResultCoordiNormal;
    KGCUserEquipWnd*    m_pResultCoordiRandom;

    KD3DStatic* m_pkScrollNum;
    KD3DStatic* m_pkNormalBtnStatic;
    KD3DStatic* m_pkRandomBtnStatic;
    KD3DStatic* m_pkGuideStatic;

protected:
    GCITEMUID   m_OuterUID;
    GCITEMUID   m_InnerUID;
    int         m_nComposeType;    
};

DEFINE_WND_FACTORY( KGCCoordiComposeBox );
DECLARE_WND_FACTORY( KGCCoordiComposeBox );
DECLARE_WND_FACTORY_NAME( KGCCoordiComposeBox );
