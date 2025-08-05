#ifndef _GCSQUAREOVERLAYUI_H_
#define _GCSQUAREOVERLAYUI_H_


//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "GCUI/KGCChatScrollBackground.h"
//#include "GCUI/KGCChatBoxBackground.h"
//#include "GCUI/KGCCheckButton.h"
//#include "GCUI/KGCAlertWnd.h"
//#include "GCUI/GCSquareSayBalloon.h"
//#include "GCUI/GCSquareUserList.h"
//#include "GCUI/GCSquarePickUserList.h"
//#include <UserPacket.h>
//#include "KGCShortCutBox.h"

class KGCShortCutBox;
class KGCChatScrollBackground;
class KGCChatBoxBackground;
class KGCCheckButton;
class KGCAlertWnd;
class KGCSquareUserList;
class KGCSquarePickUserList;
class KGCSayBalloonSquare;
class KGCGuildSquareMenu;

#define HEIGHT_MINUMUM (50.f * GC_SCREEN_DIV_HEIGHT)
#define HEIGHT_MAXIMUM (500.f * GC_SCREEN_DIV_HEIGHT)
#define D3DWE_DRAG_END K3DWE_USER+1

enum{
    SQUARE_FOCUS_WHEEL,
    SQUARE_FOCUS_PICKING,
};

class KGCChatBox;
class KGCSquareOverlayUI: public KD3DWnd,        // extends
                         public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCSquareOverlayUI );
    KGCSquareOverlayUI(void);
    ~KGCSquareOverlayUI(void);
    virtual void ActionPerformed( const KActionEvent& event );

    enum ECursorFocus
    {
        ECF_MAINWND,
        ECF_CHATBOX,
        ECF_UNKNOWN,
    };


    void StartSquare(); //오버레이 UI를 그려준다.
    //void AddChat( const std::wstring& strChat, int iColorIndex ,std::wstring strReturnMsg); //채팅 메시지 추가
    ECursorFocus GetCursorFocus( void ) const;
	void TimeCheck(void ) const;
    void HandleChatEvent( void );
    bool HasChatEventHandling( void ) const { return m_bHandleChatEvent; }
    void SetChatEventHandling( bool bHandleChatEvent ) { m_bHandleChatEvent = bHandleChatEvent; }
    void ClearChatEvent( void );
    void SetEditFocus( void );
    void ReLocate();
    void EndMessageBox();
    D3DXVECTOR2 GetMissionButtonPos(void) const;
//    void LockMessageBoxBtn(bool bChk);
    void ExitSquare( void );
    void ReLocateBalloon( D3DXVECTOR2 vPos , DWORD dwUID, int BalloonDistance = 50);
    void SetBalloonMsg( std::wstring strMsg ,DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor);
	void SetNPCBalloonMsg( std::wstring strMsg ,DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor );
    void DeleteBalloonMsg( DWORD dwCharUID);
    void DeleteAllSayBalloon();
    void UpdateSquareInfo();
    void ModalExitMsgBox( void );
    void AddSquareUserListOrder( const KSquareUserInfo &userInfo );
    void RemoveSquareUserListOrder( DWORD dwPeopleUID );
    void ClearSquareUserListOrder();
    void SetNameOnUserlistForWhisper( const std::wstring& strNickName );
    void DisableMenuButtons();

    KGCSayBalloonSquare* GetSayBalloon() const { return m_pkSayBalloon; }

    bool IsMouseControlAvailable( DWORD dwWhere ) const;
    void Picking( int iMouseBtn );
	KGCShortCutBox*		m_pkShortCutBox;

    virtual void OnInitialize();
	void LoadSquareParticleList( void );
protected:
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
	//virtual void OnD( void );
    
    KGCChatBox*			m_pkChatBox;
    KD3DWnd*            m_pkChangeViewBtn;
    bool                m_bHandleChatEvent;
    D3DXVECTOR2         m_vBeforePos;
    bool                m_bDragWindowCheck;
    bool                m_bCheckChatTypeBtns;
    DWORD               m_dwBeforeScrollHeight;
    DWORD               m_dwBeforeListHeight;
    DWORD               m_dwBeforScrollBackground;
    DWORD               m_dwBeforChatBoxBackground;
    KGCChatScrollBackground* m_pkChatScrollBackground;
    KGCChatBoxBackground*   m_pkChatBoxBackGround;

    KGCAlertWnd*        m_pkMsnButton;
    KGCAlertWnd*        m_pkAttendanceCalendar;
	KGCAlertWnd*        m_pkMissionButton;
	KGCAlertWnd*        m_pkCoupleButton;
    KD3DWnd*            m_pkOptionButton;
    KD3DWnd*            m_pkExitButton;
    KD3DWnd*            m_pkPartyButton;
    KD3DWnd*            m_pkBalloonTalkBtn1;
    KD3DWnd*            m_pkBalloonTalkBtn2;
    KD3DWnd*            m_pkButtonBackground;

    KGCSayBalloonSquare*      m_pkSayBalloon;

    //bool    m_bCheckMessageBoxBtns;  // 메시지 박스 떠 있나 여부
    bool    m_bChatButtonOff;        // 각 채팅 선택해서 클릭쉬 바로 풀어주면,
                                     // 뒤에 챗박스 클릭효과 발동되서 프레임 무브에서 풀어주기 위해 사용

    KD3DStatic*         m_pkSquareInfo;
    KD3DWnd*            m_pkSquareInfoBackCenter;
    KD3DWnd*            m_pkSquareInfoBackTopRight;
    KD3DWnd*            m_pkSquareInfoBackRight;
    KD3DWnd*            m_pkSquareInfoBackBottomRight;  

    KGCSquareUserList*  m_pkSquareUserList;
    KD3DWnd*            m_pkSquareUserListButton;
    bool                m_bUserListRender;

    KGCSquarePickUserList*  m_pkSquarePickUserList;

    KD3DWnd*            m_pkStarBtn;

    KD3DWnd*            m_pkCooperationEventBtn;

	KD3DWnd*            m_pkGuildBtn;
	KGCBillBoardChatBoard*	m_pkBillBoardChatBoard; // ???

	struct SSquareParticleInfo
	{
		SSquareParticleInfo(void)
		{
			strParticleName = "";
			iTimeInterval =0;
			iSeveralTimes =0;
			bIsTraceCamera = false;
			fPosX = 0.0f;
			fPosY = 0.0f;
			iSquereParticleTime = 0;
			bIsCreatePosCamera= false;
		}
		SSquareParticleInfo(const SSquareParticleInfo &arg_)
		{
			strParticleName = arg_.strParticleName;
			iTimeInterval = arg_.iTimeInterval;
			iSeveralTimes = arg_.iSeveralTimes;
			bIsTraceCamera = arg_.bIsTraceCamera;
			fPosX = arg_.fPosX;
			fPosY = arg_.fPosY;
			iSquereParticleTime = arg_.iSquereParticleTime;
			bIsCreatePosCamera = arg_.bIsCreatePosCamera;
		}
		const SSquareParticleInfo& operator=(const SSquareParticleInfo& arg_ )
		{	
			strParticleName = arg_.strParticleName;
			iTimeInterval = arg_.iTimeInterval;
			iSeveralTimes = arg_.iSeveralTimes;
			bIsTraceCamera = arg_.bIsTraceCamera;
			fPosX = arg_.fPosX;
			fPosY = arg_.fPosY;
			iSquereParticleTime = arg_.iSquereParticleTime;
			bIsCreatePosCamera = arg_.bIsCreatePosCamera;
			return *this; 
		}		

		std::string	strParticleName; // 파티클 이름
		int			iTimeInterval;	 // 파티클 타임 갭
		int			iSeveralTimes;      // 몇 번 사용할 것인가?
		bool		bIsTraceCamera;  // 카메라를 따라갈 것인가?
		bool		bIsCreatePosCamera;  // 카메라 위치에 파트클을 생성할 것인가?
		float		fPosX;			// 위치
		float		fPosY;			// 위치
		int			iSquereParticleTime;  // 같은 시간엔 안 뿌리게 사용될 변수.

	};




public:
	// 대만용 높이올라가기 렐리 이벤트
	bool				m_bRallyEvent;
	DWORD				m_dwRallyTime;

	// 길드광장용 버튼메뉴
	KGCGuildSquareMenu*		m_pkGuildSquareMenu;

	int						m_iStartTime;
	


	std::vector<SSquareParticleInfo> m_vecSquareParticle; // 광장 파티클 목록!!!! 여기에 모든 파티클을 생성 해준다.
	std::vector<CParticleEventSeqPTR> m_vecSquareParticleSequence; // 광장 파티클 목록!!!! 여기에 모든 파티클을 생성 해준다.
	D3DXVECTOR3 m_vCameraLookPos;

	

	void CreateParticle( void );
	void ClearParticle();

};

DEFINE_WND_FACTORY( KGCSquareOverlayUI );
DECLARE_WND_FACTORY( KGCSquareOverlayUI );
DECLARE_WND_FACTORY_NAME( KGCSquareOverlayUI );

extern KGCSquareOverlayUI* g_pkSquareOverlayUI;

#endif // _GCSQUAREOVERLAYUI_H_
