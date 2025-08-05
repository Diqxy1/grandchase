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

    /// �ɼ��� ���̺�(������ ����� ��)�Ѵ�.
    void Save_Option( void );
    /// �ɼ��� �ε�(������ ������ ��)�Ѵ�.
    void Load_Option( void );
    /// �ɼ� ����
    void Quality_Change( int More,bool bIsAuto = false );

    /// ����̽� �ʱ�ȭ
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
    //TODO : lychnis IsCartoon IsModulate2X ���ݴ�. ���� �κ� �ּ��س����ϱ� �˻��ؼ� ��������.
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
        return m_bQualitySquare && m_bSquareItemEffect; // SquareQuality�� true �̾����
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
    LPDIRECT3DDEVICE9 m_pd3dDevice;  ///< Direct3D ����̽� �������̽� ������

    int             m_iQualityCount;
    bool            m_bIsCartoon;    ///< ī�� ������ �ɼ�(ī���������� �� ���ΰ�?)
    bool            m_bEdgeRender;
    bool            m_bModulate2X;   ///< ������ �ɼ�(D3DTOP_MODULATE2X�� �������� ���ΰ�?)
    bool            m_bWindowed;     ///< â��� �ɼ�(â���� ������ ���ΰ�?)
    bool            m_bShadow;       ///< �׸��� �ɼ�(�׸��ڸ� �׸� ���ΰ�?)
    bool            m_b3Back;        ///< ȯ�� ȿ�� �ɼ�(�ʿ��� ����Ʈ ȯ�� ȿ���� �׸� ���ΰ�?)
    bool            m_b1Back;
    bool            m_bEviroment;    ///< 3�� ��� ȿ�� �ɼ�(Reserved : ���� �������� �ʾ���.)
    bool            m_bJoystic;      ///< ������
    bool            m_bLinearFilter;
    bool            m_bAutoGraphicQuality;  // �ڵ����� �׷��� �ɼ��� �ٲ��� ���ΰ�
    bool            m_bSoundEnable;
    bool            m_bMusicEnable;
    bool            m_bWhisperFilter;
    bool            m_bHitEffect;
    bool            m_bMotionBlur;

    float           m_fSoundVolume; // ���庼��
    float           m_fMusicVolume; // ��������

    bool            m_bSkillEffect;
    bool            m_bUsingVS;

    bool            m_bQualitySquare;
    bool            m_bDenyInviteMsg;

    // [3/11/2008] breadceo. �⺻���� true �̾�� �ϴ� ���� ������ �� ���� ����Ͻÿ�.
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
	DWORD			m_dwBasicMode;	    // �־��� ��쿡 �ѹ��ϴ� �⺻���
	DWORD			m_dwDefaultMode;	// �⺻������ ���� �ϴ� �⺻ ���
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

	void SetColorIndex(int iIndex) // ���� ���� �ε��� �� �����ϴ� �Լ�
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
