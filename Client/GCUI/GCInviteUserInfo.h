#pragma once

//#include "../uifx/D3DWnd.h"
//#include "UserPacket.h"

class KD3DSizingBox;
class KD3DStatic;
class KGCCheckBox;
class KGCGradeMark;

class KGCInviteUserInfo:  public KD3DWnd
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteUserInfo( const KGCInviteUserInfo& );
    const KGCInviteUserInfo& operator=( const KGCInviteUserInfo& );

public:
    DECLARE_CLASSNAME( KGCInviteUserInfo );
    KGCInviteUserInfo(void);
    virtual ~KGCInviteUserInfo(void);

    // getter/setter
    bool IsLock() const { return m_bEmpty || ( !m_bEmpty && m_bDiffGrade ); }
    void SetBackMode( ED3DWndMode eMode );
    ED3DWndMode GetBackMode() const;
    void SetCheck( bool bCheck);
    bool GetCheck() const { return m_bCheck; }
    DWORD GetUID() const { return m_dwUserUID; }

    void SetUserInfo( const KInviteUserInfo& kInfo );
	void SetFriendUserInfo( const KInviteFriendUserInfo& kInfo );
	
    void InitUserInfo();

protected:
    virtual void OnCreate( void );

private:
    bool m_bDiffGrade;
    bool m_bEmpty;
    KD3DSizingBox*  m_pkBack;
    KD3DStatic*     m_pkUserInfo;
    KGCCheckBox*    m_pkCheck;
    KGCGradeMark*   m_pkGrade;
    DWORD m_dwUserUID;
    bool m_bCheck;
    KD3DWnd*        m_pkLock;
};

DEFINE_WND_FACTORY( KGCInviteUserInfo );
DECLARE_WND_FACTORY( KGCInviteUserInfo );
DECLARE_WND_FACTORY_NAME( KGCInviteUserInfo );