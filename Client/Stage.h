#ifndef _STAGE_H_
#define _STAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ProtectedType.h"

#include "GCDeviceVertexBuffer.h"
#include "Particle/KGCParticleManager.h"

#include "PlayerComp.h"

struct KNextBlockCheck;

#define MAX_MAP_BLOCK       (12)    ///< 최대 맵 블록 개수
#define MAX_MAP_PORTAL      (4)     ///< 최대 맵 포탈 개수

class GCDeviceTexture;
class CWave;

enum MAP_PROPERTY{
    MP_PUSHINGHAND              = 0x00000001,        // 불의 사원
    MP_SMOG1                    = 0x00000002,
    MP_SMOG2                    = 0x00000004,    
    MP_SHINING_DUST             = 0x00000008,
    MP_LAVA                     = 0x00000010,
    MP_AUTOSCROLL               = 0x00000020,        // 오토 스크롤
    MP_STREAM                   = 0x00000040,        // 물줄기    
    MP_SCROLL_FAST              = 0x00000080,
    MP_SCROLL_NORMAL            = 0x00000100,
    MP_SCROLL_SLOW              = 0x00000200,
    MP_SAND_DROP                = 0x00000400,
};

struct PARTICLE_INFO{
    std::string     m_strParticleName;
    float           m_fParticlex;
    float           m_fParticley;
    bool            m_bOnUI;
};

struct StreamData
{
    int iEventStart;
    int iEventEnd;
    float fX;
    float fY;
    int iLife;
    float fYSpeed;
    int iJumpFrame;

    StreamData()
    {
        iEventStart = 0;
        iEventEnd   = 0;
        fX          = 0.0f;
        fY          = 0.0f;
        iLife       = 0;
        fYSpeed     = 0.0f;
        iJumpFrame  = 0;
    }
};

struct MapStreamData
{
    int iEvent;
    int iEventInitTime;
    int iSyncTiming;
    std::vector < StreamData > vecSD;
    std::vector < int > vecStreamSoundEvent;

    void Clear()
    {
        iEvent          = -1;
        iSyncTiming     = 0;
        iEventInitTime  = 0;
        vecSD.clear();
        vecStreamSoundEvent.clear();
    }
};

struct AmbientSound
{
    int iSoundID;
    int iEvent;
    int iEventInitTime;
    int iRate;

    AmbientSound()
    {
        iSoundID        = 0;
        iEventInitTime  = -1;
        iRate           = 0;
        iEvent          = 0;
    }
};

// 맵 공통 정보
struct MAP_INFO
{
    int                             m_iSkyID;
    int                             m_iNameID;
    DWORD                           m_dwMapProperty;
    std::vector<PARTICLE_INFO>      m_vecParticleInfo;

    MAP_INFO()
    {
        m_iSkyID        = 0;
        m_iNameID       = 0;
        m_dwMapProperty = 0;
        m_vecParticleInfo.clear();
    }

    void Clear()
    {
        m_iSkyID        = 0;
        m_iNameID       = 0;
        m_dwMapProperty = 0;
        m_vecParticleInfo.clear();
    }
};

/// @struct ShingDust 반딧불을 위한 구조체
struct ShingDust
{
    float x, y;
    float x_Speed;
    KSafeArray<float,8> Big;
    KSafeArray<float,8> Angle_Speed;
    KSafeArray<float,8> Angle;
    KSafeArray<float,8> sx;
    KSafeArray<float,8> sy;
};

/// @struct KHand 스테이지6('불의 사원')에 등장하는 손 구조체
struct KHand
{
    int     iHole;      ///< 구멍 인덱스
    int     iAlpha;     ///< 알파값(max:255)
    bool    bInc;       ///< 알파값 증가?
    bool    bPush;      ///< 밀어낼 것인가?
};

/// @class STAGE 맵 스테이지를 관리한다.
class STAGE : public lua_tinker::lua_value
{    
    KDeclareLuaInvoke
        /************************************************************************/
        /*    Lua 관련 함수
        /************************************************************************/
private:
    void RegisterLuabind();
    void LoadMapInfo();
    void LoadParticle();
    bool LoadScript(KLuaManager& luaMgr);

//public:
    // 파티클 시퀀스 벡터
    std::vector<CParticleEventSeqPTR> m_vecSequence;
    // ------------------------------------------------------------------------------------------
    // 맵 렌더링을 위한 멤버 변수
    // ------------------------------------------------------------------------------------------
public:
    GCDeviceTexture*                m_pTexSky;    ///< 맵 배경 텍스처
    KSafeArray<GCDeviceTexture*,MAX_MAP_BLOCK> m_pTexLevel1;    //T_Level2_Back[MAX_MAP_BLOCK];   ///< 맵 전경 텍스처    
    GCDeviceTexture*                m_pTexWater;
    GCDeviceTexture*                m_pTexFront;
    DWORD*                          m_pdwTexStageNum;
	GCDeviceTexture*				GetTexture( DWORD dwflag_ );
    std::vector<GCDeviceTexture*>   m_vecTexture;
    //LPDIRECT3DVERTEXBUFFER8 V_BackGround;                   ///< 맵 배경을 그릴 버텍스 버퍼
    //LPDIRECT3DVERTEXBUFFER8 V_Level_Back;                   ///< 맵 전경을 그릴 버텍스 버퍼
    SLVERTEX						V_BackGround[4];
    SLVERTEX						V_Level_Back[4];
    SLVERTEX                        V_Water[4];               ///< 맵 전경을 그릴 버텍스 버퍼
public:
    int                             Event;
    KSafeArray<MAP_INFO,NUM_TOTAL_STAGE> m_pMapInfo;

    // ------------------------------------------------------------------------------------------
    // 생성자 및 소멸자
    // ------------------------------------------------------------------------------------------
public:    
    /// Default constructor
    STAGE( );
    /// Default destructor
    ~STAGE( void );

    // ------------------------------------------------------------------------------------------
    // 리소스 관련    

    /// 스테이지를 로드한다.
    HRESULT Load_Stage( void );
    HRESULT Load_Stage( std::string strScriptName );

    /// 로드한 리소스를 모두 반환함
    void Release();

    // ------------------------------------------------------------------------------------------
    // 디바이스 관련 함수
    // ------------------------------------------------------------------------------------------
public:
    /// OnResetDevice - STAGE
    //void OnResetDevice();
    void InitDevice( void );
    /// DeleteDeviceObjects - STAGE
    void DeleteDevice( void );

    // ------------------------------------------------------------------------------------------
    // 매 프레임당 호출 함수
    // ------------------------------------------------------------------------------------------
public:
    /// FrameMove - STAGE
    void Frame_Move( void );

    // ------------------------------------------------------------------------------------------
    // 렌더링 함수
    // ------------------------------------------------------------------------------------------
//public:
    /// 맵 전경을 그린다.
    void RenderBack1( void );

    /// 맵 배경을 그린다.
    void RenderSky( void );

    /// 맵 환경 효과를 그린다.
    void FrontMap_Render( void );

    // ------------------------------------------------------------------------------------------
    // 맵 스테이지 관련 함수
    // ------------------------------------------------------------------------------------------
public:

    // 맵 정보 설정
    void SetMapInfo( int iMapNum, int iSkyID, int iNameID );
    //HRESULT        LoadResource( );
    ///
    D3DXVECTOR2 Get_Random_XY( bool CheckSafePoint = true );
    ///
    float       Get_LowHeight( float, float );
    ///
    D3DXVECTOR2 Check_SafePostionX( float x, float y = 0.0f );
    ///
    bool Check_Contact( KGCContectInfo& kInfo_ );
    D3DXVECTOR3 Check_Contact( float, float, unsigned char, bool bTagMotion = false, int* pContactX = NULL, int* pContactY = NULL, bool bNotCheckSideFH = false );
    ///
    bool        IsEnableNoCheck( float, float );
    bool        IsEnableNoJump( float, float );
	bool		IsDeadBlock(float x,float y);

    void SetStage( char cStage, bool bInitMonSlot = true/*, float fWaterHeight = 0.0f*/ );

    int GetStage( void );

    const TCHAR* GetStageName( int iStage_ ) const;
    // ------------------------------------------------------------------------------------------
    // 블록에 관한 멤버 변수
    // ------------------------------------------------------------------------------------------
public:

    KSafeArray<D3DXVECTOR2, MAX_MAP_PORTAL> PortalPosition;          ///< 포탈 지점 위치(최고 4개의 포탈 지점을 가질 수 있다.)
    float           Scroll_x;                   ///<
    BYTE            Num_Width;                  ///<
    BYTE            Num_Height;                 ///<
    BYTE            Color_r;                    ///<
    BYTE            Color_g;                    ///<
    BYTE            Color_b;                    ///<
    BYTE            Color_Stop_Count;           ///<
    BYTE            Num_Block;                  ///<
    BYTE            Background_Num;             ///<
    char            Low_Block[270];             ///<
    char            Low_Stand_Block[270];       ///<
    KSafeArray<BYTE,900*MAX_MAP_BLOCK> Field_Information; ///<    

    GCDeviceTexture* m_pTexSand;

    // ------------------------------------------------------------------------------------------
    // 블록에 관한 멤버 함수
    // ------------------------------------------------------------------------------------------
//public:
    ///
    unsigned char   Get_Field_Information(int Index, bool bFileLoad = false);
    ///
    char            Get_Low_Block(int Index);
    ///
    char            Get_Low_Stand_Block(int Index);

    bool            IsLowStandBlock(float x, float y);

    bool            IsInvBlock(float x, float y);

    bool            IsDoorUnlockBlock( float x, float y );

    // ------------------------------------------------------------------------------------------
    // 스테이지5('X-마스의 바벨')과 관련된 멤버 변수
    // ------------------------------------------------------------------------------------------
public:
    float                       m_fScrollY;         ///< 수직방향 오토스크롤 값
    bool                        m_bUpScroll;        ///< 윗방향으로 오토스크롤되고 있는가?
    bool                        m_bDeathScrollY;    ///< 오토스크롤에 의해 죽는가?

    // ------------------------------------------------------------------------------------------
    // 스테이지6('불의 사원')과 관련된 멤버 변수
    // ------------------------------------------------------------------------------------------
private:
    bool                        m_bLava;            ///< 용암이 밝아지고 있는가?
    int                         m_iLava;            ///< 용암의 밝기
    float                       m_fLavaTrans1;      ///< 아래 용암층 이동값
    float                       m_fLavaTrans2;      ///< 윗 용암층 이동값
    std::vector<KHand>          m_vecHand;          ///< 나왔다가 들어가는 손(더미 손)
public:
    std::vector<KHand>          m_vecPushHand;      ///< 나와서 밀어내는 손(미는 손)

    // ------------------------------------------------------------------------------------------
    // 스테이지6('불의 사원')과 관련된 멤버 함수
    // ------------------------------------------------------------------------------------------
public:
    /// 미는 손과 더미 손의 중복을 제거한다.
    void RemoveDuplicateHole( void );
private:
    /// 중복이 되지 않게 랜덤하게 구멍을 골라낸다.
    int GetRandomHole( void );
    /// 밀어내는 손을 초기화한다.
    void InitPushHand( void );
    /// 밀어내는 손을 리셋한다.
    void ResetPushHand( int iIdx, bool bInit = false );
    /// 손과의 충돌을 검사한다.
    void CheckHandCollision( int iIdx );

	void LoadFootholdTexture();
	void LoadFrontBGTexture();
    void LoadSkyTexture();
    void LoadStreamTable( KLuaManager& luaMgr );
    void LoadAmbientTable( KLuaManager& luaMgr );

    ShingDust*        m_pShiningDust;    ///< 반딧불 관련 ( 왜 Cloud인지는 묻지마시길 -> Cloud에서 ShingDust 로 이름 변경 )
    D3DXVECTOR2*    m_vecDustPosition; ///< 반딧불 위치를 저장
    KSafeArray<int,NUM_TOTAL_STAGE> m_iDustCount;        ///< 반딧불 갯수

    void SetShiningDustNum( int iMapNum, int DustCount );
    void SetShiningPosition( int iMapNum, int addCount, float fx_, float fy_ );

	void ON_RENDER_SHINING_DUST(GCDeviceTexture * texture);
    void ON_RENDER_LAVA(GCDeviceTexture * texture);
    void ON_RENDER_SMOG(GCDeviceTexture * texture);
    void ON_RENDER_SAND_DROP(GCDeviceTexture * texture);
    // 파티클로 빠짐
    // void ON_RENDER_BUBBLE();
    //void ON_RENDER_SHINING_STAR();
    //void ON_RENDER_SNOW();
    //--------------------------------------------------------------------------------------------
    // 스테이지8(가도센의 맵)과 스테이지9(오크 사원) 둘다 관련된 멤버 변수 
    //--------------------------------------------------------------------------------------------
private:
    float                       m_fSmogTrans;

    //--------------------------------------------------------------------------------------------
    // 스테이지9(오크 사원)와 관련된 멤버 변수 
    //--------------------------------------------------------------------------------------------
private:
    int                         m_iSandFrame;   ///< 모래 애니메이션 프레임
    KSafeArray<float,4> m_afSandDrop;

    std::vector< MapStreamData > m_vecMapStreamData;
    std::vector< AmbientSound >  m_vecAS;

public:
    void InitializeStreamEvent( int iIndex );

    // 발판을 그냥 그려준다
    void RenderMap();

    bool IsExistNextBlock( const KNextBlockCheck& kNextCheck );

    float GetXLimit();
    float GetYLimit();

    bool Check_Wall(float fX, float fY, float fOffSet);

    void ClearParticle( void );
};

#endif // _STAGE_H_
