#include "stdafx.h"
#include "GCQuestTileMap.h"
#include "MyD3D.h"

#include "QuestGameManager.h"
#include "KGCMissionProgressBox.h"
#include "KGCPartyRankInfo.h"

//
//

IMPLEMENT_CLASSNAME( KGCQuestTileMap );
IMPLEMENT_WND_FACTORY( KGCQuestTileMap );
IMPLEMENT_WND_FACTORY_NAME( KGCQuestTileMap, "gc_minimap" );

KGCQuestTileMap::KGCQuestTileMap()
{
    m_bCharDir          = true;
    m_iBossCol          = 0;
    m_iOffsetX          = 0;
    m_iOffsetY          = 0;
    m_iPlusOffX         = 0;
    m_iPlusOffY         = 0;

    m_pkTabImage        = NULL;
    m_pkMinimapDesc     = NULL;	
    m_pPartyRankInfoBox = NULL;

    LINK_CONTROL( "key_tab",        m_pkTabImage );
    LINK_CONTROL( "minimap_desc",   m_pkMinimapDesc );
	LINK_CONTROL( "mission_progress", m_pkMissionBox );
    LINK_CONTROL( "party_rank_info", m_pPartyRankInfoBox);
}

KGCQuestTileMap::~KGCQuestTileMap()
{
}

void KGCQuestTileMap::OnCreate()
{
    m_Tile.OnCreate();

    m_pkTabImage->InitState( true );
    m_pkMinimapDesc->InitState( true );
    m_pkTabImage->SetWindowColor( 0xAAFFFFFF );
    m_pkMinimapDesc->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkMinimapDesc->SetFontOutline( true );
    m_pkMinimapDesc->SetText( g_pkStrLoader->GetString( STR_ID_MINIMAP_DESC ) );

	m_pkMissionBox->ToggleRender( true );
    m_pPartyRankInfoBox->ToggleRender(true);
}

void KGCQuestTileMap::OnDestroy( void )
{
}

void KGCQuestTileMap::KeyProc(void)
{
    //if( g_pkInput->IsPressed(DIK_LALT) )
    //{
    //    float fBeforeX = m_Tile.GetWidth();
    //    float fBefore = m_Tile.GetHeight();
    //    if( g_pkInput->IsUp( DIK_NUMPADPLUS ) )
    //    {
    //        if( m_Tile.m_fTileZoom+TILE_ZOOM <= MAX_TILE_ZOOM ) m_Tile.m_fTileZoom += TILE_ZOOM;
    //    }
    //    if( g_pkInput->IsUp( DIK_NUMPADMINUS ) )
    //    {
    //        if( m_Tile.m_fTileZoom-TILE_ZOOM >= MIN_TILE_ZOOM ) m_Tile.m_fTileZoom -= TILE_ZOOM;
    //    }
    //    float fAfterX = m_Tile.GetWidth();
    //    float fAfter = m_Tile.GetHeight();

    //    // Y의 타일 갯수만큼 사이즈가 늘어나면서 위치가 아래로 내려간 듯하게 된다.
    //    // 그래서 아래의 식으로 보간한다.
    //    m_iPlusOffX -= (int)((fAfterX - fBeforeX)/2.0f * g_pkQuestManager->m_Quest.iTileX);
    //    m_iPlusOffY -= (int)(((fAfter - fBefore)/2.0f * g_pkQuestManager->m_Quest.iTileY)/2.0f);
    //}

//#if (!defined( __PATH__ ) && defined( NATION_KOREA )) || defined( _DEBUG )
//    if( g_pkInput->IsPressed(DIK_LALT) )
//    {
//        if( g_pkInput->IsUp( DIK_LEFT ) )       m_iPlusOffX -= TILE_MOVE_OFFSET;
//        if( g_pkInput->IsUp( DIK_RIGHT ) )      m_iPlusOffX += TILE_MOVE_OFFSET;
//        if( g_pkInput->IsUp( DIK_UP ) )         m_iPlusOffY -= TILE_MOVE_OFFSET;
//        if( g_pkInput->IsUp( DIK_DOWN ) )       m_iPlusOffY += TILE_MOVE_OFFSET;
//        if( g_pkInput->IsUp( DIK_MULTIPLY ) )
//        {
//            m_iPlusOffX = 0;
//            m_iPlusOffY = 0;
//        }
//    }
//#endif

}

void KGCQuestTileMap::PostChildDraw( void )
{	
    // 키보드 처리
    KeyProc();

	g_pGCDeviceManager2->Flush();
    // 현재 위치 보정
    if( m_bTrace )
    {
        TileMap* pTile = g_pkQuestManager->m_Quest.GetTileMap( g_pkQuestManager->m_iCurTileY, g_pkQuestManager->m_iCurTileX );
        if( pTile )
        {
            if( -1 != pTile->iStage && pTile->strName != "blank" )
            {
                int iCurMinimapX = (int)(g_pkQuestManager->m_Quest.iTileX * m_Tile.GetWidth());
                int iCurMinimapY = (int)(g_pkQuestManager->m_Quest.iTileY * m_Tile.GetHeight());

                m_iOffsetX = (int)( GetFixedWindowPos().x + ( GetWidth() - m_Tile.GetWidth()/2.0f  - iCurMinimapX )/2.0f );
                m_iOffsetY = (int)( GetFixedWindowPos().y + ( GetHeight() - m_Tile.GetHeight()/2.0f  - iCurMinimapY )/2.0f );
            }
        }
    }


    ////************************************************************************
    //// 아래에 랜더 타겟을 텍스쳐로 잡은 이유는 QuestTileMap의 영역에 그려진 
    //// 미니맵만 화면에 그리기 위해서 입니다.
    ////************************************************************************
    m_Tile.SetCharDirection( (bool)( m_iBossCol > g_pkQuestManager->m_iCurTileX ) );
    

    D3DVIEWPORT9 orgViewport;
    g_pd3dDevice->GetViewport( &orgViewport );
    g_pd3dDevice->SetViewport( &m_viewport );

    // 미니맵을 그린다.
    for(int i=0;i<(int)m_vtMinimapInfo.size();++i)
    {
        float fPosX = (m_iOffsetX + m_iPlusOffX) + m_vtMinimapInfo[i].iDrawCol*m_Tile.GetWidth();
        float fPosY = (m_iOffsetY + m_iPlusOffY) + m_vtMinimapInfo[i].iDrawRow*m_Tile.GetHeight();

        if( fPosX < -m_Tile.GetWidth()  || fPosX > (GC_SCREEN_WIDTH + m_Tile.GetWidth()) ) continue;
        if( fPosY < -m_Tile.GetHeight() || fPosY > (GC_SCREEN_HEIGHT + m_Tile.GetHeight()) ) continue;

        m_Tile.SetArrowBothAll(false);
        m_Tile.SetArrowInAll(false);
        m_Tile.SetArrowOutAll(false);

        //m_Tile.SetPos(fPosX , fPosY , m_fWindowScaleX, m_fWindowScaleY);
		m_Tile.SetPos(fPosX , fPosY , m_fWindowScaleX, m_fWindowScaleY);
        m_Tile.SetArrow(m_vtMinimapInfo[i].iD_Path);
        m_Tile.SetStageBackType(m_vtMinimapInfo[i].iFlag);
        m_Tile.SetStage(m_vtMinimapInfo[i].iStage);

        m_Tile.SetArrowColorType(KGCQuestTile::ARROW_RIGHT, m_vtMinimapInfo[i].bArrowColor[KGCQuestTile::ARROW_RIGHT-1]);
        m_Tile.SetArrowColorType(KGCQuestTile::ARROW_BOTTOM, m_vtMinimapInfo[i].bArrowColor[KGCQuestTile::ARROW_BOTTOM-1]);
        m_Tile.SetArrowColorType(KGCQuestTile::ARROW_LEFT, m_vtMinimapInfo[i].bArrowColor[KGCQuestTile::ARROW_LEFT-1]);
        m_Tile.SetArrowColorType(KGCQuestTile::ARROW_TOP, m_vtMinimapInfo[i].bArrowColor[KGCQuestTile::ARROW_TOP-1]);

        m_Tile.SetArrowIn(KGCQuestTile::ARROW_RIGHT, m_vtMinimapInfo[i].bArrowFromNeighbor[KGCQuestTile::ARROW_RIGHT-1]);
        m_Tile.SetArrowIn(KGCQuestTile::ARROW_BOTTOM, m_vtMinimapInfo[i].bArrowFromNeighbor[KGCQuestTile::ARROW_BOTTOM-1]);
        m_Tile.SetArrowIn(KGCQuestTile::ARROW_LEFT, m_vtMinimapInfo[i].bArrowFromNeighbor[KGCQuestTile::ARROW_LEFT-1]);
        m_Tile.SetArrowIn(KGCQuestTile::ARROW_TOP, m_vtMinimapInfo[i].bArrowFromNeighbor[KGCQuestTile::ARROW_TOP-1]);

        m_Tile.SetBrightNum( (bool)g_pkQuestManager->m_mapStageHistory[ std::make_pair(m_vtMinimapInfo[i].iRow, m_vtMinimapInfo[i].iCol) ] );
        m_Tile.SetCurStage( (bool)( g_pkQuestManager->m_iCurTileY == m_vtMinimapInfo[i].iRow 
                                    && g_pkQuestManager->m_iCurTileX == m_vtMinimapInfo[i].iCol ) );
        m_Tile.Draw();
    }

    g_pd3dDevice->SetViewport( &orgViewport );
	g_pGCDeviceManager2->Flush();
}

void KGCQuestTileMap::InitMinimap(void)
{
    m_vtMinimapInfo.clear();

    m_rtMaxBound.left       = g_pkQuestManager->m_Quest.iTileX/2;
    m_rtMaxBound.right      = g_pkQuestManager->m_Quest.iTileX/2;
    m_rtMaxBound.top        = g_pkQuestManager->m_Quest.iTileY/2;
    m_rtMaxBound.bottom     = g_pkQuestManager->m_Quest.iTileY/2;

    for(int i=0;i<g_pkQuestManager->m_Quest.iTileY;++i)
    {
        for(int j=0;j<g_pkQuestManager->m_Quest.iTileX;++j)
        {
            TileMap* pTile = g_pkQuestManager->m_Quest.GetTileMap( i, j );                        

            if( -1 == pTile->iStage || pTile->strName == "blank" || 3 == pTile->iFlag ) continue;

            if( m_rtMaxBound.left > j )       m_rtMaxBound.left     = j;
            if( m_rtMaxBound.right < j )      m_rtMaxBound.right    = j;
            if( m_rtMaxBound.top > i )        m_rtMaxBound.top      = i;
            if( m_rtMaxBound.bottom < i )     m_rtMaxBound.bottom   = i;

            stTileInfo          TileInfo;
            TileInfo.iD_Path    = pTile->iD_Path;
            TileInfo.iFlag      = pTile->iFlag;
            TileInfo.iStage     = pTile->iStage;
            TileInfo.iRow       = i;
            TileInfo.iCol       = j;
            TileInfo.iDrawRow   = i;
            TileInfo.iDrawCol   = j;

            if( KGCQuestTile::BOSS == TileInfo.iFlag ) m_iBossCol = j;

            TileInfo.bArrowFromNeighbor[KGCQuestTile::ARROW_RIGHT-1]    = IsSideArrowToHere(DIRECTION_RIGHT, pTile->iRow, pTile->iCol);
            TileInfo.bArrowFromNeighbor[KGCQuestTile::ARROW_BOTTOM-1]   = IsSideArrowToHere(DIRECTION_DOWN, pTile->iRow, pTile->iCol);
            TileInfo.bArrowFromNeighbor[KGCQuestTile::ARROW_LEFT-1]     = IsSideArrowToHere(DIRECTION_LEFT, pTile->iRow, pTile->iCol);
            TileInfo.bArrowFromNeighbor[KGCQuestTile::ARROW_TOP-1]      = IsSideArrowToHere(DIRECTION_UP, pTile->iRow, pTile->iCol);

            m_vtMinimapInfo.push_back(TileInfo);
        }
    }

    // 시작 Row와 Col이 0이 아니라 1, 2인 경우를 대비해서
    // 타일 표시용 Row, Col을 당겨준다.
    for(int i=0;i<(int)m_vtMinimapInfo.size();++i)
    {
        m_vtMinimapInfo[i].iDrawRow -= m_rtMaxBound.top;
        m_vtMinimapInfo[i].iDrawCol -= m_rtMaxBound.left;
    }

    int iMaxMinimapCol = m_rtMaxBound.right - m_rtMaxBound.left + 1;
    int iMaxMinimapRow = m_rtMaxBound.bottom - m_rtMaxBound.top + 1;

    // 타일의 최대 가로넓이 세로 넓이 만큼만 사각테두리를 그린다.
    // 하지만 너무 넓은 미니맵이라면 크기 제한을 두도록 한다.
    m_bTrace = true;
    m_Tile.m_fTileZoom = MIN_TILE_ZOOM;

    m_iPlusOffX     = 0;
    m_iPlusOffY     = 0;

    float fWidth        = GC_SCREEN_WIDTH  - MINIMAP_BLANK_INTERVAL*2 ;
    float fHeight       = GC_SCREEN_HEIGHT - MINIMAP_BLANK_INTERVAL*2 ;

    
    if( !m_pkMissionBox->IsEmpty() ) // 미션 진행상황 들어가면 잘려서..
        fWidth -= m_pkMissionBox->GetBackWidth();


    while(m_Tile.m_fTileZoom < MAX_TILE_ZOOM)
    {
        float fLastX        = (iMaxMinimapCol) * m_Tile.GetWidth();
        float fLastY        = (iMaxMinimapRow) * m_Tile.GetHeight();

        /*if( fLastX <= (GC_SCREEN_WIDTH * g_pGCDeviceManager2->GetWindowScaleX() - MINIMAP_BLANK_INTERVAL * 2 )
            && fLastY <= (GC_SCREEN_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY() - MINIMAP_BLANK_INTERVAL * 2)
            )*/
		if( fLastX <= fWidth && fLastY <= fHeight
			)
        {
            m_bTrace = false;
            break;
        }

        m_Tile.m_fTileZoom += 1.0f;
    }

    float fCurWidth     = (iMaxMinimapCol) * m_Tile.GetWidth() ;
    float fCurHeight    = (iMaxMinimapRow) * m_Tile.GetHeight();   

    if( fWidth >= fCurWidth )                    fWidth = fCurWidth;
    if( fHeight >= fCurHeight )                  fHeight = fCurHeight;
    if( fWidth < m_Tile.GetWidth() )             fWidth = m_Tile.GetWidth();
    if( fHeight < m_Tile.GetHeight() )           fHeight = m_Tile.GetHeight();

	/*fWidth*=m_fWindowScaleX;
	fHeight*=m_fWindowScaleY;*/
    SetSize((int)fWidth, (int)fHeight);
// 	SetWidth( (int)fWidth );
//  SetHeight( (int)fHeight );

	float fMissionBoxWidth = static_cast<float>( m_pkMissionBox->GetBackWidth() );
    float fRankBoxGapForMissionBox = 162.0f;
	if( m_pkMissionBox->IsEmpty() )
	{
		fMissionBoxWidth = 0;
        fRankBoxGapForMissionBox = 0.0f;

	}
	
	{
		float fCenterX = (GC_SCREEN_WIDTH * g_pGCDeviceManager2->GetWindowScaleX() - ( fWidth + fMissionBoxWidth )* m_fWindowScaleX)/2.0f / m_fWindowScaleX;
		float fCenterY = (GC_SCREEN_HEIGHT * g_pGCDeviceManager2->GetWindowScaleY() - fHeight * m_fWindowScaleY)/2.0f / m_fWindowScaleY ;

		SetWindowPos(D3DXVECTOR2(fCenterX , fCenterY));


		// 배경 윈도우와 같은 위치로 맞춰준다.
		// 또 줌 된 만큼 오프셋을 이동 시켜 위치를 맞춘다.
		m_iOffsetX      = (int)(fCenterX - (m_Tile.m_fTileZoom )/2);
		m_iOffsetY      = (int)(fCenterY - (m_Tile.m_fTileZoom )/2);
	}

	m_viewport.X		= (DWORD)(GetFixedWindowPos().x + MINIMAP_BOUND_INTERVAL) ;
	m_viewport.Y		= (DWORD)(GetFixedWindowPos().y + MINIMAP_BOUND_INTERVAL) ;
	m_viewport.Width	= (DWORD)(static_cast<float>(GetFixedWindowPos().x+GetWidth() - MINIMAP_BOUND_INTERVAL) * m_fWindowScaleX) ;
    m_viewport.Height	= (DWORD)(static_cast<float>(GetFixedWindowPos().y+GetHeight() - MINIMAP_BOUND_INTERVAL) * m_fWindowScaleY) ;
    m_viewport.MinZ		= 0.0f;
    m_viewport.MaxZ		= 1.0f;
	
	{
		D3DXVECTOR2 vPos;
		vPos.x = (float)GetWidth() + 3.f;
		vPos.y = 0.0f;
		m_pkMissionBox->SetWindowPos( vPos );
        
        D3DXVECTOR2 vPos2;
        vPos2.x = (float)GetWidth() + 3.f;
        vPos2.y = fRankBoxGapForMissionBox;

        m_pPartyRankInfoBox->SetWindowPos( vPos2 );
	}
}

bool KGCQuestTileMap::IsSideArrowToHere(int iDirection, int iRow, int iCol)
{
    switch(iDirection)
    {
    case DIRECTION_LEFT:
        {
            if( (iCol - 1) < 0 ) return false;
            TileMap *pTileMap = g_pkQuestManager->m_Quest.GetTileMap(iRow, iCol - 1);
            if( !pTileMap ) return false;

            int bRight     = (pTileMap->iD_Path >> 0) & 0x00000001;  // 1
            if( bRight ) return true;
            break;
        }
    case DIRECTION_UP:
        {
            if( (iRow - 1) < 0 ) return false;
            TileMap *pTileMap = g_pkQuestManager->m_Quest.GetTileMap(iRow - 1, iCol);
            if( !pTileMap ) return false;

            int bBottom    = (pTileMap->iD_Path >> 1) & 0x00000001;  // 4
            if( bBottom ) return true;
        }
        break;
    case DIRECTION_RIGHT:
        {
            if( (iCol + 1) >= g_pkQuestManager->m_Quest.iTileX ) return false;
            TileMap *pTileMap = g_pkQuestManager->m_Quest.GetTileMap(iRow, iCol + 1);
            if( !pTileMap ) return false;

            int bLeft      = (pTileMap->iD_Path >> 2) & 0x00000001;  // 4
            if( bLeft ) return true;
        }
        break;
    case DIRECTION_DOWN:
        {
            if( (iRow + 1) >= g_pkQuestManager->m_Quest.iTileY ) return false;
            TileMap *pTileMap = g_pkQuestManager->m_Quest.GetTileMap(iRow + 1, iCol);
            if( !pTileMap ) return false;

            int bTop       = (pTileMap->iD_Path >> 3) & 0x00000001;  // 8
            if( bTop ) return true;
        }
        break;
    }

    return false;
}

void KGCQuestTileMap::UpdateDescPos()
{
    D3DXVECTOR2 vPos = m_pBoxComponent[SBC_LEFT_BOTTOM]->GetFixedWindowLocalPos();
    vPos.x -= 3;
    vPos.y -= 24;
    m_pkTabImage->SetFixedWindowLocalPos( vPos );

    vPos = m_pBoxComponent[SBC_LEFT_BOTTOM]->GetFixedWindowLocalPos();
    vPos.x += 50;
    vPos.y -= 3;
    m_pkMinimapDesc->SetFixedWindowLocalPos( vPos );
}

void KGCQuestTileMap::FrameMoveInEnabledState( void )
{
    m_Tile.FrameMove_CurTile();     // 타일에 현재 플레이어 위치 애니매이션
}

void KGCQuestTileMap::SetPlayerRankInfo() 
{
    m_pPartyRankInfoBox->SetPlayerRankInfo();
};