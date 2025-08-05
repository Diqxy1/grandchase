#include "stdafx.h"
#include "GCHeroDungeonReward.h"
#include "GCHeroItemBox.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonReward );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonReward );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonReward, "gc_herodungeonreward" );



KGCHeroDungeonReward::KGCHeroDungeonReward(void)
{

    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        m_apkCharacterName[i] = NULL;   

        char szBuf[MAX_PATH];
        sprintf_s(szBuf, MAX_PATH - 1, "character_name%d", i);
        LINK_CONTROL( szBuf, m_apkCharacterName[i] );
    }

    for( int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
    {
        m_apkItemBox[i] = NULL;

        char szBuf[MAX_PATH];
        sprintf_s(szBuf, MAX_PATH - 1 , "hero_item_box%d", i);
        LINK_CONTROL( szBuf, m_apkItemBox[i] );
    }

    m_pkHeroItemPreview = NULL;
    LINK_CONTROL( "hero_item_preview", m_pkHeroItemPreview );

    m_pkBtnPrevPage = NULL;
    m_pkBtnNextPage = NULL;
    LINK_CONTROL( "btn_prev_page", m_pkBtnPrevPage );
    LINK_CONTROL( "btn_next_page", m_pkBtnNextPage );

    m_pkStaticPage = NULL;
    LINK_CONTROL( "static_page", m_pkStaticPage );
    m_pkStaticDesc = NULL;
    LINK_CONTROL( "static_desc", m_pkStaticDesc );

    m_mapItemInfo.clear();

    m_dwCurrentCharacter = -1;
    m_iCurrentCharacterName = -1;

    m_iCurrentItemBox = -1;
    m_bShowPreview = false;

    m_iCurrentPage = -1;
    m_iTotalPage = -1;
}

KGCHeroDungeonReward::~KGCHeroDungeonReward(void)
{

}

void KGCHeroDungeonReward::OnCreate( void )
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        if (m_apkCharacterName[i])
        {
            m_apkCharacterName[i]->InitState(false, false, this);

        }
    }

    for( int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
    {
        m_apkItemBox[i]->InitState(false, true, this);
        m_apkItemBox[i]->SetBuyType( false );
    }

    m_pkHeroItemPreview->InitState(false, true, this);

    m_pkBtnPrevPage->InitState(true, true, this);
    m_pkBtnNextPage->InitState(true, true, this);

    m_pkStaticPage->SetFontColor(0xffffffff);
    m_pkStaticPage->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticPage->SetAlign(DT_CENTER);
    m_pkStaticPage->InitState(true, false, this);    

    m_pkStaticDesc->SetFontColor(0xffffffff);
    m_pkStaticDesc->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticDesc->SetAlign(DT_LEFT);
    m_pkStaticDesc->InitState(true, false, this);  
    m_pkStaticDesc->SetMultiLine( true );
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_HERO_REWARD_DESC ) );
}

void KGCHeroDungeonReward::OnCreateComplete( void )
{
    // 아이템 정보 세팅
    SetHeroItemInfo();
    UpdateCharacterInfo();
}

void KGCHeroDungeonReward::UpdateCharacterInfo() 
{
    if ( m_iCurrentCharacterName >= 0 && m_iCurrentCharacterName < GC_CHAR_NUM) 
    {
        m_apkCharacterName[m_iCurrentCharacterName]->ToggleRender(false);
    }
    
    // 캐릭터 세팅
    int iCharType = g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType;
    m_dwCurrentCharacter = static_cast<DWORD>(pow(2.0f, iCharType));
    m_iCurrentCharacterName = iCharType;

    m_apkCharacterName[m_iCurrentCharacterName]->ToggleRender(true);

    int nItemNum = GetItemCount( m_dwCurrentCharacter );

    // 페이지 세팅
    ResetPage( nItemNum );

    if( m_iCurrentPage > -1 )
    {
        SetHeroItemBox(m_dwCurrentCharacter, m_mapItemInfo, m_iCurrentPage);
        UpdateRewardButton();
    }
}

void KGCHeroDungeonReward::OnDestroy( void )
{
    for(int i=0 ; i<NUM_HERO_ITEM_BOX ; ++i)
    {
        m_apkItemBox[i]->SetPreview(false);
        m_apkItemBox[i]->SetLock(false);
    }

    m_dwCurrentCharacter = -1;
    m_iCurrentCharacterName = -1;

    m_iCurrentItemBox = -1;
    m_bShowPreview = false;
    m_pkHeroItemPreview->SetClose(true);

    g_pkUIScene->m_pkToolTip->SetWindowZOrder(D3DWZ_NOTOPMOST);
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_NOTOPMOST);

    m_iCurrentPage = -1;
    m_iTotalPage = -1;
}

void KGCHeroDungeonReward::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkBtnPrevPage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnPrevPage();

    if( event.m_pWnd == m_pkBtnNextPage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnNextPage();

    for( int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
    {
        if( event.m_pWnd == m_apkItemBox[i] && m_apkItemBox[i]->IsPreview() && !m_apkItemBox[i]->IsLock() )
        {
            m_bShowPreview = true;
            m_pkHeroItemPreview->SetClose(false);
            m_iCurrentItemBox = i;
            std::vector<GCItem*> vecItem;
            vecItem.push_back(m_apkItemBox[i]->GetItem());
            m_pkHeroItemPreview->SetPreviewWindow(m_iCurrentCharacterName, vecItem);

            m_pkBtnPrevPage->SetSelfInputCheck(false);
            m_pkBtnNextPage->SetSelfInputCheck(false);

            for( int j = 0 ; j < NUM_HERO_ITEM_BOX ; ++j )
                m_apkItemBox[j]->SetLock(true);

            break;
        }
    }
}

void KGCHeroDungeonReward::OnDestroyComplete( void )
{
}

void KGCHeroDungeonReward::FrameMoveInEnabledState( void )
{
    if( g_pkInput->IsUp(DIK_ESCAPE) )
    {
        if( m_iCurrentItemBox != -1 )
            ReleaseLock();
    }  
}

void KGCHeroDungeonReward::PostChildDraw( void )
{
    // 아이템 정보 
    DrawItemInfo();

    // 미리보기 
    ShowItemPreview();
}

void KGCHeroDungeonReward::SetHeroItemInfo( void )
{
    if( !m_mapItemInfo.empty() )
        return;

    m_mapItemInfo.clear();


    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator ItemIter;

    std::map<INDEX_GCITEMID,KManufactureItem> _mapClearReward;

    g_kGlobalValue.m_kHeroDungeonInfo.GetClearReward( m_emGameMode, _mapClearReward );
   
    std::map<INDEX_GCITEMID,KManufactureItem>::iterator iterItem = _mapClearReward.begin();
    std::map<INDEX_GCITEMID,KManufactureItem>::iterator iterItem_end = _mapClearReward.end();

    for( ; iterItem != iterItem_end; ++iterItem )
    {
        KHeroItemInfo tempInfo;
        tempInfo.m_pairIdIndex = iterItem->first;
        tempInfo.m_ItemInfo = iterItem->second;

        DWORD dwCharType = g_pItemMgr->GetItemData(static_cast<DWORD>(iterItem->first.second/10))->dwCharType;
        ItemIter = m_mapItemInfo.find( dwCharType );

        if( m_mapItemInfo.end() == ItemIter )
        {
            std::vector<KHeroItemInfo> vecInfo;
            vecInfo.push_back(tempInfo);
            m_mapItemInfo.insert( std::make_pair(dwCharType, vecInfo) );
        }
        else
        {
            ItemIter->second.push_back(tempInfo);
        }
    }
}

void KGCHeroDungeonReward::SetHeroItemBox(DWORD dwCharType, std::map<DWORD, std::vector<KHeroItemInfo> >& mapItemInfo, int iCurPage)
{
    // 이이템 리스트 얻기, 캐릭터용 / 공용
    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator itemIter = m_mapItemInfo.find(dwCharType);
    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator commonIter = m_mapItemInfo.find( ECT_ALL );

    if( m_mapItemInfo.end() == itemIter && m_mapItemInfo.end() == commonIter )
        return;

    int iCharItem  = 0;
    int iCommonItem = 0;

    // 인덱스 
    int iStartItemIndex = iCurPage;
    --iStartItemIndex;
    iStartItemIndex *= NUM_HERO_ITEM_BOX;

    std::vector<KHeroItemInfo> tempVector;

    if ( itemIter != m_mapItemInfo.end() ) 
    {
        for(int i = 0 ; i < static_cast<int>(itemIter->second.size()) ; ++i)
            tempVector.push_back( itemIter->second[i]);

        iCharItem  = static_cast<int>(itemIter->second.size());
    }

    if ( commonIter != m_mapItemInfo.end() ) 
    {
        for(int i = 0 ; i < static_cast<int>(commonIter->second.size()) ; ++i)
            tempVector.push_back( commonIter->second[i]);

        iCommonItem = static_cast<int>(commonIter->second.size());
    }

    // 페이지 번호 표시 
    std::wostringstream strm;
    strm << m_iCurrentPage << L" / " << m_iTotalPage ;
    m_pkStaticPage->SetText( strm.str() );

    // 아이템 상자 세팅
    int iTotalItem = iCharItem + iCommonItem;

    for( int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
    {
        m_apkItemBox[i]->ClearItemBox();

        if( iStartItemIndex < iTotalItem  ) 
        {
            if ( (int)tempVector.size() <= iStartItemIndex ) 
                break;

            KHeroItemInfo tempInfo = tempVector[iStartItemIndex++];

            m_apkItemBox[i]->SetItemBox(tempInfo);
            m_apkItemBox[i]->SetGameMode( m_emGameMode );
            m_apkItemBox[i]->ToggleRender(true);
        }
        else
            m_apkItemBox[i]->ToggleRender(false);
    }
}

void KGCHeroDungeonReward::DrawItemInfo( void )
{
    if( m_bShowPreview )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
        return;
    }

    // 정보 표시 위한 변수들
    bool bShowItemInfo = false;
    GCItem* pItem;
    D3DXVECTOR2 dxvPos;

    // 영웅 주화, 주화 조각 정보 
    POINT pt = g_pkInput->GetMousePos();

    // 아이템 박스에 있는 정보
    if( !bShowItemInfo )
    {
        for( int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
        {
            if( m_apkItemBox[i]->IsItemHovered() )
            {
                //KLuaManager luaMgr;
                //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

                //LUA_GET_VALUE_DEF( "x", x, 0.0f );
                //LUA_GET_VALUE_DEF( "y", y, 0.0f );

                if( m_apkItemBox[i]->IsItemHovered() )  // 아이템일때 
                {
                    pItem = m_apkItemBox[i]->GetItem();
                    if( pItem == NULL || pItem->dwGoodsID == 0 )  continue;

                    dxvPos = m_apkItemBox[i]->GetRelocatedWindowPos();
                    dxvPos.x += 10.0f;
                    dxvPos.y += 50.0f;

                    bShowItemInfo = true;
                }
            }

            if( bShowItemInfo ) break;
        }
    }

    if( bShowItemInfo )
    {
        g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
        g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
        g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
        g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
    }
    else
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    }
}

void KGCHeroDungeonReward::ShowItemPreview()
{
    if( !m_bShowPreview || m_pkHeroItemPreview->GetClose() )
    {
        m_pkHeroItemPreview->ToggleRender(false);
        return;
    }
    else
    {

        GCItem* pItem = m_apkItemBox[m_iCurrentItemBox]->GetItem();
        if( pItem == NULL || pItem->dwGoodsID == 0 )  return;


        m_pkHeroItemPreview->ToggleRender(true);
    }
}

void KGCHeroDungeonReward::ReleaseLock( void )
{
    m_apkItemBox[m_iCurrentItemBox]->SetPreview(false);
    m_bShowPreview = false;
    m_iCurrentItemBox = -1;

    m_pkBtnPrevPage->SetSelfInputCheck(true);
    m_pkBtnNextPage->SetSelfInputCheck(true);


    for(int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
        m_apkItemBox[i]->SetLock(false);
}

void KGCHeroDungeonReward::OnPrevPage( void )
{
    --m_iCurrentPage;
    if( m_iCurrentPage < 1 )    m_iCurrentPage = 1;
    SetHeroItemBox(m_dwCurrentCharacter, m_mapItemInfo, m_iCurrentPage);
}

void KGCHeroDungeonReward::OnNextPage( void )
{
    ++m_iCurrentPage;
    if( m_iCurrentPage > m_iTotalPage )    m_iCurrentPage = m_iTotalPage;
    SetHeroItemBox(m_dwCurrentCharacter, m_mapItemInfo, m_iCurrentPage);
}

void KGCHeroDungeonReward::ResetPage(int iVecItemInfoSize)
{
    m_iTotalPage = iVecItemInfoSize / NUM_HERO_ITEM_BOX;

    if( 0 < m_iTotalPage && 0 != iVecItemInfoSize % NUM_HERO_ITEM_BOX )
        ++m_iTotalPage;
    else if( 0 == m_iTotalPage && 0 != iVecItemInfoSize % NUM_HERO_ITEM_BOX )
        m_iTotalPage = 1;

    m_iCurrentPage = 1;
}

void KGCHeroDungeonReward::UpdateRewardButton()
{
    int nSelect = SiKGCWorldMapManager()->GetDungeonRewardNum( m_emGameMode, g_kGlobalValue.m_kUserInfo.mapStagePlayable );
    bool bOpen = nSelect > 0 ? true : false;

    for( int i = 0 ; i < NUM_HERO_ITEM_BOX ; ++i )
    {
        m_apkItemBox[i]->RenderSelectBtn( bOpen );
    }
}

int KGCHeroDungeonReward::GetItemCount( DWORD _dwChar )
{
    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator mit = m_mapItemInfo.find( _dwChar );
    std::map<DWORD, std::vector<KHeroItemInfo> >::iterator commonMit = m_mapItemInfo.find( ECT_ALL );

    int nItemNum = 0;

    if ( mit != m_mapItemInfo.end() ) 
    {
        nItemNum += mit->second.size();
    }

    if ( commonMit != m_mapItemInfo.end() ) 
    {
        nItemNum += commonMit->second.size();
    }

    return nItemNum;
}