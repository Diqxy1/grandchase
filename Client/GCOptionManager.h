#ifndef _GCOPTIONMANAGER_H_
#define _GCOPTIONMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>

#ifdef WEAPONCHANGE
#define NUM_USE_KEY 12
#else
#define NUM_USE_KEY 11
#endif
#define NUM_USE_SKILL_KEY 5

class KGCOption;

class KGCOptionManager
{
public:
    enum EKGCOPTIONMANAGER {
        EGCOM_KEYBOARD = 0,
        EGCOM_JOYSTICK = 1,
    };

public:
    KGCOptionManager(void);
    ~KGCOptionManager(void);

    /// 옵션을 세이브(게임이 종료될 때)한다.
    void Save_Option( void );
    /// 옵션을 로드(게임이 시작할 때)한다.
    void Load_Option( void );
    /// 옵션 변경
    void Quality_Change( int More,bool bIsAuto = false );

    /// 디바이스 초기화
    void InitDevice( LPDIRECT3DDEVICE9 pd3dDevice );

    void SetSound();
    void SetMusic();
    void SetCartoon(bool bCartoon){m_bIsCartoon = bCartoon;}
    void SetShadow(bool bShadow){m_bShadow = bShadow;}
    void Set1Back(bool b1Back){m_b1Back = b1Back;}
    void Set3Back(bool b3Back){m_b3Back = b3Back;}
    void SetModulate2X(bool bModulate2X){m_bModulate2X = bModulate2X;}
    void SetLinearFilter(bool bLinearFilter){m_bLinearFilter = bLinearFilter;}
#if defined( USE_JOYSTICK )
    void SetJoystic(bool bJoystic){m_bJoystic = bJoystic;}
#endif
    void SetEnv(bool bEnv){m_bEviroment = bEnv;}
    void SetHitEffect( bool bHiteffect ){ m_bHitEffect = bHiteffect; }
    void SetAutoGraphicQuality( bool bAutoGraphicQuality ){ m_bAutoGraphicQuality = bAutoGraphicQuality; }
    void SetMotionBlur( bool bMotionBlur ) { m_bMotionBlur = bMotionBlur; }
    bool IsEdgeRender(){ return m_bEdgeRender; }
    //TODO : lychnis IsCartoon IsModulate2X 없앴다. 쓰던 부분 주석해놨으니까 검색해서 정리하자.
    bool IsCartoon();
    bool IsShadow(){return m_bShadow;}
    bool Is1Back(){return m_b1Back;}
    bool Is3Back(){return m_b3Back;}
    //bool IsModulate2X(){return m_bModulate2X;}
    bool IsLinearFilter(){return m_bLinearFilter;}
#if defined( USE_JOYSTICK )
    bool IsJoystic(){return m_bJoystic;}
#endif
    bool IsEnv(){return m_bEviroment;}
    bool IsHiteffect(){ return m_bHitEffect; }
    bool IsAutoGraphicQuality(){ return m_bAutoGraphicQuality; }
    bool IsMotionBlur() { return m_bMotionBlur; }
    void SetUsingVS();
    int GetQualityCount(){return m_iQualityCount;}
    void SetSaveID( bool bSave, WCHAR* wcsUserID );
#if defined (NATION_THAILAND)
	void SetSaveFunboxPostFix( bool bSave, int iFunboxPostFix);
#endif

    void SetSkillEffect( bool bSkillEffect_ ) { m_bSkillEffect = bSkillEffect_; }
    bool IsSkillEffect() const { return m_bSkillEffect; }

    bool GetSquareQuality() const { return m_bQualitySquare; }
    void SetSquareQuality( bool bQuality ) { m_bQualitySquare = bQuality; }

    bool IsDenyInviteMsg() const { return m_bDenyInviteMsg; }
    void SetDenyInviteMsg( bool bDeny ) { m_bDenyInviteMsg = bDeny; }

    bool GetSquareItemEffect() const
    {
        return m_bQualitySquare && m_bSquareItemEffect; // SquareQuality가 true 이어야함
    }
    void GetSquareItemEffect(bool val) { m_bSquareItemEffect = val; }

	BYTE GetLastState() { return m_ucLastState;}
	void SetLastState(BYTE ucLastState_) { m_ucLastState = ucLastState_;}
	BYTE GetLastWorldMap() { return m_ucLastWorldMap; }
	void SetLastWorldMap(BYTE ucLastWorldMap_) { m_ucLastWorldMap = ucLastWorldMap_;};
	int GetSampleInfoNum();
	D3DMULTISAMPLE_TYPE GetSampleInfoFromNum(int iNum);
	std::wstring GetStringSampleInfo(D3DMULTISAMPLE_TYPE eSampleinfo);
	D3DMULTISAMPLE_TYPE GetSampleInfo();
    void SetAlphaAnimation(bool bAlphaAni) { m_bAlphaAni = bAlphaAni; }
    bool GetAlphaAnimation() { return m_bAlphaAni; }

    int GetSettingDeviceType(){ return m_iSettingDeviceType;}
    void SetSettingDeviceType(int iType){ m_iSettingDeviceType = iType;}

	bool GetPetViewHide() { return m_bPetViewHide; }
	void SetPetViewHide(bool bPetViewHide) { m_bPetViewHide = bPetViewHide; }

    bool GetBasicEquipEnable() { return m_bBasicEquipEnable; }
    void SetBasicEquipEnable(bool bBasicEquipEnable) { m_bBasicEquipEnable = bBasicEquipEnable; }

    bool GetRoomAnimationEnable() { return m_bRoomAnimationEnable; }
    void SetRoomAnimationEnable(bool bRoomAnimationEnable) { m_bRoomAnimationEnable = bRoomAnimationEnable; }

public:
    bool            m_bSaveID;
    WCHAR           m_wcsUserID[MAX_LOGIN_ID_SIZE];

private:
    LPDIRECT3DDEVICE9 m_pd3dDevice;  ///< Direct3D 디바이스 인터페이스 포인터

    int             m_iQualityCount;
    bool            m_bIsCartoon;    ///< 카툰 렌더링 옵션(카툰렌더링을 할 것인가?)
    bool            m_bEdgeRender;
    bool            m_bModulate2X;   ///< 라이팅 옵션(D3DTOP_MODULATE2X로 라이팅할 것인가?)
    bool            m_bWindowed;     ///< 창모드 옵션(창모드로 보여줄 것인가?)
    bool            m_bShadow;       ///< 그림자 옵션(그림자를 그릴 것인가?)
    bool            m_b3Back;        ///< 환경 효과 옵션(맵에서 프론트 환경 효과를 그릴 것인가?)
    bool            m_b1Back;
    bool            m_bEviroment;    ///< 3차 배경 효과 옵션(Reserved : 아직 구현되지 않았음.)
    bool            m_bJoystic;      ///< 미지원
    bool            m_bLinearFilter;
    bool            m_bAutoGraphicQuality;  // 자동으로 그래픽 옵션을 바꿔줄 것인가
    bool            m_bSoundEnable;
    bool            m_bMusicEnable;
    bool            m_bWhisperFilter;
    bool            m_bHitEffect;
    bool            m_bMotionBlur;

    float           m_fSoundVolume; // 사운드볼륨
    float           m_fMusicVolume; // 뮤직볼륨

    bool            m_bSkillEffect;
    bool            m_bUsingVS;

    bool            m_bQualitySquare;
    bool            m_bDenyInviteMsg;

    // [3/11/2008] breadceo. 기본값이 true 이어야 하는 값이 있으면 이 값을 사용하시오.
    bool            m_bSquareItemEffect;
	BYTE            m_ucLastState;
	BYTE			m_ucLastWorldMap;

    bool            m_bAlphaAni;
	bool			m_bPetViewHide;
    bool			m_bBasicEquipEnable;
    bool			m_bRoomAnimationEnable;


#if defined(NATION_THAILAND)
	int				m_iFunBoxPostfix;
#endif

public:
#if defined(NATION_THAILAND)
	inline int GetiFunBoxPostFix()
	{
		return m_iFunBoxPostfix;
	}
#endif

    KSafeArray<BYTE,NUM_USE_KEY + NUM_USE_SKILL_KEY> m_KeyTable;
    KSafeArray<BYTE,NUM_USE_KEY + NUM_USE_SKILL_KEY> m_JoystickKeyTable;
    bool             m_bEnableKeySetting;
    int             m_iSettingDeviceType;
private:
	float			m_fScaleX;
	float			m_fScaleY;
	DWORD			m_dwBasicMode;	    // 최악의 경우에 롤백하는 기본모드
	DWORD			m_dwDefaultMode;	// 기본적으로 셋팅 하는 기본 모드
    DWORD           m_dwCurrentModeOld;
    DWORD           m_dwFullScreenModeOld;

	DWORD           m_dwCurrentMode;
	DWORD           m_dwFullScreenMode;
	DWORD           m_dwScreenMode;
    DWORD           m_dwPrevMode;


	D3DMULTISAMPLE_TYPE m_eSampleinfo;
	D3DMULTISAMPLE_TYPE m_eSampleinfoOld;
	int m_iIndexColorTable;


public:
    DWORD   GetBasicMode()              { return m_dwBasicMode; }
    DWORD   GetDefaultMode()              { return m_dwDefaultMode; }
    D3DXVECTOR2 GetBasicWindowScale()   { return D3DXVECTOR2(1.0f, 1.0f); }
    float   GetBasicWindowScaleX()      { return 1.0f; }
    float   GetBasicWindowSacleY()      { return 1.0f; }

    DWORD   GetCurrentMode();
    D3DXVECTOR2 GetWindowScale();
    float   GetWindowScaleX();
    float   GetWindowSacleY();

	void SetColorIndex(int iIndex) // 색상 정보 인덱스 값 저장하는 함수
	{
		m_iIndexColorTable = iIndex;
	}

	inline int GetColorIndex()
	{
		return m_iIndexColorTable;
	}


    struct SGCResolution
    {
        float width;
        float height;
    };
    int GetNumOfResolution();
    bool GetResolution( IN int iMode, OUT D3DModeInfo& sResolution );
    bool SetResolution( int iMode );
	BOOL SetSampleing( D3DMULTISAMPLE_TYPE eSample );

    void InitResolutionOption();
    bool IsEnableResolution( DWORD width_, DWORD height_ );

    void LoadOptionData_10002( HANDLE& DBF );
};

#endif // _GCOPTIONMANAGER_H_
