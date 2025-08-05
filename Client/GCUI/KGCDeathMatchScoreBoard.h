#ifndef _KGCDEATHMATCHSCOREBOARD_H_
#define _KGCDEATHMATCHSCOREBOARD_H_

//#include "../uifx/D3DWnd.h"

class KD3DStatic;
class KD3DSizingBox;
class KGCDeathMatchCharInfoBar;

#define CHAR_INFO_BAR_NUM 6

class KGCDeathMatchScoreBoard : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCDeathMatchScoreBoard );
	/// Default constructor
	KGCDeathMatchScoreBoard( void );
	/// Default destructor
	virtual ~KGCDeathMatchScoreBoard( void );
	/// Copy constructor
	KGCDeathMatchScoreBoard( const KGCDeathMatchScoreBoard& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDeathMatchScoreBoard& operator=( const KGCDeathMatchScoreBoard& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void OnCreate();
    void UpdateGameData(); //UI에 현재 상태를 적용합니다
    
    virtual void PostChildDraw( void );

    KD3DStatic* m_pkStaticServer;
    KD3DStatic* m_pkStaticChannel;
    KD3DStatic* m_pkStaticRoomName;

    KD3DStatic* m_pkStaticOpenClose;
    
    KD3DSizingBox* m_pkBackPan;

    KD3DWnd* m_pkTextList;
    KD3DWnd* m_pkTeamComponent;
    KD3DWnd* m_pkPlayerListTeam;

    KD3DWnd* m_pkScorePosHelperSerdin;
    KD3DWnd* m_pkScorePosHelperCanaban;

    KD3DWnd* m_pkDeathMatchTitle;
    KD3DWnd* m_pkFatalDeathMatchTitle;

    D3DXVECTOR2 m_vPlayerListPos;
    D3DXVECTOR2 m_vTextListPos;
    float m_fCharInfoPosX;

    KSafeArray<KGCDeathMatchCharInfoBar*,CHAR_INFO_BAR_NUM> m_apkCharInfo;

};

DEFINE_WND_FACTORY( KGCDeathMatchScoreBoard );
DECLARE_WND_FACTORY( KGCDeathMatchScoreBoard );
DECLARE_WND_FACTORY_NAME( KGCDeathMatchScoreBoard );

#endif //_KGCDEATHMATCHSCOREBOARD_H_