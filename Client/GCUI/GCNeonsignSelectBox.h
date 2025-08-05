#pragma once


//#include "UserPacket.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;
class KGCNeonsignItemDesc;
class KGCNeonsignMessage;
class KGCNeonsignItemList;

class KGCNeonsignSelectBox : public KD3DDialog
                           , public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCNeonsignSelectBox( const KGCNeonsignSelectBox& );
    const KGCNeonsignSelectBox& operator=( const KGCNeonsignSelectBox& );

public:
    DECLARE_CLASSNAME( KGCNeonsignSelectBox );
    KGCNeonsignSelectBox(void);
    virtual ~KGCNeonsignSelectBox(void);

    virtual void ActionPerformed( const KActionEvent& event );
    void SetTitleText( bool bPreview );
    void SetItem( int nItemID );
    void SetSignBoardType( KSignBoardData::SIGN_BOARD_TYPE eType );

    void OnClose();
    void OnPreviewBtn();

protected:
    virtual void OnCreate();
    virtual void OnDestroyComplete();

private:
    KD3DStatic* m_pkTitle;
    KD3DWnd*    m_pkCloseBtn;
    KD3DWnd*    m_pkPreviewBtn;
    KGCNeonsignItemDesc* m_pkItemDesc;
    KGCNeonsignMessage* m_pkMessage;
    KGCNeonsignItemList* m_pkItemList;
};

DEFINE_WND_FACTORY( KGCNeonsignSelectBox );
DECLARE_WND_FACTORY( KGCNeonsignSelectBox );
DECLARE_WND_FACTORY_NAME( KGCNeonsignSelectBox );