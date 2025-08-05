//---------------------------------------------------------------
//					사용법
//---------------------------------------------------------------
/*
REG_PACKET_HANDLER(PacketID, Class이름, Handler함수, TimeOut시간)

Handler함수의 형태는 void Func(DWORD, DWORD)여야함.
TimeOut의 시간 단위는 ms임. 0일경우, TimeOut 되지 않음.
*/
//---------------------------------------------------------------
// ToDo : UI의 메세지 처리 부분을 막자!
// 다른 Event가 발생 하면??? 대략 좋지 않을 듯...
//---------------------------------------------------------------

#pragma once

#include "Command.h"
#include <map>
#include <Mmsystem.h>
#include <assert.h>
#include <dbg/dbg.hpp>

//#define REG_PACKET_HANDLER(msgid, cls, func, timeout) g_PacketManager.RegisterHandler( msgid, new CCommandInvoker<##cls>(this, func ), timeout );
template < typename _T >
class CMessageManager
{
    struct stTimeOut
    {
        stTimeOut()
        {
            stTimeOut(0);
        }
        stTimeOut(DWORD dwTime)
        {
            m_dwRegTime = timeGetTime();
            m_dwTime = dwTime;
        }
        DWORD m_dwRegTime;		// 등록시간
        DWORD m_dwTime;			// 타임아웃 시간
    };
public:
    CMessageManager(void){ m_bLogEnable = true; }
    virtual ~CMessageManager(void){ ClearMessage(); }

    void RegisterHandler(_T dwMsgID,CCommand* pCommand, DWORD dwTimeOut = 0)
    {
        std::map<_T, CCommand*>::iterator itor = m_mapHandler.find(dwMsgID);

        if( itor != m_mapHandler.end() )
        {
            delete itor->second;
        }

        m_mapHandler[dwMsgID] = pCommand;

        SetTimeOut(dwMsgID, dwTimeOut);
    }

    void UnRegisterHandler( _T dwMsgID )
    {
        std::map<_T, CCommand*>::iterator itor = m_mapHandler.find(dwMsgID);

        if( itor == m_mapHandler.end() )
        {
            assert(!"Error! UnRegisterHandler");
            return;
        }

        delete itor->second;
        m_mapHandler.erase( itor );
    }

    int OnEvent( _T dwMsgID, void* pData, DWORD lParam2 = 0 )
    {
        std::map<_T, CCommand*>::iterator itor = m_mapHandler.find(dwMsgID);

        if( itor == m_mapHandler.end() )
        {
            //assert( !"Message Handler연결XXXXXXXXXXXX" );
            if( m_bLogEnable == true )
                START_LOG( cerr, L"해당 패킷이 등록되어 있지 않습니다." )
                << BUILD_LOG( dwMsgID )
                << BUILD_LOG( LOWORD( dwMsgID ) )
                << BUILD_LOG( HIWORD( dwMsgID ) );
            return -1;
        }		

        itor->second->Execute( false, (DWORD)pData, lParam2);

        //TimeOut Event 제거하기
        if( m_mapTimeOut.find(dwMsgID) != m_mapTimeOut.end() )
            m_mapTimeOut.erase( m_mapTimeOut.find(dwMsgID) );

        return 0;
    }

    void SetTimeOut( _T dwMsgID, DWORD dwTime )
    {
        if( dwTime == 0 )
            return;

        m_mapTimeOut[dwMsgID] = stTimeOut(dwTime);
    }

    bool OnTimeOut( _T	dwMsgID)
    {
        std::map<DWORD, CCommand*>::iterator itor = m_mapHandler.find(dwMsgID);
        assert( itor != m_mapHandler.end() );

        itor->second->Execute( true, 0, 0 );

        return true;
    }

    void FrameMove()
    {
        DWORD dwNowTime = timeGetTime();

        std::map<_T, stTimeOut>::iterator itor = m_mapTimeOut.begin();
        std::vector<_T> vecTimeOut;

        while(itor != m_mapTimeOut.end())
        {
            stTimeOut& timeout = itor->second;
            if( dwNowTime - timeout.m_dwRegTime > timeout.m_dwTime )
            {
                vecTimeOut.push_back(itor->first);
                // it's timeout!!!
                OnTimeOut(itor->first);
            }
            itor++;
        }

        // clear timeout...
        for(int i = 0; i < (int)vecTimeOut.size(); i++)
        {
            m_mapTimeOut.erase( m_mapTimeOut.find(vecTimeOut[i]) );
        }
    }

    void ClearMessage(){
        std::map<_T, CCommand*>::iterator itor = m_mapHandler.begin();
        for( itor = m_mapHandler.begin() ; itor != m_mapHandler.end() ; ++itor )
            delete itor->second;

        m_mapHandler.clear();
    }

    void SetLogEnable( bool bEnable ){ m_bLogEnable = bEnable; }

protected:
    bool m_bLogEnable;
    std::map<_T, CCommand*> m_mapHandler;
    std::map<_T, stTimeOut> m_mapTimeOut;
};

#define MSG_HANDLER(PacketID) void On_##PacketID(bool bTimeOut,DWORD lParam, DWORD lParam2)
#define MSG_HANDLER_CLS(Cls, PacketID) void Cls##::On_##PacketID(bool bTimeOut,DWORD lParam, DWORD lParam2)
