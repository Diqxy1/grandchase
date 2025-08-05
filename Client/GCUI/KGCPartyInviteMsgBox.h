#pragma once

class KD3DStatic;
class KGCBarControl;

class KGCPartyInviteMsgBox : public KD3DDialog,
                        public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCPartyInviteMsgBox );
    /// Default constructor
    KGCPartyInviteMsgBox( void );
    /// Default destructor
    virtual ~KGCPartyInviteMsgBox( void );
    /// Copy constructor
    KGCPartyInviteMsgBox( const KGCPartyInviteMsgBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPartyInviteMsgBox& operator=( const KGCPartyInviteMsgBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void OnClickNo();
    void OnClickYes();

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete();

private:
    KD3DStatic*     m_pkTitle;
    KD3DStatic*     m_pkInvite;
    KGCBarControl*  m_pkBar;
    KD3DWnd*        m_pkYes;
    KD3DWnd*        m_pkNo;
    bool            m_bInviteAccept;
    DWORD           m_dEndTime;

};

DEFINE_WND_FACTORY( KGCPartyInviteMsgBox );
DECLARE_WND_FACTORY( KGCPartyInviteMsgBox );
DECLARE_WND_FACTORY_NAME( KGCPartyInviteMsgBox );