#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NATION_KOREA
#define EXTENSION_CONTOLS
#endif

// for "ShortCutSlot" Key Setting
const CHAR		KEY_UP		= -1;

#include "ProtectedType.h"
//#include "GCDeviceLib/GCDeviceTexture.h"

class GCDeviceTexture;

class Controls
{
public:

private:
    enum
    {
        NUM_MOUSE_TEXTURE = 3,
    };

    KSafeArray<GCDeviceTexture*,NUM_MOUSE_TEXTURE> m_aptexMouse;

public:

    BYTE			m_keyUp;            // 윗 방향 키
    BYTE			m_keyDown;          // 아래 방향 키
    BYTE			m_keyLeft;          // 왼쪽 방향 키
    BYTE			m_keyRight;         // 오른쪽 방향 키
    BYTE			m_keyAttack;        // 액션 키
    BYTE			m_keyUseItem;       // 아이템 사용 키
    BYTE			m_keyChangeItem;    // 아이템 전환 키
    BYTE			m_keyProvocation;   // 도발 키
//	BYTE			m_keyRebirth;		// 수반사용 키
    BYTE			m_keyExit;          // 종료 키
	BYTE			m_keyChangeSkill;	// 기술 전환 키
	BYTE			m_keyUsePetSkill;	// 펫 스킬 사용 키
    BYTE			m_keySpecialSkillKey;  // 스킬트리에서 쓰는 키..
    BYTE			m_keyWeaponSwap;    // 무기체인지 키
    BYTE            m_keyGameInventory; // 게임내 장비창 
    BYTE            m_keyCameraZoom;    // 게임내 카메라 줌 키
#if !defined(__PATH__)
    BYTE            m_keyMonsterDirection;  // 몬스터 액션에 Lock이 걸렸을때 바라보는 방향을 바꿔주는 키
#endif

	// 단축 슬롯 사용 ㅋㅣ		crey21.
	BYTE			m_keyShowShortCut;		// 단축슬롯 창 보이기||감추기
	
	CHAR			k_ShortCut_Item;		// 아이템 단축 슬롯이 눌렸음
	CHAR			k_ShortCut_Emot;		// 이모티콘 단축 슬롯이 눌렸음
    CHAR			k_ShortCut_MacroCommunity;		// 매크로 커뮤니티 단축 슬롯이 눌렸음
	CHAR			k_FreshSkillNum;
    CHAR            k_MonsterAction;        // 몬스터 액셔 리모콘 
    CHAR			k_Game_Item;		    // 게임아이템 사용키( 1, 2, 3 )

	std::vector< BYTE > m_vecKeyShortCutEmot;
	std::vector< BYTE > m_vecKeyShortCutItem;
    std::vector< BYTE > m_vecKeySkill;
    std::vector< BYTE > m_vecKeyCnt;
    std::vector< BYTE > m_vecKeyMonsterAction;
    std::vector< bool > m_vecAutoMPSpecial;

	// 게임패드 키 매핑하기 30개로 늘어남
	// 0 - DOWN
	// 1 - UP
	// 2 - RIGHT
	// 3 - LEFT
	// 4 - BUTTON1
	// 5 - BUTTON2
	// 6 - BUTTON3
	// 7 - BUTTON4
#if defined(USE_JOYSTICK)
    KSafeArray<BYTE,17> m_aiGamePad;
    KSafeArray<BYTE,17> m_aiSettingGamePad;
    KSafeArray<BYTE,17> m_aiKeyGamePad;
#endif

    bool            k_Up;
    bool            k_Down;
	bool            k_Left;
	bool            k_Right;
	
	bool            k_Fresh_Up;
    bool            k_Fresh_Down;
    bool            k_Fresh_Left;
    bool            k_Fresh_Right;
    bool            k_Fresh_Keep_Left;
    bool            k_Fresh_Keep_Right;

    bool            k_Fresh_Punch;
    bool            k_Fresh_Punch_Curse;
    bool            k_Fresh_Punch_Charging;
    bool            k_Fresh_Fire;
	// 진 - 버닝게이지
	bool            k_Push_Burnning;
	unsigned char   Burnning_X_Count;
	unsigned char   Skill_Key_Count;

    bool            k_Use_Item;
    bool            k_Select_Item;
	bool			k_Change_SkillType;
	bool			k_Use_PetSkill;

    bool            k_Fresh_Exit;
    bool            k_Fresh_Joke;
    bool            k_Push_X_Charging;
    bool            k_Push_Up_Charging;
    bool            k_DotaShop;

    float			m_x;
    float           m_y;
    float           Drag_m_x;
    float           Drag_m_y;
    KProtectedType< float >     Energy;

    unsigned char   Left_Dash_Count;
    unsigned char   Right_Dash_Count;
	unsigned char   Up_Dash_Count;
	unsigned char   Down_Dash_Count;
	unsigned char   Double_Z_Count;
	unsigned char   Double_X_Count;
    unsigned char   Double_C_Count;
	unsigned char   X_Count;
    unsigned char   Mouse_Power;
	
	bool            k_Fresh_Right_Dash;
	bool            k_Fresh_Left_Dash;
	bool			k_Fresh_Down_Dash;
	bool			k_Fresh_Up_Dash;
	bool			k_Fresh_Double_X;
	bool			k_Fresh_Double_Z;
    bool			k_Fresh_Double_C;

    bool            k_Fresh_Left_Right_Sync;

    bool            k_Fresh_Left_Right;
    bool            k_Fresh_Right_Left;

	bool			k_Fresh_Skill_Key;
	bool			k_Skill_Key;
	CHAR			k_SkillNum;
	bool			k_ShowShortCut;
	D3DXVECTOR3		m_vMousePos;

    bool            m_bSleep; // 자냐?
    DWORD           m_dwSleepFrame;
    
    bool            k_Game_Inventory;
    bool            k_keyCameraZoom;
    bool            k_keyAIConnection;
    bool            k_keyEmbarkation;

	int				m_iCheckForConfusion2;

    enum
    {
        GUARD_RIGHT,
        GUARD_LEFT,
        GUARD_TOTAL,
        BLOCK_ENABLE_FRAME = LATENCY + 2,
    };

    KSafeArray<KSafeArray<bool,BLOCK_ENABLE_FRAME>,GUARD_TOTAL> m_bUserInputGuard;
public:
    Controls( void );

    void            Into_Packet( void );
    void            InitDevice();
    void            DeleteDevice( void );
    void            Mouse_Render( void );
#if defined(USE_JOYSTICK)
	void            SetGamePadKey( void );
    void            SetVirtualGamePadKey( void );
    void            InitVirtualGamePadKey( );
    void            ReadJoystick( void );
    int             GetJoystickKey( void );
    void            SetJoystickKey( BYTE byteKey , int Select);
    int             GetJoystickSelectedKey( int index );
    void SetVirtualGamePad( bool bDefault );
    int GetJoystickVirtualKey( int index );
#endif
	void            Picking( D3DXVECTOR3& vPos );
	void            Picking( D3DXVECTOR2& vPos );
	void            Picking( float& fX, float& fY );
	void			UpdateMousePos( D3DXVECTOR3 vPos );
	void			UpdateMousePos( D3DXVECTOR2 vPos );
	void			UpdateMousePos( float fX, float fY );

	// 단축 슬롯 키 설정 함수 crey21.
	void			SetShortCutItemKey( void );
	void			SetShortCutEmotKey( void );

    void            CheckSleepFramemove();
    void            NoSleep();

    void            RegisterLuaBind();

    bool            GetIsBlackMagic();


	bool			IsUseKeyCheck();
	void			InitDirectionKey();
    void            CheckChange();

	STICKYKEYS m_stickykeys;
};

extern POINT ptCursor;
extern POINT ptOldCursor;
extern bool MPushR;
extern bool MOld_PushR;
extern bool MPushL;
extern bool MOld_PushL;

#endif // _CONTROLS_H_
