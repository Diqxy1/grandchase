#pragma once
#include "SimLayer.h"
#include "MRPacket.h"

class KMRSimLayer : public KSimLayer
{
    DeclareSingleton( KMRSimLayer );
    NiDeclareRTTI;
    DeclToStringW;

public:
    struct KMSUserInfo {
        DWORD           m_dwUserUID;
        std::wstring    m_strMsgServer;

        CON_COPYCON_ASSIGNOP( KMSUserInfo, right )
        {
            m_dwUserUID     = right.m_dwUserUID;
            m_strMsgServer  = right.m_strMsgServer;

            return *this;
        }
    };

    KMRSimLayer(void);
    virtual ~KMRSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

    bool DestroyAllUsers();

    bool GetMSUserInfo( IN DWORD dwUID, OUT KMSUserInfo& kInfo );
    bool AddMSUserInfo( IN const KMSUserInfo& kInfo );
    bool DeleteMSUserInfo( IN DWORD dwUID );

    size_t GetMSUserSize() { return m_mapMSUser.size(); }

public: // for lua..
    DWORD               m_dwShutdownTimeLimit;  // 060113. florist. 서버 종료시 DB처리를 위한 대기시간.

protected:
    std::map<DWORD,KMSUserInfo>         m_mapMSUser;
    std::vector<std::string>            m_vecMSList;
};

DefSingletonInline( KMRSimLayer );