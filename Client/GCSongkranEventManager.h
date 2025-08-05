#pragma once

#include "KNCSingleton.h"

class GCSongkranEventManager
{
	DeclareSingleton( GCSongkranEventManager );

public:
	enum 
	{
		CHAR_ELESIS = 0,
		CHAR_ZERO	= 1,
		CHAR_LAS	= 2,
		CHAR_ARME	= 3,
		CHAR_REY	= 4,
		CHAR_SIZE	= 5,
	};

public:
	GCSongkranEventManager(void);
	~GCSongkranEventManager(void);

public:
	void SetInfoData( KEVENT_SONGKRAN_SCRIPT_INFO_ACK kRecv );
	void SetUserInfo( KEVENT_SONGKRAN_USER_INFO_ACK kRecv );

	void SetScore( DWORD dwScore ) { m_dwScore = dwScore; }
	void SetGrade( DWORD dwGrade ) { m_dwGrade = dwGrade; }
	void SetBombCount( DWORD dwBombCount ) { m_dwWaterBombDuration = dwBombCount; }
	
	bool IsGetInfo() { return m_bGetInfo; }
	bool IsEventOpen();
	void Reset();

public:
	float		GetPlayerSpeed() { return m_fPlayerCharMoveSpeed; }
	float		GetBombSpeed() { return m_fWaterBombFlyingSpeed; }
	float		GetBombRange() { return m_fWaterBombRange; } 

	
	DWORD		GetTargetScore( int nChar );
	float		GetTargetSpeed( int nChar );

	int			GetTeleportFrame( int nChar );
	int			GetResetFrame( int nChar );

	DWORD		GetGameScore() { return m_dwScore; }
	DWORD		GetGrade() { return m_dwGrade; }

	int			GetBombCount();
	bool		IsHaveReward();

	
	
	void		ReqRewardItem();
	void		SetReqReward( bool _bSet ) { m_bReqReward = _bSet; }
	void		SetLastSupplyNot( bool _bSupplyNot ) { m_bLastSupplyNot = _bSupplyNot; }


	DWORD		GetDailySupplyDuration() { return m_dwDailySupplyDuration; }
	DWORD		GetSupplyDuration() { return m_dwSupplyDuration; }

	
	
	GCITEMID	GetRewardItem();
	DWORD		GetEndPoint();
	float		GetRewardRate();
	KSongkranEventRewardInfo*	GetSpecialRewardItem( DWORD nSeq );

	KSongkranEventTargetInfo*	GetTargetInfo( int nChar );
	KSongkranEventRewardInfo*	GetRewardInfo( DWORD dwGrade );


	void		SetRemainTime( int nTime );
	int 		GetRemainTime();

public:
	bool m_bGetInfo;		//	정보를 가져왔나?
	bool m_bReqReward;

	int	 m_iSyncPlayTime;

public:
	float               m_fPlayerCharMoveSpeed;  // 유저 캐릭터 이동 속도
	float               m_fWaterBombFlyingSpeed; // 물폭탄 비행 속도
	float				m_fWaterBombRange;       // 물폭탄 범위
	int					m_nChargeTickTime;		 // 충천주기
	DWORD				m_dwSupplyDuration;			// 물풍선 지급 갯수
	DWORD				m_dwDailySupplyDuration;	// 최초 물풍선 지급 개수


	DWORD                                               m_dwScore;                // 점수
	DWORD												m_dwGrade;			      // 유저 현재 등급
	int													m_nAccTime;				  // 물풍선 충전까지 남은 시간	
	DWORD												m_dwWaterBombDuration;	  // 물풍선 갯수
	bool												m_bLastSupplyNot;		  // 물풍선 라스트 지급	

	std::map< USHORT, KSongkranEventTargetInfo >        m_mapTargetInfoList;     // 타겟 정보 컨테이너 first는 캐릭터 타입 enum 이용
	std::map< DWORD, KSongkranEventRewardInfo >         m_mapGradeRewardList;    // 보상 컨테이너 ( 등급, EndPoint, 보상아이템벡터 )

};
DefSingletonInline( GCSongkranEventManager );


