#ifndef _GCKGCMISSIONUI_S4_H_
#define _GCKGCMISSIONUI_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../mission/GCMissionManager.h"
#include "KGCMissionMain_S4.h"
#include "KGCMissionSub_S4.h"

class KGCCharacterGetInfoDlg;

class KGCMissionUI_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionUI_S4 );
    /// Default constructor
    KGCMissionUI_S4( void );
    /// Default destructor
    virtual ~KGCMissionUI_S4( void );
    /// Copy constructor
    KGCMissionUI_S4( const KGCMissionUI_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionUI_S4& operator=( const KGCMissionUI_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnDestroy( void );
    virtual void OnDestroyComplete();
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd                 *m_pkBnt_Close;
    KGCMissionMain_S4       *m_pkMissionMainDlg; 
    KGCMissionSub_S4        *m_pkMissionSubDlg;
    KGCCharacterGetInfoDlg  *m_pkCharGetInfoDlg;

private:
    bool                    m_bShowSub;
    SMissionReward          m_sReward;
    D3DXVECTOR2             m_vecOriPos;

private:
    bool                    m_bBefoeCharDlg;
    bool                    m_bNowCharDlg;

private:
    void OnClickClsoe();
    void OnEnterCursor();

public:
    void ShowMissionComplete( const SMissionReward& sReward );
    void ShowGetCharacterUI( const SMissionReward& sReward );
    void SetGetNewCharInfo( bool bNewChar, int iCharType, int iPromotion );

public:
    bool GetPriview(){  return m_bShowSub;  }
    void RefreshUI();
    KGCMissionSub_S4* GetSubMission(){   return m_pkMissionSubDlg;   }
    KGCMissionMain_S4* GetMainMission(){   return m_pkMissionMainDlg;   }
};

DEFINE_WND_FACTORY( KGCMissionUI_S4 );
DECLARE_WND_FACTORY( KGCMissionUI_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionUI_S4 );

#endif  // _GCKGCMISSIONUI_S4_H_