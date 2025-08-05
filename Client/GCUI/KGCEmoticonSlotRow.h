#ifndef _GC_EMOTICON_SLOT_ROW_H_
#define _GC_EMOTICON_SLOT_ROW_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

#define EMOTICON_SLOT_MAX   8
#define EMOTICON_MAX_ID     7228

class KGCEmoticon;
class KGCEmoticonElement;
class KGCEmoticonSlotRow : public KD3DWnd,          // extends
                           public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEmoticonSlotRow );
    /// Default constructor
    KGCEmoticonSlotRow( void );
    /// Default destructor
    virtual ~KGCEmoticonSlotRow( void );
    /// Copy constructor
    KGCEmoticonSlotRow( const KGCEmoticonSlotRow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEmoticonSlotRow& operator=( const KGCEmoticonSlotRow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    void UpdateSlotInfo( void );
    void SetRowIndex( int index );
    bool GetIsRowClick( void ) const;
    void SetIsRowClick( bool bIsClick ) { m_bIsClick = bIsClick; }
    void TurnOffSlotEdge( int iColumn );
    void TurnOnSlotEdge( int iColumn );
    int GetColumnIndex( void ) const { return m_iClickIndex; }
    void SetClickable( bool bClickable ) { m_bClickable = bClickable; }
    KGCEmoticon* GetEmoticonData();
    // 3개 다 찍고나서 찍은 놈들!
    void InitOverColumnIndex( void ) { m_iOverClickIndex = -1; }
    int GetOverColumnIndex( void ) const { return m_iOverClickIndex; }
    KGCEmoticon* GetOverEmoticonData();
    int FindElement( int iItemID );
    void SetColumnIndex( int iIndex )
    {
        m_iClickIndex = iIndex;
    }

private:
    KSafeArray<KGCEmoticonElement*,EMOTICON_SLOT_MAX> m_pkEmoticonSlot;
    int                         m_iRowIndex;

    // row가 클릭 됐는가?
    bool                        m_bIsClick;
    // 클릭된 row의 몇번째 slot인가?
    int                         m_iClickIndex;
    // 3개 다 클릭하고 찍은 아히!
    int                         m_iOverClickIndex;
    // 더 클릭할 수 있니? (최대 3개밖에...)
    bool                        m_bClickable;
    
};


DEFINE_WND_FACTORY( KGCEmoticonSlotRow );
DECLARE_WND_FACTORY( KGCEmoticonSlotRow );
DECLARE_WND_FACTORY_NAME( KGCEmoticonSlotRow );

#endif 
