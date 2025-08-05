#pragma once

//#include "../uifx/D3DWnd.h"

class KGCBarControl :   public KD3DWnd
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCBarControl( const KGCBarControl& );
    const KGCBarControl& operator=( const KGCBarControl& );

public:
    DECLARE_CLASSNAME( KGCBarControl );
    KGCBarControl(void);
    virtual ~KGCBarControl(void);

    void SetGage( float fPercent );

protected:
    virtual void OnCreate();
    virtual void OnCreateComplete();

private:
    KD3DWnd* m_pkLeft;
    KD3DWnd* m_pkCenter;
    KD3DWnd* m_pkRight;
    KD3DWnd* m_pkGage;
    KD3DWnd* m_pkGageEnd;
    DWORD m_dwGageWidth;
};

DEFINE_WND_FACTORY( KGCBarControl );
DECLARE_WND_FACTORY( KGCBarControl );
DECLARE_WND_FACTORY_NAME( KGCBarControl );