#pragma once

//#include "../uifx/D3DWnd.h"

class KD3DStatic;
class GCDeviceTexture;
class KD3DSizingBox;

class KGCNeonsignItemDesc : public KD3DWnd
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCNeonsignItemDesc( const KGCNeonsignItemDesc& );
    const KGCNeonsignItemDesc& operator=( const KGCNeonsignItemDesc& );

public:
    DECLARE_CLASSNAME( KGCNeonsignItemDesc );
    KGCNeonsignItemDesc(void);
    virtual ~KGCNeonsignItemDesc(void);

    void SetItem( int nItemID );

protected:
    virtual void PostChildDraw();
    virtual void OnCreate();

private:
    KD3DStatic* m_pkHelp;
    int         m_iItemID;
    KD3DStatic* m_pkItemDesc;
    GCDeviceTexture* m_pItemTexture;
    KD3DSizingBox* m_pkItemBack;
};

DEFINE_WND_FACTORY( KGCNeonsignItemDesc );
DECLARE_WND_FACTORY( KGCNeonsignItemDesc );
DECLARE_WND_FACTORY_NAME( KGCNeonsignItemDesc );