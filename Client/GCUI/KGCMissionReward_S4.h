#ifndef _GCKGCMISSIONREWARD_S4_H_
#define _GCKGCMISSIONREWARD_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCMissionRewardBox_S4.h"

#define NUM_REWARDITEM_BOX  4
#define ANIMATION_SPEED 8
#define REWARDBOX_ORI_HEIGHT GC_SCREEN_DIV_SIZE_INT(166)

class KGCMissionReward_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionReward_S4 );
    /// Default constructor
    KGCMissionReward_S4( void );
    /// Default destructor
    virtual ~KGCMissionReward_S4( void );
    /// Copy constructor
    KGCMissionReward_S4( const KGCMissionReward_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionReward_S4& operator=( const KGCMissionReward_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd                 *m_pkBackGround;
    KD3DWnd                 *m_pkBnt_Rew_Nor;
    KD3DWnd                 *m_pkBnt_Rew_Sel;
    KD3DWnd                 *m_pkPerentBtnComplate;
    KD3DSizingBox           *m_pkBack_Rew_Nor;
    KD3DSizingBox           *m_pkBack_Rew_Sel;
    KGCMissionRewardBox_S4  *m_pkRewardBox_Nor[ NUM_REWARDITEM_BOX ];
    KGCMissionRewardBox_S4  *m_pkRewardBox_Sel[ NUM_REWARDITEM_BOX ];

private:
    DWORD                   m_dwAniEndHeight;
    DWORD                   m_dwSelRewardItem;
    DWORD                   m_dwMainMissionID;

private:
    void OnClickSelItem( KGCMissionRewardBox_S4* pWnd );

public:
    void CleanUp();
    void SetMissionID( DWORD dwMissionID )      {   m_dwMainMissionID = dwMissionID;    }
    void SetComplateInstance( KD3DWnd* pWnd )   {   m_pkPerentBtnComplate = pWnd;       }
    void SetRewardItem( KGCMissionRewardBox_S4* pItemWnd, const SMissionInfo::SReward& reawrd );
    void SetRewardItem( const std::vector< SMissionInfo::SReward >& vecNorItem, const std::vector< SMissionInfo::SReward >& vecSelItem );
    void SetFirstRewardBox();

public:
    DWORD GetSelRewardItem()                    {   return m_dwSelRewardItem;           }
    float GetBackGroundPosY()                   {   return m_pkBackGround?m_pkBackGround->GetFixedWindowLocalPos().y : 0.0f;    }

private:
    void RefreshWndPosition();
    void AnimationControll();
    void Animation( int iAniSpeed );
    void SetPosFromGapBackGroundAndControll(KD3DWnd *pControll, float fGap);
    void SetRenderState( bool bSelectBox, bool bNormalBox );
    bool SetItemInfoBox( KGCMissionRewardBox_S4 *pWnd );
    void DrawItemInfoBox( bool bRender, DWORD dwGoodsID, char cGrade, D3DXVECTOR2 vPos );
};

DEFINE_WND_FACTORY( KGCMissionReward_S4 );
DECLARE_WND_FACTORY( KGCMissionReward_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionReward_S4 );

#endif  // _GCKGCMISSIONREWARD_S4_H_