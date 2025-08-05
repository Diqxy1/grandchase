#ifndef _KGCGuildMatch_CapTain_H_
#define _KGCGuildMatch_CapTain_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"

class KGCGuildMatch_CapTain : public KGCGameModeInterface
{
public:
	KGCGuildMatch_CapTain(void);
	virtual ~KGCGuildMatch_CapTain(void);

public:
	virtual HRESULT InitAtGame();
	virtual HRESULT InitAtLoading(){ return S_OK; }
	virtual HRESULT CustomFrameMove( float fElapsedTime );
	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();

	void EqualizeStat( bool blue, bool red, bool IsMigration = false );
	void SetCaptain( char cBlueTeam, char cRedTeam );
	void SendGameEnd( bool _IsBlueWin );
	int IsCaptainIndex( int _playerIndex );
	void processDebuff( void );
	void RenderCaptainSign( void );

	KSafeArray<int,TEAM_NUM> m_iCapTainIndex;
	int         iWinningTeam;
	std::vector<int> teamSerdin;
	std::vector<int> teamCanaban;
};

#endif // _KGCGuildMatch_CapTain_H_
