#ifndef _GCARENABATTLEDLG_H_
#define _GCARENABATTLEDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KD3DStatic;

class KGCArenaBattleDlg : public KD3DWnd 
{
public:
    DECLARE_CLASSNAME( KGCArenaBattleDlg );
    /// Default constructor
    KGCArenaBattleDlg( void );
    /// Default destructor
    virtual ~KGCArenaBattleDlg( void );
    /// Copy constructor
    KGCArenaBattleDlg( const KGCArenaBattleDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCArenaBattleDlg& operator=( const KGCArenaBattleDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    // 변수
    KD3DWnd*            m_pkScoreBoard;
    KSafeArray<KD3DStatic*,MAX_PLAYER_NUM + 2> m_apkKillDeathStatic;   // 2는 Team을 표현해 주기 위함이다.
    UINT                m_uiStrCount;

    // 함수
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    void Init();

    void AddString(  std::wstring strPlayerName_, int iKills, int iDeath, DWORD dwColor_ );
	void AddTeamSection( std::wstring strTeam_, DWORD dwColor_, UINT uiTeam_ );
    void UpdateDlg();
};

DEFINE_WND_FACTORY( KGCArenaBattleDlg );
DECLARE_WND_FACTORY( KGCArenaBattleDlg );
DECLARE_WND_FACTORY_NAME( KGCArenaBattleDlg );
extern KGCArenaBattleDlg* g_pkArenaBattleDlg;

#endif // _GCARENABATTLEDLG_H_