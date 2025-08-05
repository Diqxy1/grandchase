/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2013년 05월 23일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 달성과제 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#pragma once
#include "KGCObj.h"
#include "GCUI\KGCInGameMessage.h"

class KGCSubjectAction
{
public:
    KGCSubjectAction() {    Init(); }

public:
    void Init()
    {
        m_iStateIndex = 0;
        strStateID.clear();
        strFunctionName.clear();
    }

public:
    int         m_iStateIndex;
    std::string strStateID;
    std::string strFunctionName;
};

struct KGCSubjectReward_Buff
{
    int iBuffID;
    int iBuffLevel;
    float fBuffTime;
    bool bIsLocalBuff;
    KGCSubjectReward_Buff()
        : iBuffID( 0 )
        , iBuffLevel( 0 )
        , fBuffTime( 0.0f )
        , bIsLocalBuff( false ) {}
};

struct KGCSubjectReward_Item
{
    DWORD dwItemID;
    int iGrade;
    int iPeriod;
    int iCount;
    KGCSubjectReward_Item()
        : dwItemID( 0 )
        , iGrade( 0 )
        , iPeriod( 0 )
        , iCount( 0 ) {}
};

class KGCSubjectReward
{
public:
    KGCSubjectReward()
    {
        Init();
    }

    void Init()
    {
        m_iGp = 0;
        m_iExp = 0;
        m_vecBuffList.clear();
        m_vecItemList.clear();
    }

public:
    int m_iGp;
    int m_iExp;
    std::vector<KGCSubjectReward_Buff>    m_vecBuffList;
    std::vector<KGCSubjectReward_Item>    m_vecItemList;
};

class KGCSubjectTemplate
{
public:
    KGCSubjectTemplate()     {   Init(); };
    ~KGCSubjectTemplate()    {           };

public:
    enum
    {
        CALLBACK_INIT,
        CALLBACK_HIT_MONSTER_DMG,
        CALLBACK_END,
        CALLBACK_MAX,
    };

public:
    void Init();
    std::string GetCallBackKeyName( int iIndex );
    std::string GetCallBackFuncName( int iIndex ) const;

public:
    DWORD       m_dwTypeID;
    DWORD       m_dwSimpleDescStringID;
    std::string m_strActionScript;
    std::vector< std::pair<std::string, KGCSubjectAction> > m_vecActionList;
    std::map<int, std::string> m_mapCallBackList;
    KGCSubjectReward    m_kRewardInfo;
};

class KGCSubject
{
public:
    KGCSubject( DWORD dwUID, KGCSubjectTemplate* pTemplate );
    ~KGCSubject(void);

public:
    void Init();
    void Release();

public:
    void FrameMove();
    void TextRender();

public:
    static void RegisterLuaBind();
private:
    void CallLuaFunc_Action();
    void CallLuaFunction( const char *szFunction );
    void CallLuaFunction_HitMonDamageCallback( const char *szFunction, int iAttackerID, int iHitterID, int iDamageID );
public:
    void CallLuaFunc_Callback( int iCallbackIndex );
    void CallLuaFunc_Callback_To_HitMonDamage( int iAttackerID, int iHitterID, int iDamageID );

private:
    const KGCSubjectAction* GetActionFromIndex( int iIndex );
    const KGCSubjectAction* GetActionFromState( const char *szState );

public:
    void SetSubjectTGID(int iID)            {   m_iTG_ID = iID;      }
    void SetSubjectStageID(int iID)         {   m_iStageID = iID;      }
    DWORD GetSubjectUID()                   {   return m_dwUID;      }
    DWORD GetSubjectType();
    DWORD GetSubjectSimpleStringID();

public:
    void AddUIParticle( char *szSequence, float fPosX, float fPosY );

public:
    void SetState( const char *szState );
    const char *GetState();
    bool IsState( char *szState );
    void EndSubject(  bool bSuccess = false );
    int GetSucdcess()  {   return m_iSuccess;  }
    int GetTiggerID()  {   return m_iTG_ID;    }
    int GetStageID()  {   return m_iStageID;    }

public:
    void SetTempValue( char *strKey, float fValue );
    float GetTempValue( char *strKey );

public:
    void GetRewardInfo( KInGameMessageAttribute *pMsgAttribute );

public:
    int GetCombo();
    int GetAerialCombo();
    void ResetCombo();

public:
    int GetProgressNow()               {   return m_iProgressNow;  }
    void SetProgressNow(int iValue)    {   m_iProgressNow = iValue;  }
    int GetProgressMax()               {   return m_iProgressMax;  }
    void SetProgressMax(int iValue)    {   m_iProgressMax = iValue;  }

public:
    void UnLockAddChecker();
    void AddBackAttack();
    void AddSpecial();
    void AddDamageHitCnt();
    void AddDamageVolume( float fDamage );
    void AddDieCnt();
    void SetComboBreak();

private:
    DWORD               m_dwUID;
    std::string         m_strNowAction;
    int                 m_iTG_ID;
    int                 m_iStageID;
    int                 m_iSuccess;

public:
    bool                m_bDeleteSubject;

private:
    std::wstring        m_strReward;

private:
    std::map<std::string, float>  m_mapTempValue;

public:
    const KGCSubjectTemplate*  m_pSubjectTemplate;
    KGCStringLoader*           m_pStrLoader;

public:// 과제 통계용 데이터
    int                 m_iBackAttackCnt;       // 백어택 카운트
    int                 m_iSpecialCnt;          // 필살기 카운트
    int                 m_iDamageHittedCnt;     // 받은 데미지 히트 카운트
    float               m_fDamageVolumeCnt;     // 받은 데미리 량 카운트
    int                 m_iDieCnt;              // 죽은 카운트
    bool                m_bComboBreak;          // 콤보 끊킴여부

private:
    int                 m_iProgressNow;
    int                 m_iProgressMax;

private:
    enum
    {
                        SUBJECT_ADDCHECKER_BACKATTACK,
                        SUBJECT_ADDCHECKER_SPECIAL,
                        SUBJECT_ADDCHECKER_DAMAGE_CNT,
                        SUBJECT_ADDCHECKER_DAMAGE_VOLUME,
                        SUBJECT_ADDCHECKER_DIE_CNT,
                        SUBJECT_ADDCHECKER_COMBO_BREAK,
                        MAX_SUBJECT_ADDCHECKER,
    };
    bool                m_bAddChecker[MAX_SUBJECT_ADDCHECKER];
};