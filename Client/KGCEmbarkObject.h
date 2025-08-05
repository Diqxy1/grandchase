/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2012년 11월 15일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 탑승물 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#pragma once
#include "KGCObj.h"

class KGCEmbarkAction
{
public:
    KGCEmbarkAction()
    {
        Init();
    }

    void Init()
    {
        m_iStateIndex = 0;
        strStateID.clear();
        strMotionName.clear();
        strFunctionName.clear();
        strOwnerFunctionName.clear();
    }

public:
    int         m_iStateIndex;
    std::string strStateID;
    std::string strMotionName;
    std::string strFunctionName;
    std::string strOwnerFunctionName;
};

class KGCEmbarkTemplate
{
public:
    KGCEmbarkTemplate()     {   Init(); };
    ~KGCEmbarkTemplate()    {           };

public:
    void Init();

public:
    enum
    {
        CALLBACK_INIT,
        CALLBACK_UP_EMBARKATION,
        CALLBACK_DOWN_EMBARKATION,
        CALLBACK_LANDING,
        CALLBACK_CHANGE_HP,
        CALLBACK_DIE,
        CALLBACK_EMBARKATOR_DIE,
        CALLBACK_CHANGE_CHAR_HP,
        CALLBACK_CRASH_DAMAGE,
        CALLBACK_SYNC,
        CALLBACK_MAX,
    };
    std::string GetCallBackKeyName( int iIndex );
    std::string GetCallBackFuncName( int iIndex ) const;

public:
    DWORD       m_dwID;
    int         m_iNameID;
    float       m_fMaxHp;
    float       m_fScale;

    float       m_fAttackWidth;
    float       m_fAttackHeight;
    float       m_fOffsetAttackWidth;
    float       m_fOffsetAttackHeight;

    float       m_fPushWidth;
    float       m_fPushHeight;
    float       m_fOffsetPushWidth;
    float       m_fOffsetPushHeight;

    float       m_fNameOffsetX;
    float       m_fNameOffsetY;

    int         m_iUpCharMotion;
    int         m_iDownCharMotion;
    int         m_iDefaultCharUpBoneID;

    bool        m_bNoPushCheck;
    bool        m_bOnlyPosBone;
    bool        m_bCheckPortal;
    bool        m_bNotDrawPetName;

    std::string m_strActionScript;
    std::vector< EXTRA_MESH > m_vecMeshInfo;
    std::vector< std::pair<std::string, KGCEmbarkAction> > m_vecActionList;
    std::map<int, std::string> m_mapCallBackList;
};

class KGCEmbarkFrame
{
public:
    KGCEmbarkFrame()    {   Init();     };
    ~KGCEmbarkFrame()   {               };

    KGCEmbarkFrame( const KGCEmbarkFrame& src )
    {
        bIsRight = src.bIsRight;
        bContact = src.bContact;
        iStateIndex = src.iStateIndex;
        cFrame = src.cFrame;
        vPos = src.vPos;
    }
    const KGCEmbarkFrame& operator=( const KGCEmbarkFrame& src )
    {
        bIsRight = src.bIsRight;
        bContact = src.bContact;
        iStateIndex = src.iStateIndex;
        cFrame = src.cFrame;
        vPos = src.vPos;
        return *this;
    }

public:
    void Init()
    {
        bIsRight = true;
        bContact = true;
        iStateIndex = 0;
        cFrame = 0;
        vPos = D3DXVECTOR3(0.0f, 0.0f, 2.5f);
    }

public:
    bool        bIsRight;
    bool        bContact;
    short       iStateIndex;
    UCHAR       cFrame;
    D3DXVECTOR3 vPos;
};

class KGCEmbarkObject : public KGCObj
{
public:
    KGCEmbarkObject( DWORD dwUID, KGCEmbarkTemplate* pTemplate );
    ~KGCEmbarkObject(void);

public:
    void Init();
    void StartEmbark();
    void EndEmbark( int iBlinkTime = 0 );
    void LoadResource();
    void LoadActionScript();
    void LoadETC();
    void Release();
private:
    void LoadObjectMesh();
    void LoadObjectMotion();
    void ReleaseMotionData();
    void ReleaseParticle();

public:
    void FrameMove();
    void TextRender();
private:
    void CheckEmbarkator();
    void UpDateToCommonKey();
    void UpDateToMotion();
    void UpDateToMove();
    void UpDateToPush();
    void UpDateToRender();
    void UpDateToLatency();
    void IncreaseFrame();
    void UpDateToETC();
public:
    void LatencyAllEqual( int iLatency = (LATENCY + SHOWLATENCY - 1) );

public:
    static void RegisterLuaBind();
    void CallLuaFunc_Callback( int iIndex );
private:
    void CallLuaFunc_Action();
    void CallLuaFunction( const char *szFunction, UCHAR cFrame );

public:
    void SetFrameData( KGC_PID_BROAD_EMBARK_FRAME_INFO *pPacket );
private:
    KGCEmbarkFrame& GetFrameData();
    KGCEmbarkFrame& GetRenderFrameData();
    const KGCEmbarkAction* GetActionFromIndex( int iIndex );
    const KGCEmbarkAction* GetActionFromState( const char *szState );

public:
    DWORD GetEmbarkUID()                   {   return m_dwUID;      }
    DWORD GetEmbarkType();
    DWORD GetOwnerUID();
    bool IsEmbarkation();               // 탑승물에 누군가가 탑승했는가?
    bool IsMyOwnerOfEmbarkObject();     // 탑승물의 주인이 나인가?
    PLAYER *GetEmbarkationPlayer();

public:
    void SendLatencyData();
    void SendSyncCallback();
    void SendEmbarkation( DWORD dwPlayerUID, DWORD dwReSendCoolTime, int iBoneID );
    bool SetUpEmbarkation( DWORD dwOwnerUID, int iCharMotionID = 0, int iCallback = 0 );
    bool SetDownEmbarkation( DWORD dwOwnerUID, int iCharMotionID = 0, int iCallback = 0 );

public:
    int GetLastFrame();
    void SetState( const char *szState, bool bSendLatency = false );
    const char *GetState();
    bool IsState( char *szState );
    float GetPosX();
    float GetPosY();
    float GetPosZ();
    float GetRenderPosX();
    float GetRenderPosY();
    float GetRenderPosZ();
    void SetPos( float fX, float fY, float fZ = 2.5f );
    bool GetIsRight();
    void SetIsRight( bool bRight );
    float GetPushWidth();
    float GetPushHeight();
    float GetAttackWidth();
    float GetAttackHeight();
    GCCollisionRect<float>& GetAttackRect();
    GCCollisionRect<float>& GetPushRect();
    bool IsPushCheck();
    
public:
    void SetSpeedX(float fX)    {   m_fSpeedX = fX; }
    void SetSpeedY(float fY)    {   m_fSpeedY = fY; }
    float GetSpeedX()           {   return m_fSpeedX;   }
    float GetSpeedY()           {   return m_fSpeedY;   }
    void SetFly(bool bFly)      {   m_bIsFly = bFly;    }
    bool GetFly()               {   return m_bIsFly;    }
public:
    void SetHP( float fHP, bool bCallBack = false );
    float GetHP()               {   return m_fHp;   }
    void SetDiffHP( float fDiffHP ) {   m_fDiffHp = fDiffHP;    }
    float GetDiffHP()               {   return m_fDiffHp;   }
public:
    void SetWhatDamage(int iWhat){  m_iWhatDamage = iWhat;  }
    int GetWhatDamage()          {  return m_iWhatDamage;  }

public:
    void SetNotEmbarkation(bool bEmbarkation){  m_bNotEmbarkation = bEmbarkation;   }
    bool GetNotEmbarkation(){  return m_bNotEmbarkation;   }
    void SetIsContact(bool bContact);
    bool GetIsContact();
    void SetNoContactFrame(DWORD dwFrame){   m_dwNoContactFrame = dwFrame; }
    DWORD GetNoContactFrame()   {   return m_dwNoContactFrame;  }
    bool IsDownJumpBlock();
    void EffectDust( float fOffsetX, float fOffsetY );
    bool GetPushCheck()             {   return m_bPushCheck;    }
    void SetPushCheck(bool bPush)   {   m_bPushCheck = bPush;    }

public:
    bool GetToggleRender()       {   return m_bRender;    }
    void SetToggleRender( bool bRender );
    void SetFrameLock(bool bLock){   m_bFrameLock = bLock; }
    bool GetFrameLock()          {   return m_bFrameLock;  }
    void SetLatency(int iLatency){   m_iLatency = iLatency; }
    int  GetLatency()            {   return m_iLatency;  }

    int GetUpCharMotion();
    int GetDownCharMotion();
    int GetDownCallback();
    int GetUpCallback();
    int GetDefaultCharUpBoneID();

    bool GetDeleteReservation()  {  return m_bDeleteReservation;    }
    bool IsCheckPortal();
    bool IsNotDrawPetName();

public:
    bool CheckPosInRange( float fPosX, float fPosY, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY );
    bool CheckInRangeToPlayer( int iPlayerIndex, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY );

public:
    void AddParticle( char *szIdentifier, char *szSeqeunce, float fOffsetX, float fOffsetY );
    void AddTraceParticle( char *szIdentifier, char *szSeqeunce, float fOffsetX, float fOffsetY, float fTraceTime = 0.5f );
    void AddTraceParticleToBone( char *szIdentifier, char *szSeqeunce, int iBoneID, float fOffsetX, float fOffsetY, float fTraceTime = 0.5f, bool bNoDirection = false );
    void InsertParticleSequence( char *szIdentifier, CParticleEventSeqPTR pSequence );
    void DestroyParticle( char *szIdentifier );
    bool IsLiveParticle( char *szIdentifier );
    void CheckParticleList();
    void SetShake( float fStrength, int iShakeFrame )
    {   m_iShakeFrame = iShakeFrame;    m_fShakeStrength = fStrength;   };
    void SetDamageCartoon( int iKeepFrame )
    {   m_iFlashCartoonFrame = iKeepFrame;  }

    void PlaySound( char* strSound, int iLoop = 0 );
    void StopSound( char* strSound );

public:
    void GetBoneMatrix( unsigned int iIndex, D3DXMATRIX* pOutMatrix );

public:
    void SetTempValue( char *strKey, float fValue );
    float GetTempValue( char *strKey );

public:
    D3DXVECTOR3 GetUserBoneAngle( int iBoneIndex );
    void SetUserBoneAngle( int iBoneIndex, float fAngleX, float fAngleY, float fAngleZ, bool bSetLatency = false );
    virtual D3DXMATRIX* ComputePositionBone( D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone );
    virtual D3DXMATRIX* ComputeAngleBone( D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic );

#if !defined(__PATH__)
    void RenderBoundingBox();
    void RenderBox( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ );
#endif

private:
    void ChangeCartoon( KGCEmbarkFrame& kRenderData );

private:
    DWORD               m_dwUID;
    DWORD               m_dwOwnerPlayerUID;
    int                 m_iHPBarID;

private:
    int                 m_iLatency;
    KGCEmbarkFrame      m_kFrameData[ LATENCY + SHOWLATENCY ];

private:
    float               m_fHp;
    float               m_fDiffHp;
    float               m_fSpeedX;
    float               m_fSpeedY;
    int                 m_iWhatDamage;
    bool                m_bIsFly;
    bool                m_bRender;
    bool                m_bFrameLock;
    bool                m_IsContact;
    bool                m_bNotEmbarkation;
    DWORD               m_dwNoContactFrame;
    GCCollisionRect<float> m_kBoundRect;
    std::map<std::string, float>  m_mapTempValue;

private:
    int                 m_iBlinkTime;
    int                 m_iShakeFrame;
    int                 m_iFlashCartoonFrame;
    float               m_fShakeStrength;
    bool                m_bDeleteReservation;
    bool                m_bPushCheck;

private:
    int                 m_iLatencyAngleBone;
    D3DXVECTOR3         m_vLatencyAngle;

private:
    int                 m_iOriCartoon;
    std::vector<int>    m_vecOriChildCartoon;

private:
    std::vector< GCDeviceMotionFRM* > m_vecMotionList;

private:
    const KGCEmbarkTemplate*  m_pOriginalTemplate;

private:
    std::map<std::string, std::vector<CParticleEventSeqPTR>>    m_mapParticleList;

public:
    D3DXVECTOR3         m_vTraceParticlePos;
    D3DXVECTOR3         m_vRealRenderPos;

#if !defined(__PATH__)
    bool                m_bIsReload;
#endif
};