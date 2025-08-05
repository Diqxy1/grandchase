#include "StdAfx.h"
#include "KGCMissionProgressBox.h"
#include "KGCRoomManager.h"
#include "mission/GCMissionManager.h"

IMPLEMENT_CLASSNAME( KGCMissionProgressBox );
IMPLEMENT_WND_FACTORY( KGCMissionProgressBox );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionProgressBox, "gc_mission_progress" );

KGCMissionProgressBox::KGCMissionProgressBox(void)
{
    m_pkBack = NULL;
    m_pkTitleMissionBack = NULL;
    m_pkTitleSubjectBack = NULL;
    m_pkTitleSubMission = NULL;
    m_pkTitleSubject = NULL;
    m_pkStaticSubMission = NULL;
    m_pkStaticCount = NULL;
    m_pkStaticSubject = NULL;
    m_pkStaticEmpty = NULL;
    
	LINK_CONTROL( "background_main", m_pkBack );
    LINK_CONTROL( "back_titlemission", m_pkTitleMissionBack );
    LINK_CONTROL( "back_titlesubject", m_pkTitleSubjectBack );
    LINK_CONTROL( "titlemission", m_pkTitleSubMission );
    LINK_CONTROL( "titlesubject", m_pkTitleSubject );
    LINK_CONTROL( "static_submission", m_pkStaticSubMission );
    LINK_CONTROL( "static_missioncnt", m_pkStaticCount );
    LINK_CONTROL( "static_subject", m_pkStaticSubject );
    LINK_CONTROL( "static_empty", m_pkStaticEmpty );
}

KGCMissionProgressBox::~KGCMissionProgressBox(void)
{
}

int KGCMissionProgressBox::MakeSubjectList()
{
    // 달성과제 리스트 만들기
    int iNumSubject = 0;
    std::wstring strSubject;
    KGCStringLoader* pStringLoader = SiKGCSubjectManager()->GetStringLoader();
    const std::map<DWORD, KGCSubject*>& mapSubject = SiKGCSubjectManager()->GetAllSubjectInstance();
    std::map<DWORD, KGCSubject*>::const_iterator mapSubjectIter = mapSubject.begin();
    for(; mapSubjectIter != mapSubject.end(); ++mapSubjectIter)
    {
        KGCSubject* pSubject = mapSubjectIter->second;
        if( NULL == pSubject )
            continue;
        
        if( MAX_ARTICLE <= iNumSubject++ )
            break;
        
        m_pkStaticEmpty->SetForceWordWrapText( pStringLoader->GetString( pSubject->GetSubjectSimpleStringID() ), true );
        const std::wstring& strWordWrap = m_pkStaticEmpty->GetText();
        strSubject += strWordWrap;
        strSubject += L"\n";
    }

    if( 0 < iNumSubject )
    {
        m_pkStaticSubject->SetTextAutoMultiline( strSubject );
        //m_pkStaticTitleSubject->SetText( g_pkStrLoader->GetString( STR_ID_SUBJECT ) );
    }

    return iNumSubject;
}

int KGCMissionProgressBox::MakeMissionList()
{
    // 미션 리스트 만들기
    std::vector<std::pair<int, std::wstring>> vecComplete;
    std::vector<std::pair<int, std::wstring>> vecInComplete;
    std::vector<std::pair<int, std::wstring>> vecFail;

    if( !SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) &&
        SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST81 ) { 
        const std::vector< std::pair< int, int > >& vecVirtualCount = g_kGlobalValue.m_kUserMission.m_vecVirtualCount;
        std::vector< std::pair< int, int > >::const_iterator vecVirCntIter = vecVirtualCount.begin();
        for(int i = 0; vecVirCntIter != vecVirtualCount.end(); ++vecVirCntIter, ++i)
        {
            const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( vecVirCntIter->first );

            if( sSubMissionInfo == NULL )
                continue;

            bool bValid = false;
            bool bControlMission = false;
            int iControlType = -1;
            int iControlCompleteCount = 0;
            std::pair< int, int > pairControlCompleteCondition = std::make_pair( -1, -1 );
            PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
            for (int j = 0 ; j < (int)sSubMissionInfo->m_vecConditionID.size(); j++ )
            {
                const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo(sSubMissionInfo->m_vecConditionID[j]);
                if ( !sCondition )
                    continue;

                if( pPlayer && (sCondition->m_pairMonLevelRange.first >= 0) || (sCondition->m_pairMonLevelRange.second >= 0) )
                {
                    int iLowLevel, iHighLevel;
                    if( (sCondition->m_pairMonLevelRange.first >= 0) )
                        iLowLevel = pPlayer->GetCurrentChar().iLevel -  sCondition->m_pairMonLevelRange.first;
                    else
                        iLowLevel = 0;

                    if( (sCondition->m_pairMonLevelRange.second >= 0) )
                        iHighLevel = pPlayer->GetCurrentChar().iLevel + sCondition->m_pairMonLevelRange.second;
                    else
                        iHighLevel = MAX_LEVEL;

                    if( !g_MyD3D->m_pkQuestGameMgr->IsQuestLevelFromInputLevel( iLowLevel, iHighLevel, SiKGCRoomManager()->GetGameMode(), SiKGCRoomManager()->GetDungeonLevel() ) )
                        continue;
                }
                else if( !( sCondition->m_iGameMode == SiKGCRoomManager()->GetGameMode() &&
                    ( sCondition->m_pairDifficultRange.first == -1 ||
                    sCondition->m_pairDifficultRange.first <= SiKGCRoomManager()->GetDungeonLevel() )) )
                    continue;

                if ( sCondition->m_iType != KGCMissionManager::TYPE_NOUSE )
                {
                    bControlMission = true;
                    iControlType = sCondition->m_iType;

                    if ( sCondition->m_iType == KGCMissionManager::TYPE_USE_POTION || 
                        sCondition->m_iType == KGCMissionManager::TYPE_SKILL_COUNT )
                    {
                        pairControlCompleteCondition = sCondition->m_pairUseCount;
                        iControlCompleteCount = sCondition->m_pairUseCount.second;
                    }
                    else
                        iControlCompleteCount = sCondition->m_iValue;
                }

                bValid = true;
                break;		
            }

            if( bValid )
            {	
                WCHAR temp[MAX_PATH] = { 0,};
                int iMissionState = EM_INCOMPLETE;

                if ( bControlMission )
                {
                    int iCount = 0;                

                    switch( iControlType )
                    {
                    case KGCMissionManager::TYPE_RANK:
                        iMissionState = EM_NON;
                        break;
                    case KGCMissionManager::TYPE_USE_POTION:
                        iCount = pPlayer->GetControlInfoPotionCount( pairControlCompleteCondition.first );
                        if ( iCount == -1 )
                            iMissionState = EM_NON;
                        else if ( iCount > iControlCompleteCount )
                            iMissionState = EM_FAIL;
                        break;
                    case KGCMissionManager::TYPE_BEATTACK:
                        iCount = pPlayer->GetControlInfoBeAttackedCount();
                        if ( iCount > iControlCompleteCount )
                            iMissionState = EM_FAIL;
                        break;
                    case KGCMissionManager::TYPE_COMBO:
                        iCount = pPlayer->GetControlInfoComboCount();
                        if ( iCount >= iControlCompleteCount )
                            iMissionState = EM_COMPLETE;
                        break;
                    case KGCMissionManager::TYPE_BACK_ATTACK:
                        iCount = pPlayer->GetControlInfoBackAttackCount();
                        if ( iCount >= iControlCompleteCount )
                            iMissionState = EM_COMPLETE;
                        break;
                    case KGCMissionManager::TYPE_SKILL_COUNT:
                        iCount = pPlayer->GetControlInfoSkillCount( pairControlCompleteCondition.first );
                        if ( iCount == -1 )
                            iMissionState = EM_NON;
                        else
                            iMissionState = EM_INCOMPLETE;
                        break;
                    default:
                        iMissionState = EM_NON;
                        break;
                    }

                    DWORD dwMainMissionID = g_kGlobalValue.m_kUserMission.GetVirtualCountMainMissionID(i);
                    bool bComplate = g_kGlobalValue.m_kUserMission.IsCompleteMission(dwMainMissionID);
                    if ( bComplate )
                        iMissionState = EM_COMPLETE;

                    if ( iMissionState != EM_NON && !bComplate )
                        wsprintf( temp,L"%d/%d", iCount , iControlCompleteCount );
                }
                else
                {
                    wsprintf( temp,L"%d/%d", vecVirCntIter->second , sSubMissionInfo->m_sCompletion.m_iCount );

                    if( vecVirCntIter->second >= sSubMissionInfo->m_sCompletion.m_iCount )
                        iMissionState = EM_COMPLETE;
                    else
                        iMissionState = EM_INCOMPLETE;
                }

                switch( iMissionState )
                {
                case EM_COMPLETE:
                    vecComplete.push_back( std::make_pair( sSubMissionInfo->m_iTitleID , temp ) );
                    break;
                case EM_FAIL:
                    vecFail.push_back( std::make_pair( sSubMissionInfo->m_iTitleID , temp ) );
                    break;
                case EM_INCOMPLETE:
                case EM_NON:
                    vecInComplete.push_back( std::make_pair( sSubMissionInfo->m_iTitleID ,temp ) );
                    break;
                }
            }       
        }
    }

    // 완료되지 않은 미션 리스트 만들기
    int iNumMission = 0;
    std::wstring strMission;
    std::wstring strMissionCnt;
    std::vector<std::pair<int, std::wstring>>::iterator vecInComIter = vecInComplete.begin();
    for(; vecInComIter != vecInComplete.end(); ++vecInComIter)
    {
        if( 5 <= iNumMission++ )
            break;

        m_pkStaticEmpty->SetForceWordWrapText( SiKGCMissionManager()->GetString( vecInComIter->first ), true );
        const std::wstring& strWordWrap = m_pkStaticEmpty->GetText();
        strMission += strWordWrap;
        strMission += L"\n";

        m_pkStaticCount->SetForceWordWrapText( vecInComIter->second, true );
        const std::wstring& strWordWrapCnt = m_pkStaticCount->GetText();
        strMissionCnt += strWordWrapCnt;
        strMissionCnt += L"\n";
    }


    // 완료된 미션 리스트 만들기
    bool bChangeColor = false;
    std::wstring strColorKey    = L"#cffff00";
    std::wstring strColorKeyEnd = L"#cX";
    std::vector<std::pair<int, std::wstring>>::iterator vecComIter = vecComplete.begin();
    for(; vecComIter != vecComplete.end(); ++vecComIter)
    {
        if( 5 <= iNumMission++ )
            break;

        if( false == bChangeColor )
        {
            strMission += strColorKey;
            strMissionCnt += strColorKey;
            bChangeColor = true;
        }

        m_pkStaticEmpty->SetForceWordWrapText( SiKGCMissionManager()->GetString( vecComIter->first ), true );
        const std::wstring& strWordWrap = m_pkStaticEmpty->GetText();
        strMission += strWordWrap;
        strMission += L"\n";

        m_pkStaticCount->SetForceWordWrapText( vecComIter->second, true );
        const std::wstring& strWordWrapCnt = m_pkStaticCount->GetText();
        strMissionCnt += strWordWrapCnt;
        strMissionCnt += L"\n";
    }

    if( bChangeColor )
    {
        strMission += strColorKeyEnd;
        strMissionCnt += strColorKeyEnd;
    }

    // 실패한 미션 리스트 만들기
    bChangeColor = false;
    strColorKey = L"#cff0000";
    std::vector<std::pair<int, std::wstring>>::iterator vecFailIter = vecFail.begin();
    for(; vecFailIter != vecFail.end(); ++vecFailIter)
    {
        if( 5 <= iNumMission++ )
            break;

        if( false == bChangeColor )
        {
            strMission += strColorKey;
            strMissionCnt += strColorKey;
            bChangeColor = true;
        }

        m_pkStaticEmpty->SetForceWordWrapText( SiKGCMissionManager()->GetString( vecFailIter->first ), true );
        const std::wstring& strWordWrap = m_pkStaticEmpty->GetText();
        strMission += strWordWrap;
        strMission += L"\n";

        m_pkStaticCount->SetForceWordWrapText( vecFailIter->second, true );
        const std::wstring& strWordWrapCnt = m_pkStaticCount->GetText();
        strMissionCnt += strWordWrapCnt;
        strMissionCnt += L"\n";
    }

    if( bChangeColor )
    {
        strMission += strColorKeyEnd;
        strMissionCnt += strColorKeyEnd;
    }

    if( 0 < iNumMission )
    {        
        m_pkStaticSubMission->SetTextAutoMultiline( strMission );
        m_pkStaticCount->SetTextAutoMultiline( strMissionCnt );
        //m_pkStaticTitleSubMission->SetText( g_pkStrLoader->GetString( STR_ID_MISSION ) );
    }

    return iNumMission;
}

void KGCMissionProgressBox::PostChildDraw( void )
{
    // 달성과제 출력은 가독성문제로 제거했음.
    int iNumSubject = 0;//MakeSubjectList();
    int iNumMission = MakeMissionList();

    // 달성과제 UI위치잡기
    D3DXVECTOR2 vPos( 3.0f, 3.0f );
    //if( 0 < iNumSubject )
    //{
    //    m_pkTitleSubjectBack->SetFixedWindowLocalPos( vPos );
    //    m_pkTitleSubject->SetFixedWindowLocalPos( D3DXVECTOR2(vPos.x, vPos.y+2.0f) );
    //    vPos.y += (m_pkTitleSubjectBack->GetHeight() + 5.0f);
    //    m_pkStaticSubject->SetFixedWindowLocalPos( vPos );
    //    POINT ptSize = m_pkStaticSubject->GetStringWidthAndHeightInPixel( false );
    //    vPos.y += (ptSize.y + 20.0f);
    //    m_pkTitleSubject->ToggleRender( true );
    //    m_pkTitleSubjectBack->ToggleRender( true );
    //    m_pkStaticSubject->ToggleRender( true );
    //}
    //else
    {
        m_pkTitleSubject->ToggleRender( false );
        m_pkTitleSubjectBack->ToggleRender( false );
        m_pkStaticSubject->ToggleRender( false );
    }

    // 미션 UI위치잡기
    if( 0 < iNumMission )
    {
        m_pkTitleMissionBack->SetFixedWindowLocalPos( vPos );
        //m_pkTitleSubMission->SetFixedWindowLocalPos( D3DXVECTOR2(vPos.x, vPos.y+2.0f) );
        vPos.y += (m_pkTitleMissionBack->GetHeight() + 5.0f);
        m_pkStaticSubMission->SetFixedWindowLocalPos( vPos );
        vPos.x = m_pkStaticCount->GetFixedWindowLocalPos().x;
        m_pkStaticCount->SetFixedWindowLocalPos( vPos );
        m_pkTitleMissionBack->ToggleRender( true );
        m_pkTitleSubMission->ToggleRender( true );
        m_pkStaticSubMission->ToggleRender( true );
        m_pkStaticCount->ToggleRender( true );
    }
    else
    {
        m_pkTitleMissionBack->ToggleRender( false );
        m_pkTitleSubMission->ToggleRender( false );
        m_pkStaticSubMission->ToggleRender( false );
        m_pkStaticCount->ToggleRender( false );
    }
    m_pkStaticEmpty->ToggleRender( false );

	if( 0 == iNumMission && 0 == iNumSubject )
		m_pkBack->ToggleRender( false );		
	else
		m_pkBack->ToggleRender( true );
}

void KGCMissionProgressBox::OnCreate( void )
{
//     m_pkStaticTitleSubMission->SetFontOutline( true );
//     m_pkStaticTitleSubMission->SetAlign( DT_CENTER );
//     m_pkStaticTitleSubject->SetFontOutline( true );
//     m_pkStaticTitleSubject->SetAlign( DT_CENTER );
    m_pkStaticSubMission->SetFontOutline( true );
    m_pkStaticCount->SetFontOutline( true );
    m_pkStaticCount->SetAlign( DT_CENTER );
    m_pkStaticSubject->SetFontOutline( true );
    m_pkStaticEmpty->SetFontOutline( true );
    m_pkStaticEmpty->SetWordWrap( true );
}

bool KGCMissionProgressBox::IsEmpty()
{
	PostChildDraw();

	if( m_pkBack->IsRenderOn() )
		return false;
	return true;
}