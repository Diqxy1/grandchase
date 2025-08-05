#pragma once

#include "GCAttendanceStampSlot.h"

const int SLOT_PER_ROW = 5;
const int MAX_STAMP_SLOT = 30;
const int SLOT_GAP = -4;

class KGCAttendanceStampSlot;
class KGCStampTitle;

class KGCAttendanceStampDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAttendanceStampDlg );
    /// Default constructor
    KGCAttendanceStampDlg( void );
    /// Default destructor
    virtual ~KGCAttendanceStampDlg( void );
    /// Copy constructor
    KGCAttendanceStampDlg( const KGCAttendanceStampDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAttendanceStampDlg& operator=( const KGCAttendanceStampDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnClose;
    KGCAttendanceStampSlot*     m_pkSlot;       // 슬롯이 위치하는 좌표값 구하는 용도
    KD3DSizingBox*              m_pkBackOuter;  // 백그라운드 사이징 박스
    KD3DSizingBox*              m_pkBackInner;  // 백그라운드 사이징 박스

    KSafeArray<KGCAttendanceStampSlot*, MAX_STAMP_SLOT> m_apStampSlot;

    DWORD m_dwOriginalOuterHeight;
    DWORD m_dwOriginalInnerHeight;
    DWORD m_dwSlotHeight;

    KGCStampTitle* m_StampTitle;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();
    void UpdateSlotInfo();
    void UIResizing( IN int nRow );
    void TitleUI( int iUiType );
    void UpdateStampTitle(void);
};

DEFINE_WND_FACTORY( KGCAttendanceStampDlg );
DECLARE_WND_FACTORY( KGCAttendanceStampDlg );
DECLARE_WND_FACTORY_NAME( KGCAttendanceStampDlg );
