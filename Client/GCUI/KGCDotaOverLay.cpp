#include "stdafx.h"
#include "KGCDotaOverLay.h"

IMPLEMENT_CLASSNAME( KGCDotaPointOverLay );
IMPLEMENT_WND_FACTORY( KGCDotaPointOverLay );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaPointOverLay, "gc_dota_point" );

KGCDotaPointOverLay::KGCDotaPointOverLay()
: m_pkStaticMyPoint( NULL )
, m_pCurDotaInfo( NULL )
, m_iMyPoint( -1 )
{
    LINK_CONTROL( "static_point", m_pkStaticMyPoint );

    ZeroMemory(m_pkStaticItem, sizeof(KD3DStatic*) * 5);
    LINK_CONTROL( "static_atk", m_pkStaticItem[0] );
    LINK_CONTROL( "static_hp", m_pkStaticItem[1] );
    LINK_CONTROL( "static_cri", m_pkStaticItem[2] );
    LINK_CONTROL( "static_npcatk", m_pkStaticItem[3] );
    LINK_CONTROL( "static_npchp", m_pkStaticItem[4] );
}

KGCDotaPointOverLay::~KGCDotaPointOverLay()
{}

void KGCDotaPointOverLay::OnDestroy( void ){};
void KGCDotaPointOverLay::OnCreateComplete( void )
{
    if( m_pkStaticMyPoint )
    {
        m_pkStaticMyPoint->SetAlign( DT_RIGHT );
        m_pkStaticMyPoint->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
        m_pkStaticMyPoint->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_pkStaticMyPoint->InitState( true );
    }

    for(int iLoop=0; iLoop<NUM_ITEM; ++iLoop)
    {
        m_pkStaticItem[iLoop]->SetMultiLine( true );
        m_pkStaticItem[iLoop]->SetFontSpace( 1.5f );
        m_pkStaticItem[iLoop]->SetFontColor( D3DCOLOR_ARGB(255, 255, 249, 217) );
        m_pkStaticItem[iLoop]->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_pkStaticItem[iLoop]->InitState( true );
    }

    m_pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();
    m_iMyPoint = -1;
};
void KGCDotaPointOverLay::ActionPerformed( const KActionEvent& event ){};
void KGCDotaPointOverLay::FrameMoveInEnabledState( void )
{
    WCHAR wszTemp[128];
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iPlayerIdx ) && m_pkStaticMyPoint &&
        ( m_iMyPoint != g_MyD3D->MyPlayer[iPlayerIdx]->Get_D_Point()) )
    {
        m_iMyPoint = g_MyD3D->MyPlayer[iPlayerIdx]->Get_D_Point();
        wsprintf(wszTemp, L"%d/%d", m_iMyPoint, g_MyD3D->MyPlayer[iPlayerIdx]->Get_MAX_D_Point() );
        m_pkStaticMyPoint->SetText( wszTemp );
    }

    if( m_pCurDotaInfo )
    {
        wsprintf(wszTemp, L"Lv.%d", m_pCurDotaInfo->sDPointItemInfo.iUseCntAtk);
        m_pkStaticItem[ITEM_ATK]->SetTextAutoMultiline( wszTemp );
        wsprintf(wszTemp, L"Lv.%d", m_pCurDotaInfo->sDPointItemInfo.iUseCntFullHp);
        m_pkStaticItem[ITEM_HP]->SetTextAutoMultiline( wszTemp );
        wsprintf(wszTemp, L"Lv.%d", m_pCurDotaInfo->sDPointItemInfo.iUseCntCri);
        m_pkStaticItem[ITEM_CRI]->SetTextAutoMultiline( wszTemp );
        wsprintf(wszTemp, L"Lv.%d", m_pCurDotaInfo->sDPointItemInfo.iUseCntNPCAtk);
        m_pkStaticItem[ITEM_NPCATK]->SetTextAutoMultiline( wszTemp );
        wsprintf(wszTemp, L"Lv.%d", m_pCurDotaInfo->sDPointItemInfo.iUseCntNPCHp);
        m_pkStaticItem[ITEM_NPCHP]->SetTextAutoMultiline( wszTemp );
    }
};


IMPLEMENT_CLASSNAME( KGCDotaCoreHpBar );
IMPLEMENT_WND_FACTORY( KGCDotaCoreHpBar );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaCoreHpBar, "gc_dota_corehpbar" );

KGCDotaCoreHpBar::KGCDotaCoreHpBar()
{
    m_pkGauge[TEAM_SERDIN] = NULL;
    LINK_CONTROL( "serdin_gauge", m_pkGauge[TEAM_SERDIN] );
    m_pkGauge[TEAM_CANABAN] = NULL;
    LINK_CONTROL( "canaban_gauge", m_pkGauge[TEAM_CANABAN] );
}

KGCDotaCoreHpBar::~KGCDotaCoreHpBar(){}
void KGCDotaCoreHpBar::OnDestroy( void ){};
void KGCDotaCoreHpBar::OnCreateComplete( void )
{
    if( m_pkGauge[TEAM_SERDIN] )
        m_pkGauge[TEAM_SERDIN]->InitState( true );
    if( m_pkGauge[TEAM_CANABAN] )
        m_pkGauge[TEAM_CANABAN]->InitState( true );

    m_pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();
};
void KGCDotaCoreHpBar::ActionPerformed( const KActionEvent& event ){};
void KGCDotaCoreHpBar::FrameMoveInEnabledState( void )
{
    if( m_pCurDotaInfo )
    {
        float fUnit;
        int iSerdinCoreIdx = m_pCurDotaInfo->iCoreSerdin_MonSlotID;
        int iCanabanCoreIdx = m_pCurDotaInfo->iCoreCanaban_MonSlotID;
		MONSTER* pMonster = g_kMonsterManager.GetMonster(iSerdinCoreIdx);
		MONSTER* pMonsterC = g_kMonsterManager.GetMonster(iCanabanCoreIdx);
		if( pMonster  != NULL && pMonsterC != NULL ) {
			fUnit = MAX_HP_WND_SIZE / pMonster->GetMaxHP();
			m_pkGauge[TEAM_SERDIN]->SetWidth( DWORD(pMonster->GetHP() * fUnit) );

			// 세르딘 HP바 위치잡기( 왼쪽에서 오른쪽으로 줄어드는 식이라 위치변경 필요함 )
			float fHpGap = pMonster->GetMaxHP() - pMonster->GetHP();
			D3DXVECTOR2 vPos = m_pkGauge[TEAM_SERDIN]->GetFixedWindowLocalPos();
			vPos.x = CANABAN_HP_BAR_ORI_POS + (fHpGap * fUnit);
			m_pkGauge[TEAM_SERDIN]->SetFixedWindowLocalPos( vPos );

			fUnit = MAX_HP_WND_SIZE / pMonsterC->GetMaxHP();
			m_pkGauge[TEAM_CANABAN]->SetWidth( DWORD(pMonsterC->GetHP() * fUnit) );
		}
    }
};



IMPLEMENT_CLASSNAME( KGCDotaCount );
IMPLEMENT_WND_FACTORY( KGCDotaCount );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaCount, "gc_dota_count" );

KGCDotaCount::KGCDotaCount()
: iNowCount( 0 )
, iAlphaCnt( 255 )
{
    ZeroMemory( m_pkNumber, sizeof(KD3DWnd*) * MAX_COUNT );

    char szTemp[128];
    for(int iLoop=0; iLoop<MAX_COUNT; ++iLoop)
    {
        sprintf(szTemp, "num%d", (iLoop+1));
        LINK_CONTROL( szTemp, m_pkNumber[iLoop] );
    }
}

KGCDotaCount::~KGCDotaCount(){}
void KGCDotaCount::OnDestroy( void ){};
void KGCDotaCount::OnCreateComplete( void )
{
    for(int iLoop=0; iLoop<MAX_COUNT; ++iLoop)
    {
        if( m_pkNumber[iLoop] )
            m_pkNumber[iLoop]->InitState( false );
    }
};
void KGCDotaCount::ActionPerformed( const KActionEvent& event ){};
void KGCDotaCount::FrameMoveInEnabledState( void )
{
    if( (0 < iAlphaCnt) && (0 < iNowCount && iNowCount <= MAX_COUNT) )
    {
        m_pkNumber[(iNowCount-1)]->SetAllWindowAlpha( iAlphaCnt );
        iAlphaCnt -= 5;
        if( iAlphaCnt < 0 ) iAlphaCnt = 0;
    }
    else if( (0 < iNowCount && iNowCount <= MAX_COUNT) )
    {
        m_pkNumber[(iNowCount-1)]->ToggleRender( false );
        this->ToggleRender( false );
    }
};
void KGCDotaCount::UpdateCount( int iCount )
{
    if( (iNowCount != iCount) && (0 < iCount && iCount <= MAX_COUNT ) )
    {
        for(int iLoop=0; iLoop<MAX_COUNT; ++iLoop)
        {
            if( iLoop == (iCount-1) )
            {
                m_pkNumber[iLoop]->SetAllWindowAlpha( iAlphaCnt );
                m_pkNumber[iLoop]->ToggleRender( true );
                this->ToggleRender( true );
            }
            else
            {
                m_pkNumber[iLoop]->SetAllWindowAlpha( 0 );
                m_pkNumber[iLoop]->ToggleRender( false );
            }
        }
        iAlphaCnt = 255;
        iNowCount = iCount;
    }
};



IMPLEMENT_CLASSNAME( KGCDotaItemImg );
IMPLEMENT_WND_FACTORY( KGCDotaItemImg );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaItemImg, "gc_dota_itemimg" );

KGCDotaItemImg::KGCDotaItemImg()
: m_pkImgSelectItem( NULL )
, m_pkItemDesc( NULL )
{
    LINK_CONTROL( "img", m_pkImgSelectItem );
    LINK_CONTROL( "static_desc", m_pkItemDesc );
}

KGCDotaItemImg::~KGCDotaItemImg(){}

void KGCDotaItemImg::OnDestroy( void )
{
    DeleteItemImg();
}
void KGCDotaItemImg::OnCreateComplete( void )
{
    if( m_pkImgSelectItem )
    {
        m_pkImgSelectItem->InitState( false );
        m_pkImgSelectItem->SetItemTexture( NULL );
    }
    if( m_pkItemDesc )
    {
        m_pkItemDesc->SetAlign( DT_CENTER );
        m_pkItemDesc->InitState( false );
    }


    if( m_vecItemImg.empty() )
        CreateItemImg();
}
void KGCDotaItemImg::ActionPerformed( const KActionEvent& event ){}
void KGCDotaItemImg::FrameMoveInEnabledState( void )
{
    if( 0 < m_iAlpha )
    {
        if( 0 == m_iViewCoolTime )  m_iAlpha -= 10;
        else                        --m_iViewCoolTime;
        if( m_iAlpha < 0 ) m_iAlpha = 0;
        m_pkImgSelectItem->SetColor( D3DCOLOR_ARGB( m_iAlpha, 255, 255, 255 ) );
    }
    else
    {
        m_iAlpha = 0;
        m_iViewCoolTime = 0;
        m_iSelectItem = -1;
        m_pkImgSelectItem->SetItemTexture( NULL );
        m_pkImgSelectItem->ToggleRender( false );
        m_pkItemDesc->ToggleRender( false );
        m_pkItemDesc->SetText(L"");
        this->ToggleRender( false );
    }
}

void KGCDotaItemImg::CreateItemImg()
{
    DeleteItemImg();

    char szTemp[128];
    GCDeviceTexture* pTex;
    for(int iLoop=0; iLoop<GC_GAME_ITEM_NUM; ++iLoop)
    {
        sprintf(szTemp, "item%02d.dds", iLoop);
        pTex = g_pGCDeviceManager2->CreateTexture( szTemp );
        m_vecItemImg.push_back( pTex );
    }
}
void KGCDotaItemImg::DeleteItemImg()
{
    if( m_pkImgSelectItem )
        m_pkImgSelectItem->SetItemTexture( NULL );

    std::vector<GCDeviceTexture*>::iterator vecIter = m_vecItemImg.begin();
    for(; vecIter != m_vecItemImg.end(); ++vecIter )
    {
        SAFE_RELEASE( (*vecIter) );
    }
    m_vecItemImg.clear();
}
void KGCDotaItemImg::ShowItemImg(int iGameItem)
{
    if( 0 > iGameItem || iGameItem > GC_GAME_ITEM_NUM )
        return;

    // 도타事?열려있다면 도타?I에서 띄우도록 해야함....(ㅠㅠ)
    if( KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_DOTA_SHOP ) )
    {
        g_pkUIScene->m_pkDotaShop->SetBuyItem(iGameItem, m_vecItemImg[iGameItem], 255, 55);
    }
    else
    {
        m_iSelectItem = iGameItem;
        m_iAlpha = 255;
        m_iViewCoolTime = 55;

        m_pkImgSelectItem->SetItemTexture( m_vecItemImg[iGameItem] );
        m_pkImgSelectItem->SetColor( D3DCOLOR_ARGB( m_iAlpha, 255, 255, 255 ) );
        m_pkImgSelectItem->ToggleRender( true );

        m_pkItemDesc->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );
        m_pkItemDesc->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_pkItemDesc->SetText( g_pkStrLoader->GetReplacedString(STR_USE_DOTA_GAMEITEM, "I", g_pkUIScene->m_pkDotaShop->GetItemName( iGameItem )) );
        m_pkItemDesc->ToggleRender( true );

        this->ToggleRender( true );
    }
}

IMPLEMENT_CLASSNAME( KGCDotaGuide );
IMPLEMENT_WND_FACTORY( KGCDotaGuide );
IMPLEMENT_WND_FACTORY_NAME( KGCDotaGuide, "gc_dota_guide" );

KGCDotaGuide::KGCDotaGuide()
: m_pCurDotaInfo( NULL )
, m_bShow( false )
, m_iNumAddGuidex( 0 )
{
    ZeroMemory(m_pkTexImg, sizeof(GCDeviceTexture*) * MAX_IMG);
    ZeroMemory(m_pkWndImg, sizeof(KGCItemImgWnd*) * MAX_GUIDE);
    ZeroMemory(m_pkStatic, sizeof(KD3DStatic*) * MAX_GUIDE);
    ZeroMemory(m_pkWndBack, sizeof(KD3DSizingBox*) * MAX_COLOR * MAX_GUIDE);
    memset(m_NowGuideID, -1, sizeof(int) * MAX_GUIDE);
    m_mapOneTimeShow.clear();

    LINK_CONTROL( "back_red1", m_pkWndBack[GUIDE_BACK_RED][GUIDE_TOP] );
    LINK_CONTROL( "back_blue1", m_pkWndBack[GUIDE_BACK_BLUE][GUIDE_TOP] );
    LINK_CONTROL( "back_red2", m_pkWndBack[GUIDE_BACK_RED][GUIDE_MIDDLE] );
    LINK_CONTROL( "back_blue2", m_pkWndBack[GUIDE_BACK_BLUE][GUIDE_MIDDLE] );
    LINK_CONTROL( "back_red3", m_pkWndBack[GUIDE_BACK_RED][GUIDE_BOTTOM] );
    LINK_CONTROL( "back_blue3", m_pkWndBack[GUIDE_BACK_BLUE][GUIDE_BOTTOM] );
    
    LINK_CONTROL( "static_desc1", m_pkStatic[GUIDE_TOP] );
    LINK_CONTROL( "static_desc2", m_pkStatic[GUIDE_MIDDLE] );
    LINK_CONTROL( "static_desc3", m_pkStatic[GUIDE_BOTTOM] );

    LINK_CONTROL( "img1", m_pkWndImg[GUIDE_TOP] );
    LINK_CONTROL( "img2", m_pkWndImg[GUIDE_MIDDLE] );
    LINK_CONTROL( "img3", m_pkWndImg[GUIDE_BOTTOM] );
}

KGCDotaGuide::~KGCDotaGuide(){}

void KGCDotaGuide::OnCreateComplete( void )
{
    for(int iLoop=0; iLoop<MAX_GUIDE; ++iLoop)
    {
        if( m_pkWndImg[iLoop] )
            m_pkWndImg[iLoop]->InitState( false, false );
        if( m_pkStatic[iLoop] )
            m_pkStatic[iLoop]->InitState( false, false );
        if( m_pkWndBack[GUIDE_BACK_RED][iLoop] )
            m_pkWndBack[GUIDE_BACK_RED][iLoop]->InitState( false, false );
        if( m_pkWndBack[GUIDE_BACK_BLUE][iLoop] )
            m_pkWndBack[GUIDE_BACK_BLUE][iLoop]->InitState( false, false );
    }

    m_bShow = false;
    m_iNumAddGuidex = 0;
    memset(m_NowGuideID, -1, sizeof(int) * MAX_GUIDE);
    m_pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();

    // 한번만 출력되어야하는 메시지 셋팅
    SetOneTimeGuide();

    // 텍스쳐 로드
    CreateTexture();
}

void KGCDotaGuide::SetOneTimeGuide()
{
    m_mapOneTimeShow.clear();
    m_mapOneTimeShow[SDotaGameGuide::DOTA_GUIDE_FIRST_DEATH] = false;
    m_mapOneTimeShow[SDotaGameGuide::DOTA_GUIDE_FIRST_KILL] = false;
    m_mapOneTimeShow[SDotaGameGuide::DOTA_GUIDE_APOINT] = false;
    m_mapOneTimeShow[SDotaGameGuide::DOTA_GUIDE_IN_ZONE] = false;
}

void KGCDotaGuide::CreateTexture( void )
{
    char szTemp[MAX_IMG][128] = 
    {
        "notice1.dds",
        "notice2.dds",
        "notice2_1.dds",
        "notice3.dds",
        "notice3_1.dds",
        "notice4.dds",
        "notice5.dds",
        "notice5_1.dds",
    };

    for(int iLoop=0; iLoop<MAX_IMG; ++iLoop)
    {
        if( NULL == m_pkTexImg[iLoop] )
        {
            m_pkTexImg[iLoop] = g_pGCDeviceManager2->CreateTexture( szTemp[iLoop] );
        }
    }
}

void KGCDotaGuide::DestroyTexture( void )
{
    for(int iLoop=0; iLoop<MAX_GUIDE; ++iLoop)
    {
        m_pkWndImg[iLoop]->SetItemTexture( NULL );
    }

    for(int iLoop=0; iLoop<MAX_IMG; ++iLoop)
    {
        SAFE_DELETE( m_pkTexImg[iLoop] );
    }
}

void KGCDotaGuide::OnDestroy( void )
{
    DestroyTexture();
}

void KGCDotaGuide::AddGuide( int iGuideID )
{
    // 가이드 중복출력 막기
    if( CheckDuplicationGuide( iGuideID ) )
        return;

    // 한번만 출력되야하는 가이드 처리
    std::map<int, bool>::iterator mapIter = m_mapOneTimeShow.find( iGuideID );
    if( mapIter != m_mapOneTimeShow.end() )
    {
        if( mapIter->second )
            return;
        else
            mapIter->second = true;
    }

    int iInsertPos;
    for(iInsertPos=0; iInsertPos<MAX_GUIDE; ++iInsertPos)
    {
        if( -1 == m_NowGuideID[iInsertPos] )
            break;    
    }

    // 가이드 메시지가 꽉참. 참지말고 밀어내자!!( 큐처리 )
    if( iInsertPos >= MAX_GUIDE )
    {
        for(int iLoop=0; iLoop<(MAX_GUIDE-1); ++iLoop)
        {
            // 가이드 아이디 확인
            if( 0 > m_NowGuideID[iLoop+1] || m_NowGuideID[iLoop+1] > SDotaGameGuide::MAX_DOTA_GUIDE )
                return;

            // 인덱스 밀어내기
            m_NowGuideID[iLoop] = m_NowGuideID[iLoop+1];

            // 슬롯 갱신
            SetGuideSlot(m_NowGuideID[iLoop], iLoop);
        }

        iInsertPos = (MAX_GUIDE-1);
    }

    // 새로운 메시지 끼워넣기
    SetGuideSlot( iGuideID, iInsertPos );

    // Show애니메이션 설정
    m_bShow = true;
    ++m_iNumAddGuidex;
}

bool KGCDotaGuide::CheckDuplicationGuide( int iGuideID )
{
    bool bResult = false;
    for(int iLoop=0; iLoop<MAX_GUIDE; ++iLoop)
    {
        if( m_NowGuideID[iLoop] == iGuideID )
            bResult = true;
    }
    return bResult;
}

void KGCDotaGuide::SetGuideSlot( int iGuideID, int iSlotID )
{
    // 가이드 아이디 갱신
    m_NowGuideID[iSlotID] = iGuideID;

    // 백그라운드 색상얻기
    int iBackColor = GetBackColor( iGuideID );

    // 백그라운드 셋팅
    m_pkWndBack[GUIDE_BACK_RED][iSlotID]->ToggleRender( ( GUIDE_BACK_RED == iBackColor ) );
    m_pkWndBack[GUIDE_BACK_BLUE][iSlotID]->ToggleRender( ( GUIDE_BACK_BLUE == iBackColor ) );

    // Static 셋팅
    if( m_pCurDotaInfo )
    {
        int iStaticID = m_pCurDotaInfo->sDotaGameGuide.mapDotaGameGuide[ m_NowGuideID[iSlotID] ];
        m_pkStatic[iSlotID]->SetTextAutoMultiline( g_pkStrLoader->GetString(iStaticID) );
        m_pkStatic[iSlotID]->ToggleRender( true );
    }

    // 아이콘 셋팅
    m_pkWndImg[iSlotID]->SetItemTexture( m_pkTexImg[GetGuideIconIndex(iGuideID)] );
    m_pkWndImg[iSlotID]->ToggleRender( true );
}

int KGCDotaGuide::GetGuideIconIndex( int iGuideID )
{
    int iIndex = IMG_NORMAL;
    int iPlayerIdx = g_MyD3D->Get_MyPlayer();
    if( false == g_MyD3D->IsPlayerIndex(iPlayerIdx) )
        return iIndex;

    switch( iGuideID )
    {
    // 하르페 아이콘
    case SDotaGameGuide::DOTA_GUIDE_STARTGAME:
    case SDotaGameGuide::DOTA_GUIDE_MINIMAP:
    case SDotaGameGuide::DOTA_GUIDE_APOINT:
    case SDotaGameGuide::DOTA_GUIDE_SHOP_CLOSE:
    case SDotaGameGuide::DOTA_GUIDE_FIRST_KILL:
        iIndex = IMG_NORMAL;
        break;
    // 상점 아이콘
    case SDotaGameGuide::DOTA_GUIDE_FIRST_DEATH:
        if( TEAM_SERDIN == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
            iIndex = IMG_SHOP_RED;
        else
            iIndex = IMG_SHOP_BLUE;
        break;
    // 제어장치 아이콘
    case SDotaGameGuide::DOTA_GUIDE_ATTACK_CORE:
        iIndex = IMG_CORE;
        break;
    // 가디언타워 아이콘
    case SDotaGameGuide::DOTA_GUIDE_BREAK_GUARDTOWER:
        if( TEAM_SERDIN == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
            iIndex = IMG_GUARDIANTOWER_BLUE;
        else
            iIndex = IMG_GUARDIANTOWER_RED;
        break;
    // 가드타워 아이콘
    case SDotaGameGuide::DOTA_GUIDE_IN_ZONE:
        if( TEAM_SERDIN == g_MyD3D->MyPlayer[iPlayerIdx]->m_kUserInfo.iTeam )
            iIndex = IMG_GUARDTOWER_BLUE;
        else
            iIndex = IMG_GUARDTOWER_RED;
        break;
    // 제어장치 아이콘
    case SDotaGameGuide::DOTA_GUIDE_BREAK_GUARDIANTOWER:
        iIndex = IMG_CORE;
    }
    return iIndex;
}

int KGCDotaGuide::GetBackColor( int iGuideID )
{
    int iColor = GUIDE_BACK_BLUE;
    switch( iGuideID )
    {
    case SDotaGameGuide::DOTA_GUIDE_STARTGAME:
    case SDotaGameGuide::DOTA_GUIDE_MINIMAP:
    case SDotaGameGuide::DOTA_GUIDE_FIRST_DEATH:
    case SDotaGameGuide::DOTA_GUIDE_APOINT:
    case SDotaGameGuide::DOTA_GUIDE_SHOP_CLOSE:
        iColor = GUIDE_BACK_BLUE;
        break;
    case SDotaGameGuide::DOTA_GUIDE_FIRST_KILL:
    case SDotaGameGuide::DOTA_GUIDE_ATTACK_CORE:
    case SDotaGameGuide::DOTA_GUIDE_BREAK_GUARDTOWER:
    case SDotaGameGuide::DOTA_GUIDE_IN_ZONE:
    case SDotaGameGuide::DOTA_GUIDE_BREAK_GUARDIANTOWER:
        iColor = GUIDE_BACK_RED;
        break;
    }
    return iColor;
}

void KGCDotaGuide::ActionPerformed( const KActionEvent& event ){}
void KGCDotaGuide::FrameMoveInEnabledState( void ){}