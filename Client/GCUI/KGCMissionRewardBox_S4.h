#ifndef _GCKGCMISSIONREWARDBOX_S4_H_
#define _GCKGCMISSIONREWARDBOX_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMissionRewardBox_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionRewardBox_S4 );
    /// Default constructor
    KGCMissionRewardBox_S4( void );
    /// Default destructor
    virtual ~KGCMissionRewardBox_S4( void );
    /// Copy constructor
    KGCMissionRewardBox_S4( const KGCMissionRewardBox_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionRewardBox_S4& operator=( const KGCMissionRewardBox_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd         *m_pkBack;
    KD3DStatic      *m_pkPeriod;
    KGCItemImgWnd   *m_pkImg;
    KD3DSizingBox   *m_pkGradeFrame[ (KItem::GRADE_NUM-1) ];

    char             m_cGrade;

public:
    void ClearItem();
    void SetItem( const SMissionInfo::SReward& reawrd );
    void SetBackWndMode( KD3DWnd::ED3DWndMode eMode );

public:
    DWORD GetSettingItemID();
    char GetGrade() { return m_cGrade; }
};

DEFINE_WND_FACTORY( KGCMissionRewardBox_S4 );
DECLARE_WND_FACTORY( KGCMissionRewardBox_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionRewardBox_S4 );

#endif  // _GCKGCMISSIONREWARDBOX_S4_H_