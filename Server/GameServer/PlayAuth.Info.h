#if !defined (__PLAY_AUTH_INFO_H__)
#define __PLAY_AUTH_INFO_H__

#define UNIQUEID_LEN			(16)
#define SHUTDOWNNAME_LEN		(25)
#define GAMECODE_LEN			(20)
#define TICKET_LEN				(100)
#define MAX_SHUTDOWNLIST_SIZE	(3)

namespace PlayAuth
{
	// 유저별 셧다운제 정보
	struct UserShutdownInfo
	{
		// 강제적셧다운제: "ForcedShutdownSystem"
		// 선택적셧다운제: "SelectiveShutdownSystem"
		// 쿨링오프: "CoolingOffSystem"
		char		m_szShutdownName[SHUTDOWNNAME_LEN];
		// 0x000000: 체크 성공
		// 0x030202: 강제적셧다운제 타임아웃(default 0시~6시)
		// 0x030203: 쿨링오프 총사용시간 오버(default:4시간)
		// 0x030204: 쿨링오프 연속게임시간 오버(default:2시간)
		// 0x030205: 선택적셧다운제 부모동의시간 오버
		int			m_nShutdownCheckResult;	// 0: 성공, 이외의 값은 에러에대한 상태값		
	};

	struct UserCheckInInfo
	{
		char				m_szUniqueID[UNIQUEID_LEN];		// User Unique ID
		bool				m_bCheckInSuccess;				// 체크인성공: true, 체크인실패: false(어떤셧다운제에서 실패했는지는 m_listShutdownInfoList를 조회해봐야한다.)
		unsigned int		m_nShutdownInfoListSize;		// m_listShutdownInfoList 크기
		UserShutdownInfo	m_listShutdownInfoList[MAX_SHUTDOWNLIST_SIZE];		// 각각의 셧다운제(강제적,선택적,쿨링오프)에 대한 체크결과값
	};

	struct UserNotiInfo
	{
		char				m_szUniqueID[UNIQUEID_LEN];
		char				m_szGameCode[GAMECODE_LEN];
		char				m_szTicket[TICKET_LEN];				// ticket정보
		unsigned int		m_nNotiType;						// 1: CheckOut요청, 2: 잔여시간 공지
		char				m_szShutdownName[SHUTDOWNNAME_LEN];	// 셧다운제이름
		unsigned int		m_nRemainTime;						// 잔여시간
	};

	struct UserStateInfo
	{
		char				m_szUniqueID[UNIQUEID_LEN];
		unsigned int		m_nUserState;						// 0: checkout상태, 1: checkin상태
		unsigned int		m_nShutdownInfoListSize;	// m_listShutdownInfoList 크기
		UserShutdownInfo	m_listShutdownInfoList[MAX_SHUTDOWNLIST_SIZE];	// 유저적용 셧다운제정보
	};
}


#endif // !defined (__PLAY_AUTH_INFO_H__)