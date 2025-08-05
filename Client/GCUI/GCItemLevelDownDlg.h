#pragma once

class KGCItemLevelDownDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemLevelDownDlg );
    /// Default constructor
    KGCItemLevelDownDlg( void );
    /// Default destructor
    virtual ~KGCItemLevelDownDlg( void );
    /// Copy constructor
    KGCItemLevelDownDlg( const KGCItemLevelDownDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemLevelDownDlg& operator=( const KGCItemLevelDownDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

public:
    void RenderItemInfoBox( D3DXVECTOR2& vPos_, KItem pInvenItem_ );    
    void UpdateItemInfo();
    void OnOK();
    void OnClose(); 


    KD3DStatic* m_pkStaticDesc;
    KD3DStatic* m_pkStaticItemName;
    KD3DStatic* m_pkStaticEquipLevel;
    KD3DStatic* m_pkStaticHeroCoin;
    KD3DStatic* m_pkStaticRemainLevelDown;
    KD3DStatic* m_pkStaticHeroCoinNum;
    KD3DStatic* m_pkStaticRemainLevelDownNum;

    KD3DWnd*    m_pkBtnOK;
    KD3DWnd*    m_pkBtnCancle;
    KD3DWnd*    m_pkBtnClose;

    KGCItemImgWnd* m_pkEquipmentItemWnd;

    GCITEMUID   m_itemUID;
    KItem       m_kItemInfo;
};

DEFINE_WND_FACTORY( KGCItemLevelDownDlg );
DECLARE_WND_FACTORY( KGCItemLevelDownDlg );
DECLARE_WND_FACTORY_NAME( KGCItemLevelDownDlg );
