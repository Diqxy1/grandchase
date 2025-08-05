#ifndef _HEADUPDISPLAY_H_
#define _HEADUPDISPLAY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "ProtectedType.h"

class GCDeviceTexture;
class GCDeviceMotionFRM;

#define MAX_DAMAGE_PRECISION 4
#define PUZZLE_KEY_EFFECT_GAP 0.14f
#define PUZZLE_KEY_EFFECT_X_OFFSET 1.08f
#define PUZZLE_KEY_EFFECT_Y_OFFSET 0.15f

enum GCResultPos
{
	GCRP_WIN = 0,
	GCRP_LOSE,
	GCRP_TOTAL
};

struct KGCExpGpInfo
{
    enum{
        EG_EXP = 0,
        EG_GP = 1,
        EG_MATCH_KILL = 2,
    };

    int iKind;
    float fPosX;
    float fPosY;
    int iPlayerIndex;
    int iExp;
    float fRenderTime;
};

struct KDamageString
{
    float fPosX;
    float fPosY;
    float fInterval;
    int iFrameCount;
    std::vector<string> vecDamageString;
    int iLastCount;

    KDamageString()
    {
        Init();
    }

    void Init()
    {
        fPosX = 0.0f;
        fPosY = 0.0f;
        fInterval = 0.0f;
        iFrameCount = -1;
        vecDamageString.clear();
        iLastCount = 0;
    }
};

class Headup
{
private:
	LPDIRECT3DDEVICE9       m_pd3dDevice;
    std::wstring            m_strReservedMsg;

	CParticleEventSeqPTR m_pExpEvent1;
	CParticleEventSeqPTR m_pExpEvent2;

public:
    GCDeviceTexture*  m_pTexPointer;
    GCDeviceTexture*  m_pTexPointer2;
    GCDeviceTexture*  m_pTexLetter;
	GCDeviceTexture*  m_pTexBlessingBar1;
	GCDeviceTexture*  m_pTexBlessingBar2;

	KSafeArray<KSafeArray<GCDeviceMotionFRM*,GCRP_TOTAL>,CID_MAX> m_pResultMotion;
	GCDeviceMotionFRM* m_pPetResultMotion;


	/*SLVERTEX			V_Head[12];
    SLVERTEX			V_Pointer[4];*/
    SLVERTEX			V_Letter[4];

    float       SuperPoint;

    int         White_Count;
	int         Show_Count;
	bool		IsBigLetter;
	char       Letter_Text[31];
	KSafeArray<float,30> Show_Text;

	bool		IsBlackMagic;
	bool		IsCheatMode;
    bool        m_IsMPDamage;

	CString		m_strText;	

	// �޺� ����
    bool        ShowCombo;
    int         Combo_Count;
    bool        bEventComboCheck;
    bool        bDisappearComboCheck;
    bool        bTotalScoreEffect;
    int         Combo_Count_Aerial;
    float       Combo_HP;
    float       Show_Combo_HP;
    DWORD       Combo_Time;
    float       Combo_Pitch;
    int         m_MaxCombo;
    void        Event_Combo(bool bAerial, float m_fHP);
    void        Render_Combo();
    void        FrameMove_Combo();

    KProtectedType<float>       Level;

	// ��Ȳ�� ����
    int         Panel_Show_Count;
	KSafeArray<char,MAX_PLAYER_NUM> Old_Life;
    KSafeArray<char,MAX_PLAYER_NUM> Change_Life;

	// Exp ��Ȳ�� ����
	KSafeArray<float,MAX_PLAYER_NUM> Score_Pitch;
	KSafeArray<BYTE,MAX_PLAYER_NUM> Score_Bright;

	//Emblem ����
	std::vector<std::string> m_strEmblemParticleName;
    std::vector<std::pair<int,int>> m_vecTitle;
	

	void InitDevice();
    //void OnResetDevice();
    void DeleteDevice();	

    inline void SetReservedMessage( const std::wstring& strReservedMsg )
    {
        m_strReservedMsg = strReservedMsg;
    }
    inline const std::wstring& GetReservedMessage( void ) const
    {
        return m_strReservedMsg;
    }

	void Start_Text(const char*,bool BigLetter=true);
	void Clear_All_Cheat();
	void Letter_Render(float x=0, float y=0);
	void Num_Render(float, float, const TCHAR*, DWORD, float Width=.066f, bool RightSort=true, bool Is2X=false, float Size=1.0f);
	HRESULT Frame_Move();

	void Alpha_Render();
    void Render();
//	void Show_Exp(bool bRight, int iPos, int iPlayerNum, bool bBlue);
//	void Show_Pet_Exp( bool bRight, int iPos, int iPlayerNum, bool bBlue );
	void RenderDurationItems();
	
	// HP ��ȭ���� �������� ���� �����ִ� ���
//	KSafeArray<float,MAX_PLAYER_NUM+MAX_MONSTER> Now_HP;

	
	

    void Add_HP_Changer(int, int, float, bool IsMPDamage = false, CString strText="", DWORD dwAttackType = 0, float fHitPosX = 0.0f, float fHitPosY = 0.0f, int iTextType = -1);
	void Draw_HP_Changer();
	void Clear_All_HP_Changer();
    void InitDataForTutorial();
	void Clear();
	void CreateEventParticle();

	Headup();
    ~Headup();

    // ���� ����� �� Exp�� �����ְ���
    std::vector< KGCExpGpInfo > m_vecShowExpGpInfo;
    // ������ �Ծ��� �� Gp�� �����ְ���
    std::vector< KGCExpGpInfo > m_vecShowGpInfo;
    void SetRenderExpGp( int iPlayerIndex, int iValue, int iKind = KGCExpGpInfo::EG_EXP, D3DXVECTOR2 vPos = D3DXVECTOR2(0,0) );
    void Render_ExpGp();

    D3DXVECTOR3 m_vCameraLookPos;

    // ���� �����Ҷ� �̺�Ʈ�� ��ƼŬ�� �ѷ��ֱ� ���� �Լ���..
    void MoonFestival();
	void SetTitleEmblem();
    void SetTitleEmblem( std::map<int,std::pair<int,int>>& mapTitleID );

    // �޺� ����� Ÿ�̹��� �������� ���� ���� �� �װų� �÷������̸� 20�� �ش�.
    DWORD m_dwBreakGap;
    void SetComboTimer( bool bSetCurtime_, DWORD dwBreakGap_ );
    void ComboClear( void );

private:
    //KSafeArray<CParticleEventSeqPTR,MAX_DAMAGE_PRECISION> m_pDamagePrecision;
    std::vector<KDamageString> m_vecDamageString;
    bool m_bRunDamagePrticle;
    int m_iDamageTextCount;
    std::map<int,std::pair<int,int>>	m_mapTitleID;

public:
    enum EDamageTextType
    {
        EDAMAGE_TEXT_MAX_NUM = 10,
        EDAMAGE_TEXT_ORANGE = 0,
        EDAMAGE_TEXT_VIOLET,
        EDAMAGE_TEXT_BLUE,
        EDAMAGE_TEXT_GREEN,
		EDAMAGE_TEXT_CRITICAL,
        EDAMAGE_TEXT_NORMAL,
    };

    void Num_Render_Particle(int Playersource, std::string ParticleText, EDamageTextType eDmgType, float fPosX, float fPoxY, float usHP ); //Sistema de Skin de Dano
    void RenderDamamgeNumber();

    // �ʻ�� ����Ű Ŀ�ǵ� ��ƼŬ
public:
    void RenderPuzzleKey( int iKeyValue_, D3DXVECTOR2 vPos_ );
    void RenderPuzzleKeyPushEffect( int iOrder_ );
    void RenderPuzzleKeyFailEffect( int iOrder_ );
    void LoadPuzzleKeySequenceList();
    void InitPuzzleKeyInfo();
    bool GetShowCombo() { return ShowCombo; }

private:
    std::map< int, std::string >    m_mapPuzzleKeySequence;
    std::vector< std::string >      m_vecPuzzleKeyPushSequence;
    std::vector< std::string >      m_vecPuzzleKeyFailSequence;
    std::vector< D3DXVECTOR2 >      m_vecPuzzleKeyEffectPos;
    int                             m_iPuzzleKeyEffectCnt;

};

#endif // _HEADUPDISPLAY_H_
