#pragma once

//#include "../uifx/D3DWnd.h"
//#include "KGCInviteManager.h"

class KD3DStatic;

class KGCInviteInfo : public KD3DWnd
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteInfo( const KGCInviteInfo& );
    const KGCInviteInfo& operator=( const KGCInviteInfo& );

public:
    DECLARE_CLASSNAME( KGCInviteInfo );
    KGCInviteInfo(void);
    virtual ~KGCInviteInfo(void);

    void SetInviteInfo( const KInviteInfo& kInfo );

protected:
    virtual void OnCreate();

private:
    KD3DStatic* m_pkChannel;
    KD3DStatic* m_pkRoomNum;
    KD3DStatic* m_pkMode;
    KD3DStatic* m_pkRoomTitle;

    KInviteInfo   m_kInviteInfo;
};

DEFINE_WND_FACTORY( KGCInviteInfo );
DECLARE_WND_FACTORY( KGCInviteInfo );
DECLARE_WND_FACTORY_NAME( KGCInviteInfo );