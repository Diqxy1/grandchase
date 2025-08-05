#ifndef _REPLAY_HEAD_H_
#define _REPLAY_HEAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TAG_CHAR_NUM 2

//#include "Gcui/KGCTagMatchSelect.h"
#include "Packet.h"
#include "Pet.h"
#include "KncP2PLib/RaknetSrc/BitStream.h"

static const float REPLAY_MINOR_VERSION = 9.0f;     //  최소 버전
static const float REPLAY_VERSION = 10.0f;          //  현재 버전

#define PET_EQUIP_ITEM_NUM 10
#define PLAYER_NAME_LENGTH 25

enum ERPSpeed   //Replay Speed
{
    RS_INVALID = -1,

    RS_STOP		= 0,
    RS_SLOW		= 1,
    RS_NORMAL	= 2,
    RS_FAST		= 3,
    RS_TOP		= 4,

    RS_NUM		= 5,
};

#pragma pack( push, 1 )
struct ReplayAPI_UserInfo
{
    WCHAR	m_strNick[30];
    int		m_iCharType;
    int		m_iPromotion;
    int		m_iCharLevel;
    int		m_iKill;
    int		m_iDeath;
    int		m_iRank;

    ReplayAPI_UserInfo()
    {
        swprintf( m_strNick, L"NULL" );
        m_iCharType = -1;
        m_iPromotion = -1;
        m_iCharLevel = -1;
        m_iKill = -1;
        m_iDeath = -1;
        m_iRank = -1;
    }
};

struct ReplayAPI_Header
{
    int		m_iMapID;
    int		m_iGameModeCategory;
    int		m_iGameMode;
    int		m_iSubGameMode;
    int		m_iPlayerTimeSecond;
    KSafeArray<ReplayAPI_UserInfo,MAX_PLAYER_NUM> m_sUser;

    ReplayAPI_Header()
    {
        m_iMapID = -1;
        m_iGameModeCategory = -1;
        m_iGameMode = -1;
        m_iSubGameMode = -1;
        m_iPlayerTimeSecond = -1;
    }
};

#pragma pack(pop)

struct ReplayPetInfo
{
    GCITEMUID                       m_dwUID;            // 해당 아이템 UID와 동일.
    DWORD                       m_dwID;             // 해당 펫의 아이템 ID와 동일
    WCHAR                       m_strName[PETNICKNAME]; // 펫 이름
    DWORD                       m_dwEXP;            // 펫 경험치
    int                         m_iLevel;           // 펫 레벨
    char                        m_cPromotion;       // 진화 상태 (기본 0)
    int                         m_iSatiation;       // 만복도(포만도) 초기값?
    KSafeArray<KSimpleItem,PET_EQUIP_ITEM_NUM> m_akEquipItem;  // 펫이 장착한 아이템
    DWORD						m_dwCostumeID;

    void Init()
    {
        m_dwUID = 0;
        m_dwID = 0;
        m_dwEXP = 0;
        m_iLevel = 0;
        m_cPromotion = 0;
        m_iSatiation = 0;
        m_dwCostumeID = 0;
        ZeroMemory( m_strName, sizeof( m_strName ) );

        KSimpleItem kTemp;
        kTemp.m_cItemType = 0;
        kTemp.m_dwUID = 0;
        kTemp.m_dwID = 0;

        for( int i = 0; i < PET_EQUIP_ITEM_NUM; i++ )
        {
            m_akEquipItem[i] = kTemp;
        }
    }
};

struct UserInfo_Ver09
{
    float	m_fStartX;
    float	m_fStartY;
    bool	m_bIsRight;
    int		m_iNowMotion;
    bool	m_bIsBlueTeam;
    bool	m_bIsLive;
    WCHAR	m_pPlayerName[PLAYER_NAME_LENGTH];
    int		m_iSelectChar;					            // 선택된 캐릭터
    KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,GC_CHAR_NUM> m_ppItemNum;	// 캐릭터별 장비 리스트
    KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,GC_CHAR_NUM> m_ppItemPatten;	// 캐릭터별 장비 리스트


    KSafeArray<ReplayPetInfo,GC_CHAR_NUM> m_akPet;			        // 캐릭터의 펫정보

    KSafeArray<int,TAG_CHAR_NUM> m_aiTagCharType;              // Tag할 케릭터
    KSafeArray<int,3> m_aiIndigoCharType;                      // 용사의 섬

    // [8/11/2008] breadceo. 왜 이렇게 싸질러 놨는지는 상구에게 문의를..
    KSafeArray<KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,NUM_JOB_LEVEL>,GC_CHAR_NUM> m_pppSkill; // 유저가 장착한 스킬

    void Init()
    {
        m_fStartX = 0.0f;
        m_fStartY = 0.0f;
        m_bIsRight = false;
        m_iNowMotion = 0;
        m_bIsBlueTeam = false;
        m_bIsLive = false;
        m_iSelectChar = 0;

        ZeroMemory( m_pPlayerName, sizeof( m_pPlayerName ) );

        for( int i=0 ; i<GC_CHAR_NUM ; ++i )
        {
            for( int j = 0 ; j < (int)m_ppItemNum[i].size() ; ++j )
            {
                m_ppItemNum[i][j] = -1;
            }
            for( int j = 0 ; j < (int)m_ppItemPatten[i].size() ; ++j )
            {
                m_ppItemPatten[i][j] = -1;
            }

            m_akPet[i].Init();
        }

        for( int i = 0 ; i < (int)m_aiTagCharType.size() ; ++i )
        {
            m_aiTagCharType[i] = -1;
        }

        for( int i = 0; i < GC_CHAR_NUM; ++i )
        {
            for( int j = 0; j < NUM_JOB_LEVEL; ++j )
            {
                for( int k = 0 ; k < (int)m_pppSkill[i][j].size() ; ++k )
                {
                    m_pppSkill[i][j][k] = -1;
                }
            }
        }
    }

};

struct UserInfo
{
    float	m_fStartX;
    float	m_fStartY;
    bool	m_bIsRight;
    int		m_iNowMotion;
    bool	m_bIsBlueTeam;
    bool	m_bIsLive;
    WCHAR	m_pPlayerName[PLAYER_NAME_LENGTH];
    int		m_iSelectChar;					            // 선택된 캐릭터
    KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,GC_CHAR_NUM> m_ppItemNum;	// 캐릭터별 장비 리스트
    KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,GC_CHAR_NUM> m_ppItemPatten;	// 캐릭터별 장비 리스트
    KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,GC_CHAR_NUM> m_ppLookItemNum;	// 캐릭터별 코디 리스트


    KSafeArray<ReplayPetInfo,GC_CHAR_NUM> m_akPet;			        // 캐릭터의 펫정보

    KSafeArray<int,TAG_CHAR_NUM> m_aiTagCharType;              // Tag할 케릭터
    KSafeArray<int,3> m_aiIndigoCharType;                      // 용사의 섬

    // [8/11/2008] breadceo. 왜 이렇게 싸질러 놨는지는 상구에게 문의를..
    KSafeArray<KSafeArray<KSafeArray<int,MAX_EQUIP_NUM>,NUM_JOB_LEVEL>,GC_CHAR_NUM> m_pppSkill; // 유저가 장착한 스킬

    void Init()
    {
        m_fStartX = 0.0f;
        m_fStartY = 0.0f;
        m_bIsRight = false;
        m_iNowMotion = 0;
        m_bIsBlueTeam = false;
        m_bIsLive = false;
        m_iSelectChar = 0;

        ZeroMemory( m_pPlayerName, sizeof( m_pPlayerName ) );

        for( int i=0 ; i<GC_CHAR_NUM ; ++i )
        {
            for( int j = 0 ; j < (int)m_ppItemNum[i].size() ; ++j )
            {
                m_ppItemNum[i][j] = -1;
            }
            for( int j = 0 ; j < (int)m_ppItemPatten[i].size() ; ++j )
            {
                m_ppItemPatten[i][j] = -1;
            }

            for ( int j = 0; j < (int)m_ppLookItemNum[i].size(); ++j ) 
            {
                m_ppLookItemNum[i][j] = -1;
            }

            m_akPet[i].Init();
        }

        for( int i = 0 ; i < (int)m_aiTagCharType.size() ; ++i )
        {
            m_aiTagCharType[i] = -1;
        }

        for( int i = 0; i < GC_CHAR_NUM; ++i )
        {
            for( int j = 0; j < NUM_JOB_LEVEL; ++j )
            {
                for( int k = 0 ; k < (int)m_pppSkill[i][j].size() ; ++k )
                {
                    m_pppSkill[i][j][k] = -1;
                }
            }
        }
    }

    void Assign( const UserInfo_Ver09& src )
    {
        m_fStartX = src.m_fStartX;
        m_fStartY = src.m_fStartY;
        m_bIsRight = src.m_bIsRight;
        m_iNowMotion = src.m_iNowMotion;
        m_bIsBlueTeam = src.m_bIsBlueTeam;
        m_bIsLive = src.m_bIsLive;
        m_iSelectChar = src.m_iSelectChar;

        wcscpy_s(m_pPlayerName, PLAYER_NAME_LENGTH, src.m_pPlayerName ); 

        m_ppItemNum = src.m_ppItemNum;
        m_ppItemPatten = src.m_ppItemPatten;
        m_akPet = src.m_akPet;
        m_aiTagCharType = src.m_aiTagCharType;
        m_pppSkill = src.m_pppSkill;        
    }
};


struct Replay_Head_Data_Ver09
{
    float				m_fVersion;
    // 게임 시작 모드 관련 정보들
    short				m_sGameModeCategory;
    short				m_sGameMode;
    short				m_sSubGameMode;
    unsigned char		m_ucStage;
    int					m_iStory;
    KSafeArray<short,MAX_NUM_MON_SLOT> m_sMonsterSlot;
    KSafeArray<UserInfo_Ver09,MAX_PLAYER_NUM> m_pUserInfo;

    void Init()
    {
        m_fVersion = 9.0f;
        m_sGameModeCategory = 0;
        m_sGameMode = 0;
        m_sSubGameMode = 0;
        m_ucStage = 0;
        m_iStory = 0;

        for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
            m_pUserInfo[i].Init();
    }
};

struct Replay_Head_Data
{
    float				m_fVersion;
    // 게임 시작 모드 관련 정보들
    short				m_sGameModeCategory;
    short				m_sGameMode;
    short				m_sSubGameMode;
    unsigned char		m_ucStage;
    int					m_iStory;
    KSafeArray<short,MAX_NUM_MON_SLOT> m_sMonsterSlot;
    KSafeArray<UserInfo,MAX_PLAYER_NUM> m_pUserInfo;

    void Init()
    {
        m_fVersion = REPLAY_VERSION;
        m_sGameModeCategory = 0;
        m_sGameMode = 0;
        m_sSubGameMode = 0;
        m_ucStage = 0;
        m_iStory = 0;

        for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
            m_pUserInfo[i].Init();
    }

    void Assign( const Replay_Head_Data_Ver09& src )
    {
        m_sGameModeCategory = src.m_sGameModeCategory;
        m_sGameMode = src.m_sGameMode;
        m_sSubGameMode = src.m_sSubGameMode;
        m_ucStage = src.m_ucStage;
        m_iStory = src.m_iStory; 
        m_sMonsterSlot = src.m_sMonsterSlot;

        for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
        {
            m_pUserInfo[i].Assign ( src.m_pUserInfo[i] );
        }
    }
};


struct Replay_PlayerFrameData
{
    char			MyNumber;
    unsigned char   Super;
    //unsigned char   ucNotSpecialSuper;

    char		    Life;
    char            Shield;    
    bool		    IsRight;

    FLOAT		    x;
    float		    y;
    float		    HP;
    float		    Body_Angle;

    unsigned short   Frame;
    UINT            Now_Motion;
    int				iKills;
    int				iDeaths;
    int				m_iSelectChar;
    bool			m_bRender;
    int				MetamorphosisForm;

    bool            m_bRenderWeapon;
    bool            m_bRenderMergeWeapon;
    char            m_cDrawSlashRight;
    char            m_cDrawSlashLeft;

    enum SLASH_ENUM { SE_NODRAW = 0, SE_WEAPON = 2, SE_LEG = 4, SE_ALL = 8 };

    void Init()
    {
        MyNumber = -1;
        Super = 0;
        Life = 0;
        Shield = 0;
        IsRight = false;
        x = 0.0f;
        y = 0.0f;
        HP = 0.0f;
        Body_Angle = 0.0f;
        Frame = 0;
        Now_Motion = 0;
        iKills = 0;
        iDeaths = 0;
        m_iSelectChar = 0;
        m_bRender = false;
        MetamorphosisForm = FORM_NORMAL;
        m_bRenderWeapon = false;
        m_bRenderMergeWeapon = false;
        m_cDrawSlashRight = SE_NODRAW;
        m_cDrawSlashLeft = SE_NODRAW;
    }

    void SetSlashOnOff( bool bWeapon, bool bLeg, char *cValue )
    {
        if( bWeapon && bLeg )
        {
            (*cValue) = SE_ALL;
            return;
        }

        if( bWeapon )
        {
            (*cValue) = SE_WEAPON;
            return;
        }

        if( bLeg )
        {
            (*cValue) = SE_LEG;
            return;
        }

        if( false == bWeapon && false == bLeg )
        {
            *cValue = SE_NODRAW;
            return;
        }

        *cValue = SE_WEAPON;
        return;
    }

    bool IsSlashOnLeft( SLASH_ENUM enSE )
    {
        if( (int)enSE == m_cDrawSlashLeft
            || (int)Replay_PlayerFrameData::SE_ALL == m_cDrawSlashLeft )
            return true;

        return false;
    }

    bool IsSlashOnRight( SLASH_ENUM enSE )
    {
        if( (int)enSE == m_cDrawSlashRight
            || (int)Replay_PlayerFrameData::SE_ALL == m_cDrawSlashLeft )
            return true;

        return false;
    }
};

struct Replay_MonsterFrameData
{
    char            MyNumber;
    char            Now_Monster;

    float           HP;

    bool            IsRight;
    char            Now_Motion;
    unsigned char   Frame;
    float           x, y;

    bool            Render_IsRight;
    char            Render_Now_Motion;
    unsigned char   Render_Frame;
    float           Render_x, Render_y;

    // 몬스터의 능력치
    float           m_fAtk_Point;
    float           m_fDef_Point;

    void Init()
    {
        MyNumber = -1;
        Now_Monster = -1;
        HP = 0.0f;
        IsRight = false;
        Now_Motion = 0;
        Frame = 0;
        x = 0.0f;
        y = 0.0f;
        Render_IsRight = false;
        Render_Now_Motion = 0;
        Render_Frame = 0;
        Render_x = 0.0f;
        Render_y = 0.0f;
        m_fAtk_Point = 0.0f;
        m_fDef_Point = 0.0f;
    }
};

struct Replay_ChatFrameData
{
    int		m_TimeFrame;		//몇 프레임에 문자가 찍혔나
    WCHAR   m_strChatData[300];	//채팅 데이터
};

struct Replay_ItemFrameData 
{
    int		m_TimeFrame;				//몇 프레임에 아이템이 작동이 되었나
    char	m_cUser;					//아이템 사용한 유저

    KSafeArray<EFFITEM,MAX_PLAYER_NUM> Eff_Item;	//아이템 번호
};

struct Replay_PetSkillFrameData
{
    int		m_TimeFrame;				// 몇 프레임에 펫 스킬을 사용했나
    char	m_cPlayer;					// 스킬을 사용한 플레이어

    float	m_fX;
    float	m_fY;
    bool	m_bIsRight;
    int		m_cMotion;
};

struct Replay_TagMatchFrameData
{
    int     m_TimeFrame;
    bool    m_bArena;                   // 아레나 된 것인가?
    char    m_cPlayer;                  // 태그한 플레이어 인덱스
};

struct Replay_GameBoardData
{
    int     m_TimeFrame;
    //UCHAR                       aHow_Kill[MAX_PLAYER_NUM];
    //UCHAR 	                    aHow_Die[MAX_PLAYER_NUM];
    KSafeArray<UCHAR,TEAM_NUM> aTeamScore;

};

struct Replay_HeadData_Compress_Header
{
    unsigned long ulCompressedSize;
    DWORD   dwRealSize;

    Replay_HeadData_Compress_Header()
    {
        ulCompressedSize = 0;
        dwRealSize = 0;
    }
};

struct Replay_FrameData_Compress_Header
{
    unsigned long ulCompressedSize;
    DWORD   dwRealSize;

    Replay_FrameData_Compress_Header()
    {
        ulCompressedSize = 0;
        dwRealSize = 0;
    }
};


#endif