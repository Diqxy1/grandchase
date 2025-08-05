#pragma once
#include "stdafx.h"

class KGCNicknameCampaignBox : 
    public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCNicknameCampaignBox );
    /// Default constructor
    KGCNicknameCampaignBox( void );
    /// Default destructor
    virtual ~KGCNicknameCampaignBox( void );
    /// Copy constructor
    KGCNicknameCampaignBox( const KGCNicknameCampaignBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNicknameCampaignBox& operator=( const KGCNicknameCampaignBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );


    void OnClose();
    bool IsValidNickname();
    bool IsValidNewNickname();
    void OnConfirmNickName();
    bool CheckNickNameWidthMsgBox();
    bool IsValidNicknameCondition();
    void SetValidNickname( bool bTrue ) { m_bValidNickname = bTrue; };
    bool GetValidNickname( ) { return m_bValidNickname; };
    void OnClickEditNick();
    void OnCheckNoRecommend();
    const std::wstring& GetNickName( void ) const;
    void SetNicknameBox(bool bTrue);

    void SetFirst( bool bFirst ) { m_bFirst = bFirst; }
    bool GetFirst() { return m_bFirst; }


private:
    KD3DWnd*            m_pkBtnClose;
    KD3DWnd*            m_pkBtnConfirm;
    KD3DStatic*         m_pkStaticNoRecommend;
    KD3DStatic*         m_pkStaticNotice;
    KD3DEdit*           m_pkEditNick;
    KD3DCheckBox*       m_pkCheckNoRecommend;
    KD3DSizingBox*      m_pkBackground;


    std::wstring        m_strNickName;

    bool                m_bValidNickname;
    bool                m_bValidNicknameCheckComplete;
    bool                m_bFirst;

};

DEFINE_WND_FACTORY( KGCNicknameCampaignBox );
DECLARE_WND_FACTORY( KGCNicknameCampaignBox );
DECLARE_WND_FACTORY_NAME( KGCNicknameCampaignBox );
