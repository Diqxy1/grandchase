#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCCheckBox;
class KD3DStatic;
class KGCInviteInfo;
class KGCBarControl;

class KGCInviteMsgBox : public KD3DDialog,
                        public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteMsgBox( const KGCInviteMsgBox& );
    const KGCInviteMsgBox& operator=( const KGCInviteMsgBox& );

public:
    DECLARE_CLASSNAME( KGCInviteMsgBox );
    KGCInviteMsgBox(void);
    virtual ~KGCInviteMsgBox(void);

    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

    void SetInviteInfo();
    void OnClickNo();

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete();

    void OnCheckBox();
    void OnClickYes();

private:
    KGCInviteInfo*  m_pkInviteInfo;
    KD3DStatic*     m_pkTitle;
    KD3DStatic*     m_pkDenyReserve;
    KD3DStatic*     m_pkInvite;
    KGCCheckBox*    m_pkCheck;
    KGCBarControl*  m_pkBar;
    KD3DWnd*        m_pkYes;
    KD3DWnd*        m_pkNo;
    bool            m_bInviteAccept;
};

DEFINE_WND_FACTORY( KGCInviteMsgBox );
DECLARE_WND_FACTORY( KGCInviteMsgBox );
DECLARE_WND_FACTORY_NAME( KGCInviteMsgBox );