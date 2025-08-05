#include "stdafx.h"
#include "KGCQALogManager.h"

ImplementSingleton( KGCQALogManager );

KGCQALogManager::KGCQALogManager( void )
: m_bQALog( false )
{
}

KGCQALogManager::~KGCQALogManager( void )
{
    EndQALog();
}

void KGCQALogManager::EndQALog( void )
{
    if( m_ofLog.is_open() ) {
        m_ofLog.close();
    }
}

void KGCQALogManager::WriteLogFile( const std::wstring& strLog_ )
{
    if( false == m_bQALog ) {
        return;
    }

    printf( GCUTIL_STR::GCStrWideToChar( strLog_.c_str() ) );

    if( false == m_ofLog.is_open() ) { 
        return;
    }

    m_ofLog << GCUTIL_STR::GCStrWideToChar( strLog_.c_str() );
}

void KGCQALogManager::SetQALogState( bool bSet_ )
{
    if( bSet_ ) { 
        m_bQALog = true;

        // 이미 열린 파일이 있으면 닫고 
        if( m_ofLog.is_open() ) { 
            m_ofLog.close();
        }

        // 시간별로 저장 
        time_t t;
        struct tm* date;
        time( &t );
        date = localtime( &t );
        std::ostringstream stm;
        stm << "QALog\\Log_" << date->tm_year+1900 << "_" << date->tm_mon+1 << "_" << date->tm_mday << "_" << date->tm_hour << "_" << date->tm_min << "_" << date->tm_sec << ".txt" ;

        m_ofLog.open( stm.str().c_str() );
        if( m_ofLog.is_open() == false ) { 
            // 파일 생성 실패 처리
            ::MessageBoxA( 0, "QA Log 파일 생성 실패", 0, 0 );
        }
        // 콘솔 열기 
        KSingleton<KncP2P>::GetInstance()->CreateConsoleWnd();
    }
    else { 
        m_bQALog = false;
        if( m_ofLog.is_open() ) { 
            m_ofLog.close();
        }
    }
}

void KGCQALogManager::StartGameLog( const KChangeRoomInfo roomInfo_ )
{
    if( !m_bQALog ) { 
        return;
    }

    std::wostringstream stm;
    stm << L"게임 시작!! - " << " GameCategory:" << static_cast<int>( roomInfo_.m_cGameCategory ) << " GameMode:" << roomInfo_.m_iGameMode << " SubGameMode:" << roomInfo_.m_iSubGameMode << std::endl;
    stm << L"게임 인원" << std::endl;
    stm << L"=================================================================" << std::endl;

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
            continue;

        SUserInfo Info = g_MyD3D->MyPlayer[i]->m_kUserInfo;
        stm << Info.strNickName << L"(" << Info.strLogin << L"-" << Info.dwUID << L")" 
            << L" CharType:" << static_cast< int >( Info.GetCurrentChar().iCharType ) << L" Promotion:" << static_cast< int >( Info.GetCurrentChar().iPromotionLevel )
            << L" Level:" << static_cast< int >( Info.GetCurrentChar().iLevel ) << L" Exp:" << Info.GetCurrentChar().biExp << std::endl;

        stm << L"착용 장비=========== " << std::endl;
        for( KUserItemVector::const_iterator vit = Info.GetCurrentChar().vecItemInfo.begin() ; vit != Info.GetCurrentChar().vecItemInfo.end() ; ++vit ) { 
            stm << g_pItemMgr->GetItemName( vit->iItemID ) << L"(" << vit->iItemID << L")" << std::endl;
        }
        stm << std::endl;

        stm<< L"착용 코디=========== " << std::endl;
        for( KUserItemVector::const_iterator vit = Info.GetCurrentChar().vecLookInfo.begin() ; vit != Info.GetCurrentChar().vecLookInfo.end() ; ++vit ) { 
            stm << g_pItemMgr->GetItemName( vit->iItemID ) << L"(" << vit->iItemID << L")" << std::endl;
        }
        stm << std::endl;

        stm << L"착용 펫============ " << std::endl;
        stm << Info.GetCurrentChar().kPetInfo.m_strName << L"(" << Info.GetCurrentChar().kPetInfo.m_dwID << L")" << L" Level:" << Info.GetCurrentChar().kPetInfo.m_iLevel << L" Exp:" << Info.GetCurrentChar().kPetInfo.m_dwEXP << std::endl;

        stm << L"----------------------------------------" << std::endl << std::endl;;
    }

    WriteLogFile( stm.str() );
}

void KGCQALogManager::EndGameLevelUpLog( const std::wstring& strNick_, const bool bLvUP, const int iStartLv_, const int iResultLv_, const bool bSLvUp_, const int iStartSLv_, const int iResultSLv_ )
{
    if( !m_bQALog ) { 
        return;
    }

    WriteLogFile( L"\n 게임 종료 레벨업 정보================================================\n" );

    std::wostringstream stm;

    if( bLvUP ) { 
        stm << strNick_;
        stm << L" LevelUP!!!===========  "<< strNick_ << L"(" << iStartSLv_ << L"->" << iResultLv_ << L")" << std::endl;

    }

    if( bSLvUp_ ) {
        stm << strNick_;
        stm << L" SkillLevelUP!!!===========  " << strNick_ << L"(" << iStartSLv_ << L"->" << iResultSLv_ << L")" << std::endl;
    }

    WriteLogFile( stm.str() );
}

void KGCQALogManager::EndGameExpLog( const std::wstring& strNick_, const int iBaseExp_, const int iExtraExp_, const int iTotalExp, const int iSPExp_ )
{
    if( !m_bQALog ) { 
        return;
    }

    WriteLogFile( L"\n 게임 종료 경험치 정보================================================\n" );

    std::wostringstream stm;
    stm << strNick_ << L" 경험치 획득====================\n";
    stm << L" 기본 경험치:" << iBaseExp_ << std::endl;
    stm << L" 추가 경험치:" << iExtraExp_ << std::endl;
    stm << L" 총 경험치:" << iTotalExp << std::endl;
    stm << L" 스킬 경험치:" << iSPExp_ << std::endl;

    WriteLogFile( stm.str() );
}

void KGCQALogManager::DungeonDropDiceLog( const std::wstring& strNick_, const DWORD dwDiceNumber_ )
{
    if( !m_bQALog ) { 
        return;
    }

    std::wstring strMsg = g_pkStrLoader->GetString( STR_ID_DICE_STRING0 ) + g_pkStrLoader->GetReplacedString( STR_ID_DICE_STRING1, "li", strNick_, dwDiceNumber_);
    strMsg += L"\n";

    WriteLogFile( strMsg );
}

void KGCQALogManager::DungeonDropGetLog( const std::wstring& strNick_, const GCItem* pItemInfo_, const std::wstring& strItemCharType_ )
{
    if( !m_bQALog ) { 
        return;
    }

    std::wostringstream stmItem;
    stmItem << pItemInfo_->strItemName << L"(" << pItemInfo_->dwGoodsID << L")" ;
    std::wstring strMsg = g_pkStrLoader->GetReplacedString( STR_ID_DROPITEM0, "lll" , strNick_, stmItem.str(), strItemCharType_ );
    strMsg += L"\n";
    WriteLogFile( strMsg );
}

void KGCQALogManager::RecvItemLog( const std::vector< std::pair< GCITEMUID, int > >& vecItem_ )
{
    if( !m_bQALog ) { 
        return;
    }

    WriteLogFile( L"\n 내가 획득한 아이템 ================================================\n" );

    std::wostringstream stm;
    std::vector< std::pair< GCITEMUID, int > >::const_iterator vecIter = vecItem_.begin();
    for( ; vecIter != vecItem_.end() ; ++vecIter ) {
        if( !g_MyD3D->m_kItemMgr.FindInventory( vecIter->first ) ) {
            continue;
        }

        KItem* pKitem = g_MyD3D->m_kItemMgr.GetInventoryItem( vecIter->first );
        if( pKitem != NULL ) {
            stm << L"(" << g_pItemMgr->GetItemName( pKitem->m_ItemID ) << L"(" << pKitem->m_ItemID << L"), " << vecIter->second << L")" << std::endl;

        }
    }

    WriteLogFile( stm.str() );
}

void KGCQALogManager::MonsterChangeHPLog( const std::wstring& strNick_, const std::wstring& strMonsterName_, const int iMonsterIndex_, const float fOldMonHP_, const float fNewMonHP_, const float fDamage_, const bool bPetDamage_, const bool bSpecial_, const bool bCritical_, const bool bSpear_ )
{
    if( !m_bQALog ) { 
        return;
    }

    std::wostringstream stm;
    stm.precision( 2 );

    if( !bPetDamage_ ) { 
        stm << L" ======= " << strNick_ << L" 이(가) " << strMonsterName_ << L"(" << iMonsterIndex_ << L")공격 ======= " << std::endl;
        stm << L"Monster HP :" << std::fixed << fOldMonHP_ << L"->" << std::fixed << fNewMonHP_ << std::endl;
        stm << L"Damage :" << std::fixed << fDamage_  << std::endl;
        stm << L"Special :" << ( bSpecial_ ? L"TRUE" : L"FALSE" ) << std::endl;
        stm << L"Critical : " << ( bCritical_ ? L"TRUE" : L"FALSE" ) << std::endl;
        stm << L"Spear : " << ( bSpear_ ? L"TRUE" : L"FALSE" ) << std::endl;
    }
    else { 
        stm << L" ======= " << strNick_ << L"의 펫이 " << strMonsterName_ << L"(" << iMonsterIndex_ << L")공격 ======= " << std::endl;
        stm << L"Monster HP :" << std::fixed << fOldMonHP_ << L"->" << std::fixed << fNewMonHP_ << std::endl;
        stm << L"Damage :" << std::fixed << fDamage_  << std::endl;
        stm << L"Critical : " << ( bCritical_ ? L"TRUE" : L"FALSE" ) << std::endl;
    }

     WriteLogFile( stm.str() );
}

void KGCQALogManager::PlayerChangeHPLog( const std::wstring& strNick_, const PLAYER::DamageInfo& damage_, const int iAttackerIndex_, const float fHP_, const float fDamage_, const bool bSpecial_, const bool bCritical_, const bool bSpear_ )
{
    if( !m_bQALog ) { 
        return;
    }

    std::wostringstream stm;
    stm.precision( 2 );

    std::wostringstream stmAttacker;
    if( damage_.IsPlayerDamage() ) { 
        g_MyD3D->MyPlayer[ iAttackerIndex_ ]->m_kUserInfo.strNickName;
    }
    else if ( damage_.IsPetDamage() ) { 
        stmAttacker << g_MyD3D->MyPlayer[ iAttackerIndex_ ]->m_kUserInfo.strNickName << L"의 펫";
    }
    else { 
        stmAttacker << g_kMonsterManager.GetMonster( iAttackerIndex_ )->GetMonsterName();
    }

    stm << L" ======= " << strNick_ << L" 이(가) " << stmAttacker.str() << L"(" << iAttackerIndex_ << L") 에게 공격 받음 ======= " << std::endl;
    stm << L"HP : " << std::fixed << fHP_ << L"->" << std::fixed << fHP_ + fDamage_ << std::endl;
    stm << L"Damage : " << std::fixed << fDamage_ << std::endl;
    stm << L"Special : " << ( bSpecial_ ? L"TRUE" : L"FALSE" ) << std::endl;
    stm << L"Critical : " << ( bCritical_ ? L"TRUE" : L"FALSE" ) << std::endl; 
    stm << L"Spear : " << ( bSpear_ ? L"TRUE" : L"FALSE" ) << std::endl;

    WriteLogFile( stm.str() );
}

