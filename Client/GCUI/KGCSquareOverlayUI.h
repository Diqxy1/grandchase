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


    void StartSquare(); //�������� UI�� �׷��ش�.
    //void AddChat( const std::wstring& strChat, int iColorIndex ,std::wstring strReturnMsg); //ä�� �޽��� �߰�
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

    //bool    m_bCheckMessageBoxBtns;  // �޽��� �ڽ� �� �ֳ� ����
    bool    m_bChatButtonOff;        // �� ä�� �����ؼ� Ŭ���� �ٷ� Ǯ���ָ�,
                                     // �ڿ� ê�ڽ� Ŭ��ȿ�� �ߵ��Ǽ� ������ ���꿡�� Ǯ���ֱ� ���� ���

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

		std::string	strParticleName; // ��ƼŬ �̸�
		int			iTimeInterval;	 // ��ƼŬ Ÿ�� ��
		int			iSeveralTimes;      // �� �� ����� ���ΰ�?
		bool		bIsTraceCamera;  // ī�޶� ���� ���ΰ�?
		bool		bIsCreatePosCamera;  // ī�޶� ��ġ�� ��ƮŬ�� ������ ���ΰ�?
		float		fPosX;			// ��ġ
		float		fPosY;			// ��ġ
		int			iSquereParticleTime;  // ���� �ð��� �� �Ѹ��� ���� ����.

	};




public:
	// �븸�� ���̿ö󰡱� ���� �̺�Ʈ
	bool				m_bRallyEvent;
	DWORD				m_dwRallyTime;

	// ��層��� ��ư�޴�
	KGCGuildSquareMenu*		m_pkGuildSquareMenu;

	int						m_iStartTime;
	


	std::vector<SSquareParticleInfo> m_vecSquareParticle; // ���� ��ƼŬ ���!!!! ���⿡ ��� ��ƼŬ�� ���� ���ش�.
	std::vector<CParticleEventSeqPTR> m_vecSquareParticleSequence; // ���� ��ƼŬ ���!!!! ���⿡ ��� ��ƼŬ�� ���� ���ش�.
	D3DXVECTOR3 m_vCameraLookPos;

	

	void CreateParticle( void );
	void ClearParticle();

};

DEFINE_WND_FACTORY( KGCSquareOverlayUI );
DECLARE_WND_FACTORY( KGCSquareOverlayUI );
DECLARE_WND_FACTORY_NAME( KGCSquareOverlayUI );

extern KGCSquareOverlayUI* g_pkSquareOverlayUI;

#endif // _GCSQUAREOVERLAYUI_H_
