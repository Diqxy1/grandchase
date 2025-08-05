#pragma once

//#include "UserPacket.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <vector>

class KD3DScrollbar;
class KD3DSizingBox;
class KD3DStatic;

#define NUM_NEONSIGN_ITEM (6)

class KGCNeonsignItemList : public KD3DWnd
                         , public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCNeonsignItemList( const KGCNeonsignItemList& );
    const KGCNeonsignItemList& operator=( const KGCNeonsignItemList& );

public:
    DECLARE_CLASSNAME( KGCNeonsignItemList );
    KGCNeonsignItemList(void);
    virtual ~KGCNeonsignItemList(void);

    int GetItemID() const { return m_iSelectItemID; }
    void SetSignBoardType( KSignBoardData::SIGN_BOARD_TYPE eType )
    {
        m_eType = eType;
    }

protected:
    virtual void FrameMoveInEnabledState();
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void PostChildDraw();

    void OnScrollPos( int iPos );
    void RefreshNeonsignItem();

private:
    KSafeArray<KD3DSizingBox*,NUM_NEONSIGN_ITEM> m_pkNeonsignItem;
    KD3DSizingBox* m_pkSelect;
    KSafeArray<int,NUM_NEONSIGN_ITEM> m_aiItemID;
    KSafeArray<int,NUM_NEONSIGN_ITEM> m_aiItemDuration;
    KD3DScrollbar* m_pkScroll;
    int m_iSelectItemID;
    int m_iScrollPos;
    std::vector< int > m_vecItemID;
    KSafeArray<GCDeviceTexture*,NUM_NEONSIGN_ITEM> m_pkItemTex;
    KSignBoardData::SIGN_BOARD_TYPE m_eType;
    KSafeArray<KD3DStatic*,NUM_NEONSIGN_ITEM> m_pkNeonsignNum;
};

DEFINE_WND_FACTORY( KGCNeonsignItemList );
DECLARE_WND_FACTORY( KGCNeonsignItemList );
DECLARE_WND_FACTORY_NAME( KGCNeonsignItemList );
