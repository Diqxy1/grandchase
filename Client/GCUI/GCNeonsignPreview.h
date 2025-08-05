#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "KGCAnnounceMgr.h"

class KD3DStatic;

class KGCNeonsignPreview : public KD3DDialog
                         , public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCNeonsignPreview( const KGCNeonsignPreview& );
    const KGCNeonsignPreview& operator=( const KGCNeonsignPreview& );

public:
    DECLARE_CLASSNAME( KGCNeonsignPreview );
    KGCNeonsignPreview(void);
    virtual ~KGCNeonsignPreview(void);

    void SetPreviewData( const KSignBoardData& kData );
    void OnReserveClose();
    bool WaitForServer() const { return m_bWaitForServer; }
    void WaitForServer( bool bWait );
    const KSignBoardData& GetAnnounceData() const { return m_kAnnounceData; }

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void OnDestroyComplete();
    virtual void PostChildDraw();
    virtual void FrameMoveInEnabledState();

    void OnClose();
    void OnReserve();

private:
    KD3DWnd*        m_pkUpWindow;
    KD3DWnd*        m_pkDownWindow;
    KD3DStatic*     m_pkTitle;
    KD3DWnd*        m_pkReserveBtn;
    KD3DWnd*        m_pkCloseBtn;
    bool            m_bReserve;

    int             m_iItemID;
    KSignBoardData  m_kAnnounceData;
    bool            m_bWaitForServer;
};

DEFINE_WND_FACTORY( KGCNeonsignPreview );
DECLARE_WND_FACTORY( KGCNeonsignPreview );
DECLARE_WND_FACTORY_NAME( KGCNeonsignPreview );