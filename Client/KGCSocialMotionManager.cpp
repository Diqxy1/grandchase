#include "stdafx.h"
#include "Procedure.h"
#include "Message.h"
#include "GCStateGame.h"
#include "GCRenderManager.h"
#include "KSingleton.h"
#include "KGCRoomManager.h"
#include "KGCChannelManager.h"
#include "./Square/GCSquare.h"
#include "./Square/GCSquarePeople.h"

#include "KGCSocialMotionManager.h"

ImplementSingleton( KGCSocialMotionManager );

KGCSocialMotionManager::KGCSocialMotionManager( void )
{
    m_mapSocilMotionInfo.clear();
    m_mapOtherMotionInfo.clear();
    
    m_mapEmoticonMotion.clear();
    m_vecEmoticonMotion.clear();
}

KGCSocialMotionManager::~KGCSocialMotionManager( void )
{
    m_mapSocilMotionInfo.clear();
    m_mapOtherMotionInfo.clear();
    
    m_mapEmoticonMotion.clear();
    m_vecEmoticonMotion.clear();
}

bool KGCSocialMotionManager::LoadSocialMotionInfo( const char* szFileName_ )
{
    m_mapSocilMotionInfo.clear();
    m_mapOtherMotionInfo.clear();
    m_mapEmoticonMotion.clear();
    m_vecEmoticonMotion.clear();

    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, szFileName_ ) == false )
        return false;

    // 소셜 모션 - 아지트 or  코디 
    if( SUCCEEDED( luaMgr.BeginTable( "SOCIAL_MOTION_LIST" ) ) )
    {
        int iMotionIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iMotionIndex++ ) ) ) 
                break;

            std::string strCommand;
            KSosicalMotionInfo tempInfo;
            bool bIsSocialMotion = true;

            luaMgr.GetValue( "Command",  strCommand );
            luaMgr.GetValue( "UseSpace", tempInfo.bUseSpace );

            if( SUCCEEDED( luaMgr.BeginTable( "MotionID" ) ) )
            {
                LUA_GET_VALUE_DEF( 1, tempInfo.prMotionID.first,  -1 );
                LUA_GET_VALUE_DEF( 2, tempInfo.prMotionID.second, -1 );
                luaMgr.EndTable();
            }

            LUA_GET_VALUE_DEF( "IsSocialMotion", bIsSocialMotion, true );
            if( bIsSocialMotion ) { 
                m_mapSocilMotionInfo[ strCommand ] = tempInfo;
            }
            else {
                m_mapOtherMotionInfo[ strCommand ] = tempInfo;
            }

            luaMgr.EndTable(); 
        }
    }
    luaMgr.EndTable();

    // 이모티콘 모션 
    if( SUCCEEDED( luaMgr.BeginTable( "EMOTICON_MOTION_LIST" ) ) ) { 
        int iInidex = 1;
        while( true ) {
            if( FAILED( luaMgr.BeginTable( iInidex++ ) ) ) { 
                break;
            }

            int iItemID;
            std::vector< std::vector< int > > vecList;
            luaMgr.GetValue( "ItemID", iItemID );

            if( SUCCEEDED( luaMgr.BeginTable( "MotionList" ) ) ) { 
                int iListIndex = 1;
                while( true ) { 
                    if( FAILED( luaMgr.BeginTable( iListIndex++ ) ) ) { 
                        break;
                    }

                    int iMotionIndex = 1;
                    std::vector< int > vecMotion;
                    while( true ) { 
                        int iMotion;
                        LUA_GET_VALUE_NOASSERT( iMotionIndex++, iMotion, break );
                        vecMotion.push_back( iMotion );
                    }
                    luaMgr.EndTable();
                    vecList.push_back( vecMotion );
                }
                luaMgr.EndTable();
            }
            m_mapEmoticonMotion.insert( std::make_pair( static_cast< GCITEMID >( iItemID ), vecList ) );
            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

    SetEmoticonMotionList();
    return true;
}

void KGCSocialMotionManager::SetMotion( const std::wstring& strCommand_ )
{    
    bool bIsSocialMotion = true;
    std::pair<int, int> prMotion = std::make_pair(-1,-1);
    std::string strConvert;
    strConvert = GCUTIL_STR::GCStrWideToChar(strCommand_.c_str());

    std::map< std::string, KSosicalMotionInfo >::iterator mit = m_mapSocilMotionInfo.find( strConvert );
    if( mit != m_mapSocilMotionInfo.end() ) { 
        prMotion = mit->second.prMotionID;
        bIsSocialMotion = true;


        if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME || 
            SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT ) { 
                return;
        }
    }

    mit = m_mapOtherMotionInfo.find( strConvert );
    if( mit != m_mapOtherMotionInfo.end() ) {
        prMotion = mit->second.prMotionID;
        bIsSocialMotion = false;

        if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME && 
            g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE ) { 
                return;
        }
    }


    if( prMotion == std::make_pair(-1,-1) ) {
        return;
    }
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE ) {
        GCSquarePeople* pPeople = SiGCSquare()->GetSquarePeople( g_kGlobalValue.m_kUserInfo.dwUID );
        if( pPeople ) {
            if( bIsSocialMotion == false && pPeople->IsEquipCoordiSetItem() == false ) {
                return;
            }
            pPeople->SetPeopleState( GCSquarePeople::SOCIAL_PEOPLE );
        }
    }
    else {
        int iPlayerIndex = g_MyD3D->Get_MyPlayer();
        if( false == g_MyD3D->IsPlayerIndex( iPlayerIndex ) ) {
            return;
        }


        if( false == g_MyD3D->MyPlayer[iPlayerIndex]->IsWaitMotion() )
            return;

        if( bIsSocialMotion == false && g_MyD3D->MyPlayer[iPlayerIndex]->IsEquipCoordiSetItem() == false ) {
            return;
        }


        int iCharSex = g_kGlobalValue.IsSexCharacter( g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar().iCharType );
        int iMotionID = iCharSex == SEX_MAN ? prMotion.first : prMotion.second;
        g_MyD3D->MyPlayer[iPlayerIndex]->SetFrameIndex(0);
        g_MyD3D->MyPlayer[iPlayerIndex]->SetPlayerState( iMotionID );
    }
}

void KGCSocialMotionManager::SetMotion( const int iMotionID_ )
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME || 
        SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT ) { 
            return;
    }

    if( iMotionID_ < 0 || g_pGraphicsHelper->GetPlayerMotionNum() <= iMotionID_ ) { 
        return;
    }

    int iPlayerIndex = g_MyD3D->Get_MyPlayer();
    if( false == g_MyD3D->IsPlayerIndex( iPlayerIndex ) ) {
        return;
    }

    // Wait 상태 아니면 무시 
    if( false == g_MyD3D->MyPlayer[iPlayerIndex]->IsWaitMotion() ) {
        return;
    }

    for( std::map< std::string, KSosicalMotionInfo >::iterator mit = m_mapSocilMotionInfo.begin() ; mit != m_mapSocilMotionInfo.end() ; ++mit ) { 
        if( iMotionID_ == mit->second.prMotionID.first || iMotionID_ == mit->second.prMotionID.second ) { 
            g_MyD3D->MyPlayer[ iPlayerIndex ]->SetIsRight( true );
            int iCharSex = g_kGlobalValue.IsSexCharacter( g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar().iCharType );
            int iMotionID = iCharSex == SEX_MAN ? mit->second.prMotionID.first : mit->second.prMotionID.second;

            g_MyD3D->MyPlayer[iPlayerIndex]->SetFrameIndex(0);
            g_MyD3D->MyPlayer[iPlayerIndex]->SetPlayerState( iMotionID );
            break;
        }
    }
}

void KGCSocialMotionManager::SetMotion( const GCITEMID itemID_, const int iPlayerIndex_ /*= -1*/ )
{
    // 게임에서만 
    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME ) { 
        return;
    }

    int iPlayerIndex = -1;
    if( iPlayerIndex_ != -1 ) { 
        iPlayerIndex = iPlayerIndex_;
    }
    else { 
        iPlayerIndex = g_MyD3D->Get_MyPlayer();
    }

    if( false == g_MyD3D->IsPlayerIndex( iPlayerIndex ) ) {
        return;
    }

    // Wait 상태 아니면 무시 
    if( false == g_MyD3D->MyPlayer[iPlayerIndex]->IsWaitMotion() || g_MyD3D->MyPlayer[iPlayerIndex]->MetamorphosisForm != FORM_NORMAL ) {
        return;
    }

    std::map< GCITEMID, std::vector< std::vector< int > > >::const_iterator mit = m_mapEmoticonMotion.find( itemID_ );
    if( mit == m_mapEmoticonMotion.end() || mit->second.empty() ) { 
        return;
    }

    int iMotionSize = static_cast<int>( mit->second.size() );
    srand( time( NULL ) );
    int iSelMotion = rand() % iMotionSize;

    std::vector< int > vecMotion = mit->second[ iSelMotion ];
    std::vector< int >::const_iterator vitMotion = vecMotion.begin();

    GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerActionMotion( *vitMotion );

    g_MyD3D->MyPlayer[ iPlayerIndex ]->SetFrameIndex( 0 );
    g_MyD3D->MyPlayer[ iPlayerIndex ]->SetPlayerState( *vitMotion );
}

bool KGCSocialMotionManager::IsSocialMotionCommand( const std::wstring& strMsg_, bool bIsCoordiMotion_ /* = false */ )
{
    if( strMsg_.empty() ) {
        return false;
    }

    if( strMsg_[0] != L'/' ) {
        return false;
    }

    bool bResult;
    std::map< std::string, KSosicalMotionInfo >::iterator mit;
    std::wstring strCommand = strMsg_.substr( 1 );
    std::string strConvert =  GCUTIL_STR::GCStrWideToChar( strCommand.c_str() );

    if( false == bIsCoordiMotion_ ) { 
        mit = m_mapSocilMotionInfo.find( strConvert );
    if( mit == m_mapSocilMotionInfo.end() ) { 
            bResult = false;
        } else if ( mit->second.bUseSpace == true ) {
            bResult = false;
    }
        else { 
            bResult = true;
        }
    }
    else {
        mit = m_mapOtherMotionInfo.find( strConvert );
        if( mit == m_mapOtherMotionInfo.end() ) { 
            bResult = false;
        } else if( mit->second.bUseSpace == true ) {
            bResult = false;
        }
        else { 
            bResult = false;
        }
    }

    return bResult;
}

bool KGCSocialMotionManager::GetSocialMotion( std::set<int>& setMotionList_ )
{
    bool bResult = false;
    std::map< std::string, KSosicalMotionInfo >::iterator mit = m_mapSocilMotionInfo.begin();
    for( mit; mit != m_mapSocilMotionInfo.end(); ++mit ) {
        setMotionList_.insert(mit->second.prMotionID.first);
        setMotionList_.insert(mit->second.prMotionID.second);
        bResult = true;
    }

    mit = m_mapOtherMotionInfo.begin();
    for( mit; mit != m_mapOtherMotionInfo.end(); ++mit ) {
        setMotionList_.insert(mit->second.prMotionID.first);
        setMotionList_.insert(mit->second.prMotionID.second);
        bResult = true;
    }

    return bResult;
}

void KGCSocialMotionManager::SetEmoticonMotionList( void )
{
    m_vecEmoticonMotion.clear();
    std::map< GCITEMID, std::vector< std::vector< int > > >::const_iterator mit = m_mapEmoticonMotion.begin();
    for( ; mit != m_mapEmoticonMotion.end() ; ++mit ) { 
        std::vector< std::vector< int > >::const_iterator vitList = mit->second.begin();
        for( ; vitList != mit->second.end() ; ++vitList ) { 
            std::vector< int >::const_iterator vitMotion = vitList->begin();
            for( ; vitMotion != vitList->end() ; ++vitMotion ) { 
                m_vecEmoticonMotion.push_back( *vitMotion );
            }
        }
    }
}

void KGCSocialMotionManager::GetEmoticonMotionList( OUT std::vector< int >& vecList_ )
{
    vecList_.clear();
    vecList_ = m_vecEmoticonMotion;
}

