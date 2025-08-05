#pragma once
#include "SimLayer.h"
#include "MatchSvrConnector.h"

class KMatchSimLayer : public KSimLayer
{
    DeclareSingleton( KMatchSimLayer );
    NiDeclareRTTI;
    DeclToStringW;

public:

    KMatchSimLayer(void);
    virtual ~KMatchSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua( boost::shared_ptr<lua_State> L );
    bool DestroyAllUsers();

public: // for lua..
    DWORD           m_dwShutdownTimeLimit;  // 060113. florist. 서버 종료시 DB처리를 위한 대기시간.
    DWORD           m_dwSendTimeLimit; // 070402. AGUser 에서 내 정보를 보고하는 시간 간격.
};

DefSingletonInline( KMatchSimLayer );