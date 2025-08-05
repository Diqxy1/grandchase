#ifndef _GCSTATEMACHINE_H_
#define _GCSTATEMACHINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include <string>
//#include "GCGlobal.h"
#include "ProtectedType.h"
#include "KGCState.h"
//#include <list>
//#include "GCEnum.h"
#ifdef _ENUM
    #undef _ENUM
    #include "GCStateDef.h"
#else
    #include "GCStateDef.h"
#endif
#include "knc/ClientErr.h"

enum ESafeServerState
{
    SSS_CHANNEL_LOBBY = 5,
    SSS_CHANNEL = 6,
    SSS_ROOM = 7,

};

class KGCStateMachine
{
public:
    KGCStateMachine( void );
    ~KGCStateMachine( void );

public:
    KGCState* GetStatePtr( GAME_STATE eState ){ return m_mapState[eState]; }
    GAME_STATE GetState( void ) const;
    std::string GetStateString();
    std::string GetStateString( GAME_STATE eState );
    std::string Dump();
    void SetCurrentState( GAME_STATE eGameState );
    void SavePrevState( GAME_STATE ePrevState ){ m_ePrevRoomState = ePrevState; }
    std::string GetCurrentLocationIcon();
    std::wstring GetCurrentLocation();

    HRESULT FrameMove( float );
    HRESULT Render( void );

    int GetRoomNumber(void) { return m_iCurrRoomNum; }

    bool GoLogo( void );
    bool GoChannel( void );
    bool GoGuild( void );
    bool GoCashShop( void );
#ifndef DISABLE_MAP_SHOP // Kawan>
    bool GoGPShop( void );
#endif
    bool GoRoom( int iRoomNum, const std::wstring& strRoomName, const std::wstring& strRoomPassword );
    bool GoRoom( void );
    void ExitRoom( bool bSendToServer, UINT uiError_ = KFailRate::P2P_NO_ERROR, bool bSubtractRP = false, bool bStateChange = true );
    bool GoGame( void );
    bool GoMyInfo( void );
    bool GoMyInfoFromRoom( void );
	bool GoCharSelectFromRoom( void );
    bool GoPrev( void );
    bool GoLoading( void );
    bool GoLogin( void );
    bool GoRegister( void );
	bool GoIntro( void );
    bool GoSquareLoading( void );
    bool GoSquare( void );
    bool GoTutorial( int iChar_ = -1 );
    bool GoReplay();
    bool GoQuestZero( int iChar_ = 0 );
    bool GoServer();
	bool GoCharSelect();
	bool GoCharGain();
    bool GoMatch( DWORD dwChannelUID = 0xFFFFFFFF );
    bool GoWorldMap( void );
    void GoExitGame( KGCGlobal::GAME_EXIT_TYPE eType, int nSockError );
    
    void GoExitClient();
    bool IsEnableExitClient();

#if defined USE_COORDI_SHOP
    bool GoCoordiShop( void );
#endif

    bool GoState( GAME_STATE dwState_, bool bValidState_ = true );
    bool JumpToSafeState( GAME_STATE eState_ );

    ESafeServerState GetSafetyState( GAME_STATE eState_ );

    // 방에서 강제로 나옴
    void OnExitGame();
    void BindToLua();


    bool IsStateRoom();
    bool IsStateMyInfo();
	bool IsStateCharSelect();


protected:
    bool                    m_bJoinedChannel;
    bool                    m_bJoinedGuild;
public:
    void    SetJoinChannel(bool bJoin_ )    { m_bJoinedChannel = bJoin_; }
    void    SetJoinGuild(bool bJoin_ )        { m_bJoinedGuild = bJoin_; }
    bool    IsJoinedChannel()    {return m_bJoinedChannel; }
    bool    IsJoinedGuild()        {return m_bJoinedGuild; }

protected:
    std::list<GAME_STATE>    m_lSavedSate;            //이전상태저장용 리스트
public:
    void Pushback_SavedState( GAME_STATE eState )    { m_lSavedSate.push_back( eState ); }
    void Pushfront_SavedState( GAME_STATE eState )    { m_lSavedSate.push_front( eState ); }
    void Clear_SavedState()                            { m_lSavedSate.clear(); }
    void Set_SavedState( const std::list<GAME_STATE>& lState ) { m_lSavedSate = lState; }
    std::list<GAME_STATE> Get_SavedState() const { return m_lSavedSate; }
    bool SaveStateEmpty() const { return m_lSavedSate.empty(); }

    void SetGameState(const GAME_STATE eGameState_ ) { 
        KLocker lockcheck(m_csForGameState);
        m_eGameState = eGameState_; 
    }

protected:
    KProtectedType< GAME_STATE >    m_eGameState;
    KProtectedType< GAME_STATE >    m_ePrevRoomState;   //방에 들어오기 전 상태
    
    CRITICAL_SECTION                m_csForGameState;

    GAME_STATE                m_ePrevGameState;
    GAME_STATE                m_ePrevChattingState;       // 채팅창이 있던 이전 스테이트
    int                        m_iCurrRoomNum;
    KGCState*               m_pCurrState;
    std::map<DWORD, KGCState*> m_mapState;

    enum KIND_OF_RESOURCE{
        KOR_UI,
        KOR_SOUND,
    };    

protected:
    DWORD m_dwStayTime;
    std::vector< std::pair<char, unsigned short> > m_vecSaveUserPath; // 유저의 행적을 기록하기 위한 벡터
public:
    void SaveUserPath();
    std::vector< std::pair<char, unsigned short> >& GetUserHistory() 
    {
        SaveUserPath();
        return m_vecSaveUserPath; 
    }
    GAME_STATE GetPrevGameState() {return m_ePrevGameState; }
    GAME_STATE GetPRevChattingState() { return m_ePrevChattingState; }

    bool BeginScene();
    void EndScene();
};

#endif // _GCSTATEMACHINE_H_
