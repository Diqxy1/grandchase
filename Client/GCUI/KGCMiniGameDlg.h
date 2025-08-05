#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"
//#include "Procedure.h"

#include "CommonPacket.h"

#define MINIGAME_NUM    3
#define RANKER_NUM      5

class KD3DStatic;
class KD3DSizingBox;
class KGCMinigameRanker;
class KGCMinigameCategory;

class KGCMinigameDlg : public KD3DDialog,
	                   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCMinigameDlg );
	/// Default constructor
	KGCMinigameDlg( void );
	/// Default destructor
	virtual ~KGCMinigameDlg( void );
	/// Copy constructor
	KGCMinigameDlg( const KGCMinigameDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMinigameDlg& operator=( const KGCMinigameDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void SetGameCategory( int iCurrentGameIndex );
    void SetRanker( std::vector<KMiniGameRankInfo>& vecRankInfo );
    std::wstring GetRandkerName( int iRank );
    void SetNPCSpeech( void );
    void UpdateMiniGameData( int iCurrentGameIndex );
    void SetMyHighScore( int iCurrentGameIndex );
	int  GetCurrentGameMode(){return m_iCurrentGameIndex;}

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*                m_pkCloseBtn;
    KD3DWnd*                m_pkLeftBtn;
    KD3DWnd*                m_pkRightBtn;
    KD3DWnd*                m_pkStartGameBtn;
    KSafeArray<KD3DWnd*,MINIGAME_NUM> m_pkMiniGameBtn;

    KD3DStatic*             m_pkMyHighScore;
    KSafeArray<KD3DSizingBox*,MINIGAME_NUM> m_pkGameImage;

    KSafeArray<KGCMinigameRanker*,RANKER_NUM> m_pkRanker;
    KSafeArray<KGCMinigameCategory*,MINIGAME_NUM> m_pkMiniGameCategory;

    int                     m_iCurrentGameIndex;
    int                     m_iPrevGameIndex;
};

DEFINE_WND_FACTORY( KGCMinigameDlg );
DECLARE_WND_FACTORY( KGCMinigameDlg );
DECLARE_WND_FACTORY_NAME( KGCMinigameDlg );
