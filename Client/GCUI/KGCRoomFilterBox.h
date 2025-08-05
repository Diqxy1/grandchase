// 사이즈에 따라 조절하는 기능이 필요함..-ㅅ -

#ifndef _KGCROOMFILETERBOX_H_
#define _KGCROOMFILETERBOX_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCCategoryBox;
class KGCRoomFilterList;

class KGCRoomFilterBox : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRoomFilterBox );
    /// Default constructor
    KGCRoomFilterBox( void );
    /// Default destructor
    virtual ~KGCRoomFilterBox( void );
    /// Copy constructor
    KGCRoomFilterBox( const KGCRoomFilterBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomFilterBox& operator=( const KGCRoomFilterBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState();

protected:
    virtual void OnCreate();
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    KGCCategoryBox*     m_pkCurrentCategory;
    KGCRoomFilterList*  m_pkList;

    int                 m_iRenderFrame;
    int                 m_iCurrentCategoryIndex;

public:
    void ShowList();
    void Update();
};

DEFINE_WND_FACTORY( KGCRoomFilterBox );
DECLARE_WND_FACTORY( KGCRoomFilterBox );
DECLARE_WND_FACTORY_NAME( KGCRoomFilterBox );

#endif //_KGCROOMFILETERBOX_H_