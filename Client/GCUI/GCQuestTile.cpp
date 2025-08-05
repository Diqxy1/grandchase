#include "stdafx.h"
#include "GCQuestTile.h"



KGCQuestTile::KGCQuestTile()
: m_pML_Dungeon(NULL)
{
    m_pDungeon          = NULL;
    m_bCurStage         = false;
    m_iStageBackType    = -1;
    m_iStage            = -1;
    m_iDirection        = -1;
    m_bBrightNum        = false;
    m_fArrowZoom        = -2.5f;
    m_fTileZoom         = -36.0f;
    m_fStageBGZoom      = 45.0f;
    m_fNumberZoom       = 78.0f;
    m_iCurStageY        = 0;
    m_bCharDir          = true;
    m_bCurStageY_State  = false;

	m_fWindowScaleX = 1.0f;
	m_fWindowScaleY = 1.0f;

    m_rtRender.SetRect(0, 0, 100, 100);

    for(int i=0;i<QT_NUM_OF_ARROW;++i)
    {
        m_bDirBoth[i]           = false;
        m_bArrowIn[i]           = false;
        m_bArrowColorType[i]    = false;
    }
}

KGCQuestTile::~KGCQuestTile()
{
    SAFE_RELEASE(m_pDungeon);
    SAFE_RELEASE(m_pML_Dungeon);
}

void KGCQuestTile::OnCreate()
{
    SAFE_RELEASE(m_pDungeon);
    SAFE_RELEASE(m_pML_Dungeon);
    m_pDungeon = g_pGCDeviceManager2->CreateTexture("dungeon.dds");
    if(SiKGCMultipleLanguages()->UseMultipleLanguages())
        m_pML_Dungeon = g_pGCDeviceManager2->CreateTexture("dungeon_string.dds");


    m_rtStageBack[0].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(395,    512)
                            , CONVERT_PIXEL_TO_TEX_SIZE(1,      512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((395+74), 512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((1+74),   512)
                            );               // STAGE_ACTIVE

    m_rtStageBack[1].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(320,    512)
                            , CONVERT_PIXEL_TO_TEX_SIZE(1,      512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((320+74), 512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((1+74),   512)
                            );               // STAGE_NOTACTIVE

    m_rtStageBack[2].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(1,      512)
                            , CONVERT_PIXEL_TO_TEX_SIZE(111,    512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((1+74),   512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((111+74), 512)
                            );               // START
    
    m_rtStageBack[3].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(75,     512)
                            , CONVERT_PIXEL_TO_TEX_SIZE(111,    512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((75+74),  512)
                            , CONVERT_PIXEL_TO_TEX_SIZE((111+74), 512)
                            );               // BOSS
    
    m_rtCurStage.SetRect( CONVERT_PIXEL_TO_TEX_SIZE(151,    512)
                        , CONVERT_PIXEL_TO_TEX_SIZE(111,    512)
                        , CONVERT_PIXEL_TO_TEX_SIZE((151+74), 512)
                        , CONVERT_PIXEL_TO_TEX_SIZE((111+74), 512)
                        ); 

    // 첫번째 배열 : 화살표수
    // 두번째 배열 : 쌍방향 화살표 여부
    // 세번째 배열 : 흑백, 컬러 화살표 여부
    for(int i=0;i<2;++i)
    {
        m_rtArrow[0][0][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(93,     512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((93+74),  512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291+34-105*i), 512)
                                ); 
        m_rtArrow[0][1][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(93,     512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((359-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((93+74),  512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((359+34-105*i), 512)
                                ); 
        m_rtArrow[1][0][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(202,    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((202+34), 512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291+74-105*i), 512)
                                ); 
        m_rtArrow[1][1][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(237,    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((237+34), 512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291+74-105*i), 512)
                                ); 
        m_rtArrow[2][0][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(93,     512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((326-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((93+74),  512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((326+34-105*i), 512)
                                ); 
        m_rtArrow[2][1][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(93,     512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((359-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((93+74),  512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((359+34-105*i), 512)
                                ); 

        m_rtArrow[3][0][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(167,    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((167+34), 512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291+74-105*i), 512)
                                ); 
        m_rtArrow[3][1][i].SetRect( CONVERT_PIXEL_TO_TEX_SIZE(237,    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291-105*i),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((237+34), 512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((291+74-105*i), 512)
                                ); 
    }

    for(int i=0;i<QT_NUM_OF_NUMBER;++i)
    {
        m_rtNumber[i][0].SetRect( CONVERT_PIXEL_TO_TEX_SIZE((237 + i*20.1f),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE(136,            512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((237+17 + i*20.1f),  512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((136+17),         512)
                                );
    }

    for(int i=0;i<QT_NUM_OF_NUMBER;++i)
    {
        m_rtNumber[i][1].SetRect( CONVERT_PIXEL_TO_TEX_SIZE((237 + i*20.1f),    512)
                                , CONVERT_PIXEL_TO_TEX_SIZE(116,            512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((237+17 + i*20.1f),  512)
                                , CONVERT_PIXEL_TO_TEX_SIZE((116+17),         512)
                                );
    }

    // 반쪽짜리 텍스쳐를 준비해둔다.
    for(int j=0;j<2;++j)
    {
        for(int i=0;i<2;++i)
        {
            m_rtTArrowIn[ARROW_RIGHT-1][i][j] = m_rtArrow[ARROW_LEFT-1][i][j];
            m_rtTArrowIn[ARROW_RIGHT-1][i][j].right = m_rtTArrowIn[ARROW_RIGHT-1][i][j].right - (m_rtTArrowIn[ARROW_RIGHT-1][i][j].right - m_rtTArrowIn[ARROW_RIGHT-1][i][j].left)/2.0f;
            m_rtTArrowOut[ARROW_RIGHT-1][i][j] = m_rtArrow[ARROW_RIGHT-1][i][j];
            m_rtTArrowOut[ARROW_RIGHT-1][i][j].right = m_rtTArrowOut[ARROW_RIGHT-1][i][j].right - (m_rtTArrowOut[ARROW_RIGHT-1][i][j].right - m_rtTArrowOut[ARROW_RIGHT-1][i][j].left)/2.0f;

            m_rtTArrowIn[ARROW_BOTTOM-1][i][j] = m_rtArrow[ARROW_TOP-1][i][j];
            m_rtTArrowIn[ARROW_BOTTOM-1][i][j].bottom = m_rtTArrowIn[ARROW_BOTTOM-1][i][j].bottom - (m_rtTArrowIn[ARROW_BOTTOM-1][i][j].bottom - m_rtTArrowIn[ARROW_BOTTOM-1][i][j].top)/2.0f;
            m_rtTArrowOut[ARROW_BOTTOM-1][i][j] = m_rtArrow[ARROW_BOTTOM-1][i][j];
            m_rtTArrowOut[ARROW_BOTTOM-1][i][j].bottom = m_rtTArrowOut[ARROW_BOTTOM-1][i][j].bottom - (m_rtTArrowOut[ARROW_BOTTOM-1][i][j].bottom - m_rtTArrowOut[ARROW_BOTTOM-1][i][j].top)/2.0f;

            m_rtTArrowIn[ARROW_LEFT-1][i][j] = m_rtArrow[ARROW_RIGHT-1][i][j];
            m_rtTArrowIn[ARROW_LEFT-1][i][j].left = m_rtTArrowIn[ARROW_LEFT-1][i][j].left + (m_rtTArrowIn[ARROW_LEFT-1][i][j].right - m_rtTArrowIn[ARROW_LEFT-1][i][j].left)/2.0f;
            m_rtTArrowOut[ARROW_LEFT-1][i][j] = m_rtArrow[ARROW_LEFT-1][i][j];
            m_rtTArrowOut[ARROW_LEFT-1][i][j].left = m_rtTArrowOut[ARROW_LEFT-1][i][j].left + (m_rtTArrowOut[ARROW_LEFT-1][i][j].right - m_rtTArrowOut[ARROW_LEFT-1][i][j].left)/2.0f;

            m_rtTArrowIn[ARROW_TOP-1][i][j] = m_rtArrow[ARROW_BOTTOM-1][i][j];
            m_rtTArrowIn[ARROW_TOP-1][i][j].top = m_rtTArrowIn[ARROW_TOP-1][i][j].top + (m_rtTArrowIn[ARROW_TOP-1][i][j].bottom - m_rtTArrowIn[ARROW_TOP-1][i][j].top)/2.0f;
            m_rtTArrowOut[ARROW_TOP-1][i][j] = m_rtArrow[ARROW_TOP-1][i][j];
            m_rtTArrowOut[ARROW_TOP-1][i][j].top = m_rtTArrowOut[ARROW_TOP-1][i][j].top + (m_rtTArrowOut[ARROW_TOP-1][i][j].bottom - m_rtTArrowOut[ARROW_TOP-1][i][j].top)/2.0f;
        }
    }
}

void KGCQuestTile::SetArrow( int iDir )
{
    m_iDirection = iDir;
}

void KGCQuestTile::SetArrowBoth( QT_ARROW_TYPE enDir, bool bBoth )
{
    int iArrowType = (int)enDir - 1;
    m_bDirBoth[iArrowType] = bBoth;
}

void KGCQuestTile::SetArrowBothAll( bool bBoth )
{
    for(int i=0;i<QT_NUM_OF_ARROW;++i)
    {
        m_bDirBoth[i] = bBoth;
    }
}

void KGCQuestTile::SetStageBackType( int iType )
{
	if(4==iType)// 플래포머 맵이면
		iType=0;

    _JIF( iType < QT_NUM_OF_STAGE_BACK && iType >= -1, return );

    m_iStageBackType = iType;
    if( -1 == m_iStageBackType ) m_bBrightNum = false;
}

void KGCQuestTile::SetStage( int iStage )
{
    m_iStage = iStage;
}

void KGCQuestTile::Draw(void)
{  
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


        Draw_StageBack();

#if (!defined( __PATH__ ) && defined( NATION_KOREA )) || defined( _DEBUG )
        // 스테이지 번호는 [스테이지 배경 타입]이 [스테이지 모드]일때만 그린다.
        if( m_iStageBackType == STAGE_ACTIVE )
        {
            Draw_Number_1();
            Draw_Number_10();
        }
#endif
        Draw_Arrow();

        if( m_bCurStage )
        {
            FRect rtCurTile = GetZoomRect();

            // 보는 방향에 따라서 좌우 방향을 바꾼다.
            FRect rtCurStageUV = m_rtCurStage;
            if( !m_bCharDir )
                swap(rtCurStageUV.left, rtCurStageUV.right);
			
			g_pGCDeviceManager2->DrawInScreen(m_pDungeon
				, rtCurTile.left * m_fWindowScaleX
				, (rtCurTile.top + m_iCurStageY) * m_fWindowScaleY
				, rtCurTile.right * m_fWindowScaleX
				, (rtCurTile.bottom + m_iCurStageY) * m_fWindowScaleY
				, rtCurStageUV.left
				, rtCurStageUV.top
				, rtCurStageUV.right
				, rtCurStageUV.bottom);
          
        }
    }
	g_pGCDeviceManager2->PopState();    
}

void KGCQuestTile::Draw_StageBack(void)
{
    if( -1 == m_iStageBackType ) return;

    // Active 상태 이면, Bright 상태로 숫자를 표시해야하는지 체크하고
    // Bright 상태로 그려야 하면 그에 맞게 텍스쳐 영역 인덱스를 보정한다.

    int index = (m_iStageBackType + 1);

    if( STAGE_ACTIVE == m_iStageBackType )
        index -= (int)m_bBrightNum;

    FRect rtDraw = GetZoomRect();
	
    if(SiKGCMultipleLanguages()->UseMultipleLanguages() && (m_iStageBackType == START || m_iStageBackType == BOSS)){
        g_pGCDeviceManager2->DrawInScreen(m_pML_Dungeon
            ,rtDraw.left * m_fWindowScaleX
            , rtDraw.top * m_fWindowScaleY
            , rtDraw.right * m_fWindowScaleX
            , rtDraw.bottom * m_fWindowScaleY
            , m_rtStageBack[index].left
            , m_rtStageBack[index].top
            , m_rtStageBack[index].right
            , m_rtStageBack[index].bottom);
    }
    else{
        g_pGCDeviceManager2->DrawInScreen(m_pDungeon
            ,rtDraw.left * m_fWindowScaleX
            , rtDraw.top * m_fWindowScaleY
            , rtDraw.right * m_fWindowScaleX
            , rtDraw.bottom * m_fWindowScaleY
            , m_rtStageBack[index].left
            , m_rtStageBack[index].top
            , m_rtStageBack[index].right
            , m_rtStageBack[index].bottom);
    }

}

void KGCQuestTile::Draw_Arrow(void)
{
    SetArrowOutAll(false);

    switch( m_iDirection )
    {
        case 0:
            break;
        case 1:
            SetArrowOut(ARROW_RIGHT, true);
            break;
        case 2:
            SetArrowOut(ARROW_BOTTOM, true);
            break;
        case 3:
            SetArrowOut(ARROW_RIGHT, true);
            SetArrowOut(ARROW_BOTTOM, true);
            break;
        case 4:
            SetArrowOut(ARROW_LEFT, true);
            break;
        case 5:
            SetArrowOut(ARROW_LEFT, true);
            SetArrowOut(ARROW_RIGHT, true);
            break;
        case 6:
            SetArrowOut(ARROW_LEFT, true);
            SetArrowOut(ARROW_BOTTOM, true);
            break;
        case 7:
            SetArrowOut(ARROW_LEFT, true);
            SetArrowOut(ARROW_BOTTOM, true);
            SetArrowOut(ARROW_RIGHT, true);
            break;
        case 8:
            SetArrowOut(ARROW_TOP, true);
            break;
        case 9:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_RIGHT, true);
            break;
        case 10:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_BOTTOM, true);
            break;
        case 11:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_BOTTOM, true);
            SetArrowOut(ARROW_RIGHT, true);
            break;
        case 12:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_LEFT, true);
            break;
        case 13:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_LEFT, true);
            SetArrowOut(ARROW_RIGHT, true);
            break;
        case 14:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_LEFT, true);
            SetArrowOut(ARROW_BOTTOM, true);
            break;
        case 15:
            SetArrowOut(ARROW_TOP, true);
            SetArrowOut(ARROW_LEFT, true);
            SetArrowOut(ARROW_RIGHT, true);
            SetArrowOut(ARROW_BOTTOM, true);
            break;
    }
    
    Draw_Arrow_InOut(ARROW_TOP);
    Draw_Arrow_InOut(ARROW_LEFT);
    Draw_Arrow_InOut(ARROW_RIGHT);
    Draw_Arrow_InOut(ARROW_BOTTOM);
}

void KGCQuestTile::GetArrowDrawRect( QT_ARROW_TYPE enDir, FRect &rtArrow )
{
    const int iArrowIndex = (int)enDir - 1;
    const int iBothIndex = (int)m_bDirBoth[iArrowIndex];

    FRect rtTileZoom = GetTileZoomRect();
    rtArrow = GetZoomRect();

    switch( enDir )
    {
    case ARROW_RIGHT:
        {
            // 화살의 넓이를 타일크기의 반으로 한다.
            rtArrow.left = m_rtRender.left + (m_rtRender.right - m_rtRender.left)/2.0f;
            
            // 화살을 줌한다.
            ZoomWidthHeight(m_fArrowZoom, rtArrow);

            // 화살의 높이를 55% 축소시킨다. (세로 화살표기 때문에 크기를 맞추기 위해 축소시킨다.)
            ZoomHeight(ARROW_IN_ZOOM, rtArrow);

            // 타일 줌으로 늘어난 오차만큼 이동시킨다.
            float fInterval = (rtTileZoom.right - rtArrow.right);
            rtArrow.right += fInterval;
            rtArrow.left += fInterval;
        }
        break;
    case ARROW_BOTTOM:
        {
            // 화살의 높이를 타일 크기의 반으로 한다.
            rtArrow.top = m_rtRender.top + (m_rtRender.bottom - m_rtRender.top)/2.0f;

            // 화살을 줌한다.
            ZoomWidthHeight(m_fArrowZoom, rtArrow);

            // 화살의 넓이를 55% 축소시킨다. (세로 화살표기 때문에 크기를 맞추기 위해 축소시킨다.)
            ZoomWidth(ARROW_IN_ZOOM, rtArrow);

            // 타일 줌으로 늘어난 오차만큼 이동시킨다.
            float fInterval = (rtTileZoom.bottom - rtArrow.bottom);
            rtArrow.bottom += fInterval;
            rtArrow.top += fInterval;
        }
        break;
    case ARROW_LEFT:
        {
            // 화살의 넓이를 타일 크기의 반으로 한다.
            rtArrow.right = m_rtRender.left + (m_rtRender.right - m_rtRender.left)/2.0f;

            // 화살을 줌한다.
            ZoomWidthHeight(m_fArrowZoom, rtArrow);

            // 화살의 넓이를 55% 축소시킨다. (세로 화살표기 때문에 크기를 맞추기 위해 축소시킨다.)
            ZoomHeight(ARROW_IN_ZOOM, rtArrow);

            // 타일 줌으로 늘어난 오차만큼 이동시킨다.
            float fInterval = (rtArrow.left - rtTileZoom.left);
            rtArrow.left -= fInterval;
            rtArrow.right -= fInterval;
        }
        break;
    case ARROW_TOP:
        {
            // 화살의 높이를 타일 크기의 반으로 한다.
            rtArrow.bottom = m_rtRender.top + (m_rtRender.bottom - m_rtRender.top)/2.0f;

            // 화살을 줌한다.
            ZoomWidthHeight(m_fArrowZoom, rtArrow);

            // 화살의 높이를 55% 축소시킨다. (세로 화살표기 때문에 크기를 맞추기 위해 축소시킨다.)
            ZoomWidth(ARROW_IN_ZOOM, rtArrow);

            // 타일 줌으로 늘어난 오차만큼 이동시킨다.
            float fInterval = (rtArrow.top - rtTileZoom.top);
            rtArrow.top -= fInterval;
            rtArrow.bottom -= fInterval;
        }
        break;
    }
}

void KGCQuestTile::GetTArrowInDrawRect( QT_ARROW_TYPE enDir, FRect &rtTArrow )
{
    const int iArrowIndex   = (int)enDir - 1;
    const int iBothIndex    = (int)m_bDirBoth[iArrowIndex];
    const int iColorIndex   = (int)m_bArrowColorType[iArrowIndex];

    switch( enDir )
    {
    case ARROW_RIGHT:
        rtTArrow = m_rtTArrowIn[ARROW_RIGHT-1][iBothIndex][iColorIndex];
        break;
    case ARROW_BOTTOM:
        rtTArrow = m_rtTArrowIn[ARROW_BOTTOM-1][iBothIndex][iColorIndex];
        break;
    case ARROW_LEFT:
        rtTArrow = m_rtTArrowIn[ARROW_LEFT-1][iBothIndex][iColorIndex];
        break;
    case ARROW_TOP:
        rtTArrow = m_rtTArrowIn[ARROW_TOP-1][iBothIndex][iColorIndex];
        break;
    }
}

void KGCQuestTile::GetTArrowOutDrawRect( QT_ARROW_TYPE enDir, FRect &rtTArrow )
{
    const int iArrowIndex   = (int)enDir - 1;
    const int iBothIndex    = (int)m_bDirBoth[iArrowIndex];
    const int iColorIndex   = (int)m_bArrowColorType[iArrowIndex];

    switch( enDir )
    {
    case ARROW_RIGHT:
        rtTArrow = m_rtTArrowOut[ARROW_RIGHT-1][iBothIndex][iColorIndex];
        break;
    case ARROW_BOTTOM:
        rtTArrow = m_rtTArrowOut[ARROW_BOTTOM-1][iBothIndex][iColorIndex];
        break;
    case ARROW_LEFT:
        rtTArrow = m_rtTArrowOut[ARROW_LEFT-1][iBothIndex][iColorIndex];
        break;
    case ARROW_TOP:
        rtTArrow = m_rtTArrowOut[ARROW_TOP-1][iBothIndex][iColorIndex];
        break;
    }
}

void KGCQuestTile::Draw_Arrow_In( QT_ARROW_TYPE enDir )
{
    FRect rtArrow;
    FRect rtTArrow;
    GetArrowDrawRect( enDir, rtArrow );
    GetTArrowInDrawRect( enDir, rtTArrow );

	g_pGCDeviceManager2->DrawInScreen(m_pDungeon
		,rtArrow.left * m_fWindowScaleX
		, rtArrow.top * m_fWindowScaleY
		, rtArrow.right * m_fWindowScaleX
		, rtArrow.bottom * m_fWindowScaleY
		, rtTArrow.left
		, rtTArrow.top 
		, rtTArrow.right 
		, rtTArrow.bottom );

}

void KGCQuestTile::Draw_Arrow_Out(QT_ARROW_TYPE enDir)
{
    FRect rtArrow;
    FRect rtTArrow;
    GetArrowDrawRect( enDir, rtArrow );
    GetTArrowOutDrawRect( enDir, rtTArrow );	

	g_pGCDeviceManager2->DrawInScreen(m_pDungeon
		,rtArrow.left * m_fWindowScaleX
		, rtArrow.top * m_fWindowScaleY
		, rtArrow.right * m_fWindowScaleX
		, rtArrow.bottom * m_fWindowScaleY
		, rtTArrow.left 
		, rtTArrow.top
		, rtTArrow.right 
		, rtTArrow.bottom);


}

void KGCQuestTile::Draw_Arrow_InOut( QT_ARROW_TYPE enDir )
{
    const int iArrowIndex = (int)enDir - 1;

    // 쌍방향 화살표를 그려야하는지 확인하고 그렇다면 체크!
    if( m_bArrowIn[ iArrowIndex ] && m_bArrowOut[ iArrowIndex ] )
        SetArrowBoth(enDir, true);
    else
        SetArrowBoth(enDir, false);

    if( m_bArrowIn[ iArrowIndex ] )         // 이웃에서 내타일로 들어오는 화살표를 그려야 하나?
    {
        Draw_Arrow_In(enDir);
    }
    else if( m_bArrowOut[ iArrowIndex ] )
    {
        Draw_Arrow_Out(enDir);              // 내타일에서 이웃으로 나가는 화살표를 그려야 하나?
    }
}

void KGCQuestTile::Draw_Number_1(void)
{
    if( 0 > m_iStage ) return;

    FRect rtDraw = GetZoomRect();
    ZoomWidthHeight(m_fNumberZoom, rtDraw);
    ZoomWidth(20, rtDraw);


    // 숫자의 넓이와 높이를 백업해둔다. ( 숫자의 위치를 Zoom된 타일크기에 다시 맞추기 때문. )
    const float fWidth = rtDraw.right - rtDraw.left;
    const float fHeight = rtDraw.bottom - rtDraw.top;

    // Zoom 된 타일크기에 숫자의 위치와 크기를 다시 맞춘다.
    FRect rtBack = GetZoomRect();
    rtDraw.bottom = rtBack.bottom - 2;
    rtDraw.left = rtBack.right - fWidth;
    rtDraw.right = rtBack.right;
    rtDraw.top = rtBack.bottom - fHeight;

    const int iValue_1 = m_iStage % 10;

	g_pGCDeviceManager2->DrawInScreen(m_pDungeon
		, rtDraw.left * m_fWindowScaleX
		, rtDraw.top * m_fWindowScaleY
		, rtDraw.right * m_fWindowScaleX
		, rtDraw.bottom * m_fWindowScaleY
		, m_rtNumber[iValue_1][(int)m_bBrightNum].left
		, m_rtNumber[iValue_1][(int)m_bBrightNum].top
		, m_rtNumber[iValue_1][(int)m_bBrightNum].right
		, m_rtNumber[iValue_1][(int)m_bBrightNum].bottom);

}

void KGCQuestTile::Draw_Number_10(void)
{
    if( 0 > m_iStage ) return;

    FRect rtDraw = GetZoomRect();
    ZoomWidthHeight(m_fNumberZoom, rtDraw);
    ZoomWidth(20, rtDraw);

    // 숫자의 넓이와 높이를 백업해둔다. ( 숫자의 위치를 Zoom된 타일크기에 다시 맞추기 때문. )
    float fWidth = rtDraw.right - rtDraw.left;
    float fHeight = rtDraw.bottom - rtDraw.top;

    // Zoom 된 타일크기에 숫자의 위치와 크기를 다시 맞춘다.
    FRect rtBack = GetZoomRect();
    rtDraw.bottom = rtBack.bottom - 2;
    rtDraw.left = rtBack.right - fWidth;
    rtDraw.right = rtBack.right;
    rtDraw.top = rtBack.bottom - fHeight;

    const int iValue_10 = m_iStage / 10;

	g_pGCDeviceManager2->DrawInScreen(m_pDungeon
		,  (rtDraw.left - fWidth) * m_fWindowScaleX
		, rtDraw.top * m_fWindowScaleY
		, (rtDraw.right - fWidth) * m_fWindowScaleX
		, rtDraw.bottom * m_fWindowScaleY
		, m_rtNumber[iValue_10][(int)m_bBrightNum].left
		, m_rtNumber[iValue_10][(int)m_bBrightNum].top
		, m_rtNumber[iValue_10][(int)m_bBrightNum].right
		, m_rtNumber[iValue_10][(int)m_bBrightNum].bottom);

}

void KGCQuestTile::SetPos(D3DXVECTOR2 vPos, D3DXVECTOR2 vScale)
{
    SetPos(vPos.x, vPos.y,vScale.x, vScale.y);
}

void KGCQuestTile::SetPos(float fX, float fY,float fWindowScaleX, float fWindowScaleY)
{
	m_fWindowScaleX = fWindowScaleX;
	m_fWindowScaleY = fWindowScaleY;

    const float fWidth = (m_rtRender.right - m_rtRender.left);
    const float fHeight = (m_rtRender.bottom - m_rtRender.top);

    m_rtRender.left     = fX;
    m_rtRender.top      = fY;
    m_rtRender.right    = m_rtRender.left + fWidth;
    m_rtRender.bottom   = m_rtRender.top + fHeight;
}

D3DXVECTOR2 KGCQuestTile::GetPos(void)
{
    D3DXVECTOR2 vPos;
    vPos.x = m_rtRender.left;
    vPos.y = m_rtRender.top;

    return vPos;
}

void KGCQuestTile::SetWidthHeight(D3DXVECTOR2 vWH)
{
    SetWidth(vWH.x);
    SetHeight(vWH.y);
}

D3DXVECTOR2 KGCQuestTile::GetWidthHeight(void)
{
    D3DXVECTOR2 vWH;
    vWH.x = GetWidth();
    vWH.y = GetHeight();

    return vWH;
}

void KGCQuestTile::SetWidth(float fWidth)
{
    m_rtRender.right = m_rtRender.left + fWidth;
}

float KGCQuestTile::GetWidth(void)
{
    FRect rtTile = GetTileZoomRect();

    return (rtTile.right - rtTile.left);
}

void KGCQuestTile::SetHeight(float fHeight)
{
    m_rtRender.bottom = m_rtRender.top + fHeight;
}

float KGCQuestTile::GetHeight(void)
{
    FRect rtTile = GetTileZoomRect();

    return (rtTile.bottom - rtTile.top);
}

void KGCQuestTile::SetArrowIn( QT_ARROW_TYPE enDir, bool bShow )
{
    const int iArrowIndex = (int)enDir - 1;
    m_bArrowIn[iArrowIndex] = bShow;
}

void KGCQuestTile::SetArrowInAll( bool bShow )
{
    for(int i=0;i<QT_NUM_OF_ARROW;++i)
    {
        m_bArrowIn[i] = bShow;
    }
}

void KGCQuestTile::SetArrowOutAll( bool bShow )
{
    for(int i=0;i<QT_NUM_OF_ARROW;++i)
    {
        m_bArrowOut[i] = bShow;
    }
}

void KGCQuestTile::SetArrowOut( QT_ARROW_TYPE enDir, bool bShow )
{
    const int iArrowIndex = (int)enDir - 1;
    m_bArrowOut[iArrowIndex] = bShow;
}

void KGCQuestTile::SetArrowColorType( QT_ARROW_TYPE enDir, bool bShow )
{
    const int iArrowIndex = (int)enDir - 1;
    m_bArrowColorType[iArrowIndex] = bShow;
}

void KGCQuestTile::SetArrowColorTypeAll( bool bShow )
{
    for(int i=0;i<QT_NUM_OF_ARROW;++i)
    {
        m_bArrowColorType[i] = bShow;
    }
}

float KGCQuestTile::Percent(float fPercent, float fOriginal)
{
    return (fOriginal*fPercent / 100.0f);
}

void KGCQuestTile::ZoomWidth(float fPercent, FRect &rtRect)
{
    float fWidth = rtRect.right - rtRect.left;
    float fZoomSize = Percent(fPercent, fWidth)/2.0f;      // 화살표가 타일보다 55% 정도 작아서.

    rtRect.left += fZoomSize;
    rtRect.right -= fZoomSize;
}

void KGCQuestTile::ZoomHeight(float fPercent, FRect &rtRect)
{
    float fHeight = rtRect.bottom - rtRect.top;
    float fZoomSize = Percent(fPercent, fHeight)/2.0f;      // 화살표가 타일보다 55% 정도 작아서.

    rtRect.top += fZoomSize;
    rtRect.bottom -= fZoomSize;
}

void KGCQuestTile::ZoomWidthHeight(float fPercent, FRect &rtRect)
{
    ZoomWidth(fPercent, rtRect);
    ZoomHeight(fPercent, rtRect);
}

void KGCQuestTile::SetBrightNum( bool bBrightNum )
{
    m_bBrightNum = bBrightNum;
}

FRect KGCQuestTile::GetZoomRect(void)
{
    FRect rtZoom = m_rtRender;
    ZoomWidthHeight(m_fTileZoom, rtZoom);
    ZoomWidthHeight(m_fStageBGZoom, rtZoom);

    return rtZoom;
}

FRect KGCQuestTile::GetTileZoomRect(void)
{
    FRect rtZoom = m_rtRender;
    ZoomWidthHeight(m_fTileZoom, rtZoom);

    return rtZoom;
}

void KGCQuestTile::FrameMove_CurTile(void)
{
    if( m_iCurStageY > 0 )
        m_bCurStageY_State = false;
    else if( m_iCurStageY < -5 )
        m_bCurStageY_State = true;

    if( m_bCurStageY_State )
        m_iCurStageY++;
    else
        m_iCurStageY--;
}

void KGCQuestTile::SetCharDirection( bool bRight )
{
    m_bCharDir = bRight;   
}
