#ifndef _GCGAMERESULT_H_
#define _GCGAMERESULT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
#include <limits.h>
#include "ProtectedType.h"

#define CHAR_EXP_INVALID INT_MIN

struct KEndGameReq; // forward declaration

extern KProtectedType<int>     g_aiArrowScore[MAX_PLAYER_NUM][3];
extern bool IsBlueWin;
extern bool IsDetermineWin;
extern bool IsResultComplete;

extern unsigned char	GetPetLevel( DWORD dwExp );
extern float			GetPetExpPercent( DWORD dwExp );
extern unsigned char    Exp_2_Level( GCEXPTYPE biExp_ );
extern DWORD			Pet_Exp_For_Next( DWORD Exp );
extern GCEXPTYPE        Level_2_Exp( int iLevel_ );
extern float            Get_MyGetExp( int iPlayerIndex, char cCharType, bool bLimit30Lv, float fRatio_ );
extern float            Get_MyGetGP( int iPlayerIndex, bool bLimit30Lv, float fRatio_ );
extern GCEXPTYPE        Exp_For_Next( GCEXPTYPE biExp_ );
extern float            Exp_For_Percent( GCEXPTYPE biExp_ );

extern void             MakeAllGameResult( KEndGameReq& kNot );
extern bool				MakeGameResult( KGameResultIn& kgr, int iPlayer, bool bExitGame = false );
extern void             Auto_BoundCheckGPandEXP( int iPlayerIndex );
extern float            GetExpGpRatio( int iKillPlayerIndex_, int iKilledPlayerIndex_ );
extern float            GetPaneltyExpGpRatio( int iKillPlayerIndex_, int iKilledPlayerIndex_ );
extern bool             IsActiveEvent( DWORD dwID_, int iPlayerIdx_ = -1 );
extern bool             IsExistPairEventItem( int iItemID1_, int iItemID2_ );
//배율 적용 안함
//extern void			GetStartPlayerTotalExpGp( DWORD& dwOutExp, DWORD& dwOutGp );
extern void				GetPlayerExpGp( int iPlayerIdx, char cCharType, int& dwOutExp, int& dwOutGp );
extern void				GetPlayerExpGpRatio( int iPlayerIdx, float& fOutRatioExp, float& fOutRatioGp );
extern void				GetPlayerExpGpExtra( int iPlayerIdx, DWORD& dwOutExp, DWORD& dwOutGp );	//돈 먹은거 처리해줘야 함...-_-ㅋ
extern GCEXPTYPE        GetCharacterExp( std::map< char, GCEXPTYPE > mapExp, char cCharType );
extern float			GetCurrentExpPercent( GCEXPTYPE biExp_ );

extern GCEXPTYPE        GetExpFromLevel( int iLevel );
extern int              GetGPFromLevel( int iLevel );

extern bool             SetExpTable( const std::map<DWORD, GCEXPTYPE>& _mapExpTable );

void FileOutLevelUpStageCalc();

#define                 EXP_RATIO_KILL_HIGH_LEVEL           0.3f;
#define                 EXP_RATIO_KILL_SAME_LEVEL           0.2f;
#define                 EXP_RATIO_KILL_LOW_LEVEL            0.1f;
#define                 EXP_PANELTY_RATIO_KILLED_HIGH_LEVEL 0.1f
#define                 EXP_PANELTY_RATIO_KILLED_SAME_LEVEL 0.05f
#define                 EXP_PANELTY_RATIO_KILLED_LOW_LEVEL  0.02f
#define					LIMIT_HIGH_PERCENT_EXP_GP           1.5f
#define					LIMIT_LOW_PERCENT_EXP_GP            -1.0f
#define					LIMIT_PENALTY_LEVEL					10

#define                 JEWEL_OF_QUEEN      (4241)
#define                 JEWEL_OF_QUEEN2             (7393)      // 홍콩은 4241번 여왕의 보석이 수량제입니다. 기간제를 7393으로 추가합니다.
#define                 BRUNCH_OF_FLOWERS           (9745)

#define					FRIENDSHIP_RING		(10463)
#define					FRIENDSHIP_RING_ELESIS		(11647)	// 우정 기사 반지
#define					FRIENDSHIP_RING_LIR			(11648)	// 우정 궁수 반지
#define					FRIENDSHIP_RING_ARME		(11649)	// 우정 마법사 반지
#define					FRIENDSHIP_RING_LAS			(11650)	// 우정 도적 반지
#define					FRIENDSHIP_RING_RYAN		(11651)	// 우정 드루이드 반지
#define					FRIENDSHIP_RING_RONAN		(11652)	// 우정 마검사 반지
#define					FRIENDSHIP_RING_AMY			(11653)	// 우정 무희 반지

#define                 COUPLERING_VALENTINE      (28350)


extern float gcround( float x );
extern float gcround( float x, int iPow10 );

#endif // _GCGAMERESULT_H_
