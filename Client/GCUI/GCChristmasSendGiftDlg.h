#pragma once

class KGCChristmasFriendList;

class KGCChristmasSendGiftDlg : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChristmasSendGiftDlg );
    /// Default constructor
    KGCChristmasSendGiftDlg( void );
    /// Default destructor
    virtual ~KGCChristmasSendGiftDlg( void );
    /// Copy constructor
    KGCChristmasSendGiftDlg( const KGCChristmasSendGiftDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChristmasSendGiftDlg& operator=( const KGCChristmasSendGiftDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnFriendList;
    KD3DWnd*                    m_pkBtnSendGift;
    KD3DWnd*                    m_pkBtnClose;

    KD3DStatic*                 m_pkStaticSendGift;
    KD3DStatic*                 m_pkStaticDesc;
    KD3DStatic*                 m_pkStaticEditDesc;

    KD3DEdit*                   m_pkEditName;

    KGCItemImgWnd*              m_pkGiftItemImgWnd;

    KGCChristmasFriendList*     m_pkFriendList;


    //
    DWORD               m_dwReceiveUserUID;
    KDropItemInfo       m_kDropItemInfo;

    wstring             m_strEditName;


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void InitEventInfo();
    void OnClickSendGift();
    void OnClickFriendList();
    void OnClickClose();


    void SetSendGiftItemInfo( GCITEMID dwItemID, KDropItemInfo kDropItemInfo_ );

    void SendGift( DWORD dwReceiveUserUID );

    void GetNameFromFriendList( wstring& strEditName );
    wstring& GetUserNameToRecieve(){ return m_strEditName; }


};

DEFINE_WND_FACTORY( KGCChristmasSendGiftDlg );
DECLARE_WND_FACTORY( KGCChristmasSendGiftDlg );
DECLARE_WND_FACTORY_NAME( KGCChristmasSendGiftDlg );
