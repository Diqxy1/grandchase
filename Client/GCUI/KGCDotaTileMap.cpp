#include "stdafx.h"
#include "KGCDotaTileMap.h"

IMPLEMENT_CLASSNAME( KGCDotaMiniMap );
IMPLEMENT_WND_FACTORY( KGCDotaMiniMap );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaMiniMap, "gc_dota_minimap" );

KGCDotaMiniMap::KGCDotaMiniMap()
: m_pkMyMark( NULL )
, m_pkWndCore1( NULL )
, m_pkWndCore2( NULL )
, m_fAniPos( 0.0f )
, m_bAniSwitch( false )
, m_iMaxZone( 0 )
, m_iAlpha( 0 )
, m_bAlphaSwitch( false )
{
    char szTemp[128];
    for(int iLoop=1; iLoop<MAX_ZONE; ++iLoop)
    {
        m_pkZoneWnd[iLoop-1] = NULL;
        sprintf(szTemp, "zone%d", iLoop);
        LINK_CONTROL(szTemp, m_pkZoneWnd[iLoop-1]);
    }

    LINK_CONTROL( "core1", m_pkWndCore1 );
    LINK_CONTROL( "core2", m_pkWndCore2 );
    LINK_CONTROL("mymark", m_pkMyMark);
}
KGCDotaMiniMap::~KGCDotaMiniMap(){}
void KGCDotaMiniMap::OnDestroy( void ){}
void KGCDotaMiniMap::OnCreateComplete( void )
{
    if( m_pkMyMark )
        m_pkMyMark->InitState( false );

    if( m_pkWndCore1 )
        m_pkWndCore1->InitState( false );

    if( m_pkWndCore2 )
        m_pkWndCore2->InitState( false );

    m_iMaxZone = 0;
    for(int iLoop=0; iLoop<MAX_ZONE; ++iLoop)
    {
        if( m_pkZoneWnd[iLoop] )
            m_pkZoneWnd[iLoop]->InitState( true );
        else
        {
            m_iMaxZone = iLoop;
            break;
        }
    }

    // 플레이어 ZoneID 셋팅
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( TEAM_SERDIN == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam )
            g_MyD3D->MyPlayer[iLoop]->SetDotaStage( (ZONE_1 + 1) );
        else
            g_MyD3D->MyPlayer[iLoop]->SetDotaStage( m_iMaxZone );
    }

    UpdateMyMark();

    m_iAlpha = 0;
    m_bAlphaSwitch = false;
}

void KGCDotaMiniMap::ActionPerformed( const KActionEvent& event ){}
void KGCDotaMiniMap::FrameMoveInEnabledState( void )
{
    // 적 코어 표시 애니메이션 처리
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayerIdx ) )
    {
        if( m_bAlphaSwitch )
        {
            m_iAlpha -= 10;
            if( m_iAlpha <= 0 )
            {
                m_bAlphaSwitch = false;
                m_iAlpha = 0;
            }
        }
        else
        {
            m_iAlpha += 10;
            if( m_iAlpha >= 255 )
            {
                m_bAlphaSwitch = true;
                m_iAlpha = 255;
            }

        }

        if( TEAM_SERDIN == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
        {
            m_pkWndCore2->SetAllWindowAlpha( m_iAlpha );
            m_pkWndCore2->ToggleRender( true );
        }
        else
        {
            m_pkWndCore1->SetAllWindowAlpha( m_iAlpha );
            m_pkWndCore1->ToggleRender( true );
        }
    }

    // 마크 애니메이션 처리
    if( m_pkMyMark && m_pkMyMark->IsRenderOn() )
    {
        if( m_fAniPos < 10.0f )
            m_fAniPos++;
        else
        {
            m_fAniPos = 0;
            m_bAniSwitch = !m_bAniSwitch;
        }

        D3DXVECTOR2 vPos = m_pkMyMark->GetFixedWindowLocalPos();
        vPos.y += m_bAniSwitch ? 1.0f : -1.0f;
        m_pkMyMark->SetFixedWindowLocalPos( vPos );
    }
}
void KGCDotaMiniMap::PostChildDraw( void ){}
void KGCDotaMiniMap::UpdateMyMark()
{
    m_pkMyMark->ToggleRender( false );
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex(iPlayerIdx) )
    {
        int iZoneIdx = g_MyD3D->MyPlayer[iPlayerIdx]->GetDotaStage() - 1;
        if( ZONE_1 <= iZoneIdx && iZoneIdx < m_iMaxZone )
        {
            D3DXVECTOR2 vPos = m_pkZoneWnd[iZoneIdx]->GetFixedWindowLocalPos();
            if( ZONE_1 == iZoneIdx || iZoneIdx == (m_iMaxZone - 1) )
            {
                vPos.x += 20.0f;
                vPos.y -= 10.0f;
            }
            else
            {
                vPos.x += 8.0f;
                vPos.y -= 10.0f;
            }

            m_pkMyMark->SetFixedWindowLocalPos( vPos );
            m_pkMyMark->ToggleRender( true );
        }
    }
    m_bAniSwitch = false;
    m_fAniPos = 0.0f;
}


IMPLEMENT_CLASSNAME( KGCDotaTileMap );
IMPLEMENT_WND_FACTORY( KGCDotaTileMap );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaTileMap, "gc_dota_tilemap" );

KGCDotaTileMap::KGCDotaTileMap()
: m_pkBack( NULL )
, m_pkBackList( NULL )
, m_pkMybar( NULL )
, m_pkStatickeytab( NULL )
, m_pkWndkeytab( NULL )
, m_pImgNumber( NULL )
, m_pImgTeamMark( NULL )
, m_iNowCoolTime( 0 )
, m_iUpdateCoolTime( 0 )
, m_dwBackOriHeight( 0 )
, m_fTabMarkOriPosY( 0.0f )
{
    LINK_CONTROL( "background", m_pkBack );
    LINK_CONTROL( "back_list", m_pkBackList );
    LINK_CONTROL( "back_mybar", m_pkMybar );
    LINK_CONTROL( "key_tab", m_pkWndkeytab );
    LINK_CONTROL( "keytab_desc", m_pkStatickeytab );

    char szTemp[128];
    for(int iLoop=0; iLoop<NUM_MIANIMAP; ++iLoop)
    {
        sprintf( szTemp, "dota_minimap%d", (iLoop+1) );
        LINK_CONTROL( szTemp, m_pkMiniMap[iLoop] );
    }
    
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        m_pkStaticNickName[iLoop] = NULL;
        sprintf( szTemp, "nickname%d", (iLoop+1) );
        LINK_CONTROL( szTemp, m_pkStaticNickName[iLoop] );

        m_pkStaticPos[iLoop] = NULL;
        sprintf( szTemp, "pos%d", (iLoop+1) );
        LINK_CONTROL( szTemp, m_pkStaticPos[iLoop] );

        m_pkStaticKillCnt[iLoop] = NULL;
        sprintf( szTemp, "killcount%d", (iLoop+1) );
        LINK_CONTROL( szTemp, m_pkStaticKillCnt[iLoop] );

        m_pkStaticDeathCnt[iLoop] = NULL;
        sprintf( szTemp, "deathcount%d", (iLoop+1) );
        LINK_CONTROL( szTemp, m_pkStaticDeathCnt[iLoop] );

        m_pkWndRanking[iLoop] = NULL;
        sprintf( szTemp, "back_ranking%d", (iLoop+1) );
        LINK_CONTROL( szTemp, m_pkWndRanking[iLoop] );
    }

    m_vecPosName.push_back(L"S");
    m_vecPosName.push_back(L"C1");
    m_vecPosName.push_back(L"B2");
    m_vecPosName.push_back(L"B1");
    m_vecPosName.push_back(L"C2");
    m_vecPosName.push_back(L"S");
}

KGCDotaTileMap::~KGCDotaTileMap()
{
    SAFE_RELEASE( m_pImgNumber );
    SAFE_RELEASE( m_pImgTeamMark );
}

void KGCDotaTileMap::OnDestroy( void ){};
void KGCDotaTileMap::OnCreateComplete( void )
{
    if( m_pkBack )
    {
        m_pkBack->InitState( true );
        if( 0 == m_dwBackOriHeight )
            m_dwBackOriHeight = 79;
    }

    if( m_pkBackList )
        m_pkBackList->InitState( true );
    if( m_pkMybar )
        m_pkMybar->InitState( false );

    if( m_pkStatickeytab )
    {
        m_pkStatickeytab->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
        m_pkStatickeytab->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_pkStatickeytab->InitState( false );
        m_pkStatickeytab->SetFontSpace( 2.0f );
        m_pkStatickeytab->SetText( g_pkStrLoader->GetString( 8767 ) );
    }

    if( m_pkWndkeytab )
    {
        m_pkWndkeytab->InitState( false );
        if( 0.0f == m_fTabMarkOriPosY )
            m_fTabMarkOriPosY = 268.0f;
    }

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( m_pkStaticNickName[iLoop] )
        {
            m_pkStaticNickName[iLoop]->SetWordWrap( true );
            m_pkStaticNickName[iLoop]->InitState( true );
        }
        if( m_pkStaticPos[iLoop] )
        {
            m_pkStaticPos[iLoop]->SetAlign( DT_CENTER );
            m_pkStaticPos[iLoop]->InitState( true );
        }
        if( m_pkStaticKillCnt[iLoop] )
        {
            m_pkStaticKillCnt[iLoop]->SetAlign( DT_CENTER );
            m_pkStaticKillCnt[iLoop]->InitState( true );
        }
        if( m_pkStaticDeathCnt[iLoop] )
        {
            m_pkStaticDeathCnt[iLoop]->SetAlign( DT_CENTER );
            m_pkStaticDeathCnt[iLoop]->InitState( true );
        }
        if( m_pkWndRanking[iLoop] )
        {
            m_pkWndRanking[iLoop]->InitState( false );
        }
    }

    if( NULL == m_pImgNumber )
    {
        m_pImgNumber = g_pGCDeviceManager2->CreateTexture( "Athens.dds" );
    }

    if( NULL == m_pImgTeamMark )
    {
        m_pImgTeamMark = g_pGCDeviceManager2->CreateTexture( "room.dds" );
    }

    for( int iLoop=0; iLoop<NUM_MIANIMAP; ++iLoop)
    {
        if( m_pkMiniMap[iLoop] )
            m_pkMiniMap[iLoop]->InitState( false );
    }
    
    if( g_kGlobalValue.IsDotaMapID( g_kGlobalValue.m_iCurDotaMapID ) )
        m_pkMiniMap[(g_kGlobalValue.m_iCurDotaMapID-1)]->ToggleRender( true );

    SetTextureUV();
}

void KGCDotaTileMap::SetTextureUV()
{
    // 랭킹 숫자 이미지 UV셋팅
    D3DXVECTOR4 vLocalPos(3.0f, 149.0f, 16.0f, 161.0f);
    D3DXVECTOR4 vDiffPos(3.0f, 106.0f, 16.0f, 123.0f);
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        m_v4RectUVNumber[NUMBER_RECT_LOCAL][iLoop].x = vLocalPos.x;
        m_v4RectUVNumber[NUMBER_RECT_LOCAL][iLoop].y = vLocalPos.y;
        m_v4RectUVNumber[NUMBER_RECT_LOCAL][iLoop].z = vLocalPos.z;
        m_v4RectUVNumber[NUMBER_RECT_LOCAL][iLoop].w = vLocalPos.w;
        vLocalPos.x += 21.0f;   vLocalPos.z += 21.0f;

        m_v4RectUVNumber[NUMBER_RECT_DIFF][iLoop].x = vDiffPos.x;
        m_v4RectUVNumber[NUMBER_RECT_DIFF][iLoop].y = vDiffPos.y;
        m_v4RectUVNumber[NUMBER_RECT_DIFF][iLoop].z = vDiffPos.z;
        m_v4RectUVNumber[NUMBER_RECT_DIFF][iLoop].w = vDiffPos.w;
        vDiffPos.x += 16.5f;    vDiffPos.z += 16.5f;
    }

    // 팀 마크 이미지 UV셋팅
    m_v4RectUVTeam[TEAM_SERDIN].x = 135.0f;
    m_v4RectUVTeam[TEAM_SERDIN].y = 149.0f;
    m_v4RectUVTeam[TEAM_SERDIN].z = 195.0f;
    m_v4RectUVTeam[TEAM_SERDIN].w = 209.0f;
    m_v4RectUVTeam[TEAM_CANABAN].x = 195.0f;
    m_v4RectUVTeam[TEAM_CANABAN].y = 149.0f;
    m_v4RectUVTeam[TEAM_CANABAN].z = 255.0f;
    m_v4RectUVTeam[TEAM_CANABAN].w = 209.0f;
}

void KGCDotaTileMap::ActionPerformed( const KActionEvent& event ){};
void KGCDotaTileMap::FrameMoveInEnabledState( void )
{
    if( m_iNowCoolTime >= m_iUpdateCoolTime )
    {
        m_iNowCoolTime = 0;
        UpdateUserRanking();
    }
    ++m_iNowCoolTime;
}

void KGCDotaTileMap::PostChildDraw( void )
{
    int iUIIndex = 0;
    RenderUI( iUIIndex, TEAM_SERDIN );
    RenderUI( iUIIndex, TEAM_CANABAN );
}

void KGCDotaTileMap::RenderUI( int &iUIIndex, int iTeam )
{
    int iPlayerTeam;
    D3DXVECTOR2 v2Pos;
    D3DXVECTOR4 v4WndPos;
    std::list<pair<int, int>>::iterator listIter = m_listRanking.begin();
    for(; listIter != m_listRanking.end(); ++listIter)
    {
        if( g_MyD3D->IsPlayerIndex(listIter->first) )
        {
            //iPlayerTeam = vecTemp[listIter->first].second.second;
            iPlayerTeam = g_MyD3D->MyPlayer[listIter->first]->m_kUserInfo.iTeam;

            if( iPlayerTeam == iTeam )
            {
                v2Pos = m_pkWndRanking[iUIIndex]->GetCurrentWindowPos();

                // 순위번호랜더링
                v4WndPos.x = v2Pos.x + 4;       v4WndPos.z = v2Pos.x + 16.0f;
                v4WndPos.y = v2Pos.y + 3;       v4WndPos.w = v2Pos.y + 16.0f;
                if( g_MyD3D->MyPlayer[listIter->first]->IsLocalPlayer() )
                    RenderTexture( m_pImgNumber, v4WndPos, m_v4RectUVNumber[NUMBER_RECT_LOCAL][(listIter->second-1)] );
                else
                    RenderTexture( m_pImgNumber, v4WndPos, m_v4RectUVNumber[NUMBER_RECT_DIFF][(listIter->second-1)] );

                // 팀 마크 출력
                v2Pos.x += 42.5;
                v4WndPos.x = v2Pos.x;           v4WndPos.y = v2Pos.y;
                v4WndPos.z = v2Pos.x + 21.0f;   v4WndPos.w = v2Pos.y + 19.0f;
                RenderTexture( m_pImgTeamMark, v4WndPos, m_v4RectUVTeam[iPlayerTeam] );

                // UI인덱스 증가
                ++iUIIndex;
            }
        }
    }
}

#define PIXELTOTEXEL(p) ( (p)/512.0f )
void KGCDotaTileMap::RenderTexture( GCDeviceTexture* pTex, D3DXVECTOR4 &vRectWnd, D3DXVECTOR4 &vRectUV )
{
    if( pTex )
    {
        g_pGCDeviceManager2->DrawInScreenResize( pTex,
            vRectWnd.x,    vRectWnd.y,   vRectWnd.z,    vRectWnd.w,
            PIXELTOTEXEL(vRectUV.x),     PIXELTOTEXEL(vRectUV.y),
            PIXELTOTEXEL(vRectUV.z),     PIXELTOTEXEL(vRectUV.w) );
    }
}

void KGCDotaTileMap::UpdateUserRanking()
{
    int iUIIndex = 0;

    SortRanking();
    UpdateUI( iUIIndex, TEAM_SERDIN );
    UpdateUI( iUIIndex, TEAM_CANABAN );
    UpdateFinishUI( iUIIndex );

    if( g_kGlobalValue.IsDotaMapID( g_kGlobalValue.m_iCurDotaMapID ) )
        m_pkMiniMap[(g_kGlobalValue.m_iCurDotaMapID-1)]->UpdateMyMark();
}

void KGCDotaTileMap::SortRanking()
{
    //vecTemp.push_back(std::make_pair(L"A", std::make_pair(0, 0)));
    //vecTemp.push_back(std::make_pair(L"B", std::make_pair(0, 0)));
    //vecTemp.push_back(std::make_pair(L"C", std::make_pair(0, 0)));
    //vecTemp.push_back(std::make_pair(L"D", std::make_pair(0, 0)));
    //vecTemp.push_back(std::make_pair(L"E", std::make_pair(0, 0)));
    //vecTemp.push_back(std::make_pair(L"F", std::make_pair(0, 0)));

    //char szTemp[128];
    //FILE *pFile = fopen("c:\\Data.txt", "rt");
    //for(int iLoopTemp=0; iLoopTemp<6; ++iLoopTemp)
    //{
    //    fgets(szTemp, 128, pFile);  sscanf(szTemp, "%d %d", &vecTemp[iLoopTemp].second.first, &vecTemp[iLoopTemp].second.second);
    //}
    //fclose(pFile);

    // Kill수 처리 후 순위대로 정렬
    int iRankingCnt;
    m_listRanking.clear();
    for(int iLoopX=0; iLoopX<MAX_PLAYER_NUM; ++iLoopX)
    {
        if( false == g_MyD3D->MyPlayer[iLoopX]->IsLive() )
            continue;

        iRankingCnt = 1;
        for(int iLoopY=0; iLoopY<MAX_PLAYER_NUM; ++iLoopY)
        {
            if( (iLoopX == iLoopY) ||
                (false == g_MyD3D->MyPlayer[iLoopY]->IsLive()) )
            //if( iLoopX == iLoopY )
                continue;

            if( g_pGameBoard->GetHowKill( iLoopX ) < g_pGameBoard->GetHowKill( iLoopY ) )
            //if( vecTemp[iLoopX].second.first < vecTemp[iLoopY].second.first )
                ++iRankingCnt;
        }

        // 삽입시점찾기
        std::list<pair<int, int>>::iterator listIter = m_listRanking.begin();
        for(; listIter != m_listRanking.end(); ++listIter)
        {
            if( listIter->second > iRankingCnt )
                break;
        }

        // Kill등수대로 PlayerIdx와 순위 저장
        m_listRanking.insert(listIter, std::make_pair(iLoopX, iRankingCnt) );
    }
}

void KGCDotaTileMap::UpdateUI( int &iUIIndex, int iTeam )
{
    // 순위대로 UI셋팅
    int iPlayerTeam;
    WCHAR wszTemp[128];
    std::list<pair<int, int>>::iterator listIter = m_listRanking.begin();
    for(; listIter != m_listRanking.end(); ++listIter)
    {
        //iPlayerTeam = vecTemp[listIter->first].second.second;
        iPlayerTeam = g_MyD3D->MyPlayer[listIter->first]->m_kUserInfo.iTeam;

        if( iPlayerTeam == iTeam )
        {
            // String셋팅
            if( m_pkStaticNickName[iUIIndex] )
            {
                if( g_MyD3D->MyPlayer[listIter->first]->IsLocalPlayer() )
                    m_pkStaticNickName[iUIIndex]->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
                else
                    m_pkStaticNickName[iUIIndex]->SetFontOutline( false );

                //m_pkStaticNickName[iUIIndex]->SetText( const_cast<WCHAR*>(vecTemp[listIter->first].first.c_str()) );
                m_pkStaticNickName[iUIIndex]->SetText( g_MyD3D->MyPlayer[listIter->first]->GetStrUserName() );
            }
            if( m_pkStaticPos[iUIIndex] )
            {
                if( g_MyD3D->MyPlayer[listIter->first]->IsLocalPlayer() )
                    m_pkStaticPos[iUIIndex]->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
                else
                    m_pkStaticPos[iUIIndex]->SetFontOutline( false );

                int iZone = g_MyD3D->MyPlayer[listIter->first]->GetDotaStage() - 1;
                if( 0 <= iZone && iZone < int(m_vecPosName.size()) )
                    m_pkStaticPos[iUIIndex]->SetText( m_vecPosName[iZone].c_str() );
                else
                    m_pkStaticPos[iUIIndex]->SetText( L"" );
            }
            if( m_pkStaticKillCnt[iUIIndex] )
            {
                if( g_MyD3D->MyPlayer[listIter->first]->IsLocalPlayer() )
                    m_pkStaticKillCnt[iUIIndex]->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
                else
                    m_pkStaticKillCnt[iUIIndex]->SetFontOutline( false );

                wsprintf(wszTemp, L"%d", g_pGameBoard->GetHowKill( listIter->first ));
                //wsprintf(wszTemp, L"%d", vecTemp[listIter->first].second.first);
                m_pkStaticKillCnt[iUIIndex]->SetText( wszTemp );
            }
            if( m_pkStaticDeathCnt[iUIIndex] )
            {
                if( g_MyD3D->MyPlayer[listIter->first]->IsLocalPlayer() )
                    m_pkStaticDeathCnt[iUIIndex]->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
                else
                    m_pkStaticDeathCnt[iUIIndex]->SetFontOutline( false );

                wsprintf(wszTemp, L"%d", g_pGameBoard->GetHowDie( listIter->first ));
                //wsprintf(wszTemp, L"%d", vecTemp[listIter->first].second.first);
                m_pkStaticDeathCnt[iUIIndex]->SetText( wszTemp );
            }
            if( m_pkWndRanking )
            {
                m_pkWndRanking[iUIIndex]->ToggleRender( true );
            }

            // 내꺼면 MyBar출력
            if( g_MyD3D->MyPlayer[listIter->first]->IsLocalPlayer() )
            {
                D3DXVECTOR2 vPos = m_pkStaticNickName[iUIIndex]->GetFixedWindowLocalPos();
                D3DXVECTOR2 vBarPos = m_pkMybar->GetFixedWindowLocalPos();
                vBarPos.y = vPos.y;// -= 4.0f;
                m_pkMybar->SetFixedWindowLocalPos( vBarPos );
                m_pkMybar->ToggleRender( true );
            }

            // UI 인덱스 증가
            ++iUIIndex;
        }
    }
}

void KGCDotaTileMap::UpdateFinishUI( int &iUIIndex )
{
    // 백그라운드 사이즈 및 Tab마크 위치 처리
    if( m_pkBack && m_pkBackList && m_pkWndkeytab && m_pkStatickeytab )
    {
        DWORD dwHeight = m_dwBackOriHeight + ( (iUIIndex-1) * 20 );
        m_pkBack->SetHeight( dwHeight );

        dwHeight = (m_dwBackOriHeight-59) + ( (iUIIndex-1) * 20 );
        m_pkBackList->SetHeight( dwHeight );

        D3DXVECTOR2 vPos = m_pkWndkeytab->GetFixedWindowLocalPos();
        vPos.y = m_fTabMarkOriPosY + ( (iUIIndex-1) * 20 );
        m_pkWndkeytab->SetWindowPosY( vPos.y );
        m_pkWndkeytab->ToggleRender( true );

        vPos = m_pkStatickeytab->GetFixedWindowLocalPos();
        vPos.y = (m_fTabMarkOriPosY+4.0f) + ( (iUIIndex-1) * 20 );
        m_pkStatickeytab->SetWindowPosY( vPos.y );
        m_pkStatickeytab->ToggleRender( true );
    }

    // 빈 리스트 처리
    for(; iUIIndex<MAX_PLAYER_NUM; ++iUIIndex)
    {
        if( m_pkStaticNickName[iUIIndex] )
            m_pkStaticNickName[iUIIndex]->SetText(L"");
        if( m_pkStaticPos[iUIIndex] )
            m_pkStaticPos[iUIIndex]->SetText(L"");
        if( m_pkStaticKillCnt[iUIIndex] )
            m_pkStaticKillCnt[iUIIndex]->SetText(L"");
        if( m_pkStaticDeathCnt[iUIIndex] )
            m_pkStaticDeathCnt[iUIIndex]->SetText(L"");
        if( m_pkWndRanking )
            m_pkWndRanking[iUIIndex]->ToggleRender( false );
    }
}