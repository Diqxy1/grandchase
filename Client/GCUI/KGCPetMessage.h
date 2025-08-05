#ifndef _KGCPETMESSAGE_H_
#define _KGCPETMESSAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
#include "GCUI/GCScene.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DSizingBox.h"
//#include "../uifx/ActionListener.h"

struct SMessageInfo
{
	std::wstring	strMsg;
	int				iShowTime;
	int				iBoxHeight;
	int				iBoxWidth;
	D3DXVECTOR2		vSayPos;
};

enum SPetMentState
{
	PET_MENT_INVALID = -1,
	PET_MENT_HUNGRY = 1,
    PET_MENT_SHOP = 2,
	PET_MENT_ROOM = 3,
	PET_MENT_MYINFO = 4,
	PET_MENT_FEEDING = 5,
	PET_MENT_HUNGRY_ZERO = 6,
	PET_MENT_LEVELUP = 7,
	PET_MENT_CHAR_LEVELUP = 8,
	PET_MENT_FEEDING_3000 = 9,
	PET_MENT_FEEDING_6000 = 10,
	PET_MENT_FIRST_MEETING = 11,
    PET_MENT_PLEASE_HATCH = 12,
};

class KGCPetMessage :	public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCPetMessage );

	virtual void OnCreate( void );		
	virtual void PostChildDraw( void );
	virtual void FrameMoveInEnabledState();

	void LoadPetMent();

	void MsgFrameMove( bool bMsgSizing = true );

	void PetMessaging( std::wstring strText_, int iShowTime = 300, bool bNow_ = false);	//메시지를 메시지 큐에 넣습니다.
	void SetMsgPos( int ix_, int iy_ );	//메시지를 출력할 위치를 설정합니다. 픽셀단위입니다. ㅋㅋㅋ!!!
	void ConnectWithPetWnd( KD3DWnd* pkWnd_ )	{ m_pkWhoCallMe = pkWnd_; }
	void SetPetID( int iPetID_, int iEvolutionLevel_ );
	void PutStateMessage( int iState_ );

	void SetMentState(int iState_ ) { m_iPetMentState = iState_; }

	KD3DSizingBox*	m_pkPetMsgBox;
	KD3DStatic*		m_pkPetMsg;
	KD3DWnd*		m_pkSay;

	KGCPetMessage(void);
	~KGCPetMessage(void);

protected:
	// 펫 메시지의 기본적인 위치를 저장해 둡니다.
	int m_iPosX;
	int m_iPosY;

	//펫에 따라 다른 자동 메시지를 출력해야 할 수 있으므로 PetID를 저장해 둡니다.
	//앞으로 메시지는 펫의 스크립트로 부터 읽어들입니다.
	int m_iPetID;
    int m_iEvolutionLevel;

	KD3DWnd* m_pkWhoCallMe;	//현재 펫을 그리고 있는 UIWnd를 저장해 두고 이 UIWnd가 Render Off되면 같이 종료 처리 해 줍니다.

	//펫 메시지를 얼마나 지속시킬지, 배고픔이나 각종 자동메시지들의 출력시간을 셋팅해 둡니다.
	int m_iSystemPetMsgTime;
	int m_iPetMsgShowTime;	//현재 펫의 메시지가 지속되는 시간입니다.
	int m_iPetMsgShowTimeCount;	//현재 펫의 메시지의 시간을 Count 해서 시간이 지나면 메시지를 가려줍니다.

	int m_iPetMentState;	//펫의 멘트를 날릴 상태입니다.

	std::vector<SMessageInfo> m_vPetMsg;

	std::map<int, std::pair<int, std::vector<int> > > m_vPetAutoMsg;	//게임내에서 자동적으로 출력하도록 되어 있는 메시지들입니다. pair의 first는 멘트를 날리는 딜레이 시간입니다. ex) 20초마다 1번

    int m_iAlpha;
};

DEFINE_WND_FACTORY( KGCPetMessage );
DECLARE_WND_FACTORY( KGCPetMessage );
DECLARE_WND_FACTORY_NAME( KGCPetMessage );

#endif //_KGCPETMESSAGE_H_