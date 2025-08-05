// 현재는 아이콘이 전체모드, 대전, 태그매치 세개만 고려해서 만들어져있다..
// 모드가 추가된다거나.. 유동적으로 바뀌어야 한다면..
// 자동으로 사이즈 변하고 이런걸 구현해 줘야함..
// 추가로 글자 길이에 따라서 가로 길이 설정하는것도 해주면 해외에서 스트링이 길어져도 문제없겠지..

#ifndef _KGCROOMFILETERLIST_H_
#define _KGCROOMFILETERLIST_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "KGCCategoryBox.h"

class KD3DStatic;
class KD3DSizingBox;

class KGCRoomFilterList : public KD3DWnd,
                          public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRoomFilterList );
    /// Default constructor
    KGCRoomFilterList( void );
    /// Default destructor
    virtual ~KGCRoomFilterList( void );
    /// Copy constructor
    KGCRoomFilterList( const KGCRoomFilterList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomFilterList& operator=( const KGCRoomFilterList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState();

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    KD3DSizingBox*      m_pkSizingBox;

    KSafeArray<KD3DWnd*, KGCRoomListManager::NUM_CATEGORY > m_apkIcon;
    KSafeArray<KD3DStatic*, KGCRoomListManager::NUM_CATEGORY > m_apkName;
    KSafeArray<bool, KGCRoomListManager::NUM_CATEGORY > m_abEnabled;

    int                 m_iSelectCategoryIndex;
    bool                m_bInputCheck;

public:
    int GetSelectCategoryIndex(){ return m_iSelectCategoryIndex; }
    void InitTextColor();
    void SetChildInputCheck( bool bCheck ){ m_bInputCheck = bCheck; }
    void Update();
    void SetChildFontAlpha( BYTE byteFontAlpha );
    bool GetCheckClick(){ return m_bClick; }
    void SetCheckClick(bool bCheck){ m_bClick = bCheck; }
    
    BYTE                m_byteFontAlpha;
    bool                m_bClick;
};

DEFINE_WND_FACTORY( KGCRoomFilterList );
DECLARE_WND_FACTORY( KGCRoomFilterList );
DECLARE_WND_FACTORY_NAME( KGCRoomFilterList );

#endif //_KGCROOMFILETERLIST_H_
