#include "StdAfx.h"
#include "TGSubject.h"
#include "MyD3D.h"


CTGSubject::CTGSubject(void)
{
    enType              = TG_SUBJECT;
    InitData();
}

CTGSubject::~CTGSubject(void)
{
}

void CTGSubject::InitData(void)
{
    m_vecConCharType.clear();
    m_vecConLevel.clear();
    m_vecConJobLevel.clear();
    m_vecSubject.clear();
}

void CTGSubject::FrameMove(void)
{
    if( IsComplete() == true ) return;
	
    if( false == m_bBeginTG )
    {
        m_bBeginTG = true;
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        LoadSubject();
        if( !m_bSendPacket )
        {
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        LoadSubject();
    }
}

void CTGSubject::LoadSubject()
{
    if( m_vecSubject.empty() )
        goto END_TG_SUBJECT;

    int iMyPlayer = g_MyD3D->Get_MyPlayer();
    if( false == g_MyD3D->IsPlayerIndex(iMyPlayer) )
    {
        goto END_TG_SUBJECT;
    }

    PLAYER *pPlayer = g_MyD3D->MyPlayer[ iMyPlayer ];
    SCharInfo& sCharInfo = pPlayer->GetCurrentChar();

    // 캐릭터 타입 검사
    if( false == m_vecConCharType.empty() )
    {
        std::vector<int>::iterator vecIter = m_vecConCharType.begin();
        for(; vecIter != m_vecConCharType.end(); ++vecIter )
        {
            if( sCharInfo.iCharType == (*vecIter) )
                break;
        }

        if( vecIter == m_vecConCharType.end() )
            goto END_TG_SUBJECT;
    }
    
    // 캐릭터 레벨 검사
    if( false == m_vecConLevel.empty() )
    {
        std::vector<int>::iterator vecIter = m_vecConLevel.begin();
        for(; vecIter != m_vecConLevel.end(); ++vecIter )
        {
            if( sCharInfo.iLevel == (*vecIter) )
                break;
        }

        if( vecIter == m_vecConLevel.end() )
            goto END_TG_SUBJECT;
    }

    // 캐릭터 전직레벨 검사
    if( false == m_vecConJobLevel.empty() )
    {
        std::vector<int>::iterator vecIter = m_vecConJobLevel.begin();
        for(; vecIter != m_vecConJobLevel.end(); ++vecIter )
        {
            if( sCharInfo.iPromotionLevel == (*vecIter) )
                break;
        }

        if( vecIter == m_vecConJobLevel.end() )
            goto END_TG_SUBJECT;
    }

    // 과제 선택
    {
        /*==================================================================================
            < 기획요청 >
            설정된 확률에 따라 과제를 선택하되,!!!!!
            이전 과제와 동일한 과제는 제외하고,
            하나도 선택되지 않았다면 확률 무시하고 하나는 실행되도록 해달라...
        ====================================================================================*/

        // 확률에 따라 과제를 선택하자!( 이전 과제는 제외되도록 하고!! )
        int iSubjectType = SelectSubject( SiKGCSubjectManager()->m_dwBeforeSubject );

        // 선택된 과제가 없다면!!! 확률이 어쨋든 하나는 실행되도록!!
        int iSubjectNum = static_cast<int>(m_vecSubject.size());
        if( (-1 == iSubjectType) && (0 < iSubjectNum) )
        {
            int iLoop=0;
            for(int iLoop=1; iLoop<static_cast<int>(m_vecSubject.size()); ++iLoop )
            {
                int iRandIndex = (rand()%iSubjectNum);
                if( m_vecSubject[iRandIndex].first == SiKGCSubjectManager()->m_dwBeforeSubject )
                    continue;
                iSubjectType = m_vecSubject[iRandIndex].first;
                break;
            }

            if( -1 == iSubjectType )
                iSubjectType = m_vecSubject[0].first;
        }

        // 휴.. 이제 과제 생성하자!!
        KGCSubject* pSubject = SiKGCSubjectManager()->CreateSubject( iSubjectType, iMyPlayer );
        if( pSubject )
        {
            pSubject->SetSubjectTGID( m_iTG_ID );
            pSubject->SetSubjectStageID( g_kEveryTriggerManager.GetActiveStage() );
            SiKGCSubjectManager()->m_dwBeforeSubject = iSubjectType;
        }
    }

END_TG_SUBJECT:
    SetComplete( true );
    return;
}

int CTGSubject::SelectSubject( int iNoSelectID )
{
    int iResult = -1;
    std::vector<std::pair<int, int>>::iterator vecIter = m_vecSubject.begin();
    for(; vecIter != m_vecSubject.end(); ++vecIter)
    {
        float fRadio = (vecIter->second / 100.0f);
        if( (randf() <= fRadio) && 
            (iNoSelectID != vecIter->first) )
        {
            iResult = vecIter->first;
            break;
        }
    }

    return iResult;
}

HRESULT CTGSubject::LoadItemFromLua(KLuaManager &luaMgr)
{
    InitData();

    if( SUCCEEDED(luaMgr.BeginTable( "Con_CharType" )) )
    {
        int iCharType = -1;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iCharType )); ++iLoop)
        {
            m_vecConCharType.push_back( iCharType );
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED(luaMgr.BeginTable( "Con_Level" )) )
    {
        int iLevel = -1;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iLevel )); ++iLoop)
        {
            m_vecConLevel.push_back( iLevel );
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED(luaMgr.BeginTable( "Con_JobLevel" )) )
    {
        int iJobLevel = -1;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iJobLevel )); ++iLoop)
        {
            m_vecConJobLevel.push_back( iJobLevel );
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED(luaMgr.BeginTable( "Subject" )) )
    {
        int iRadio = 100;
        int iSubjectType = 0;
        for(int iLoop=1; SUCCEEDED(luaMgr.BeginTable( iLoop )); ++iLoop)
        {
            LUA_GET_VALUE_DEF( 1, iSubjectType, 0 );
            LUA_GET_VALUE_DEF( 2, iRadio, 100 );
            m_vecSubject.push_back( std::make_pair(iSubjectType, iRadio) );
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }

    return S_OK;
}