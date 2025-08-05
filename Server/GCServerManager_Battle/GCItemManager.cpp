#include "StdAfx.h"
#include "GCItemManager.h"
#include <string>

KGCItemManager::KGCItemManager()
{
    m_dwNumItem = 0;
    m_pItemData = NULL;

    DWORD   dwRead;
    HANDLE  hFile;

    std::wstring strItemDataFileName = L"Sort.stg";

    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFile( strItemDataFileName.c_str(),
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL ) ) )
    {
        return;
    }

    if ( FALSE == ::ReadFile( hFile, &m_dwNumItem, sizeof( DWORD ), &dwRead, NULL ) )
    {
        ::CloseHandle( hFile );
        return;
    }

    ++m_dwNumItem; // 선두의 숫자는 최대 인덱스이므로 하나 늘려줘야 총 개수가 된다.

    m_pItemData = new GCItem[m_dwNumItem];

    if ( NULL == m_pItemData )
    {
        ::CloseHandle( hFile );
        return;
    }

    if ( FALSE == ::ReadFile( hFile, m_pItemData, m_dwNumItem * sizeof( GCItem ), &dwRead, NULL ) )
    {
        ::CloseHandle( hFile );
        return;
    }

    if ( FALSE == ::CloseHandle( hFile ) )
    {
        return;
    }
}

KGCItemManager::~KGCItemManager()
{
    m_dwNumItem = 0;
    SAFE_DELETE_ARRAY( m_pItemData );
}

void KGCItemManager::GetItemInfo( std::vector<KAdminUseItemInfo>& vecItemInv )
{
    KAdminUseItemInfo kauii;
    for(DWORD i = 0; i < m_dwNumItem; i++)
    {
        kauii.m_dwGoodsID      = i * 10;
        kauii.m_strGoodsName   = m_pItemData[i].szName;
        kauii.m_bCash          = m_pItemData[i].bCash;
        kauii.m_iStrongLevel   = m_pItemData[i].iStrongLevel;
        kauii.m_iDurationNum   = m_pItemData[i].shReserved2;
        if( m_pItemData[i].dwExtra1 & 0x00000001 ) //엘리시스
            kauii.m_iCharType = 0;
        else if( m_pItemData[i].dwExtra1 & 0x00000002 ) //리르
            kauii.m_iCharType = 1;
        else if( m_pItemData[i].dwExtra1 & 0x00000004 ) //아르메
            kauii.m_iCharType = 2;
        kauii.m_bShowWeb       = ( ( m_pItemData[i].dwExtra1 | 0x00004000 ) == 0x00004000 ? true : false );
        kauii.m_iGem           = m_pItemData[i].iGem;
        kauii.m_iCrystal       = m_pItemData[i].iCrystal;

        vecItemInv.push_back( kauii );
    }
}

//char  szName[30];
//char  szDesc[100];
//bool  bCash;
//BYTE  btLevel;
//short shAttack;
//short shDefence;
//short shReserved1;  // 세트번호 (ver 1.1)
//short shReserved2;  // 수량 (ver 1.1)
//short shHpInc;
//DWORD dwPrice;
//DWORD dwPatternA;   // 패턴A 가격 (ver 1.1)
//DWORD dwPatternB;   // 패턴B 가격 (ver 1.1)
//DWORD dwExtra1;
//DWORD dwExtra2;
//DWORD dwExtra3;     // (ver 1.2)
//int   iGem;         // 젬 가격 (ver 1.2)
//int   iStrongLevel; // 강화 레벨 (ver 1.2)
//bool  bEffect;      // 이펙트 여부 (ver 1.2)
//int   iCrystal;     // 크리스탈 가격 (ver 1.3)
//bool KGCItemManager::Verify( void ) const
//{
//    return ( VerifyItemData() && VerifyItemEquip() && VerifyInventory() && VerifyShopList() );
//}

//bool KGCItemManager::VerifyItemData( void ) const
//{
//    ASSERT( m_dwNumItem != 0 );
//    ASSERT( m_aItemData != NULL );
//
//    return ( m_dwItemDataCRC == CRC.CalcCRC( reinterpret_cast<BYTE*>( m_aItemData ),
//                                             m_dwNumItem * sizeof( GCItem ) ) );
//}

//bool KGCItemManager::VerifyItemEquip( void ) const
//{
//    for ( int i = 0; i < END_CharacterNum; ++i )
//    {
//        if ( m_adwItemEquipCRC[i] != CRC.CalcCRC( &m_avecItemEquip[i].front(),
//                                                  GetEquipSize( i ) * sizeof( KInventoryItem ) ) )
//        {
//            return false;
//        }
//    }
//    return true;
//}

//bool KGCItemManager::VerifyInventory( void ) const
//{
//    for ( int i = 0; i < END_CharacterNum; ++i )
//    {
//        if ( m_adwInventoryCRC[i] != CRC.CalcCRC( &m_avecInventory[i].front(),
//                                                  GetInventorySize( i ) * sizeof( KInventoryItem ) ) )
//        {
//            return false;
//        }
//    }
//    return true;
//}

//bool KGCItemManager::VerifyShopList( void ) const
//{
//    for ( int i = 0; i < NUM_SHOP_ITEM_TYPE; ++i )
//    {
//        for ( int j = 0; j < END_CharacterNum; ++j )
//        {
//            if ( m_aadwShopListCRC[i][j] != CRC.CalcCRC( &m_aavecShopList[i][j].front(),
//                                                         GetShopListSize( i, j ) * sizeof( int ) ) )
//            {
//                return false;
//            }
//        }
//    }
//    return true;
//}

//void KGCItemManager::CalcChksumItemData( void )
//{
//    // 샵 리스트 메모리에 대한 체크섬을 계산한다.
//    m_dwItemDataCRC = CRC.CalcCRC( (BYTE*)( m_aItemData ), m_dwNumItem * sizeof( GCItem ) );
//}

//void KGCItemManager::CalcChksumItemEquip( void )
//{
//    // 캐릭터별 장비 리스트 메모리에 대한 체크섬을 계산한다.
//    for ( int i = 0; i < END_CharacterNum; ++i )
//    {
//        m_adwItemEquipCRC[i] = CRC.CalcCRC( &m_avecItemEquip[i].front(),
//                                            GetEquipSize( i ) * sizeof( KInventoryItem ) );
//    }
//}

//void KGCItemManager::CalcChksumInventory( void )
//{
//    // 총 인벤토리 메모리에 대한 체크섬을 계산한다.
//    m_dwTotalInventoryCRC = CRC.CalcCRC( &m_vecTotalInventory.front(),
//                                         m_vecTotalInventory.size() * sizeof( KInventoryItem ) );
//
//    // 캐릭터별로 나누고 정렬한다.
//    GroupInventory();
//    SortInventory();
//
//    // 캐릭터별 인벤토리 메모리에 대한 체크섬을 계산한다.
//    for ( int i = 0; i < END_CharacterNum; ++i )
//    {
//        m_adwInventoryCRC[i] = CRC.CalcCRC( &m_avecInventory[i].front(),
//                                            GetInventorySize( i ) * sizeof( KInventoryItem ) );
//    }
//}

//void KGCItemManager::CalcChksumShopList( void )
//{
//    // 종류와 캐릭터별로 나누고 정렬한다.
//    GroupShopItemList();
//    SortShopItemList();
//
//    // 종류 및 캐릭터별로 샵 리스트 메모리에 대한 체크섬을 계산한다.
//    for ( int i = 0; i < NUM_SHOP_ITEM_TYPE; ++i )
//    {
//        for ( int j = 0; j < END_CharacterNum; ++j )
//        {
//            m_aadwShopListCRC[i][j] = CRC.CalcCRC( &m_aavecShopList[i][j].front(),
//                                                   GetShopListSize( i, j ) * sizeof( int ) );
//        }
//    }
//}

//bool KGCItemManager::LoadItemDataFile( void )
//{
//    m_dwNumItem = 0;
//    m_aItemData = NULL;
//    m_dwItemDataCRC = 0;
//
//    DWORD   dwRead;
//    HANDLE  hFile;
//
//#ifdef _JAPANESE            // 일본어       (일본)
//    std::string strItemDataFileName = g_strCurrentPath + "Stage\\Sort_j.stg";
//#elif _CHINESE_SIMPLIFIED   // 중국어 간체  (중국)
//    std::string strItemDataFileName = g_strCurrentPath + "Stage\\Sort_cs.stg";
//#elif _CHINESE_TRADITIONAL  // 중국어 번체  (대만)
//    std::string strItemDataFileName = g_strCurrentPath + "Stage\\Sort_ct.stg";
//#else // _KOREAN            // 한국어       (한국)
//    std::string strItemDataFileName = "Sort.stg";
//#endif
//
//    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFile( strItemDataFileName.c_str(),
//                                                         GENERIC_READ,
//                                                         0,
//                                                         NULL,
//                                                         OPEN_EXISTING,
//                                                         FILE_ATTRIBUTE_NORMAL,
//                                                         NULL ) ) )
//    {
//        return false;
//    }
//
//    if ( FALSE == ::ReadFile( hFile, &m_dwNumItem, sizeof( DWORD ), &dwRead, NULL ) )
//    {
//        return false;
//    }
//
//    ++m_dwNumItem; // 선두의 숫자는 최대 인덱스이므로 하나 늘려줘야 총 개수가 된다.
//
//    m_aItemData = new GCItem[m_dwNumItem];
//
//    if ( NULL == m_aItemData )
//    {
//        return false;
//    }
//
//    if ( FALSE == ::ReadFile( hFile, m_aItemData, m_dwNumItem * sizeof( GCItem ), &dwRead, NULL ) )
//    {
//        return false;
//    }
//
//    if ( FALSE == ::CloseHandle( hFile ) )
//    {
//        return false;
//    }
//
//    //CalcChksumItemData();
//    //CalcChksumShopList();
//
//    return true;
//}
//
//void KGCItemManager::UnloadItemDataFile( void )
//{
//    SAFE_DELETE_ARRAY( m_aItemData );
//    m_dwNumItem = 0;
//    m_dwItemDataCRC = 0;
//}
//
////void KGCItemManager::GroupShopItemList( void )
////{
////    ASSERT( m_dwNumItem != 0 );
////    ASSERT( m_aItemData != NULL );
////
////    // 샵 리스트 클리어
////    for ( int i = 0; i < NUM_SHOP_ITEM_TYPE; ++i )
////    {
////        for ( int j = 0; j < END_CharacterNum; ++j )
////        {
////            m_aavecShopList[i][j].clear();
////        }
////    }
////
////    // 샵 리스트를 종류별로 분류한다.
////    for ( i = 0; i < NUM_SHOP_ITEM_TYPE; ++i )
////    {
////        DWORD dwCategory = 0;
////
////        switch ( i )
////        {
////            case SIT_ARMOR:
////            {
////                dwCategory = ITEM_HEAD                  // 머리
////                           | ITEM_BODY1                 // 상의
////                           | ITEM_BODY2                 // 하의
////                           | ITEM_HAND                  // 장갑
////                           | ITEM_SHOOE                 // 신발
////                           | ITEM_BACKCLOTH_ACC;        // 망또
////                break;
////            }
////            case SIT_SET:
////            {
////                dwCategory = ITEM_SET;                  // 세트
////                break;
////            }
////            case SIT_WEAPON:
////            {
////                dwCategory = ITEM_WEAP;                 // 무기
////                break;
////            }
////            case SIT_ACCESSORY:
////            {
////                dwCategory = ITEM_HAND_ACC              // 손 악세서리
////                           | ITEM_BODY1_ACC             // 상의 악세서리
////                           | ITEM_BODY2_ACC             // 하의 악세서리
////                           | ITEM_FACEUP_ACC            // 눈 위 악세서리
////                           | ITEM_FACEDOWN_ACC          // 눈 아래 악세서리
////                           | ITEM_ARM_ACC               // 팔 악세서리
////                           | ITEM_SHOOE_ACC             // 발 악세서리
////                           | ITEM_NEWLAYER_ACC;         // 특별 악세서리
////                break;
////            }
////            case SIT_QUEST:
////            {
////                dwCategory = ITEM_SCROLL_FOR_JOB_LV2    // 전직 증명서
////                           | ITEM_UNABLE_EQUIP;         // 장착 불능 아이템
////                break;
////            }
////        }
////
////        for ( int j = 0; j < (int)m_dwNumItem; ++j )
////        {
////            if ( !( m_aItemData[j].dwExtra1 & ITEM_NOT_SALE ) )
////            {
////                if ( m_aItemData[j].dwExtra1 & dwCategory )
////                {
////                    if ( SIT_ARMOR == i && ( m_aItemData[j].dwExtra1 & ITEM_SET ) ) continue;
////
////                    if ( m_aItemData[j].dwExtra1 & ITEM_CHAR0 )
////                    {
////                        m_aavecShopList[i][ICT_KNIGHT].push_back( j );
////                    }
////                    if ( m_aItemData[j].dwExtra1 & ITEM_CHAR1 )
////                    {
////                        m_aavecShopList[i][ICT_ARCHER].push_back( j );
////                    }
////                    if ( m_aItemData[j].dwExtra1 & ITEM_CHAR2 )
////                    {
////                        m_aavecShopList[i][ICT_WIZARD].push_back( j );
////                    }
////                }
////            }
////        }
////    }
////}
//
////void KGCItemManager::SortShopItemList( void )
////{
////    for ( int i = 0; i < NUM_SHOP_ITEM_TYPE; ++i )
////    {
////        for ( int j = 0; j < END_CharacterNum; ++j )
////        {
////            std::sort( m_aavecShopList[i][j].begin(),
////                       m_aavecShopList[i][j].end(),
////                       KShopItemLess() );
////        }
////    }
////}
//
////void KGCItemManager::GroupInventory( void )
////{
////    ASSERT( m_dwNumItem != 0 );
////    ASSERT( m_aItemData != NULL );
////
////    // 캐릭터별 인벤토리 클리어
////    for ( int i = 0; i < END_CharacterNum; ++i )
////    {
////        m_avecInventory[i].clear();
////    }
////
////    // 캐릭터별 인벤토리로 분류한다.
////    for ( i = 0; i < (int)m_vecTotalInventory.size(); ++i )
////    {
////        if ( m_aItemData[m_vecTotalInventory[i].m_iItemID].dwExtra1 & ITEM_CHAR0 )
////        {
////            m_avecInventory[ICT_KNIGHT].push_back( m_vecTotalInventory[i] );
////        }
////        if ( m_aItemData[m_vecTotalInventory[i].m_iItemID].dwExtra1 & ITEM_CHAR1 )
////        {
////            m_avecInventory[ICT_ARCHER].push_back( m_vecTotalInventory[i] );
////        }
////        if ( m_aItemData[m_vecTotalInventory[i].m_iItemID].dwExtra1 & ITEM_CHAR2 )
////        {
////            m_avecInventory[ICT_WIZARD].push_back( m_vecTotalInventory[i] );
////        }
////    }
////}
//
////void KGCItemManager::SortInventory( void )
////{
////    // 캐릭터별 인벤토리를 정렬한다.
////    for ( int i = 0; i < END_CharacterNum; ++i )
////    {
////        std::sort( m_avecInventory[i].begin(), m_avecInventory[i].end(), KInventoryItemLess() );
////    }
////}
//
//void KGCItemManager::ClearInventory( void )
//{
//    m_vecTotalInventory.clear();
//    //CalcChksumInventory();
//}
//
//void KGCItemManager::AddInventory( const std::vector<KItemInfo>& vecItemInfo )
//{
//    for ( int i = 0; i < (int)vecItemInfo.size(); ++i )
//    {
//        // 이미 인벤토리에 존재한다면 교체한다.
//        for ( int j = 0; j < (int)m_vecTotalInventory.size(); ++j )
//        {
//            if ( vecItemInfo[i].m_dwGoodsUID == m_vecTotalInventory[j].m_dwUID )
//            {
//                m_vecTotalInventory[j].m_iItemID    = vecItemInfo[i].m_dwGoodsID / 10;
//                m_vecTotalInventory[j].m_iPattern   = vecItemInfo[i].m_dwGoodsID % 10;
//                m_vecTotalInventory[j].m_iDuration  = -1;
//                break;
//            }
//        }
//
//        // 인벤토리에 없다면 새로 추가한다.
//        if ( j == m_vecTotalInventory.size() )
//        {
//            KInventoryItem kInvItem;
//            kInvItem.m_iItemID  = vecItemInfo[i].m_dwGoodsID / 10;
//            kInvItem.m_iPattern = vecItemInfo[i].m_dwGoodsID % 10;
//            kInvItem.m_dwUID    = vecItemInfo[i].m_dwGoodsUID;
//            m_vecTotalInventory.push_back( kInvItem );
//        }
//    }
//
//    //CalcChksumInventory();
//}
//
////void KGCItemManager::AddInventory( const std::vector<KDurationItemInfo>& vecDurationItemInfo )
////{
////    std::vector<DWORD> vecDeleteItem;
////
////    vecDeleteItem.clear();
////    for ( int i = 0; i < (int)vecDurationItemInfo.size(); ++i )
////    {
////        // 이미 인벤토리에 존재한다면 교체한다.
////        for ( int j = 0; j < (int)m_vecTotalInventory.size(); ++j )
////        {
////            if ( vecDurationItemInfo[i].m_dwDurationItemUID == m_vecTotalInventory[j].m_dwUID )
////            {
////                m_vecTotalInventory[j].m_iItemID    = vecDurationItemInfo[i].m_dwGoodsID / 10;
////                m_vecTotalInventory[j].m_iPattern   = vecDurationItemInfo[i].m_dwGoodsID % 10;
////                m_vecTotalInventory[j].m_iDuration  = vecDurationItemInfo[i].m_dwDurationNum;
////                break;
////            }
////        }
////
////        // 인벤토리에 없다면 새로 추가한다.
////        if ( j == m_vecTotalInventory.size() )
////        {
////            KInventoryItem kInvItem;
////            kInvItem.m_iItemID   = vecDurationItemInfo[i].m_dwGoodsID / 10;
////            kInvItem.m_iPattern  = vecDurationItemInfo[i].m_dwGoodsID % 10;
////            kInvItem.m_iDuration = vecDurationItemInfo[i].m_dwDurationNum;
////            kInvItem.m_dwUID     = vecDurationItemInfo[i].m_dwDurationItemUID;
////            m_vecTotalInventory.push_back( kInvItem );
////        }
////
////        // 수량이 0이라면 제거할 아이템으로 등록한다.
////        if ( vecDurationItemInfo[i].m_dwDurationNum == 0 )
////        {
////            vecDeleteItem.push_back( vecDurationItemInfo[i].m_dwDurationItemUID );
////        }
////    }
////
////    // 수량이 0인 아이템을 제거한다.
////    for ( i = 0; i < (int)vecDeleteItem.size(); ++i )
////    {
////        RemoveInventory( vecDeleteItem[i] );
////    }
////
////    // NOTE : 넷마블 PC방에서는 레나스 아이템의 무한 사용이 가능하다.
////
////    // 넷마블 PC방인 경우
////    if ( g_bNetmarblePC )
////    {
////        bool bFoundRenas    = false;
////        bool bFoundJinRenas = false;
////
////        for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
////        {
////            if ( m_vecTotalInventory[i].m_iDuration > 0 )
////            {
////                // 레나스 아이템 발견
////                if ( m_vecTotalInventory[i].m_iItemID == 652 ) bFoundRenas = true;
////                // 진레나스 아이템 발견
////                if ( m_vecTotalInventory[i].m_iItemID == 653 ) bFoundJinRenas = true;
////            }
////        }
////
////        if ( !bFoundRenas && !bFoundJinRenas ) // 레나스 아이템이 없다면 넣어준다.
////        {
////            KInventoryItem kInvItem;
////            kInvItem.m_iItemID   = 652;
////            kInvItem.m_iDuration = 99;
////            m_vecTotalInventory.push_back( kInvItem );
////        }
////    }
////
////    //CalcChksumInventory();
////}
//
//int KGCItemManager::FindInventory( DWORD dwUID ) const
//{
//    // 유니크 아이디로 검색해서 찾은 아이템의 번호를 넘겨준다.
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( dwUID == m_vecTotalInventory[i].m_dwUID )
//        {
//            return m_vecTotalInventory[i].m_iItemID;
//        }
//    }
//    return -1;
//}
//
//void KGCItemManager::RemoveInventory( DWORD dwUID )
//{
//    // 유니크 아이디로 검색해서 인벤토리 안의 아이템을 제거한다.
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( dwUID == m_vecTotalInventory[i].m_dwUID )
//        {
//            m_vecTotalInventory.erase( m_vecTotalInventory.begin() + i );
//            //CalcChksumInventory();
//            break;
//        }
//    }
//}
//
//void KGCItemManager::RemoveInventory( const std::vector<KItemInfo>& vecItemInfo )
//{
//    for ( int i = 0; i < (int)vecItemInfo.size(); ++i )
//    {
//        RemoveInventory( vecItemInfo[i].m_dwGoodsUID );
//    }
//}
//
//void KGCItemManager::RemoveInventory( const std::vector<KDurationItemInfo>& vecDurationItemInfo )
//{
//    for ( int i = 0; i < (int)vecDurationItemInfo.size(); ++i )
//    {
//        RemoveInventory( vecDurationItemInfo[i].m_dwDurationItemUID );
//    }
//}
//
//bool KGCItemManager::ClearConsumptionItemFromEquip( void )
//{
//    bool bErase = false;
//
//    // 장착된 아이템 중 수량이 소진된 아이템은 제거한다.
//    for ( int i = 0; i < END_CharacterNum; ++i )
//    {
//        for ( int j = 0; j < CapsSetItems && j < GetEquipSize( i ); ++j )
//        {
//            if ( GetEquipItemData( i, j ).shReserved2 > 0 && GetEquipItem( i, j ).m_iDuration == 0 )
//            {
//                RemoveEquip( i, j );
//                bErase = true;
//            }
//        }
//    }
//
//    return bErase;
//}
//
//void KGCItemManager::ClearEquip( int iChar )
//{
//    switch ( iChar )
//    {
//        case ICT_KNIGHT: case ICT_ARCHER: case ICT_WIZARD:
//        {
//            m_avecItemEquip[iChar].clear();
//            //CalcChksumItemEquip();
//            break;
//        }
//        default:
//        {
//            ASSERT( false && "ClearEquip : invalid character type" );
//            break;
//        }
//    }
//}
//
//void KGCItemManager::SetEquip( int iChar, const KInventoryItem& kEquip )
//{
//    switch ( iChar )
//    {
//        case ICT_KNIGHT: case ICT_ARCHER: case ICT_WIZARD:
//        {
//            if ( m_avecItemEquip[iChar].size() < CapsSetItems )
//            {
//                m_avecItemEquip[iChar].push_back( kEquip );
//                //CalcChksumItemEquip();
//            }
//            else
//            {
//                ASSERT( false && "Equip full!" );
//            }
//            break;
//        }
//        default:
//        {
//            ASSERT( false && "SetEquip : invalid character type" );
//            break;
//        }
//    }
//}
//
//void KGCItemManager::RemoveEquip( int iChar, int iIndex )
//{
//    switch ( iChar )
//    {
//        case ICT_KNIGHT: case ICT_ARCHER: case ICT_WIZARD:
//        {
//            if ( 0 <= iIndex && iIndex < GetEquipSize( iChar ) )
//            {
//                m_avecItemEquip[iChar].erase( m_avecItemEquip[iChar].begin() + iIndex );
//                //CalcChksumItemEquip();
//            }
//            break;
//        }
//        default:
//        {
//            ASSERT( false && "RemoveEquip : invalid character type" );
//            break;
//        }
//    }
//}
//
//void KGCItemManager::DecDurationItem( int iItemID )
//{
//    int iDuration = 0;
//
//    // 총 인벤토리에서 아이템을 찾아서 Duration을 1만큼 줄여준다.
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( m_vecTotalInventory[i].m_iItemID == iItemID )
//        {
//            if ( m_vecTotalInventory[i].m_iDuration > 0 ) m_vecTotalInventory[i].m_iDuration--;
//            iDuration = m_vecTotalInventory[i].m_iDuration;
//
//            // 수량이 0이라면 아이템을 제거한다.
//            if ( iDuration == 0 )
//            {
//                RemoveInventory( m_vecTotalInventory[i].m_dwUID );
//            }
//        }
//    }
//
//    //CalcChksumInventory();
//
//    // 내 장비에서 Duration을 1만큼 줄여준다.
//    for ( i = 0; i < END_CharacterNum; ++i )
//    {
//        for ( int j = 0; j < CapsSetItems; j++)
//        {
//            if ( m_avecItemEquip[i][j].m_iItemID == iItemID )
//            {
//                m_avecItemEquip[i][j].m_iDuration = iDuration;
//            }
//        }
//    }
//
//    //CalcChksumItemEquip();
//}
//
//void KGCItemManager::DecDurationItem( const std::vector<KDelDurationCountInfo>& vecDecDurationItem )
//{
//    for ( int i = 0; i < (int)vecDecDurationItem.size(); ++i )
//    {
//        DecDurationItem( static_cast<int>( vecDecDurationItem[i].m_dwGoodsID / 10 ) );
//    }
//}
//
////void KGCItemManager::EquipItem( int  iChar,
////                                int  iItemID,
////                                int  iPattern   /* = 0 */,
////                                int  iDuration  /* = -1 */,
////                                bool bCheckCert /* = true */,
////                                bool bCheckChar /* = true */ )
////{
////    ASSERT( m_dwNumItem != 0 );
////    ASSERT( m_aItemData != NULL );
////    ASSERT( 0 <= iItemID && iItemID < (int)m_dwNumItem );
////
////    // 장착용 장비인지 체크
////    if ( m_aItemData[iItemID].dwExtra1 & ITEM_UNABLE_EQUIP )
////    {
////        //if ( g_pkUIScene != NULL )
////        //{
////        //    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////        //                             g_pkStrLoader->GetString( STR_ID_UNABLE_EQUIP1 ),
////        //                             g_pkStrLoader->GetString( STR_ID_UNABLE_EQUIP2 ) );
////        //}
////        return;
////    }
////
////    // 해당 캐릭터와 일치하는 장비인지 체크
////    if ( bCheckChar )
////    {
////        if ( !( m_aItemData[iItemID].dwExtra1 & ItemCharTypeToItemFlag( iChar ) ) ) return;
////    }
////
////    if ( bCheckCert ) // 착용 자격을 확인해야 할 경우
////    {
////        // NOTE : 일단 구입했으면 아무나 입을 수 있도록 기획이 수정됨.
//////{{ Deleted by parkch: 2005-03-17 오전 10:12:58
//////        // 길드마스터 복장이라면 길드자격이 되는지 체크
//////        if ( ( m_aItemData[iItemID].dwExtra3 & ITEM_GUILD_MASTER_ONLY ) &&
//////             ( g_ucGuildUserLevel != 3 && g_ucGuildUserLevel != 5 ) )
//////        {
//////            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
//////                                     g_pkStrLoader->GetString( STR_ID_GUILD_ONLY_EQUIP1 ),
//////                                     g_pkStrLoader->GetString( STR_ID_GUILD_ONLY_EQUIP2 ) );
//////            return;
//////        }
//////}} Deleted by parkch: 2005-03-17 오전 10:12:58
////
////        // 장비 제한 레벨 이상인지 체크
////        if ( m_aItemData[iItemID].btLevel > Exp_2_Level( MyExp[iChar] ) ) return;
////
////        // 전직 무기라면 자격증이 있는지 체크
////        if ( m_aItemData[iItemID].dwExtra1 & ITEM_JOB_LV2 )
////        {
////            for ( int i = 0, iSize = GetInventorySize( iChar ); i < iSize; ++i )
////            {
////                DWORD dwFlag = GetInventoryItemData( iChar, i ).dwExtra1;
////                if ( CheckCertifyItem( GetInventoryItem( iChar, i ).m_iItemID ) &&
////                     ( dwFlag & ITEM_SCROLL_FOR_JOB_LV2 ) &&
////                     ( dwFlag & ItemCharTypeToItemFlag( iChar ) ) ) break;
////            }
////
////            if ( i == iSize ) // 자격증을 찾지 못했다면
////            {
////                //if ( g_pkUIScene != NULL )
////                //{
////                //    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                //                             g_pkStrLoader->GetString( STR_ID_CERT_NEED_EQUIP1 ),
////                //                             g_pkStrLoader->GetString( STR_ID_CERT_NEED_EQUIP2 ) );
////                //}
////                return;
////            }
////        }
////    }
////
////    // 이미 착용 중인 장비인지 체크
////    for ( int i = 0; i < CapsSetItems && i < GetEquipSize( iChar ); ++i )
////    {
////        if ( iItemID == GetEquipItem( iChar, i ).m_iItemID ) return;
////    }
////
////    // 이미 착용 중인 장비와 충돌이 발생한다면 착용 중인 장비를 벗긴다.
////    for ( i = 0; i < CapsSetItems && i < GetEquipSize( iChar ); ++i )
////    {
////        if ( CheckFlagConflict( GetEquipItemData( iChar, i ).dwExtra1,
////                                m_aItemData[iItemID].dwExtra1 ) )
////        {
////            RemoveEquip( iChar, i-- );
////        }
////    }
////
////    // 아이템을 추가로 더 장착할 수 있는지 확인
////    if ( GetEquipSize( iChar ) < CapsSetItems )
////    {
////        KInventoryItem kInsertItem;
////        kInsertItem.m_iItemID   = iItemID;
////        kInsertItem.m_iPattern  = iPattern;
////        kInsertItem.m_iDuration = iDuration;
////        m_avecItemEquip[iChar].push_back( kInsertItem );
////        //CalcChksumItemEquip();
////    }
////    //else
////    //{
////    //    // 더 이상 장착할 수 없음.
////    //    if ( g_pkUIScene != NULL )
////    //    {
////    //        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////    //                                 g_pkStrLoader->GetString( STR_ID_NO_MORE_EQUIP1 ),
////    //                                 g_pkStrLoader->GetString( STR_ID_NO_MORE_EQUIP2 ) );
////    //    }
////    //}
////}
//
//void KGCItemManager::UnequipItem( int iChar, int iItemID )
//{
//    switch ( iChar )
//    {
//        case ICT_KNIGHT: case ICT_ARCHER: case ICT_WIZARD:
//        {
//            for ( int i = 0; i < CapsSetItems && i < GetEquipSize( iChar ); ++i )
//            {
//                if ( iItemID == GetEquipItem( iChar, i ).m_iItemID )
//                {
//                    m_avecItemEquip[iChar].erase( m_avecItemEquip[iChar].begin() + i );
//                    //CalcChksumItemEquip();
//                    break;
//                }
//            }
//            break;
//        }
//        default:
//        {
//            ASSERT( false && "UnequipItem : invalid character type" );
//            break;
//        }
//    }
//}
//
////bool KGCItemManager::PatternUpItem( int iChar, int iInventoryIdx, int iPattrnIdx )
////{
////    // 인덱스 체크
////    if ( iPattrnIdx < 0 || iPattrnIdx >= NUM_PATTERN_TYPE ) return false;
////
////    // A패턴이 없는 아이템의 A패턴을 요구했는지 체크
////    if ( ( IPT_PATTERN_A == iPattrnIdx ) &&
////        !( GetInventoryItemData( iChar, iInventoryIdx ).dwExtra1 & ITEM_PATTERN_A ) ) return false;
////
////    // B패턴이 없는 아이템의 B패턴을 요구했는지 체크
////    if ( ( IPT_PATTERN_B == iPattrnIdx ) &&
////        !( GetInventoryItemData( iChar, iInventoryIdx ).dwExtra1 & ITEM_PATTERN_B ) ) return false;
////
////    KInventoryItem kItem = GetInventoryItem( iChar, iInventoryIdx );
////
////    // 이미 패턴업이 되어있는지 체크
////    if ( ( iPattrnIdx == IPT_PATTERN_A && kItem.m_iPattern == PatternTypeToPatternNum( IPT_PATTERN_A ) ) ||
////         ( iPattrnIdx == IPT_PATTERN_B && kItem.m_iPattern == PatternTypeToPatternNum( IPT_PATTERN_B ) ) )
////    {
////        return false;
////    }
////
////    // 웹브라우저를 띄우기 전에 버퍼 카운트를 맞춰준다.
////    //for ( int cnt = 0; !g_MyD3D->IsPrimaryBuffer && cnt < 100; ++cnt )
////    //{
////    //    if ( D3D_OK == g_MyD3D->m_pd3dDevice->Present( NULL, NULL, NULL, NULL ) )
////    //    {
////    //        g_MyD3D->IsPrimaryBuffer = !g_MyD3D->IsPrimaryBuffer;
////    //    }
////    //}
////    if ( 100 == cnt ) return false;
////
////    RECT rc =
////    {
////        W_Width  / 2 - DBOX_SIZE_X / 2,
////        W_Height / 2 - DBOX_SIZE_Y / 2,
////        W_Width  / 2 - DBOX_SIZE_X / 2 + DBOX_SIZE_X,
////        W_Height / 2 - DBOX_SIZE_Y / 2 + DBOX_SIZE_Y,
////    };
////    //if ( false == g_Browser.Create( rc, g_MyD3D->m_hWnd ) ) return false;
////
////    std::ostringstream strmWebPage;
////#   ifdef _JAPANESE
////    {
////        extern char MyPlayerName[25];
////
////        // NOTE : 일본에서는 임시 페이지에서 테스트할 수 있도록 한다.
////        strmWebPage << "http://218.40.61.1/gameshop/buy_item.asp?"
////                    << "gamecode=" << MyPlayerName
////                    << "&goodsid=" << 10 * kItem.m_iItemID + PatternTypeToPatternNum( iPattrnIdx )
////                    << "&goodsuid=" << kItem.m_dwUID;
////    }
////#   else // _KOREAN
////    {
////        strmWebPage << "http://www.netmarble.net/cp_site/chase/shop/gameshop/buy_item.asp?gamecode=1015&goodsid="
////                    << 10 * kItem.m_iItemID + PatternTypeToPatternNum( iPattrnIdx )
////                    << "&goodsuid=" << kItem.m_dwUID;
////    }
////#   endif
////    g_Browser.Navigate( strmWebPage.str().c_str() );
////
////    MSG     msg;
////    DWORD   dwChkTime = ::timeGetTime() - 3000;
////
////    while ( false == g_Browser.IsClosed() )
////    {
////        DWORD dwCurTime = ::timeGetTime();
////
////        if ( dwCurTime - dwChkTime >= 3000 ) // 3초에 한번씩 체크
////        {
////            dwChkTime = dwCurTime;
////
////            #ifndef _DISABLE_HACK_CHECK_
////            {
////                if ( CheckHackTool() ) // 해킹툴이 있는지 검사한다.
////                {
////                    g_MyD3D->ExitWithHackAlert();
////                    return false;
////                }
////            }
////            #endif // !_DISABLE_HACK_CHECK_
////        }
////
////        while ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
////        {
////            if ( FALSE == ::IsDialogMessage( g_Browser.GetWndHandle(), &msg ) ) ::DispatchMessage( &msg );
////        }
////
////        ::Sleep( 10 );
////    }
////
////    KP2P::GetInstance()->Send_InventoryInfoReq( "Server" );
////
////    Result_Buy = 0;
////
////    DWORD dwStartTime = ::timeGetTime();
////    while ( !Result_Buy )
////    {
////        if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
////        {
////            ::DispatchMessage( &msg );
////        }
////
////        if ( dwStartTime + 10000 < ::timeGetTime() ) Result_Buy = 1;
////    } 
////
////    for ( int i = 0; i < GetEquipSize( iChar ); ++i )
////    {
////        KInventoryItem kEquip = GetEquipItem( iChar, i );
////        if ( kItem.m_iItemID == kEquip.m_iItemID && kItem.m_iPattern == kEquip.m_iPattern )
////        {
////            m_avecItemEquip[iChar][i].m_iPattern = PatternTypeToPatternNum( iPattrnIdx );
////            break;
////        }
////    }
////
////    CalcChksumItemEquip();
////    CheckItemConflict( true );
////
////    return true;
////}
//
////void KGCItemManager::BuyItem( int iItemID )
////{
////    if ( GetItemData( iItemID ).bCash ) // 캐쉬 아이템일 경우
////    {
////        // 웹브라우저를 띄우기 전에 버퍼 카운트를 맞춰준다.
////        //for ( int cnt = 0; !g_MyD3D->IsPrimaryBuffer && cnt < 100; ++cnt )
////        //{
////        //    if ( D3D_OK == g_MyD3D->m_pd3dDevice->Present( NULL, NULL, NULL, NULL ) )
////        //    {
////        //        g_MyD3D->IsPrimaryBuffer = !g_MyD3D->IsPrimaryBuffer;
////        //    }
////        //}
////        //if ( 100 == cnt ) return;
////
////        RECT rc =
////        {
////            W_Width  / 2 - DBOX_SIZE_X / 2,
////            W_Height / 2 - DBOX_SIZE_Y / 2,
////            W_Width  / 2 - DBOX_SIZE_X / 2 + DBOX_SIZE_X,
////            W_Height / 2 - DBOX_SIZE_Y / 2 + DBOX_SIZE_Y,
////        };
////        if ( false == g_Browser.Create( rc, g_MyD3D->m_hWnd ) ) return;
////
////        std::ostringstream strmWebPage;
////#       ifdef _JAPANESE
////        {
////            extern char MyPlayerName[25];
////
////            // NOTE : 일본에서는 임시 페이지에서 테스트할 수 있도록 한다.
////            strmWebPage << "http://218.40.61.1/gameshop/buy_item.asp?"
////                        << "gamecode=" << MyPlayerName
////                        << "&goodsid=" << 10 * iItemID
////                        << "&goodsuid=0";
////        }
////#       else // _KOREAN
////        {
////            strmWebPage << "http://www.netmarble.net/cp_site/chase/shop/gameshop/buy_item.asp?gamecode=1015&goodsid="
////                        << 10 * iItemID << "&goodsuid=0";
////        }
////#       endif
////        g_Browser.Navigate( strmWebPage.str().c_str() );
////
////        DWORD dwChkTime = ::timeGetTime() - 3000;
////        while ( false == g_Browser.IsClosed() )
////        {
////            DWORD dwCurTime = ::timeGetTime();
////            if ( dwCurTime - dwChkTime >= 3000 ) // 3초에 한번씩 체크
////            {
////                dwChkTime = dwCurTime;
////
////                #ifndef _DISABLE_HACK_CHECK_
////                {
////                    if ( CheckHackTool() ) // 해킹툴이 있는지 검사한다.
////                    {
////                        g_MyD3D->ExitWithHackAlert();
////                        return;
////                    }
////                }
////                #endif // !_DISABLE_HACK_CHECK_
////            }
////
////            MSG msg;
////            while ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
////            {
////                if ( FALSE == ::IsDialogMessage( g_Browser.GetWndHandle(), &msg ) )
////                {
////                    ::DispatchMessage( &msg );
////                }
////            }
////
////            ::Sleep( 10 );
////        }
////
////        KP2P::GetInstance()->Send_InventoryInfoReq( "Server" );
////    }
////    else // GP 아이템을 구입하는 경우
////    {
////        ASSERT( g_pkUIScene != NULL );
////
////        // 자격증 아이템을 구입하는 경우에는 다음 조건을 확인하고 처리한다.
////        switch ( iItemID )
////        {
////            case ITEM_CERT_KNIGHT:
////            case ITEM_CERT_ARCHER:
////            case ITEM_CERT_WIZARD:
////            {
////                // 찢어진 자격증이 있는지 찾아보고 있다면
////                // 찢어진 자격증을 함께 이용한 인증을 권유한다.
////                for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
////                {
////                    if ( ITEM_CERT_BROKEN == m_vecTotalInventory[i].m_iItemID )
////                    {
////                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                                 g_pkStrLoader->GetString( STR_ID_CERT_BROKEN_USE1 ),
////                                                 g_pkStrLoader->GetString( STR_ID_CERT_BROKEN_USE2 ) );
////                        return; // exit
////                    }
////                }
////                break;
////            }
////            case ITEM_CERT_BROKEN_KNIGHT:
////            case ITEM_CERT_BROKEN_ARCHER:
////            case ITEM_CERT_BROKEN_WIZARD:
////            {
////                // 찢어진 자격증을 이용한 인증이라면 찢어진 자격증이 있는지 찾아보고 없으면 경고한다.
////                for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
////                {
////                    if ( ITEM_CERT_BROKEN == m_vecTotalInventory[i].m_iItemID ) break;
////                }
////                if ( i == m_vecTotalInventory.size() )
////                {
////                    if ( g_pkUIScene != NULL )
////                    {
////                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                                 g_pkStrLoader->GetString( STR_ID_CERT_BROKEN_NEED1 ),
////                                                 g_pkStrLoader->GetString( STR_ID_CERT_BROKEN_NEED2 ) );
////                        return; // exit
////                    }
////                }
////
////                // 찢어진 자격증을 가지고 있다면 일반 자격증으로 바꿔서 처리해준다.
////                switch ( iItemID )
////                {
////                    case ITEM_CERT_BROKEN_KNIGHT: iItemID = ITEM_CERT_KNIGHT; break;
////                    case ITEM_CERT_BROKEN_ARCHER: iItemID = ITEM_CERT_ARCHER; break;
////                    case ITEM_CERT_BROKEN_WIZARD: iItemID = ITEM_CERT_WIZARD; break;
////                }
////                break;
////            }
////        }
////
////        // 레벨제한을 체크한다.
////        switch ( iItemID )
////        {
////            case ITEM_CERT_KNIGHT:
////            {
////                if ( MyExp[ICT_KNIGHT] < 560 )
////                {
////                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                             g_pkStrLoader->GetString( STR_ID_CERT_KNIGHT_LEVEL1 ),
////                                             g_pkStrLoader->GetString( STR_ID_CERT_KNIGHT_LEVEL2 ) );
////                    return; // exit
////                }
////                break;
////            }
////            case ITEM_CERT_ARCHER:
////            {
////                if ( MyExp[ICT_ARCHER] < 560 )
////                {
////                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                             g_pkStrLoader->GetString( STR_ID_CERT_ARCHER_LEVEL1 ),
////                                             g_pkStrLoader->GetString( STR_ID_CERT_ARCHER_LEVEL2 ) );
////                    return; // exit
////                }
////                break;
////            }
////            case ITEM_CERT_WIZARD:
////            {
////                if ( MyExp[ICT_WIZARD] < 560 )
////                {
////                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                             g_pkStrLoader->GetString( STR_ID_CERT_WIZARD_LEVEL1 ),
////                                             g_pkStrLoader->GetString( STR_ID_CERT_WIZARD_LEVEL2 ) );
////                    return; // exit
////                }
////                break;
////            }
////        }
////
////        Result_Buy = 0;
////
////        if ( CheckCertifyItem( iItemID ) ) // 자격증일 경우
////        {
////            KP2P::GetInstance()->Send_BuyCertForItemReq( "Server", 10 * iItemID );
////        }
////        else if ( CheckChuseokEventSetItem( iItemID ) )
////        {
////            KP2P::GetInstance()->Send_ChuseokEventItemReq( "Server", 10 * iItemID );
////        }
////        else
////        {
////            KP2P::GetInstance()->Send_BuyItemForGPReq( "Server", 10 * iItemID, 0 );
////        }
////
////        MSG     msg;
////        DWORD   dwStartTime = ::timeGetTime();
////        while ( !Result_Buy )
////        {
////            if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
////            {
////                ::DispatchMessage( &msg );
////            }
////
////            if ( dwStartTime + 10000 < ::timeGetTime() ) Result_Buy = 2;
////        }
////
////        if ( Result_Buy == 1 ) // 성공
////        {
////            g_KDSound.Play( "78" );
////            if ( MyEventItem < 0 ) MyEventItem = 0;
////            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                     g_pkStrLoader->GetString( STR_ID_BUY_ITEM_SUCCEEDED1 ),
////                                     g_pkStrLoader->GetString( STR_ID_BUY_ITEM_SUCCEEDED2 ) );
////        }
////        else if ( Result_Buy == 2 ) // 타임아웃
////        {
////            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1 ),
////                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_NO_RESPONSE2 ) );
////        }
////        else // 실패
////        {
////            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
////                                     g_pkStrLoader->GetString( STR_ID_BUY_ITEM_FAILED1 ),
////                                     g_pkStrLoader->GetString( STR_ID_BUY_ITEM_FAILED2 ) );
////        }
////    }
////}
//
////bool KGCItemManager::CheckItemConflict( bool bInventoryCheck /* = true */ )
////{
////    bool bIsClean = true;
////
////    // NOTE : 액션 리그 서버라면 인벤토리에 없어도 입을 수 있게 한다.
////    if ( g_cContestServer == 2 ) bInventoryCheck = false;
////
////    for ( int i = 0; i < END_CharacterNum; ++i )
////    {
////        std::vector<KInventoryItem> vecCleanEquip;
////        DWORD                       dwFlag = 0;
////
////        for ( int j = 0; j < CapsSetItems && j < GetEquipSize( i ); ++j )
////        {
////            KInventoryItem  kEqipItem   = GetEquipItem( i, j );
////            GCItem          kItemData   = GetEquipItemData( i, j );
////
////            // 아이템 인덱스가 유효한지 체크
////            if ( kEqipItem.m_iItemID < 0 || kEqipItem.m_iItemID >= (int)m_dwNumItem )
////            {
////                bIsClean = false;
////                continue;
////            }
////
////            // 장착용이 아닌 아이템을 장착했는지 체크
////            if ( kItemData.dwExtra1 & ITEM_UNABLE_EQUIP )
////            {
////                bIsClean = false;
////                continue;
////            }
////
////            // 해당 캐릭터가 장착할 수 없는 아이템을 장착했는지 체크
////            if ( !( kItemData.dwExtra1 & ItemCharTypeToItemFlag( i ) ) )
////            {
////                bIsClean = false;
////                continue;
////            }
////
////            // 이미 장착된 부위에 또다른 아이템이 장착되어 있는지 체크
////            if ( CheckFlagConflict( dwFlag, kItemData.dwExtra1 ) )
////            {
////                bIsClean = false;
////                continue;
////            }
////
////            if ( bInventoryCheck ) // 인벤토리에 있는 아이템인지 체크한다면
////            {
////                // 레벨 제한으로 장착할 수 없는 아이템인지 체크
////                if ( kItemData.btLevel > Exp_2_Level( MyExp[i] ) )
////                {
////                    bIsClean = false;
////                    continue;
////                }
////
////                // NOTE : 일단 구입했으면 아무나 입을 수 있도록 기획이 수정됨.
//////                // 길드마스터 복장이라면 길드자격이 되는지 체크
//////                if ( ( kItemData.dwExtra3 & ITEM_GUILD_MASTER_ONLY ) &&
//////                     ( g_ucGuildUserLevel != 3 && g_ucGuildUserLevel != 5 ) )
//////                {
//////                    bIsClean = false;
//////                    continue;
//////                }
////
////                // 인벤토리에 없거나 자격증 없이 전직무기를 착용하고 있는지 체크
////                if ( false == CheckBaseItem( kEqipItem.m_iItemID ) ) // 기본 아이템이 아닐 경우
////                {
////                    bool bFound     = false;
////                    bool bCertFree  = true;
////
////                    if ( kItemData.dwExtra1 & ITEM_JOB_LV2 ) bCertFree = false;
////
////                    for ( int k = 0; k < GetInventorySize( i ); ++k )
////                    {
////                        KInventoryItem  kInvItem        = GetInventoryItem( i, k );
////                        GCItem          kInvItemData    = GetInventoryItemData( i, k );
////
////                        if ( kEqipItem.m_iItemID  == kInvItem.m_iItemID &&
////                             kEqipItem.m_iPattern == kInvItem.m_iPattern )
////                        {
////                            kEqipItem.m_iDuration = kInvItem.m_iDuration;
////                            bFound = true;
////                        }
////
////                        if ( false == bCertFree )
////                        {
////                            if ( CheckCertifyItem( kInvItem.m_iItemID ) &&
////                                 ( kInvItemData.dwExtra1 & ItemCharTypeToItemFlag( i ) ) &&
////                                 ( kInvItemData.dwExtra1 & ITEM_SCROLL_FOR_JOB_LV2 ) )
////                            {
////                                bCertFree = true;
////                            }
////                        }
////                    } // for - k
////
////                    if ( !bFound || !bCertFree )
////                    {
////                        bIsClean = false;
////                        continue;
////                    }
////                } // if ( false == CheckBaseItem( kEqipItem.m_iItemID ) )
////            } // if ( bInventoryCheck )
////
////            dwFlag |= kItemData.dwExtra1;
////            vecCleanEquip.push_back( kEqipItem );
////        } // for - j
////
////        m_avecItemEquip[i] = vecCleanEquip;
////
////        // 필수 요소가 빠져있는지 검사하고 없다면 기본 아이템으로 채워넣는다.
////        if ( !( dwFlag & ITEM_HEAD ) ) // 머리가 없다면 기본 머리를 넣는다.
////        {
////            switch ( i )
////            {
////                case ICT_KNIGHT: EquipItem( i, 0 );  break; // 기사머리
////                case ICT_ARCHER: EquipItem( i, 5 );  break; // 궁수머리
////                case ICT_WIZARD: EquipItem( i, 10 ); break; // 법사머리
////            }
////            bIsClean = false;
////        }
////        if ( !( dwFlag & ITEM_BODY1 ) ) // 상의가 없다면 기본 상의를 넣는다.
////        {
////            switch ( i )
////            {
////                case ICT_KNIGHT: EquipItem( i, 1 );  break; // 기사몸(상)
////                case ICT_ARCHER: EquipItem( i, 6 );  break; // 궁수몸(상)
////                case ICT_WIZARD: EquipItem( i, 11 ); break; // 법사몸(상)
////            }
////            bIsClean = false;
////        }
////        if ( !( dwFlag & ITEM_BODY2 ) ) // 하의가 없다면 기본 하의를 넣는다.
////        {
////            switch ( i )
////            {
////                case ICT_KNIGHT: EquipItem( i, 2 );  break; // 기사몸(하)
////                case ICT_ARCHER: EquipItem( i, 7 );  break; // 궁수몸(하)
////                case ICT_WIZARD: EquipItem( i, 12 ); break; // 법사몸(하)
////            }
////            bIsClean = false;
////        }
////        if ( !( dwFlag & ITEM_WEAP ) ) // 무기가 없다면 기본 무기를 넣는다.
////        {
////            switch ( i )
////            {
////                case ICT_KNIGHT: EquipItem( i, 3 );  break; // 기사무기(R)
////                case ICT_ARCHER: EquipItem( i, 8 );  break; // 궁수무기(R)
////                case ICT_WIZARD: EquipItem( i, 13 ); break; // 법사무기(R)
////            }
////            bIsClean = false;
////        }
////    } // for - i
////
////    //CalcChksumItemEquip();
////
////    return bIsClean;
////}
//
////bool KGCItemManager::CheckFlagConflict( DWORD dwFlag1, DWORD dwFlag2 ) const
////{
////    if ( dwFlag1 & ITEM_HEAD          & dwFlag2 ) return true; // 머리 충돌
////    if ( dwFlag1 & ITEM_BODY1         & dwFlag2 ) return true; // 상의 충돌
////    if ( dwFlag1 & ITEM_BODY2         & dwFlag2 ) return true; // 하의 충돌
////    if ( dwFlag1 & ITEM_WEAP          & dwFlag2 ) return true; // 무기 충돌
////    if ( dwFlag1 & ITEM_HAND          & dwFlag2 ) return true; // 장갑 충돌
////    if ( dwFlag1 & ITEM_SHOOE         & dwFlag2 ) return true; // 발 충돌
////    if ( dwFlag1 & ITEM_HAND_ACC      & dwFlag2 ) return true; // 손 악세서리 충돌
////    if ( dwFlag1 & ITEM_BODY1_ACC     & dwFlag2 ) return true; // 상의 충돌
////    if ( dwFlag1 & ITEM_BODY2_ACC     & dwFlag2 ) return true; // 하의 충돌
////    if ( dwFlag1 & ITEM_BACKCLOTH_ACC & dwFlag2 ) return true; // 망또 충돌
////    if ( dwFlag1 & ITEM_FACEUP_ACC    & dwFlag2 ) return true; // 눈 위 악세서리 충돌
////    if ( dwFlag1 & ITEM_FACEDOWN_ACC  & dwFlag2 ) return true; // 눈 아래 악세서리 충돌
////    if ( dwFlag1 & ITEM_ARM_ACC       & dwFlag2 ) return true; // 팔 악세서리 충돌
////    if ( dwFlag1 & ITEM_SHOOE_ACC     & dwFlag2 ) return true; // 발 악세서리 충돌
////    if ( dwFlag1 & ITEM_NEWLAYER_ACC  & dwFlag2 ) return true; // 특수 악세서리 충돌
////    return false;
////}
//
//bool KGCItemManager::CheckBaseItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 0:     // 기사머리
//        case 1:     // 기사몸(상)
//        case 2:     // 기사몸(하)
//        case 3:     // 기사무기(R)
//        case 4:     // 기사무기(L)
//        case 5:     // 궁수머리
//        case 6:     // 궁수몸(상)
//        case 7:     // 궁수몸(하)
//        case 8:     // 궁수무기(R)
//        case 9:     // 궁수무기(L)
//        case 10:    // 법사머리
//        case 11:    // 법사몸(상)
//        case 12:    // 법사몸(하)
//        case 13:    // 법사무기(R)
//        case 14:    // 법사무기(L)
//        {
//            return true; // 기본 아이템으로 분류
//        }
//    }
//    return false; // 일반 아이템으로 분류
//}
//
//bool KGCItemManager::CheckAdminItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 81:    // 초고수 기사
//        case 82:    // 초고수 궁수
//        case 83:    // 초고수 법사
//        case 84:    // 운영자 기사
//        case 85:    // 운영자 궁수
//        case 86:    // 운영자 법사
//        case 202:   // 상선씨 머리
//        case 203:   // 상선씨 몸
//        case 204:   // 뿅망치
//        {
//            return true; // 운영자 전용 아이템으로 분류
//        }
//    }
//    return false; // 그 외의 아이템으로 분류
//}
//
//bool KGCItemManager::CheckSpecialItem( int iItemID ) const
//{
//    if ( CheckBaseItem( iItemID ) || CheckAdminItem( iItemID ) ) return true;
//
//    switch ( iItemID )
//    {
//        // TODO : 이벤트 기간동안 판매하지 않을 아이템은 여기에 추가해두면 된다.
//        //case 816: // 한복 재단용 바늘 -> 팔 수 있게 하자.
//        //case 817: // 한복 재단용 실   -> 팔 수 있게 하자.
//        //case 818: // 한복 재단용 비단 -> 팔 수 있게 하자.
//        //case 819: // 한복 재단용 구슬 -> 팔 수 있게 하자.
//        // NOTE : 길드 방패는 되팔 수 없다.
//        case 1150:  // 누적1위 길드 방패
//        case 1151:  // 누적2위 길드 방패
//        case 1152:  // 누적3위 길드 방패
//        case 1153:  // 주간1위 길드 방패
//        //case 1254:  // 크리스마스 복권 -> 팔 수 있게 하자.
//        //case 1280:  // 짱 마크 -> 팔 수 있게 하자.
//        //case 1281:  // 온게임넷 마크 -> 팔 수 있게 하자.
//        //case 1378:  // 설날 복주머니 -> 팔 수 있게 하자.
//        case 1253:   // 카제아제 지도조각1
//        case 1369:   // 카제아제 지도조각2
//        case 1413:   // 카제아제 지도조각3
//        case 1517:   // 카제아제 지도조각4
//        {
//            return true; // 특수 아이템으로 분류
//        }
//    }
//
//    return false; // 그 외의 아이템으로 분류
//}
//
//bool KGCItemManager::CheckEquipItem( int iItemID ) const
//{
//    for ( int i = 0; i < END_CharacterNum; ++i )
//    {
//        for ( int j = 0; j < CapsSetItems && j < GetEquipSize( i ); ++j )
//        {
//            if ( iItemID == m_avecItemEquip[i][j].m_iItemID )
//            {
//                return true; // found
//            }
//        }
//    }
//    return false; // not found
//}
//
//bool KGCItemManager::CheckEquipItem( int iCharNum, int iItemID ) const
//{
//    for ( int j = 0; j < CapsSetItems && j < GetEquipSize( iCharNum ); ++j )
//    {
//        if ( iItemID == m_avecItemEquip[iCharNum][j].m_iItemID )
//        {
//            return true; // found
//        }
//    }
//    return false; // not found
//}
//
//bool KGCItemManager::CheckEquipItemAttribFlag( int iChar, DWORD dwFlag ) const
//{
//    for ( int i = 0; i < CapsSetItems && i < GetEquipSize( iChar ); ++i )
//    {
//        if ( GetEquipItemData( iChar, i ).dwExtra2 & dwFlag )
//        {
//            return true; // found
//        }
//    }
//    return false; // not found
//}
//
//bool KGCItemManager::CheckCertifyItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 229:   // 창병 자격증
//        case 230:   // 찢어진 자격증
//        case 231:   // 찢어진 자격증에 창병 승인
//        case 294:   // 석궁 자격증
//        case 295:   // 찢어진 자격증에 석궁 승인
//        case 377:   // 전투연금술사 자격증
//        case 378:   // 찢어진 자격증에 술사 승인
//        {
//            return true; // 자격증 아이템으로 분류
//        }
//    }
//    return false; // 그 외의 아이템으로 분류
//}
//
//bool KGCItemManager::CheckRenasItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 652:   // 레나스 아티펙트
//        case 653:   // 진 레나스 아티펙트
//        {
//            return true; // 레나스 아이템으로 분류
//        }
//    }
//    return false; // 그 외의 아이템으로 분류
//}
//
//bool KGCItemManager::CheckRandomMoneyItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 245:   // 세뱃돈
//        {
//            return true; // 랜덤 가격의 아이템으로 분류
//        }
//    }
//    return false; // 그 외의 아이템으로 분류
//}
//
//bool KGCItemManager::CheckChuseokEventSetItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 869: // 떠돌이기사 한복세트
//        case 870: // 떠돌이궁수 한복세트
//        case 871: // 떠돌이법사 한복세트
//        case 872: // 용병기사 한복세트
//        case 873: // 용병궁수 한복세트
//        case 874: // 용병법사 한복세트
//        case 875: // 왕립기사 한복세트
//        case 876: // 왕립궁수 한복세트
//        case 877: // 왕립법사 한복세트
//        {
//            return true; // 추석 이벤트 세트 상품으로 분류
//        }
//    }
//    return false; // 그 외의 아이템으로 분류
//}
//
//bool KGCItemManager::CheckChuseokEventPartItem( int iItemID ) const
//{
//    switch ( iItemID )
//    {
//        case 816: // 한복 제단용 바늘
//        case 817: // 한복 제단용 실
//        case 818: // 한복 제단용 비단
//        case 819: // 한복 제단용 구슬
//        {
//            return true; // 추석 이벤트 재료 상품으로 분류
//        }
//    }
//    return false; // 그 외의 아이템으로 분류
//}
//
//int KGCItemManager::GetSumChuseokEventPartItem( void ) const
//{
//    for ( int sum = 0, i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( CheckChuseokEventPartItem( m_vecTotalInventory[i].m_iItemID ) )
//        {
//            sum += m_vecTotalInventory[i].m_iDuration;
//        }
//    }
//    return sum;
//}
//
//bool KGCItemManager::FindItemFromInventory( int iItemID ) const
//{
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( iItemID == m_vecTotalInventory[i].m_iItemID ) return true;
//    }
//    return false;
//}
//
//int KGCItemManager::GetDurationFromInventory( int iItemID ) const
//{
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( iItemID == m_vecTotalInventory[i].m_iItemID )
//        {
//            return m_vecTotalInventory[i].m_iDuration;
//        }
//    }
//    return 0;
//}
//
////DWORD KGCItemManager::Auto_Set_My_State( int PlayerNumber, bool CheckState /* = false */ )
////{
////    bool    bAcc            = false;    // 악세서리인가?
////    DWORD   dwBonus         = 0;        // 아이템의 특수 능력 플래그
////    bool    bDurationOper   = false;    // 수량 감소 처리 여부
////
////    std::vector<DWORD> vecDecDurationGoodsIDList;   // 수량을 감소시킬 아이템의 상품아이디 리스트
////    vecDecDurationGoodsIDList.clear();
////
////    // 수량 감소 처리는 나 자신만 한다.
////    if ( CheckState && g_MyD3D->Get_MyPlayer() == PlayerNumber ) bDurationOper = true;
////
////    for ( int i = 0; i < CapsSetItems; i++ )
////    {
////        int iItemID = g_MyD3D->MyPlayer[PlayerNumber].GetItemID( i );
////        if ( iItemID < 0 ) break;
////
////        // 장비에 의한 상승치를 1/2로 줄였음.
////        g_MyD3D->MyPlayer[PlayerNumber].m_fAtk_Point += (float)GetItemData( iItemID ).shAttack * 0.005f;
////        g_MyD3D->MyPlayer[PlayerNumber].m_fDef_Point += (float)GetItemData( iItemID ).shDefence * 0.0025f;
////        g_MyD3D->MyPlayer[PlayerNumber].m_fHP_Point  += (float)GetItemData( iItemID ).shHpInc * 0.005f;
////
////        shMyAttack  += GetItemData( iItemID ).shAttack;
////        shMyDefence += GetItemData( iItemID ).shDefence;
////        shMyHpInc   += GetItemData( iItemID ).shHpInc;
////
////        int     iSetNumber      = GetItemData( iItemID ).shReserved1;   // 세트 번호
////        bool    bDurationItem   = false;                                // 수량 아이템인가?
////
////        // 수량이 지정되어 있다면 수량 아이템이다.
////        if ( GetItemData( iItemID ).shReserved2 > 0 ) bDurationItem = true;
////
////        // 악세서리 플래그가 체크되어 있다면 악세서리이다.
////        if ( GetItemData( iItemID ).dwExtra1 & ( ITEM_HAND_ACC|ITEM_BODY1_ACC|ITEM_BODY2_ACC|
////                                                 ITEM_FACEUP_ACC|ITEM_FACEDOWN_ACC|ITEM_ARM_ACC|
////                                                 ITEM_SHOOE_ACC|ITEM_NEWLAYER_ACC ) )
////        {
////            bAcc = true;
////        }
////        else
////        {
////            bAcc = false;
////        }
////
////        // 이 아이템의 보너스를 기록해둔다.
////        DWORD   TempBonus       = GetItemData( iItemID ).dwExtra2;
////        int     iNumSetItem     = 0;
////        int     iMyNumSetItem   = 0;
////
////        // 현재 아이템이 세트 번호가 있는가?
////        if ( iSetNumber > -1 )
////        {
////            // 이 세트의 아이템이 총 몇개인지 확인한다.
////            // 세트에서 무기는 제외된다. 세트 자체도 제외된다.
////            // 0-14번은 기본 파트이므로 확인할 필요가 없다.
////            for ( int j = 15; j < (int)GetNumItem(); j++ )
////            {
////                GCItem data = GetItemData( j );
////                if ( iSetNumber == data.shReserved1 &&
////                     !( data.dwExtra1 & ITEM_WEAP ) &&
////                     !( data.dwExtra1 & ITEM_SET ) )
////                {
////                    iNumSetItem++;
////                }
////            }
////
////            // 이 세트 아이템 중 몇개를 장착했는지 확인한다.
////            for ( j = 0; j < CapsSetItems; j++ )
////            {
////                int iPlayerItemID = g_MyD3D->MyPlayer[PlayerNumber].GetItemID( j );
////                if ( iPlayerItemID < 0 ) break;
////
////                GCItem data = GetItemData( iPlayerItemID );
////                if ( ( iSetNumber == data.shReserved1 ||
////                       // 악세서리의 경우 3캐릭터 공용 아이템일 수도 있으므로 다음과 같이 체크한다.
////                       ( bAcc && ( iSetNumber + 1 == data.shReserved1 ||
////                                   iSetNumber + 2 == data.shReserved1 ) ) ) &&
////                     // 세트에서 무기는 제외된다. 세트 자체도 제외된다.
////                     !( data.dwExtra1 & ITEM_WEAP ) && !( data.dwExtra1 & ITEM_SET ) )
////                {
////                    iMyNumSetItem++;
////                }
////            }
////        }
////
////        // NOTE : 세트가 모두 구비되어야 특수 능력이 발휘되는 아이템 때문에 세트 개수를 확인한다.
////
////        // 세트가 모두 구비된 경우이거나 세트가 없는 아이템이라면
////        if ( iNumSetItem == iMyNumSetItem )
////        {
////            if ( bDurationItem ) // 수량 아이템이라면
////            {
////                if ( g_MyD3D->MyPlayer[PlayerNumber].GetItemDuration( i ) > 0 ) // 수량이 최소 0보다 크다면
////                {
////                    if ( CheckState ) // 판수 체크를 정확히 해야하는 상황(게임을 시작할 때)
////                    {
////                        bool EnableUse = true;
////                        switch ( iItemID )
////                        {
////                            case 459: // 거북이 등껍질(기사)
////                            case 460: // 거북이 등껍질(궁수)
////                            case 461: // 거북이 등껍질(법사)
////                            case 652: // 레나스
////                            case 653: // 진레나스
////                            {
////                                // 대전과 길드전에서만 가능하게 하는 하드코딩
////                                if ( !( GC_GMC_MATCH == g_MyD3D->GetGameModeCategory() ||
////                                        GC_GMC_GUILD_BATTLE == g_MyD3D->GetGameModeCategory() ) )
////                                {
////                                    // 대전이나 길드전이 아닌 경우에는 사용하지 않는다.
////                                    EnableUse = false;
////                                }
////                                break;
////                            }
////                            case QUEST_REBIRTH_ITEM_ID: // 수호의 반지(퀘스트부활아이템)
////                            {
////                                // NOTE : 수호의 반지는 게임 도중에 수량이 감소한다.
////                                EnableUse = false;
////                                break;
////                            }
////							case O2MASK_ITEM_ID:
////							{
////								if ( !( GC_GMC_QUEST == g_MyD3D->GetGameModeCategory() &&
////									GC_GM_QUEST3 == g_MyD3D->GetGameMode()) )
////								{
////									EnableUse = false;									
////								}
////								break;
////							}
////                            default:
////                            {
////                                break;
////                            }
////                        }
////
////                        if ( EnableUse ) // 사용하는 경우라면
////                        {
////                            // NOTE : 넷마블 PC방에서는 레나스 아이템을 무한으로 사용할 수 있다.
////                            if ( !g_bNetmarblePC || ( iItemID != 652 && iItemID != 653 ) )
////                            {
////                                g_MyD3D->MyPlayer[PlayerNumber].DecItemDuration( i );
////
////                                if ( bDurationOper )
////                                {
////                                    DecDurationItem( iItemID );
////                                    vecDecDurationGoodsIDList.push_back( 10 * iItemID );
////                                }
////                            }
////
////                            dwBonus |= TempBonus;
////                        }
////                    }
////                    else // 판수 체크를 안 한다면
////                    {
////                        dwBonus |= TempBonus;
////                    }
////                }
////            }
////            else // 수량 아이템 아니라면
////            {
////                dwBonus |= TempBonus;
////            }
////        } // if ( iNumSetItem == iMyNumSetItem )
////    } // for
////
////    if ( bDurationOper )
////    {
////        if ( vecDecDurationGoodsIDList.size() > 0 )
////        {
////            ::Sleep( 100 ); // 패킷 소실을 막기 위한 장치
////            // 게임 시작 패킷이 먼저 날아간 이후에 보내야 한다.
////            KP2P::GetInstance()->Send_UseDurationItemNot( "Server", vecDecDurationGoodsIDList );
////        }
////    }
////
////    return dwBonus; // 조합된 특수 능력 플래그를 리턴한다.
////}
//
//DWORD KGCItemManager::My_State_Exclusive_Item( void )
//{
//    shMyAttack  = 0;
//    shMyDefence = 0;
//    shMyHpInc   = 0;
//
//    int iChar = 0;//g_MyD3D->MyPlayer[7].END_CharacterNum;
//    int aiItemID[CapsSetItems];
//    int aiItemPattern[CapsSetItems];
//    int aiItemDuration[CapsSetItems];
//
//    for ( int i = 0; i < CapsSetItems; ++i )
//    {
//        KInventoryItem kItem = GetEquipItem( iChar, i );
//        aiItemID[i]       = kItem.m_iItemID;
//        aiItemPattern[i]  = kItem.m_iPattern;
//        aiItemDuration[i] = kItem.m_iDuration;
//    }
//    //g_MyD3D->MyPlayer[7].CopyItemInfo( aiItemID, aiItemPattern, aiItemDuration );
//
//    return 0;//Auto_Set_My_State( 7 );
//}
//
////void KGCItemManager::LoadSetItemList( void )
////{
////    std::string strSetItemScript = g_strCurrentPath + "Stage\\SetItem.stg";
////    SetTemplet  setTemplet;
////    int         setNum;
////    int         i;
////    int         j;
////    char        tableName[30] = {0,};
////    char        partTableName[30] = {0,};
////    std::string partName;
////    KLuaManager kLuaMgr;
////
////#ifdef _JAPANESE            // 일본어       (일본)
////	std::string strStringScript = g_strCurrentPath + "Stage\\str_j.stg";
////#elif _CHINESE_SIMPLIFIED   // 중국어 간체  (중국)
////	std::string strStringScript = g_strCurrentPath + "Stage\\str_cs.stg";
////#elif _CHINESE_TRADITIONAL  // 중국어 번체  (대만)
////	std::string strStringScript = g_strCurrentPath + "Stage\\str_ct.stg";
////#else // _KOREAN            // 한국어       (한국)
////	std::string strStringScript = g_strCurrentPath + "Stage\\str.stg";
////#endif
////
////	if( FAILED( kLuaMgr.DoFile( strStringScript.c_str() ) ) )
////		return;
////
////    if( FAILED( kLuaMgr.DoFile( strSetItemScript.c_str() ) ) ) 
////        return;
////
////    kLuaMgr.BeginTable( "SET_ITEM" );
////    {
////        kLuaMgr.GetValue( "SET_NUM", setNum );
////
////        for( i = 0; i < setNum; i++ )
////        {
////            ZeroMemory( &setTemplet, sizeof(SetTemplet) );
////            sprintf( tableName, "SET%d", i );
////
////            kLuaMgr.BeginTable( tableName );
////            {
////                kLuaMgr.GetValue( "NAME",     setTemplet.setName );
////                kLuaMgr.GetValue( "PART_NUM", setTemplet.partNum );
////                
////                for( j = 0; j < setTemplet.partNum; j++ )
////                {
////                    sprintf( partTableName, "PART%d", j );
////                    kLuaMgr.GetValue( partTableName, partName );
////                    setTemplet.partName.push_back( partName );
////                }
////            }
////            kLuaMgr.EndTable();
////            m_SetList.push_back( setTemplet );
////        }
////    }
////    kLuaMgr.EndTable();
////}
//
////std::string KGCItemManager::GetEquipSetItemName( int playerNum )
////{
////    GCItem itemData;
////    int match;
////    std::string retval = "";
////
////    for( int i = 0; i < (int)m_SetList.size(); i++ )
////    {
////        match = 0;
////
////        for( int k = 0; k < m_SetList[i].partNum; k++ )
////        {
////            for( int j = 0; j < CapsSetItems; j++ )
////            {
////                if( g_MyD3D->MyPlayer[playerNum].GetItemID( j ) != -1 )
////                {
////                    itemData = g_MyD3D->m_kItemMgr.GetItemData( g_MyD3D->MyPlayer[playerNum].GetItemID( j ) );
////                    if( itemData.szName == m_SetList[i].partName[k] )
////                    {
////                        match++;
////                    }
////                }
////            }
////        }
////        if( match ==  m_SetList[i].partNum )
////        {
////            return m_SetList[i].setName;
////        }
////    }
////
////    return retval;
////}
//
//bool KGCItemManager::IsExistCrystalDiscountCard( void ) const
//{
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( ITEM_CRYSTAL_DISCOUNT_CARD == m_vecTotalInventory[i].m_iItemID )
//        {
//            return true;
//        }
//    }
//    return false;
//}
//
//bool KGCItemManager::IsExistVivivicZzangMark( void ) const
//{
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( 1280 == m_vecTotalInventory[i].m_iItemID )
//        {
//            return true;
//        }
//    }
//    return false;
//}
//
//bool KGCItemManager::IsExistOnGameNetMark( void ) const
//{
//    for ( int i = 0; i < (int)m_vecTotalInventory.size(); ++i )
//    {
//        if ( 1281 == m_vecTotalInventory[i].m_iItemID )
//        {
//            return true;
//        }
//    }
//    return false;
//}