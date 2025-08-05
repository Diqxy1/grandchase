#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DSizingBox;
class KD3DStatic;
class KGCInviteSquareInfo;

#define MAX_INVITE_SQUARE_NUM 8

class KGCInviteSquareList:  public KD3DWnd,
                            public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteSquareList( const KGCInviteSquareList& );
    const KGCInviteSquareList& operator=( const KGCInviteSquareList& );

public:
    DECLARE_CLASSNAME( KGCInviteSquareList );
    KGCInviteSquareList(void);
    virtual ~KGCInviteSquareList(void);

    void SetTitle( const std::wstring& strTitle );
    void SetSquareList();
    void InitSquareList();

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete();
    void OnSquareInfo( KD3DWnd* pWnd, DWORD dwCode );

private:
    KD3DSizingBox*  m_pkBack;
    KD3DStatic*     m_pkTitle;
    KSafeArray<KGCInviteSquareInfo*,MAX_INVITE_SQUARE_NUM> m_pkSquareInfo;
};

DEFINE_WND_FACTORY( KGCInviteSquareList );
DECLARE_WND_FACTORY( KGCInviteSquareList );
DECLARE_WND_FACTORY_NAME( KGCInviteSquareList );