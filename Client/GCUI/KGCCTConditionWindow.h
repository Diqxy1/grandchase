#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DScrollbar;
class KD3DStatic;
class KGCSubMissionBar;
class KD3DSizingBox;

class KGCCTConditionWindow : public KD3DDialog,          // extends
                        public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCTConditionWindow );
    /// Default constructor
    KGCCTConditionWindow( void );
    /// Default destructor
    virtual ~KGCCTConditionWindow( void );
    /// Copy constructor
    KGCCTConditionWindow( const KGCCTConditionWindow& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCTConditionWindow& operator=( const KGCCTConditionWindow& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum { NUM_OF_LIST = 6 };

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );

    void SetMission( int iMissionID );
    int GetFirstConditionIndex(void);
    int GetSlotIndexByIndex(int iIndex);

    void RefreshAll(void);

private:
    KSafeArray<KGCSubMissionBar*,NUM_OF_LIST> m_pkConditionList;
    KD3DStatic*             m_pkTitle;
    KD3DWnd*                m_pkOk;
    KD3DWnd*                m_pkClose;
    KD3DScrollbar*          m_pkScrollBar;
    KD3DSizingBox*          m_pkBack;
	int						m_iMissionID;
	int						m_iTopIndex;

	void SetSubMissionList(int iMissionID);
};

DEFINE_WND_FACTORY( KGCCTConditionWindow );
DECLARE_WND_FACTORY( KGCCTConditionWindow );
DECLARE_WND_FACTORY_NAME( KGCCTConditionWindow );
