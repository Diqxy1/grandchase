#pragma once

#include "KGCShortMenuCharInfo.h"
#include "KGCShortMenuCharView.h"

enum 
{
	SMB_CHARACTER_SCENE			= 0, // 캐릭터 선택
	SMB_AGIT_ENTER				= 1, // 아지트
	SMB_POST					= 2, // 우편
	SMB_SURVEY					= 3, // 설문조사
	SMB_MISSION					= 4, // 미션
	SMB_FORGE					= 5, // 제작/해제
	SMB_HERO					= 6, // 영웅던전
	SMB_AD						= 7, // 광고 모음 
	SMB_REC						= 8, // 녹화
	SMB_COUPLE					= 9, // 커플
	SMB_GACHA					= 10, //가차
	SMB_SQUARE					= 11, // 광장
	SMB_CALENDAR				= 12, // 출석부
	SMB_END						= 13, // 종료 버튼
	SMB_AGIT_BACK				= 14, // 아지트 종료
	SMB_CLOSET					= 15, // 용도 불명의 버튼
	SMB_MSN						= 16, // 메신저
	SMB_OPTION					= 17, // 옵션
	SMB_ENCHANT					= 18, // 강화
	SMB_CARD					= 19, // 카드 조합
	SMB_MYSTERY2				= 20, // 용도 불명의 버튼2
	SMB_EVENT					= 21, // 이벤트 리스트
	SMB_DEPOT					= 22, // 창고
	SMB_CHANNEL					= 23, // 채널
	SMB_VIP						= 24, // VIP
	SMB_SKILLTREE				= 25, // 스킬트리
	SMB_PARTY					= 26, // 파티
	SMB_GUILD					= 27, // 길드
	SMB_ENTER_GUILD				= 28, // 길드 입장 UI
	SMB_REP						= 29, // 리플레이
	SMB_ROYAL_VIP				= 30,
	SMB_GUIDE					= 31,
	SMB_GACHA_PONG				= 32,

	SMB_WEB					= 33, // 이벤트 리스트
	SMB_VIPSTORE					= 34, // 이벤트 리스트
};

typedef void (*pFunc)();

class KGCShortMenuDlg :
	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCShortMenuDlg );
	/// Default constructor
	KGCShortMenuDlg( void );
	/// Default destructor
	virtual ~KGCShortMenuDlg( void );
	/// Copy constructor
	KGCShortMenuDlg( const KGCShortMenuDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCShortMenuDlg& operator=( const KGCShortMenuDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& );
	virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

	virtual void OnCreate();
	virtual void OnInitialize();
	void UpdateButtonState();

private:
	void OnClose();
	void DisableCharView();
	void OnClickDepot();
	void OnClickMission();
	void OnClickPost();
	void OnClickGacha();
	void OnClickMSN();
	void OnClickCalendar();
	void OnClickMonster();
	void OnClickGuild();
	void OnClickAgit();
	void OnClickSquare();
	void OnClickCouple();
	void OnClickReplayRecord();
	void OnClickCloset();
	void OnClickOption();
	void OnClickCharSelectScene();
	void OnClickServerScene();
	void OnClickEventMiniGameBanner();
	void OnClickEnd();
#ifndef DISABLE_GACHA_PON
	void OnClickGachaPong();
#endif
	void OnClickVipStore();
	bool IsPrivateCharDisableMenu( int nMenu );
	bool IsCharViewDisable();
	void OnClickGuide();

private:

	enum
	{
		BTN_DEPOT,
		BTN_MISSION,
		BTN_MAIL,
		BTN_GACHA,
		BTN_MSN,
		BTN_CALENDAR,
		BTN_MONSTERCARD,
		BTN_EVENT,
		BTN_GUILD,
		BTN_AGIT,
		BTN_SQUARE,
		BTN_COUPLE,
		BTN_REC,
		BTN_CLOSET,
		BTN_VIPSTORE,
		BTN_GACHAPONG,
		BTN_CHANGECHARACTER,
		BTN_OPTION,
		BTN_GUIDE,
		BTN_END,
		MAX_BUTTONS
	};


	KD3DSizingBox*						m_pkBackGround1;
	KD3DSizingBox*						m_pkBackGround2;

	KD3DSizingBox*						m_pkBackGroundClose1;
	KD3DSizingBox*						m_pkBackGroundClose2;

	KD3DSizingBox*						m_pkBackGroundMenu1;
	KD3DSizingBox*						m_pkBackGroundMenu2;

	KD3DSizingBox*						m_pkBackGroundBar1;
	KD3DSizingBox*						m_pkBackGroundBar2;

	KD3DWnd*							m_pkCloseBtn;
	KD3DWnd*							m_pkCloseBtn2;

	KGCShortMenuCharInfo*				m_pkShortMenuCharInfo;
	KGCShortMenuCharView*				m_pkShortMenuCharView;

	KSafeArray<int, MAX_BUTTONS>		m_saButtonsServer;
	KSafeArray<KD3DWnd*, MAX_BUTTONS>	m_saButtons;
};

DEFINE_WND_FACTORY( KGCShortMenuDlg );
DECLARE_WND_FACTORY( KGCShortMenuDlg );
DECLARE_WND_FACTORY_NAME( KGCShortMenuDlg );