/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2013년 06월 30일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 게임 내 메시지박스( DotaMessage개편 )

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "StdAfx.h"
#include "KGCInGameMessage.h"


IMPLEMENT_CLASSNAME( KGCInGameMessage );
IMPLEMENT_WND_FACTORY( KGCInGameMessage );
IMPLEMENT_WND_FACTORY_NAME( KGCInGameMessage, "gc_ingame_message" );

KGCInGameMessage::KGCInGameMessage()
: m_iAlpha( 0 )
, m_iStringID( 0 )
, m_iShowFrame( 0 )
, m_iInShowFrame( 0 )
, m_iOutShowFrame( 0 )
, m_fAlphaChanger( 0.0f )
, m_iSelectImg( -1 )
, m_bIsAlphaAnim( false )
, m_pkSkillWnd( NULL )
{
    m_pkBack = NULL;
    m_pkBack_ItemList = NULL;
    m_pkBack_Skill = NULL;
    m_pkStaticDesc = NULL;

    LINK_CONTROL( "back", m_pkBack );
    LINK_CONTROL( "back_itemlist", m_pkBack_ItemList );
    LINK_CONTROL( "back_skill", m_pkBack_Skill );
    LINK_CONTROL( "static_desc", m_pkStaticDesc );

    char szTemp[128];
    for(int iLoop=0; iLoop<MAX_INGAME_MESSAGE_IMG; ++iLoop)
    {
        m_apkImg[iLoop] = NULL;
        sprintf(szTemp, "img%d", iLoop);
        LINK_CONTROL( szTemp, m_apkImg[iLoop] );
    }

    m_vecItemWnd.clear();
}

KGCInGameMessage::~KGCInGameMessage()
{
    //DestroyItemBox();
    //DestroySkill();
}

void KGCInGameMessage::OnCreateComplete( void )
{
    m_pkBack->InitState( false );
    m_pkBack_ItemList->InitState( false );
    m_pkBack_Skill->InitState( false );

    m_pkStaticDesc->SetAlign( DT_LEFT );
    m_pkStaticDesc->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize());
    m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(255, 255, 249, 217) );
    m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
    m_pkStaticDesc->SetMultiLine( true );
#if defined ( NATION_USA )
    m_pkStaticDesc->SetFontSpace( 0 );
#else
    m_pkStaticDesc->SetFontSpace( 1.5f );
#endif
    m_pkStaticDesc->InitState( false );

    for(int iLoop=0; iLoop<MAX_INGAME_MESSAGE_IMG; ++iLoop)
    {
        if( m_apkImg[iLoop] )
            m_apkImg[iLoop]->InitState( false );
    }

    m_iSelectImg = -1;
}
void KGCInGameMessage::ActionPerformed( const KActionEvent& event ){}
void KGCInGameMessage::FrameMoveInEnabledState( void )
{
    if( 0 < m_iShowFrame )
    {
        if( m_bIsAlphaAnim )
        {
            if( m_iInShowFrame < m_iShowFrame )
                m_iAlpha += int(m_fAlphaChanger+0.5f);
            else if( m_iOutShowFrame > m_iShowFrame )
                m_iAlpha -= int(m_fAlphaChanger+0.5f);

            if( 0 > m_iAlpha ) m_iAlpha = 0;
            else if( 255 < m_iAlpha ) m_iAlpha = 255;

            m_pkBack->SetAllWindowAlpha( m_iAlpha );

            for(int iLoop=0; iLoop<MAX_INGAME_MESSAGE_IMG; ++iLoop)
            {
                if( m_apkImg[iLoop]->IsRenderOn() )
                {
                    m_apkImg[iLoop]->SetAllWindowAlpha( m_iAlpha );
                }
            }

            m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(m_iAlpha, 255, 249, 217) );
            m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0) );
        }

        --m_iShowFrame;
    }
    else
    {
        m_pkBack->ToggleRender( false );
        m_pkBack_ItemList->ToggleRender( false );
        m_pkBack_Skill->ToggleRender( false );
        for(int iLoop=0; iLoop<MAX_INGAME_MESSAGE_IMG; ++iLoop)
        {
            if( m_apkImg[iLoop]->IsRenderOn() )
            {
                m_apkImg[iLoop]->ToggleRender( false );
            }
        }
        m_pkStaticDesc->ToggleRender( false );
        DestroyItemBox();
        DestroySkill();
        this->ToggleRender( false );
    }
}

void KGCInGameMessage::SetMessage( KInGameMessageAttribute *pAttribute )
{
    if( NULL == pAttribute )
        return;
    
    // 애니메이션 설정
    SetAlphaAnim( pAttribute );

    // 마크 이미지 출력여부 설정
    SetImg( pAttribute );

    // 윈도우들 크기 및 위치 정리
    SetWndArrangement( pAttribute );

    // 아이템 설정
    SetItem( pAttribute );

    // 스킬아이콘 설정
    SetSkill( pAttribute );

    this->ToggleRender( true );
}

void KGCInGameMessage::SetAlphaAnim( KInGameMessageAttribute *pAttribute )
{
    m_iAlpha = 0;
    m_iShowFrame = pAttribute->iShowFrame;
    m_bIsAlphaAnim = pAttribute->bIsAlphaAnim;
    if( 0 < m_iShowFrame && m_bIsAlphaAnim )
    {
        // 메시지출력할때 알파 애니메이션
        // (iShowFrame/3.0f)는 InShow(1/5), View(3/5), OutShow(1/5) 타이밍임.
        m_fAlphaChanger = float(255.0f / (m_iShowFrame/5.0f));
        m_iInShowFrame = int(m_iShowFrame - (m_iShowFrame/5.0f));
        m_iOutShowFrame = int(m_iShowFrame/3.0f);
    }
    else
    {
        m_fAlphaChanger = 0.0f;
        m_iInShowFrame = 0;
        m_iOutShowFrame = 0;
        m_iAlpha = 255;
    }
}

void KGCInGameMessage::SetDesc( KInGameMessageAttribute *pAttribute )
{
    m_iStringID = pAttribute->iStringID;
    m_pkStaticDesc->SetAlign( pAttribute->iFontAlign );
    m_pkStaticDesc->SetFontSize( pAttribute->iFontSize );
    m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(m_iAlpha, 255, 249, 217) );

    if( pAttribute->bIsOutline )
        m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0) );
    else
        m_pkStaticDesc->SetFontOutline( false, D3DCOLOR_ARGB(0, 0, 0, 0) );

    if( pAttribute->strDirectString )
        m_pkStaticDesc->SetTextAutoMultiline( pAttribute->strDirectString );
    else
    {
        KGCStringLoader* pStringLoader = g_pkStrLoader;

        // 로더 선택
        switch( pAttribute->iSelectStrLoader )
        {
        case STR_LOADER_SUBJECT:
            pStringLoader = SiKGCSubjectManager()->GetStringLoader();
            break;
        }

        // @ 처리
        if( INT_MAX != pAttribute->iValue1 )
            m_pkStaticDesc->SetTextAutoMultiline( pStringLoader->GetReplacedString(m_iStringID, "i", pAttribute->iValue1) );
        else if( pAttribute->strValue1 && pAttribute->strValue2 )
            m_pkStaticDesc->SetTextAutoMultiline( pStringLoader->GetReplacedString(m_iStringID, "ss", pAttribute->strValue1, pAttribute->strValue2) );
        else if( pAttribute->strValue1 )
            m_pkStaticDesc->SetTextAutoMultiline( pStringLoader->GetReplacedString(m_iStringID, "s", pAttribute->strValue1) );
        else
            m_pkStaticDesc->SetTextAutoMultiline( pStringLoader->GetString(m_iStringID) );
    }
}

void KGCInGameMessage::SetImg( KInGameMessageAttribute *pAttribute )
{
    m_iSelectImg = pAttribute->iSelectImg;
    for(int iLoop=0; iLoop<MAX_INGAME_MESSAGE_IMG; ++iLoop)
    {
        m_apkImg[iLoop]->ToggleRender( false );
    }
    if( -1 < m_iSelectImg && m_iSelectImg < MAX_INGAME_MESSAGE_IMG )
    {
        m_apkImg[m_iSelectImg]->ToggleRender( true );
        m_apkImg[m_iSelectImg]->SetAllWindowAlpha( m_iAlpha );
    }
}

void KGCInGameMessage::SetWndArrangement( KInGameMessageAttribute *pAttribute )
{
    // ㅅㅂ 졸라 짜증난다. 아..... 
    // 윈도우들 해상도에 따른 위지정리하는 지저분한 코드들 여기에 넣습니다.

    // 백그라운드 넓이지정
    int iBackWidth = static_cast<int>(pAttribute->fBackWidth);
    m_pkBack->OnSetWidth( iBackWidth );

    // 화면 가운대로 백그라운드 이동
    float fScreenHalfWidth = GC_SCREEN_WIDTH / 2.0f;
    D3DXVECTOR2 vBackPos = m_pkBack->GetFixedWindowLocalPos();
    vBackPos.x = fScreenHalfWidth-(iBackWidth/2.0f);
    vBackPos.y = pAttribute->fShowPosY;
    m_pkBack->SetWindowPosDirect( vBackPos );

    // 마크 이미지가 있고, 없고에 따라 
    // Static박스의 가로 위치와 크기를 지정한다.
    D3DXVECTOR2 vDescPos = m_pkStaticDesc->GetFixedWindowLocalPos();
    if( -1 < m_iSelectImg && m_iSelectImg < MAX_INGAME_MESSAGE_IMG )
    {
        float fGapPosOfBack = 80.0f;
        D3DXVECTOR2 vImgPos = m_apkImg[m_iSelectImg]->GetFixedWindowLocalPos();
        m_apkImg[m_iSelectImg]->SetWindowPosDirect( D3DXVECTOR2(vBackPos.x + fGapPosOfBack, vImgPos.y)  );
        
        float fImgWidth = static_cast<float>(m_apkImg[m_iSelectImg]->GetWidth());
        float fDescWidth = m_pkBack->GetWidth()-(fGapPosOfBack+fImgWidth);
        vDescPos.x = (vBackPos.x + fGapPosOfBack + fImgWidth);
        m_pkStaticDesc->SetWidthDirect( static_cast<int>(fDescWidth) );
        m_pkStaticDesc->SetWindowPosDirect( vDescPos );
    }
    else
    {
        vDescPos.x = vBackPos.x + 10;
        m_pkStaticDesc->SetWidthDirect( static_cast<int>(m_pkBack->GetWidth()) );
        m_pkStaticDesc->SetWindowPosDirect( vDescPos );
    }

    // Static박스에 String셋팅
    SetDesc( pAttribute );

    // Static높이에 따라 Back높이 변경( 기본 높이는 유지 : (ptDescSize.y + 20) )
    POINT ptDescSize = m_pkStaticDesc->GetStringWidthAndHeightInPixel( false, pAttribute->iFontSize );
    if( pAttribute->fBackHeight < (ptDescSize.y + 20) )
        m_pkBack->OnSetHeight( (ptDescSize.y + 35) );
    else
        m_pkBack->OnSetHeight( static_cast<int>(pAttribute->fBackHeight) );

    // 글자 중앙에 띄우고 싶다
    vDescPos.y = vBackPos.y + ((m_pkBack->GetHeight() - ptDescSize.y) / 2.0f);
    m_pkStaticDesc->SetFixedWindowLocalPos( vDescPos );

    m_pkBack->ToggleRender( true );
    m_pkStaticDesc->ToggleRender( true );
    m_pkBack->SetAllWindowAlpha( m_iAlpha );
    this->SetAllWindowAlpha( m_iAlpha );
}

void KGCInGameMessage::SetItem( KInGameMessageAttribute *pAttribute )
{
    DestroyItemBox();
    if( false == pAttribute->m_vecItemList.empty() )
    {
        // fGapSize : 아이템 박스 사이간격
        float fGapSize = 17.0f;
        // iNumItem : 아이템 갯수
        int iNumItem = static_cast<int>(pAttribute->m_vecItemList.size());
        // fAllGapWidth : 전체 아이템 박스 사이간격
        float fAllGapWidth = (iNumItem-1) * fGapSize;
        // fScreenHalfWidth : 게임화면 절반크기
        float fScreenHalfWidth = GC_SCREEN_WIDTH/2.0f;
        // iItemBoxWidth : 박스 하나 넓이
        int iItemBoxWidth = 66;
        // iAllItemBoxWidth : 전체 아이템 박스 크기
        int iAllItemBoxWidth = iNumItem * iItemBoxWidth;

        // 백그라운드 크기와 위치 잡기
        float fSideGap = fGapSize * 2.0f;
        D3DXVECTOR2 vPos = m_pkBack->GetFixedWindowLocalPos();
        vPos.x = fScreenHalfWidth - ((iAllItemBoxWidth + fAllGapWidth)/2) - fSideGap;
        vPos.y += m_pkBack->GetHeight() + 5.0f;
        m_pkBack_ItemList->OnSetWidth( static_cast<int>(iAllItemBoxWidth + fAllGapWidth + (fSideGap*2.0f) + 1) );
        m_pkBack_ItemList->SetWindowPosDirect( vPos );
        m_pkBack_ItemList->ToggleRender( true );
        vPos.y += 10.0f;
        
        // 아이템 박스 생성 및 출력위치 잡기
        std::vector<KGCInGameMessageItem>::iterator vecItemIter = pAttribute->m_vecItemList.begin();
        for(int iLoop=0; vecItemIter != pAttribute->m_vecItemList.end(); ++iLoop, ++vecItemIter)
        {
            GCItem* pGCItem = g_pItemMgr->GetItemData( (vecItemIter->dwItemID/10) );
            KGCInGameMsgItemWnd* pWnd = (KGCInGameMsgItemWnd*)g_pkUIMgr->CreateTemplateUI( this, "gc_ingamemsg_item_wnd", "InGameItembox.stg", "box_item_ingamemsg", true, true );

            iItemBoxWidth = pWnd->GetWidth();
            float fAllItemHalfWidth = (iItemBoxWidth/2.0f) * iNumItem;
            float fStartPosX = fScreenHalfWidth - (fAllItemHalfWidth + (fAllGapWidth/2));
            vPos.x = fStartPosX + (iLoop*iItemBoxWidth) + (iLoop*fGapSize);

            pWnd->SetFixedWindowLocalPos( D3DXVECTOR2(vPos.x, vPos.y) );
            pWnd->SetItem( pGCItem, vecItemIter->iGrade, vecItemIter->iPeriod, vecItemIter->iCount );
            pWnd->ToggleRender( true );
            m_vecItemWnd.push_back( pWnd );
        }
    }
    else
        m_pkBack_ItemList->ToggleRender( false );
}

void KGCInGameMessage::SetSkill( KInGameMessageAttribute *pAttribute )
{
    DestroySkill();
    m_pkBack_Skill->ToggleRender( false );

    if( NULL == pAttribute )
        return;

    if( -1 == pAttribute->iSkillID )
        return;

    const GCSkill* pSkill = SiGCSKT()->GetSkill( static_cast<EGCSkillTree>(pAttribute->iSkillID) );
    if( pSkill == NULL )
        return;

    
    m_pkSkillWnd = (KGCInGameMsgItemWnd*)g_pkUIMgr->CreateTemplateUI( this, "gc_ingamemsg_item_wnd", "InGameItembox.stg", "box_item_ingamemsg", true, true );
    m_pkSkillWnd->SetETCImg( pSkill->GetTextureFileName() );

    int iBoxWidth = 66;
    float fGapSize = 32.0f;
    float fScreenHalfWidth = (GC_SCREEN_WIDTH/2.0f);
    float fSideGap = fGapSize * 2.0f;

    D3DXVECTOR2 vPos = m_pkBack->GetFixedWindowLocalPos();
    vPos.x = fScreenHalfWidth - ((iBoxWidth + fSideGap) / 2.0f);
    vPos.y += m_pkBack->GetHeight() + 5.0f;
    m_pkBack_Skill->OnSetWidth( static_cast<int>(iBoxWidth + fSideGap) );
    m_pkBack_Skill->SetWindowPosDirect( vPos );
    m_pkSkillWnd->SetFixedWindowLocalPos( D3DXVECTOR2((vPos.x + fGapSize), (vPos.y + 15.0f)) );
    m_pkSkillWnd->ToggleRender( true );
    m_pkBack_Skill->ToggleRender( true );
}

void KGCInGameMessage::DestroyItemBox()
{
    if( false == m_vecItemWnd.empty() )
    {
        std::vector< KGCInGameMsgItemWnd* >::iterator vecItemIter = m_vecItemWnd.begin();
        for(; vecItemIter != m_vecItemWnd.end(); ++vecItemIter)
        {
            (*vecItemIter)->ToggleRender( false );
            (*vecItemIter)->Destroy();
        }
        m_vecItemWnd.clear();
    }
}

void KGCInGameMessage::DestroySkill()
{
    if( m_pkSkillWnd )
    {
        m_pkSkillWnd->ToggleRender( false );
        m_pkSkillWnd->Destroy();
    }
    m_pkSkillWnd = NULL;
}

bool KGCInGameMessage::IsStringID( int iStringID )
{
    return (m_iStringID == iStringID);
}

void KGCInGameMessage::SetStringID( int iStringID )
{
    m_iStringID = iStringID;
}

int KGCInGameMessage::GetStringID()
{
    return m_iStringID;
}

int KGCInGameMessage::GetShowFrmae()
{
    return m_iShowFrame;
}

void KGCInGameMessage::SetShowFrmae( int iFrame )
{
    m_iShowFrame = iFrame;
}

bool KGCInGameMessage::IsRender()
{
    return this->IsRenderOn();
}

int KGCInGameMessage::IsRenderToString( int iStringID )
{
    if( iStringID == m_iStringID )
        return this->IsRenderOn();
    return -1;
}

void KGCInGameMessage::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)    
    lua_tinker::class_add<KInGameMessageAttribute>( L, "KInGameMessageAttribute" );
    lua_tinker::class_con<KInGameMessageAttribute>( L, lua_tinker::constructor<>() );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iStringID",               &KInGameMessageAttribute::iStringID  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iFontAlign",              &KInGameMessageAttribute::iFontAlign  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iFontSize",               &KInGameMessageAttribute::iFontSize  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iSelectImg",              &KInGameMessageAttribute::iSelectImg  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iSelectStrLoader",        &KInGameMessageAttribute::iSelectStrLoader  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iSkillID",                &KInGameMessageAttribute::iSkillID  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "bIsAlphaAnim",            &KInGameMessageAttribute::bIsAlphaAnim  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "bIsOutline",              &KInGameMessageAttribute::bIsOutline  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iShowFrame",              &KInGameMessageAttribute::iShowFrame  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "iValue",                  &KInGameMessageAttribute::iValue1  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "fBackWidth",              &KInGameMessageAttribute::fBackWidth  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "fBackHeight",             &KInGameMessageAttribute::fBackHeight  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "fShowPosY",               &KInGameMessageAttribute::fShowPosY  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "strValue1",               &KInGameMessageAttribute::strValue1  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "strValue2",               &KInGameMessageAttribute::strValue2  );
    lua_tinker::class_mem<KInGameMessageAttribute>( L, "strDirectString",         &KInGameMessageAttribute::strDirectString  );
    
    lua_tinker::class_def<KInGameMessageAttribute>( L,  "Init",                   &KInGameMessageAttribute::Init  );
    lua_tinker::class_def<KInGameMessageAttribute>( L,  "SetStrValue1",           &KInGameMessageAttribute::SetStrValue1  );
    lua_tinker::class_def<KInGameMessageAttribute>( L,  "SetStrValue2",           &KInGameMessageAttribute::SetStrValue2  );
    lua_tinker::class_def<KInGameMessageAttribute>( L,  "SetDirectStr",           &KInGameMessageAttribute::SetDirectStr  );

    lua_tinker::class_add<KGCInGameMessage>( L, "KGCInGameMessage" );
    lua_tinker::class_def<KGCInGameMessage>( L,  "SetMessage", 	    &KGCInGameMessage::SetMessage  );
    lua_tinker::class_def<KGCInGameMessage>( L,  "SetStringID", 	&KGCInGameMessage::SetStringID  );
    lua_tinker::class_def<KGCInGameMessage>( L,  "GetStringID", 	&KGCInGameMessage::GetStringID  );
    lua_tinker::class_def<KGCInGameMessage>( L,  "SetShowFrmae", 	&KGCInGameMessage::SetShowFrmae  );
    lua_tinker::class_def<KGCInGameMessage>( L,  "GetShowFrmae", 	&KGCInGameMessage::GetShowFrmae  );
    lua_tinker::class_def<KGCInGameMessage>( L,  "IsRender", 	    &KGCInGameMessage::IsRender  );
}