#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"
//#include "GCResultNick.h"
//#include "GCResultScore.h"
#include "GCDrawCharacter.h"
//#include "KGCRoomManager.h"

#define RT_DEFAULT_HEIGHT (170)
#define RT_HEIGHT_TERM	(15)
#define RT_LOWER_BOUND	(530)
#define RT_VIEWPORT_HEIGHT (200)
#define RT_VIEWPORT_WIDTH  (200)

class KGCResultNick;
class KGCResultScore;
class GCResultDamage;

class KGCResultTower : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCResultTower );
	/// Default constructor
	KGCResultTower( void );
	/// Default destructor
	virtual ~KGCResultTower( void );
	/// Copy constructor
	KGCResultTower( const KGCResultTower& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCResultTower& operator=( const KGCResultTower& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void ActionPerformed(const KActionEvent& event ) {};
	virtual void FrameMoveInEnabledState();
	virtual void OnCreate();
	virtual void PostDraw();
	virtual void PostChildDraw();

public:
	void Init();	//모든 값을 초기화 하고 높이를 맞춤.
	void SetTeam( int iTeam );	//세르딘인지 카나반인지 셋팅 ( enum 을 이용합니다. )
	void SetNickName( int iLevel, std::wstring strNick );	//유저의 닉네임 결정. PlayerIndex를 넣으면 알아서 셋팅 됨.
	void SetRank( int iRank );	// 순위 셋팅. 높이가 달라진다.	1위에서 6위까지 있다.
    void SetPlayerIndex( int iIndex );	//케릭터의 정보와 KILL, Death 정보를 위해서..
	void SetKill( int iKill );
	void SetDeath( int iDeath );
	void SetLevelUp( bool bLevelUp = true );	// 레벨 업 했을때 호출 해 줍니다.
	void SetSPLevelUp( int iSPLevelUp = 0 );	// 레벨 업 했을때 호출 해 줍니다.

	void SetMVP( bool bMVP = true );	//MVP일때 호출해 줍니다. 꼭 1등일때만 MVP인가???
	void SetCharacter( SUserInfo& sUserInfo );
	void SetMotionToChar( int iPlayerIndex, bool bWin );
	void SetTowerHeight( DWORD dwHeight );
	void SetWin( bool bWin );
	void SetEndHeight( int iRank );
	bool IsTeamMode();
    void RenderEmoticon( void );
    void SetDamage( int iDamage);

protected:
	KSafeArray<KD3DSizingBox*,TEAM_NUM> m_pkColorFrame;
	KGCResultNick*		m_pkNickName;
	KGCResultScore*		m_pkScore;
    GCResultDamage*     m_pkDamage;
	KD3DWnd*			m_pkWin;
	KD3DWnd*			m_pkLose;
	KD3DWnd*			m_pkLevelUp;
	KD3DWnd*			m_pkSPLevelUp;
	KD3DWnd*			m_pkLocalUser;
	KD3DWnd*			m_pkMVP;
	KSafeArray<KD3DWnd*,MAX_PLAYER_NUM> m_pkRankNum;
    KD3DWnd*            m_pkEmoticonBox;

	KGCDrawCharacter	m_kCharacter;

protected:
	int		m_iPlayerIndex;
	int		m_iTeam;
	int		m_iRank;
	int		m_iExp;
	int		m_iGp;
	int		m_iKill;
	int		m_iDeath;
	bool	m_bLevelUp;
	bool	m_bSPLevelUp;

	bool	m_bMVP;
	bool	m_bRenderExpGp;
	int		m_iAnimFrame;
	DWORD	m_dwEndHeight;
	std::wstring m_strNick;

    bool    m_bLevelUpEffectOn;
    bool    m_bSkillUpEffectOn;

    int m_iDamage;
    int m_iHealDamage;

};

DEFINE_WND_FACTORY( KGCResultTower );
DECLARE_WND_FACTORY( KGCResultTower );
DECLARE_WND_FACTORY_NAME( KGCResultTower );
