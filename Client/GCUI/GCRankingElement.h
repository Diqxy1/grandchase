#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "GCCharacterUI.h"
//#include "GCSizingThreeBox.h"
//#include "UserPacket.h"
//#include "GCGuildMark.h"
//#include "define.h"

class KGCCharacterUI;
class KGCSizingThreeBox;
class KD3DImageIcon;

class KGCRankingElement:	public KD3DWnd,
							public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRankingElement );
	/// Default constructor
	KGCRankingElement( void );
	/// Default destructor
	virtual ~KGCRankingElement( void );
	/// Copy constructor
	KGCRankingElement( const KGCRankingElement& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRankingElement& operator=( const KGCRankingElement& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	//virtual void OnDestroyComplete();
	//virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	virtual void PostChildDraw( void );
	//virtual void OnDestroyComplete( void );

	void SetGuildMark( const std::wstring& strMarkFile );

protected:
	KD3DStatic*			m_pkRank;
	KD3DStatic*			m_pkNick;
	KD3DStatic*			m_pkWin;
	KD3DStatic*			m_pkLose;
	KD3DStatic*			m_pkWinRate;
	KD3DWnd*			m_pkNoGuildMark;
	KGCCharacterUI*		m_pkChar;
	KGCSizingThreeBox*	m_pkGuage;
	KGCSizingThreeBox*	m_pkGuageFrame;
	KD3DImageIcon*		m_pkGuildMark;

	int	m_iGuageGap;

	KSafeArray<BYTE,GUILD_MARK_COLOR_NUM> m_aMark;
	//------------------------------------------------------------------------
	//	길드 랭킹을 위한 추가 데이터( 토글해야 함 ) 20091204 : 이동석
	KD3DStatic*			m_pkGuildName;	//	길드 이름
	KD3DStatic*			m_pkGPoint;		//	길드 포인트
public:
	void ToggleUseForGuild( bool bGuildUse );
	void SetRankingInfo( const KNGuildRankInfo*	ranking, const float fGPointTop = 0.0f );
	//------------------------------------------------------------------------
public:
	void ExpRankingMode();
	void WinrateRankingMode();
	void TotalRankingMode();
	void reset();

	void SetRankingInfo( const KWinRateRank* ranking );
	void SetRankingInfo( const KExpRank* ranking );
    void SetRankingInfo( const KMatchRank* ranking );
};

DEFINE_WND_FACTORY( KGCRankingElement );
DECLARE_WND_FACTORY( KGCRankingElement );
DECLARE_WND_FACTORY_NAME( KGCRankingElement );