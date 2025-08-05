#pragma once
//#include "../uifx/D3DWnd.h"
//#include "GCResultTower.h"

class KGCResultTower;

#define GAME_END_FRAME_COUNT  550
class KGCMatchResultUI : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCMatchResultUI );
	/// Default constructor
	KGCMatchResultUI( void );
	/// Default destructor
	virtual ~KGCMatchResultUI( void );
	/// Copy constructor
	KGCMatchResultUI( const KGCMatchResultUI& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchResultUI& operator=( const KGCMatchResultUI& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

protected:
	void ActionPerformed(const KActionEvent& event ) {};
	void FrameMoveInEnabledState();
    void PostChildDraw();
	void OnCreate();
	void OnDestroy();    

	void AllocPlayerToTower( int iTeam, int iPlayerIndex );
	void BackGroundEnable( bool bRender = true );

public:
	void ShowResultStart();
	void InitPlayerIndex();
	void InitTowers();
    void initBattlePoint();
    void SetGuildPanel( std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_ );
    void CheckGuildMark( std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_ );

protected:
	KSafeArray<KGCResultTower*,MAX_PLAYER_NUM> m_pkTower;
	KD3DWnd*		m_pkBackground;
	std::vector< std::pair<int, int> >	m_vecPlayerIndex;	//게임을 플레이한 유저들 팀과 인덱스;    
   
	unsigned int m_iGameEndCount;

	int m_iSerdinCount;
	int m_iCanabanCount;
private:
    KD3DSizingBox*  m_pkBPointBar_R;
    KD3DSizingBox*  m_pkBPointBar_L;
    KD3DSizingBox*	m_pkBPointBackground_R;
    KD3DSizingBox*	m_pkBPointBackground_L;
    KD3DWnd*		m_pkGuildBorder_R;
    KD3DWnd*		m_pkGuildBorder_L;
    KD3DWnd*		m_pkGuildmarkIcon_R;
    KD3DWnd*		m_pkGuildmarkIcon_L;    
    KD3DWnd*		m_pkBPoint_R;
    KD3DWnd*		m_pkBPoitn_L;
    KD3DWnd*		m_pkVersus;
    KD3DImageIcon*  m_pkGuildImage_R;
    KD3DImageIcon*  m_pkGuildImage_L;

    KD3DWnd*        m_pkPlusLeft;
    KD3DWnd*        m_pkMinusLeft;
    KD3DWnd*        m_pkPlusRight;
    KD3DWnd*        m_pkMinusRight;

private:
    std::vector <int> m_vecDiffPoint;
    int               m_StateMark;
    bool              m_bFirstNagative;
    bool              m_bSecondNagative;
};

DEFINE_WND_FACTORY( KGCMatchResultUI );
DECLARE_WND_FACTORY( KGCMatchResultUI );
DECLARE_WND_FACTORY_NAME( KGCMatchResultUI );
