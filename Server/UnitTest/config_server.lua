fr = GetFailRate()
fr:SetWriteToDB( false )

SimLayer = GetSimLayer()
SimLayer.m_uiNickNameAvailable = 12
SimLayer.m_dwShutdownTimeLimit = 60 * 1000  -- 종료시 db 처리 대기시간 : 1분.
SimLayer.m_iMaxGamePoint        = 499999
SimLayer.m_dwIndigoLevel        = 0
SimLayer.m_bRecommendKREnable 	= false
SimLayer.m_bWriteClientInfo		= true -- 클라이언트 사양 조사

-- 상하이 드랍 적용 가입일.
SimLayer:SetSHAdhustTime( 2007, 04, 30, 09, 30 )

--출석부 시스템
SimLayer.m_dwAccUpdateTime 	    = 60 * 1000 -- 달력 갱신 시간 ms 
SimLayer.m_nAttendTime   	    = 70 -- 출석 인정 시간, 단위:분
SimLayer.m_nGCPointDay		    = 100 -- 일일 출석시 지급되는 지씨포인트(임시)
SimLayer.m_nGCPointWeek		    = 100 -- 주개근시 지급되는 지씨포인트(임시)
SimLayer.m_nGCPointRecommend    = 700 -- 추천 했을때 보상으로 주는 포인트


--신규 유저 강제.
SimLayer:SetForceNewUser( true, 2020, 3, 22 )
SimLayer:ClearForceMode()
SimLayer:AddForceMode( 26 ) -- Quest Zero
SimLayer:AddForceMode( 29 ) -- tutorial
SimLayer:SetTableChecksum( 0 )


SimLayer:AddAuthTypeFlag( KSimLayer.AT_NETMARBLE )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_DEBUG_AUTH )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_INTERNAL_TESOP  + KGSSimLayer.ST_GUILD ) -- + KGSSimLayer.ST_BROADCASTING )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_NPGG_ON + KGSSimLayer.ST_NPGG_KILL )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_BROADCASTING )
--SimLayer:AddTypeFlag( KGSSimLayer.ST_GUILD )
-- NetLayer의 속성은 초기화시 한번만 유효하다. 이후 콘솔에서 수정될 수 없다.
NetLayer = GetNetLayer()
NetLayer.m_usPortMaster = 9400
NetLayer.m_usUdpPort    = 9401


NetLayer:AddURServerAddress( '59.25.186.181', 9600 )
NetLayer:AddURServerAddress( '59.25.186.181', 9610 )
NetLayer:AddTRServerAddress( '127.0.0.1', 9700 ) --로그남기지 않기위해 

CenterProxy = GetCenterProxy()
--CenterProxy:SetRemoteAddress( '218.153.10.201', 9500 )

DBLayer   = GetDBLayer()
DBLayer:Init_ODBC( 'odbc_internal.dsn', 'odbc_stat.dsn' )
DBLayer:Init( 2 )

function msg( m )
    ChannelManager:Send_NotMsg( m )
    print( m )
end

Adv = KPCBangAdvantage()
Server = GetGameServer()

Adv:EnablePCBang( true )
for itemid = 17020, 17220, 10 do -- 기사, 궁수 법사용 pc방 복장셋트(총 21개).
    Adv:AddNormal( itemid )
end
Adv:AddDuration( 17510, 30 ) -- 수호의 반지(30회)

Adv:SetMsg( 'str_00' )
Server:AddPublisherPCBangAdv( KSimLayer.AT_NETMARBLE, Adv, '__odbcNMPCBang' ) -- 넷마블 PC방 설정

Adv:SetMsg( 'str_08' )
Server:SetSelfPCBangAdv( Adv, '__odbcGCPCBang' )      -- GC PC방 설정

DBLayer:AddODBC( '__odbcNMPCBang', 'odbc_internal_pcbang.dsn' )
DBLayer:AddODBC( '__odbcGCPCBang', 'odbc_internal_pcbang.dsn' )

-- 어뷰저 방지.
SetHackingProtectionMode( true, true, 30000 )

log( 2 ) -- loglevel : 0 ~ 2
Server:ClearChecksum()

Server:AddChecksum( 0 )             -- 051108. 98에서는 클라이언트가 체크섬을 제대로 구하지 못한다!


if( SimLayer:CheckTypeFlag( KGSSimLayer.ST_INDIGO ) ) then -- 용사의섬 서버인 경우

    tm = GetOpenTime()
    tm:SetEnable( true )
    
end

-- 통계
StatManager = GetStatisticsManager()
StatManager:AddStatistics( KStatisticsManager.SI_CPL, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_PLAY_TIME, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_MAP_COUNT, 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_LOGINOUT_STAT, 30 * 60 * 1000, false, 10 ) -- tslayer
StatManager:AddStatistics( KStatisticsManager.SI_STAT_CHECK_IN, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_P2P_CRITICAL, 60 * 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_MINIGAME, 3 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_LEAVE_GAME, 3 * 60 * 1000, true, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAMELOADING, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DEATH, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_SPX, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DISCONNECT, 60 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_COUPLE_ROOM, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_GAME_START_GAP, 30 * 1000, false, 10 )
StatManager:AddStatistics( KStatisticsManager.SI_STAT_DUNGEON, 60 * 1000, true, 10 )