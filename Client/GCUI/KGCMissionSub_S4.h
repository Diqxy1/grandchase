#ifndef _GCKGCMISSIONSUB_S4_H_
#define _GCKGCMISSIONSUB_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCMissionSubInfoBox_S4.h"
#include "KGCMissionReward_S4.h"

class KGCMissionSub_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionSub_S4 );
    /// Default constructor
    KGCMissionSub_S4( void );
    /// Default destructor
    virtual ~KGCMissionSub_S4( void );
    /// Copy constructor
    KGCMissionSub_S4( const KGCMissionSub_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionSub_S4& operator=( const KGCMissionSub_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );

private:
    enum
    {
        BTN_PRIVIEW = 1,
        BTN_COMPLATE = 2,
        BTN_DELETE = 4,
    };
    enum
    {
        TAB_ALL,
        TAB_NORMAL,
        TAB_CHARACTER,
        TAB_EVENT
    };
    enum
    {
        EFFECT_NONE,
        EFFECT_COMPLETE,
        EFFECT_REWARD,
    };

private:
    KD3DWnd                 *m_pkBtn_Preview;
    KD3DWnd                 *m_pkBtn_Complate;
    KD3DWnd                 *m_pkBtn_Delete;
    KGCMissionSubInfoBox_S4 *m_pkSubInfoBox;
    KGCMissionReward_S4     *m_pkSubRewardInfo;

private:
    int                     m_iCompleteEffect;
    int                     m_iEffectCnt;
    DWORD                   m_dwMainMissionID;
    DWORD                   m_dwBtnRenderState;
    D3DXVECTOR2             m_vecOriPos;
    std::vector<GCITEMID>   m_vecPreviewEquip;

    bool                    m_bCompleteSubEff;  // 미션 완료시 지정 이펙트 

public:
    void OnEnterCursor();
    void OnClickPreview();
    void OnClickComplate();
    void OnClickDelete();
    void OnClickSelItem( KGCMissionRewardBox_S4* pWnd );

private:
    bool CheckEquipPriview(const std::vector< SMissionInfo::SReward > &vecNormal, const std::vector< SMissionInfo::SReward > &vecSelect);
    void SetDeleteBtnPosY( float fPos );

public:
    void CleanUpSubMission();
    void RefreshSubMission( DWORD dwMissionUID );
    void SetCompleteEffect( int iSwitch )           {   m_iCompleteEffect = iSwitch;    }
    KGCMissionSubInfoBox_S4* GetSubInfoInstance()   {  return m_pkSubInfoBox;           }
};

DEFINE_WND_FACTORY( KGCMissionSub_S4 );
DECLARE_WND_FACTORY( KGCMissionSub_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionSub_S4 );

#endif  // _GCKGCMISSIONSUB_S4_H_