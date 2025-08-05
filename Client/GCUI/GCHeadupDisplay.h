#ifndef _GCHEADUPDISPLAY_H_
#define _GCHEADUPDISPLAY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../define.h"
//#include "../uifx/D3DWnd.h"
//#include "GCMyHeadup.h"

class KGCMyHeadup;
class KGCPlayerStatusBar;
class GCOnGameExpBar;
class GCOnGameScoreBar;
class KGCPingAntenna;
class KGCPlayerRank;
class KGCComboUI_S6;

class KGCHeadupDisplay : public KD3DWnd // extends
{
public:
    DECLARE_CLASSNAME( KGCHeadupDisplay );
    /// Default constructor
    KGCHeadupDisplay( void );
    /// Default destructor
    virtual ~KGCHeadupDisplay( void );
    /// Copy constructor
    KGCHeadupDisplay( const KGCHeadupDisplay& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeadupDisplay& operator=( const KGCHeadupDisplay& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void ShowHUD( bool bShow, bool bShowPlayerStatus );
    int  GetPlayerIndex(int slot);          //keyboard 1~6에 대응하는 위치에 있는 PlayerID를 돌려줌
	void OnInitialize();

    void SetRoomMemberPingInfo(std::vector<std::pair<DWORD,DWORD>>& vecRoomMemberPingInfo_ );

protected:
    enum
    {
        MAX_LEN_HP_GAUGE    = 216,
        MAX_LEN_MP_GAUGE    = 215,
        NUM_STATUS_BAR      = 6,
    };

protected:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
 
public:
    void SetPlayerStatusBar( void );    
    void SetPlayerRank( float fRankPoint );

protected:
	bool		    m_bShowPlayerStatus;
    bool            m_bShow;
    int             m_iPlayerIdx;

    // vs
    KD3DWnd*        m_pkVS;
    
    KGCPingAntenna*  m_pkMyPingAntenna;

public:
	KGCMyHeadup*    m_pMyHeadup;
    GCOnGameExpBar* m_pkExpBar;

    KGCPlayerRank*  m_pkPlayerRank;
    KGCComboUI_S6* m_pkComboUIS6;

    // player status bar
    KSafeArray<KGCPlayerStatusBar*,NUM_STATUS_BAR> m_apkPlayerStatusBar;
};

DEFINE_WND_FACTORY( KGCHeadupDisplay );
DECLARE_WND_FACTORY( KGCHeadupDisplay );
DECLARE_WND_FACTORY_NAME( KGCHeadupDisplay );

extern KGCHeadupDisplay* g_pkHUD;

#endif // _GCHEADUPDISPLAY_H_