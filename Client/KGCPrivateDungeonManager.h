#ifndef _KGCPRIVATE_DUNGEON_H_
#define _KGCPRIVATE_DUNGEON_H_
#pragma once


#include "KNCSingleton.h"

enum EMMoveStateType
{
    EM_MOVE_STATE_NONE = -1,
    EM_CREATE_ROOM_TYPE = 0,
    EM_START_GAME_TYPE = 1,
    EM_EXIT_ROOM_TYPE = 3,
    EM_CLEAR_DUNGEON = 4,       //  졸업여부 체크
    EM_CLEAR_NEXT_LEVEL = 5,    //  전용던전 클리어시 다음 난이도 선택
    EM_MOVE_STATE_SIZE,
};

struct SCharPrivateDungeon
{
public:
    int m_nCharType;
    EGCGameMode m_emStartDungeon;
    EGCWorldMap m_emWorldMap;
    
    bool m_bStartDungeonCheck;
    
    std::map<EGCGameMode, std::vector<int>> m_mapPrivateDungeon;

public:
    SCharPrivateDungeon()
    {
        Init();
    }

    ~SCharPrivateDungeon(){}

    void Init()
    {
        m_nCharType = GC_GM_INVALID;
        m_emStartDungeon = GC_GM_INVALID;
        m_emWorldMap = NUM_WORLDMAP;
        m_bStartDungeonCheck = false;
        m_mapPrivateDungeon.clear();
    }
};


class KGCPrivateDungeonManager
{
    DeclareSingleton( KGCPrivateDungeonManager );

public:
    // 캐릭터 전용 던전 정보( 캐릭터 id, 던전id, 난이도 )
    std::map<int, SCharPrivateDungeon>  m_mapPrivateDungeonInfo;
    // 캐릭터 전용 던전 정보( 던전id, 캐릭터 타입 )
    std::map<EGCGameMode, int>          m_mapPrivateDungeonList;

    // 미졸업한 캐릭터 정보( 전용던전을 모두 클리어하지 않았다. )
    std::set<int>                       m_setNoClearChar;

    //  졸업 보상 ( 임시 저장 )
    std::vector<KItem>                  m_vecRewardItem;


    EMMoveStateType m_emState;
    bool            m_bNextLevel;   //  전용던전 다음 난이도 선택

public:
    KGCPrivateDungeonManager();
    ~KGCPrivateDungeonManager();

    void Init();
    bool LoadScript( KLuaManager& luaMgr );

public:
    void SetNoClearChar( std::set<int>& setNoClearChar ) { m_setNoClearChar = setNoClearChar; }
    void DelNoClearChar( int nCharType );

    //  캐릭터 전용 던전인가?
    bool IsPrivateDungeon( EGCGameMode eGameMode_ );        
    // 누구의 전용던전인지 확인
    int GetPrivateDungeonChar( EGCGameMode eGameMode_ );
    //  해당 캐릭터가 전용 던전이 있는 캐릭터 인지 체크    
    bool IsHavePrivateDungeon( int iCharID );
    //  해당 캐릭터가 전용 던전을 졸업했는지 체크.( 임시로 쓸 예정. 나중에 서버 올릴 내용 )
    bool IsClearPrivateDungeon( int iCharID );

    bool IsFirstDungeonChar( int iCharID );
    void SetFirstDungeonChar( int iCharID, bool bSet );

    //  던전 최초 플레이인지 확인한다.
    void CheckFirstDungeon( int iCharID );

    //  졸업 보상 아이템 지정.
    void SetClearRewardItem( std::vector<KItem>& vecRewardItem );
    void GraduateProcess();

    bool IsNextLevel() { return m_bNextLevel; }
    void SetNextLevel( bool bNextLevel ) { m_bNextLevel = bNextLevel; }
    void CheckNextLevel();

    EGCGameMode GetStartDungeon( int iCharID );
    EGCWorldMap GetMoveWorldMap( int iCharID );

    void MoveStateCondition( EMMoveStateType emState );
    void MoveStateRun( EMMoveStateType emState );
    void SetMoseState( EMMoveStateType emState ) { m_emState = emState; }
};

DefSingletonInline( KGCPrivateDungeonManager );

#endif