#include "GCFaceOffManager.h"

ImplementSingleton( GCFaceOffManager )

GCFaceOffManager::GCFaceOffManager(void)
: m_bIsFaceOffItemList(false),
m_bUseThisScript(false)
{
    m_setCharList.clear();
    m_setItemList.clear();
    m_setEmotionItemList.clear();
    LoadFaceOffItemList();
}

GCFaceOffManager::~GCFaceOffManager(void)
{
}

//SiGCFaceOffManager();


void GCFaceOffManager::LoadFaceOffItemList()
{
    if( m_setItemList.empty() ) {
        KLuaManager luaMgr;

        if( !GCFUNC::LoadLuaScript( luaMgr, "FaceOffItemList.lua", true ) )
            return;

        // 스크립트 사용 여부
        LUA_GET_VALUE_DEF( "UseThisScript", m_bUseThisScript, false );

        // 리스트 타입
        luaMgr.GetValue( "IsFaceOffItemList", m_bIsFaceOffItemList );

        // 케릭터별 오픈 리스트
        if( SUCCEEDED( luaMgr.BeginTable( "CHAR_LIST" ) ) ){
            for( int i = 1; ; ++i ) {
                DWORD dwCharType;

                if( FAILED( luaMgr.GetValue(i, dwCharType) ) )
                    break;

                m_setCharList.insert( dwCharType );
            }
        }
        luaMgr.EndTable();

        // 아이템 리스트
        if( SUCCEEDED( luaMgr.BeginTable( "ITEM_LIST" ) ) ){
            for( int i = 1; ; ++i ) {
                GCITEMID dwItemID;

                if( FAILED( luaMgr.GetValue(i, dwItemID) ) )
                    break;

                m_setItemList.insert( dwItemID / 10 );
            }
        }
        luaMgr.EndTable();

        // 표정 아이템 리스트 (출석으로 사는 거)
        if( SUCCEEDED( luaMgr.BeginTable( "EMOTION_ITEM_LIST" ) ) ){
            for( int i = 1; ; ++i ) {
                GCITEMID dwItemID;

                if( FAILED( luaMgr.GetValue(i, dwItemID) ) )
                    break;

                m_setEmotionItemList.insert( dwItemID / 10 );
            }
        }
        luaMgr.EndTable();
    }
}

bool GCFaceOffManager::IsFaceOffItem( GCITEMID dwItemID_ )
{

    // 우선 케릭터 오픈인지 확인
    GCItem* pItem = g_pItemMgr->GetItemData( dwItemID_ );

    std::set<DWORD>::iterator itChar = m_setCharList.find( pItem->dwCharType );

    if( itChar != m_setCharList.end() ) {
        return true;
    }
    
    // 아니면 리스트에서 찾기
    std::set<GCITEMID>::iterator it = m_setItemList.find( dwItemID_ );

    if( it != m_setItemList.end() ){
        if( m_bIsFaceOffItemList )
            return true;
        else
            return false;
    } else {
        if( m_bIsFaceOffItemList )
            return false;
        else
            return true;
    }
}

bool GCFaceOffManager::IsEmotionItem( GCITEMID dwItemID_ )
{
    std::set<GCITEMID>::iterator it = m_setEmotionItemList.find( dwItemID_ );

    if( it != m_setEmotionItemList.end() )
        return true;
    else
        return false;
}

bool GCFaceOffManager::IsFaceOffChar( DWORD dwChar )
{
    std::set<DWORD>::iterator itChar = m_setCharList.find( g_pItemMgr->GetCharFlag( dwChar ) );
    if ( itChar != m_setCharList.end() )
        return true;
    else
        return false;
}

