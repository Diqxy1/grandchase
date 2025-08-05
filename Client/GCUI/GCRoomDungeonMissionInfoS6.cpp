#include "stdafx.h"
#include "GCRoomDungeonMissionInfoS6.h"

IMPLEMENT_CLASSNAME( KGCRoomDungeonMissionInfoS6 );
IMPLEMENT_WND_FACTORY( KGCRoomDungeonMissionInfoS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomDungeonMissionInfoS6, "gc_room_dungeon_mission_info_s6" );

KGCRoomDungeonMissionInfoS6::KGCRoomDungeonMissionInfoS6( void )
: m_pkMissionScroll( NULL )
, m_pkMissionEmpty( NULL )
, m_pkMissionCount( NULL )
, m_nMissionIndex( 0 )
, m_dwMissionStaticWidth( 0 )
{
    LINK_CONTROL( "scroll", m_pkMissionScroll );

    char str[MAX_PATH] = {0,};
    
    for ( int i = 0; i < EM_MISSION_PAGE_NUM; ++i ) 
    {
        m_apkMissionText[i] = NULL;
        m_apkMissionState[i] = NULL;
        m_apkMissionTextCount[i] = NULL;

        sprintf( str, "mission_slot%d", i );          
        LINK_CONTROL( str,  m_apkMissionText[i] );

        sprintf( str, "mission_state%d", i );
        LINK_CONTROL( str,  m_apkMissionState[i] );

        sprintf( str, "mission_slot_count_%d", i );
        LINK_CONTROL( str,  m_apkMissionTextCount[i] );

        for ( int j = 0; j < EM_MISSION_TYPE_NUM; ++j )
        {
            m_pkMissionType[i][j] = NULL;
            sprintf( str, "type_%d_%d", i, j );
            LINK_CONTROL( str,  m_pkMissionType[i][j] );
        }
    }


    LINK_CONTROL( "mission_empty", m_pkMissionEmpty );
    LINK_CONTROL( "mission_count", m_pkMissionCount );

    m_vecMissionListState.clear();
}

KGCRoomDungeonMissionInfoS6::~KGCRoomDungeonMissionInfoS6( void )
{
}

void KGCRoomDungeonMissionInfoS6::ActionPerformed( const KActionEvent& event )
{ 
	GCWND_MAP( m_pkMissionScroll, OnScroll );
}


void KGCRoomDungeonMissionInfoS6::OnCreate( void )
{
    m_pkMissionScroll->InitState(true,false,this);
    m_pkMissionScroll->InitScroll( 0, 1, 0, EM_MISSION_PAGE_NUM, 0, 0, 1, true );

    for ( int i = 0; i < EM_MISSION_PAGE_NUM; ++i ) 
    {
        m_apkMissionText[i]->InitState( true, true, this );
        m_apkMissionState[i]->InitState( false, false, this );
        
        m_apkMissionTextCount[i]->InitState( true, true, this );
        m_apkMissionTextCount[i]->SetAlign( DT_RIGHT );

        for ( int j = 0; j < EM_MISSION_TYPE_NUM; ++j )
        {
            m_pkMissionType[i][j]->InitState(false);
        }
    }

    // +1은 두개UI 사이의 갭
    if ( m_dwMissionStaticWidth == 0 )
        m_dwMissionStaticWidth = m_apkMissionText[0]->GetWidth() + m_apkMissionTextCount[0]->GetWidth() + 1;
    
    m_pkMissionEmpty->InitState( true, true, this );
    m_pkMissionCount->InitState( true, true, this );
    m_pkMissionCount->SetAlign( DT_CENTER ); 
}

void KGCRoomDungeonMissionInfoS6::UpdateData()
{
    ClearListBox(); 
    UpdateMissionData();    
    
    m_pkMissionScroll->SetScrollPageSize( EM_MISSION_PAGE_NUM );
    m_pkMissionScroll->SetScrollRangeMax( m_vecMissionListState.size() );

    m_nMissionIndex = 0;
    m_pkMissionScroll->SetScrollPos( m_nMissionIndex );

    UpdateMissionText();
}

void KGCRoomDungeonMissionInfoS6::UpdateMissionText()
{
    for ( int i = 0; i < EM_MISSION_PAGE_NUM; ++i ) 
    {
        m_apkMissionText[i]->SetText( L"" );
        m_apkMissionState[i]->ToggleRender( false );

        m_apkMissionTextCount[i]->SetText( L"" );

        for ( int j = 0; j < EM_MISSION_TYPE_NUM; ++j )
        {
            m_pkMissionType[i][j]->ToggleRender(false);
        }
    }

    int nIndex = 0;
    DWORD dwColor = 0xffffffff;

    for ( int i = 0; i < (int)m_vecMissionListState.size(); ++i ) 
    {
        if ( i < m_nMissionIndex ) 
            continue;

        if ( nIndex >= EM_MISSION_PAGE_NUM ) 
            break;

        //  완료된 미션이라면...
        if ( m_vecMissionListState[i].first )
        {
            dwColor = 0xff00ff00;
            m_apkMissionTextCount[nIndex]->ToggleRender(false);
        }
        else
        {
            dwColor = 0xffffffff;
            m_apkMissionTextCount[nIndex]->ToggleRender(true);
        }

        m_apkMissionText[nIndex]->SetFontColor( dwColor );
        m_apkMissionTextCount[nIndex]->SetFontColor( dwColor );

        m_apkMissionTextCount[nIndex]->SetText( m_vecMissionListState[i].second.m_strMissionCount );

        //카운트표시 부분 
        DWORD width = (m_apkMissionTextCount[nIndex]->GetStringWidthAndHeightInPixel()).x;
        m_apkMissionText[nIndex]->SetWidth( m_dwMissionStaticWidth - width );

        D3DXVECTOR2 vTextPos = m_apkMissionText[nIndex]->GetFixedWindowLocalPos();
        D3DXVECTOR2 vTextCountPos = m_apkMissionTextCount[nIndex]->GetFixedWindowLocalPos();

        //카운트표시 크기부분을 뺀만큼 미션내용표시부분을 설정 - 카운트부분은 전체가 보여야함
        vTextCountPos.x = vTextPos.x + (float)m_apkMissionText[nIndex]->GetWidth() + 1.0f;
        m_apkMissionTextCount[nIndex]->SetFixedWindowLocalPos(vTextCountPos);

#ifdef  NATION_CHINA
		m_apkMissionText[nIndex]->SetText( m_vecMissionListState[i].second.m_strMissionTitle );
#else
		m_apkMissionText[nIndex]->SetForceWordWrapText( m_vecMissionListState[i].second.m_strMissionTitle, true );
#endif

        if ( m_pkMissionType[nIndex][m_vecMissionListState[i].second.m_iMissionType] )
            m_pkMissionType[nIndex][m_vecMissionListState[i].second.m_iMissionType]->ToggleRender(true);

        //  미션 완료창 갱신
        if ( m_vecMissionListState[i].first )
        {
            m_apkMissionState[nIndex]->ToggleRender( true );

            int iAdditionalHeight = g_pkFontMgr_original->GetHeight( m_apkMissionText[nIndex]->GetText().c_str()); 
            int iAdditionalHeightPlus = 6 ;

            //  MissionState 위치 조절
            D3DXVECTOR2 vPos = m_apkMissionState[0]->GetFixedWindowLocalPos();
            int nGap = ( iAdditionalHeight + iAdditionalHeightPlus ) * nIndex;
            vPos.y += nGap;

            m_apkMissionState[nIndex]->SetWindowPos( vPos );
        }

        nIndex++;
    }
}

void KGCRoomDungeonMissionInfoS6::UpdateMissionData()
{
    m_vecMissionListState.clear();

    int iLevel = SiKGCRoomManager()->GetDungeonLevel();;
    EGCGameMode eMode = SiKGCRoomManager()->GetGameMode();

    std::vector<DWORD> vecMissionList = g_kGlobalValue.m_kUserMission.GetDungeonMissionList( eMode, iLevel, false );
    std::vector<std::pair<bool,std::wstring>> vecMissionListState;

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if ( !pPlayer ) 
        return;

    int nMissionCompCount = 0;

    for( std::vector<DWORD>::iterator vit = vecMissionList.begin() ; vit != vecMissionList.end() ; ++vit )
    {
        const SMissionInfo* pInfo = SiKGCMissionManager()->GetMissionInfo( *vit );
        if ( !pInfo ) 
            continue;

        if( pPlayer && ( 0 <= pInfo->m_iCharacter) && (pPlayer->m_kUserInfo.GetCurrentChar().iCharType != pInfo->m_iCharacter) )
            continue;

        bool bComplate = g_kGlobalValue.m_kUserMission.IsCompleteMission( *vit );

        for(int iSub = 0 ; iSub < static_cast<int>(pInfo->m_vecSubMissions.size()) ; ++iSub ) 
        {
            const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( pInfo->m_vecSubMissions[iSub] );
            if ( !sSubMissionInfo ) 
                continue;

            sMissionState sMissionStateData;
            sMissionStateData.m_iMissionType = pInfo->m_iMissionType;

            for( int iCon = 0 ; iCon < static_cast<int>(sSubMissionInfo->m_vecConditionID.size()) ; ++ iCon )
            {

                const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo( sSubMissionInfo->m_vecConditionID[iCon] );
                if( !sCondition )
                    continue;

                //  현재 게임 모드 및 던전 난이도 체크
                if ( ( sCondition->m_iGameMode != -1 ) && 
                     ( sCondition->m_iGameMode != eMode || 
                     sCondition->m_pairDifficultRange.first > iLevel ) )
                    continue;

                // 캐릭터 체크
                if ( pInfo->m_iCharacter != -1 && pInfo->m_iCharacter != pPlayer->m_kUserInfo.GetCurrentChar().iCharType ) 
                    continue;


                int iComplateCnt = g_kGlobalValue.m_kUserMission.GetCompletionCount( *vit, pInfo->m_vecSubMissions[ 0 ] );
                int iMaxCnt = sSubMissionInfo->m_sCompletion.m_iCount;
                if( iComplateCnt > iMaxCnt ) iComplateCnt = iMaxCnt;
                bool bRenderCompleteCount = true;

                WCHAR wszTemp[MAX_PATH];

                if ( bComplate ) 
                {
                    nMissionCompCount++;
                    std::wstring strMission = SiKGCMissionManager()->GetString( sSubMissionInfo->m_iTitleID );
                    SiKGCMultipleLanguages()->Replacer( strMission, L"\n", L"\0" );
                    sMissionStateData.m_strMissionTitle = strMission;
                }
                else
                {
                    if ( sCondition->m_iType != KGCMissionManager::TYPE_NOUSE )
                    {
                        if ( sCondition->m_iType == KGCMissionManager::TYPE_USE_POTION || 
                            sCondition->m_iType == KGCMissionManager::TYPE_SKILL_COUNT )
                            iMaxCnt = sCondition->m_pairUseCount.second;
                        else
                            iMaxCnt = sCondition->m_iValue;

                        iComplateCnt = 0;

                        if ( sCondition->m_iType == KGCMissionManager::TYPE_RANK )
                            bRenderCompleteCount = false;
                    }

                    std::wstring strMission = SiKGCMissionManager()->GetString( sSubMissionInfo->m_iTitleID );
                    SiKGCMultipleLanguages()->Replacer( strMission, L"\n", L"\0" );
                    if ( bRenderCompleteCount )
                    {
                        sMissionStateData.m_strMissionTitle = strMission;
                        wsprintf(wszTemp, L"(%d/%d)", iComplateCnt, iMaxCnt);
                        sMissionStateData.m_strMissionCount = wszTemp;
                    }
                    else
                    {
                        sMissionStateData.m_strMissionTitle = strMission;
                    }
                }

                m_vecMissionListState.push_back( std::make_pair( bComplate, sMissionStateData ) );

                break;
            }
        }
    }

    std::wostringstream strText;
    std::wstring strColorKeyEnd = L"#cX";
    std::wstring strPartsColorKey = L"#c29c7e4";

    strText << strPartsColorKey << nMissionCompCount << strColorKeyEnd << L"/" << m_vecMissionListState.size();

    m_pkMissionCount->SetText( strText.str() );

    if ( m_vecMissionListState.empty() )
    {
        m_pkMissionEmpty->ToggleRender( true );
    }
    else
    {
        m_pkMissionEmpty->ToggleRender( false );
    }
}

void KGCRoomDungeonMissionInfoS6::ClearListBox()
{
    for ( int i = 0; i < EM_MISSION_PAGE_NUM; ++i ) 
    {
        m_apkMissionText[i]->SetText( L"" );
        m_apkMissionState[i]->ToggleRender( false );
        m_apkMissionTextCount[i]->SetText( L"" );
    }

    m_pkMissionScroll->SetScrollPageSize( EM_MISSION_PAGE_NUM );
    m_pkMissionScroll->SetScrollRangeMax( 0 );

    m_nMissionIndex = 0;
    m_pkMissionScroll->SetScrollPos( m_nMissionIndex );

}

void KGCRoomDungeonMissionInfoS6::OnScroll( )
{

    int iTop = m_pkMissionScroll->GetScrollPos();
    if ( m_nMissionIndex != iTop )
    {
        m_nMissionIndex = iTop;
        g_KDSound.Play( "73" );
        UpdateMissionText();
    }
}