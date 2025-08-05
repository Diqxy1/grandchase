#include "stdafx.h"

#include ".\gcctmissionmanager.h"



ImplementSingleton(KGCCTMissionManager);

KGCCTMissionManager::KGCCTMissionManager(void)
{
}

KGCCTMissionManager::~KGCCTMissionManager(void)
{
    m_kStringMgr.Clear();
    Reset();
}

void KGCCTMissionManager::Reset()
{
    m_mapCondition.clear();
    m_mapSubMission.clear();
    m_mapMissionList.clear();
    m_setUniqueMission.clear();
}

bool KGCCTMissionManager::LoadMissionFile( std::string strMissionScriptFile , KLuaManager* pkLuaMgr )
{
    boost::shared_ptr<KLuaManager> pkTempLuaMgr( new KLuaManager );

    if( pkLuaMgr != NULL )
        pkTempLuaMgr.reset( pkLuaMgr );

    KLuaManager& luaMgr = *pkTempLuaMgr.get();

    if( pkLuaMgr == NULL )
    {
        if( FAILED( luaMgr.DoFile( strMissionScriptFile.c_str() ) ) )
            return false;
    }

    Reset();

    if( SUCCEEDED( luaMgr.BeginTable( "Condition" ) ) )
    {
        for( int i = 1 ; ; ++i )
        {
            LUA_BEGIN_TABLE( i, break )
            {
                SConditionInfo sCondition;
                if( FAILED( luaMgr.GetValue( "ID", sCondition.m_dwID ) ) )
                {
                    ////OKBOX( "Condition Table Error : "<<i , "Error");
                    luaMgr.EndTable();
                    break;
                }
                LUA_GET_VALUE_DEF( "MonsterKill", sCondition.m_iMonsterID, -1 );
                LUA_GET_VALUE_DEF( "Mode", sCondition.m_iGameMode, -1 );
                LUA_GET_VALUE_DEF( "Stage", sCondition.m_iStage, -1 );
                if( SUCCEEDED( luaMgr.BeginTable( "DifficultRange", false ) ) )
                {
                    int iBegin, iEnd;
                    LUA_GET_VALUE_DEF( 1, iBegin, -1 );
                    LUA_GET_VALUE_DEF( 2, iEnd, -1 );
                    sCondition.m_pairDifficultRange = std::make_pair( iBegin, iEnd );
                    luaMgr.EndTable( false );
                }
                else
                {
                    sCondition.m_pairDifficultRange = std::make_pair( -1, -1 );
                }

                if( SUCCEEDED( luaMgr.BeginTable( "ScoreRange", false ) ) )
                {
                    int iBegin, iEnd;
                    LUA_GET_VALUE_DEF( 1, iBegin, -1 );
                    LUA_GET_VALUE_DEF( 2, iEnd, -1 );
                    sCondition.m_pairScoreRange = std::make_pair( iBegin, iEnd );
                    luaMgr.EndTable( false );
                }
                else
                {
                    sCondition.m_pairScoreRange = std::make_pair( -1, -1 );
                }
                LUA_GET_VALUE_DEF( "MapID", sCondition.m_iMapID , -1 );
                LUA_GET_VALUE_DEF( "Win", sCondition.m_bWin, true );
                LUA_GET_VALUE_DEF( "Solo", sCondition.m_nSolo, 0 );
                LUA_GET_VALUE_DEF( "Time", sCondition.m_iTime, -1 );
                LUA_GET_VALUE_DEF( "Kill", sCondition.m_iKill, -1 );
                LUA_GET_VALUE_DEF( "Die", sCondition.m_iDie, -1 );
                LUA_GET_VALUE_DEF( "ExLife", sCondition.m_iExLife, -1 );
                LUA_GET_VALUE_DEF( "ConditionActiveRatio", sCondition.m_fConditionRatio, 1.0f );
                KGCCondition kCondition;
                kCondition.SetConditionInfo( sCondition );
                m_mapCondition.insert( ConditionList::value_type( sCondition.m_dwID, kCondition ) );
            }
            LUA_END_TABLE( break );
        }
        luaMgr.EndTable();
    }
    else
        return false;

    if( SUCCEEDED( luaMgr.BeginTable( "Submission" ) ) )
    {
        for( int i = 1 ; ; ++i )
        {
            LUA_BEGIN_TABLE( i, break )
            {
                SSubMissionInfo sSubMissionInfo;
                if( FAILED( luaMgr.GetValue( "SubmissionID", sSubMissionInfo.m_iSubMissionID ) ) )
                {
                    ////OKBOX( "SubMission Table Error : "<<i , "Error" );
                    luaMgr.EndTable();
                    break;
                }
                LUA_GET_VALUE_DEF( "CompletionTitleID", sSubMissionInfo.m_iTitleID, -1 );
                LUA_GET_VALUE_DEF( "CompletionDescID", sSubMissionInfo.m_iDescID, -1 );
                if( SUCCEEDED( luaMgr.BeginTable( "Condition" ) ) )
                {
                    for( int i = 1; ; ++i )
                    {
                        int iConditionID;
                        LUA_GET_VALUE_NOASSERT( i, iConditionID, break );
                        sSubMissionInfo.m_vecConditionID.push_back( iConditionID );
                    }
                    luaMgr.EndTable();
                }

                if( false )//sSubMissionInfo.m_vecConditionID.empty() == true )
                {
                    ////OKBOX( "No Condition Error : "<<sSubMissionInfo.m_iSubMissionID , "Error" );
                    luaMgr.EndTable();
                    break;
                }

                if( SUCCEEDED( luaMgr.BeginTable( "Action" ) ) )
                {
                    LUA_GET_VALUE_DEF( "bMonDrop", sSubMissionInfo.m_sAction.m_bMonDrop, false );
                    LUA_GET_VALUE_DEF( "Item", sSubMissionInfo.m_sAction.m_dwItemID, -1 );
                    LUA_GET_VALUE_DEF( "Count", sSubMissionInfo.m_sAction.m_iCount, 0 );
                    luaMgr.EndTable();
                }
                else
                {
                    //OKBOX( "No Action Error : "<<sSubMissionInfo.m_iSubMissionID, "Error" );
                    luaMgr.EndTable();
                    break;
                }

                if( SUCCEEDED( luaMgr.BeginTable( "Completion" ) ) )
                {
                    LUA_GET_VALUE_DEF( "ItemID", sSubMissionInfo.m_sCompletion.m_dwItemID, -1 );
                    if( FAILED( luaMgr.GetValue( "Count", sSubMissionInfo.m_sCompletion.m_iCount ) ) )
                    {
                        //OKBOX( "No Completion Count Error : "<<sSubMissionInfo.m_iSubMissionID, "Error" );
                    }
                    LUA_GET_VALUE_DEF( "CharacterLv", sSubMissionInfo.m_sCompletion.m_iCharacterLv, -1 );
                    luaMgr.EndTable();
                }
                else
                {
                    //OKBOX( "No Completion Error : "<<sSubMissionInfo.m_iSubMissionID, "Error" );
                    luaMgr.EndTable();
                    break;
                }
                KGCSubMission kSubMission;
                kSubMission.SetSubMissionInfo( sSubMissionInfo );
                m_mapSubMission.insert( SubMissionList::value_type( sSubMissionInfo.m_iSubMissionID, kSubMission ) );
            }
            LUA_END_TABLE( break )
        }
        luaMgr.EndTable();
    }
    else
    {
        ////OKBOX( "Submission read Failed!!!" , "Error");
        return false;
    }

    LUA_BEGIN_TABLE( "Mission", return false )
    {
        for( int i = 1 ; ; ++i )
        {
            SMissionInfo sMissionInfo;
            LUA_BEGIN_TABLE( i, break )
            {
                LUA_GET_VALUE_DEF( "ID", sMissionInfo.m_dwID, -1 );
                LUA_GET_VALUE_DEF( "TitleID", sMissionInfo.m_iTitleID, -1 );
                LUA_GET_VALUE_DEF( "MissionDescID", sMissionInfo.m_iDescID, -1 );
                LUA_GET_VALUE_DEF( "CharType", sMissionInfo.m_iCharacter, -1 );
                LUA_GET_VALUE_DEF( "Promotion", sMissionInfo.m_iPromotion, -1 );

                if( SUCCEEDED( luaMgr.BeginTable( "LevelRange", false ) ) )
                {
                    int iBegin, iEnd;
                    LUA_GET_VALUE_DEF( 1, iBegin, -1 );
                    LUA_GET_VALUE_DEF( 2, iEnd, -1 );
                    sMissionInfo.m_pairLevelLimit = std::make_pair( iBegin, iEnd );
                    luaMgr.EndTable( false );
                }
                else
                {
                    sMissionInfo.m_pairLevelLimit = std::make_pair( -1, -1 );
                }

                if( SUCCEEDED( luaMgr.BeginTable( "InvalidMission" ) ) )
                {
                    for( int iIMCount = 1; ; ++iIMCount )
                    {
                        int iMissionID;
                        LUA_GET_VALUE_NOASSERT( iIMCount, iMissionID, break );
                        sMissionInfo.m_vecInvalidMissions.push_back( iMissionID );
                    }
                    luaMgr.EndTable();
                }
                if( SUCCEEDED( luaMgr.BeginTable( "InvalidCompleteMission" ) ) )
                {
                    for( int iICMCount = 1; ; ++iICMCount )
                    {
                        int iMissionID;
                        LUA_GET_VALUE_NOASSERT( iICMCount, iMissionID, break );
                        sMissionInfo.m_vecInvalidCompletMissions.push_back( iMissionID );
                    }
                    luaMgr.EndTable();
                }
                LUA_GET_VALUE_DEF( "Unique", sMissionInfo.m_bUnique, false );
                if( sMissionInfo.m_bUnique == true )
                {
                    m_setUniqueMission.insert( sMissionInfo.m_dwID );
                }
                LUA_GET_VALUE_DEF( "Period", sMissionInfo.m_iPeriod, -1 );

                if( SUCCEEDED( luaMgr.BeginTable( "Submissions" ) ) )
                {
                    for( int iSMCount = 1; ; ++iSMCount )
                    {
                        int iMissionID;
                        LUA_GET_VALUE_NOASSERT( iSMCount, iMissionID, break );
                        sMissionInfo.m_vecSubMissions.push_back( iMissionID );
                    }
                    luaMgr.EndTable();
                }

                LUA_GET_VALUE_DEF( "NewChar", sMissionInfo.m_iNewCharacter, -1 );
                LUA_GET_VALUE_DEF( "ChangeJob", sMissionInfo.m_iChangeJob, -1 );
                LUA_GET_VALUE_DEF( "GP", sMissionInfo.m_iGP, 0 );
                LUA_GET_VALUE_DEF( "EXP", sMissionInfo.m_iExp, 0 );

                if( SUCCEEDED( luaMgr.BeginTable( "RewardItem" ) ) )
                {
                    for( int iRICount = 1; ; ++iRICount )
                    {
                        if( FAILED( luaMgr.BeginTable( iRICount ) ) )
                            break;
                        int iItemID, iCount;

                        LUA_GET_VALUE_NOASSERT( 1, iItemID, continue );
                        LUA_GET_VALUE_NOASSERT( 2, iCount, continue );

                        sMissionInfo.m_vecRewardItem.push_back( SMissionInfo::SReward( iItemID, iCount, -1, -1 ) );
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();
                }

                if( SUCCEEDED( luaMgr.BeginTable( "SelectItem" ) ) )
                {
                    for( int iSICount = 1; ; ++iSICount )
                    {
                        if( FAILED( luaMgr.BeginTable( iSICount ) ) )
                            break;
                        int iItemID, iCount;
                        LUA_GET_VALUE_NOASSERT( 1, iItemID, continue );
                        LUA_GET_VALUE_NOASSERT( 2, iCount, continue );

                        sMissionInfo.m_vecSelectItem.push_back( SMissionInfo::SReward( iItemID, iCount, -1, -1 ) );
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();
                }

                KGCMission kMission;
                kMission.SetMissionInfo( sMissionInfo );
                m_mapMissionList.insert( MissionList::value_type( sMissionInfo.m_dwID, kMission ) );
            }
            LUA_END_TABLE( break )
        }
    }
    LUA_END_TABLE( return false )
        return true;
}

//bool KGCCTMissionManager::OutPutMissionFile( std::string strMissionScriptFile )
//{
//    FILE* pFile = _tfopen( strMissionScriptFile.c_str(), "wc" );
//    if( pFile == NULL )
//    {
//        return false;
//    }
//
//    //기본 선언을 적어 넣습니다.
//    fprintf( pFile, "TRUE = 1;\n" );
//    fprintf( pFile, "FALSE = 0;\n" );
//
//    //Condition들을 적어 넣읍시다.
//    fprintf( pFile , "Condition = {\n" );
//    {
//        std::map< int, KGCCondition >::iterator mit;
//        for( mit = m_mapCondition.begin(); mit != m_mapCondition.end() ; ++mit )
//        {
//            const SConditionInfo& sInfo = mit->second.GetConditionInfo();
//            fprintf( pFile, "\t{\n" );
//            fprintf( pFile, "\t\tID = %d,\n", sInfo.m_dwID );
//            fprintf( pFile, "\t\tMonsterKill = %d,\n", sInfo.m_iMonsterID );
//            fprintf( pFile, "\t\tMode = %d,\n", sInfo.m_iGameMode );
//            fprintf( pFile, "\t\tStage = %d,\n", sInfo.m_iStage );
//            fprintf( pFile, "\t\tDifficultRange = {%d,%d},\n", sInfo.m_pairDifficultRange.first, sInfo.m_pairDifficultRange.second );
//            fprintf( pFile, "\t\tMapID = %d,\n", sInfo.m_iMapID );
//            fprintf( pFile, "\t\tScoreRange = {%d,%d},\n", sInfo.m_pairScoreRange.first, sInfo.m_pairScoreRange.second );
//            fprintf( pFile, "\t\tWin = %d,\n", sInfo.m_bWin );
//            fprintf( pFile, "\t\tTime = %d,\n", sInfo.m_iTime );
//            fprintf( pFile, "\t\tKill = %d,\n", sInfo.m_iKill );
//            fprintf( pFile, "\t\tDie = %d,\n", sInfo.m_iDie );
//            fprintf( pFile, "\t\tExLife = %d,\n", sInfo.m_iExLife );
//            fprintf( pFile, "\t\tIsSolo = %s", (sInfo.m_bSolo? "TRUE" : "FALSE" ) );
//            fprintf( pFile, "\t\tConditionActiveRatio = %f,\n", sInfo.m_fConditionRatio );
//            fprintf( pFile, "\t},\n" );
//        }
//
//    }    
//    fprintf( pFile , "}\n" );
//
//    //SubMission들을 적어 넣습니다.
//    fprintf( pFile , "Submission = {\n" );
//    {
//        std::map< int, KGCSubMission >::iterator mit;
//        for( mit = m_mapSubMission.begin() ; mit != m_mapSubMission.end() ; ++mit )
//        {
//            const SSubMissionInfo& sInfo = *mit->second.GetSubMissionInfo();
//            fprintf( pFile, "\t{\n" );
//
//            fprintf( pFile, "\t\tSubmissionID = %d,\n", sInfo.m_iSubMissionID );
//            fprintf( pFile, "\t\tCompletionTitleID = %d,\n", sInfo.m_iTitleID );
//            fprintf( pFile, "\t\tCompletionDescID = %d,\n", sInfo.m_iDescID );
//            fprintf( pFile, "\t\tCondition = {" );
//            for( int i = 0 ; i < (int)sInfo.m_vecConditionID.size() ; ++i )
//            {
//                fprintf( pFile, "%d,", sInfo.m_vecConditionID[i] );
//            }
//            fprintf( pFile, "},\n" );
//            fprintf( pFile, "\t\tAction = {\n" );
//            fprintf( pFile, "\t\t\tbMonDrop = %s,\n", (sInfo.m_sAction.m_bMonDrop?"TRUE":"FALSE" ) );
//            fprintf( pFile, "\t\t\tItem = %d,\n", sInfo.m_sAction.m_dwItemID );
//            fprintf( pFile, "\t\t\tCount = %d,\n", sInfo.m_sAction.m_iCount );
//            fprintf( pFile, "\t\t},\n" );
//
//            fprintf( pFile, "\t\tCompletion = {\n" );
//            fprintf( pFile, "\t\t\tItemID = %d,\n", sInfo.m_sCompletion.m_dwItemID );
//            fprintf( pFile, "\t\t\tCount = %d,\n", sInfo.m_sCompletion.m_iCount );
//            fprintf( pFile, "\t\t},\n" );
//
//            fprintf( pFile, "\t},\n" );
//        }
//    }    
//    fprintf( pFile , "}\n" );
//
//    std::stringstream stm;
//    //Mission 들을 적어 넣습니다.
//    fprintf( pFile , "Mission = {\n" );
//    {
//        std::map< DWORD,  KGCMission >::iterator mit;
//        for( mit = m_mapMissionList.begin(); mit != m_mapMissionList.end() ; ++mit )
//        {
//            const SMissionInfo& sInfo = *mit->second.GetMissionInfo();
//            fprintf( pFile, "\t{\n" );
//            //char strMisssionItemName[MAX_PATH];
//            //WCHAR strItemName[MAX_PATH];            
//            //wsprintf( strItemName, L"%s", SiKGCMissionInSort()->GetMissionItem( sInfo.m_dwID )->szItemName);
//            //W2T2(strMisssionItemName, strItemName );
//            //::fwprintf( pFile, L"\t\tID = %d,\t--%s\n", sInfo.m_dwID, SiKGCMissionInSort()->GetMissionItem( sInfo.m_dwID )->szItemName );
//            fprintf( pFile, "\t\tID = %d,\n" , sInfo.m_dwID );
//            fprintf( pFile, "\t\t--미션에 사용되는 제목, 설명\n" );
//            fprintf( pFile, "\t\tTitleID = %d,\n", sInfo.m_iTitleID );
//            fprintf( pFile, "\t\tMissionDescID = %d,\n", sInfo.m_iDescID );
//            fprintf( pFile, "\t\t--등록조건\n" );
//            fprintf( pFile, "\t\tCharType = %d,\n", sInfo.m_iCharacter );
//            fprintf( pFile, "\t\tLevelRange = {%d,%d},\n", sInfo.m_pairLevelLimit.first, sInfo.m_pairLevelLimit.second );
//            fprintf( pFile, "\t\tPromotion = %d,\n", sInfo.m_iPromotion );
//            fprintf( pFile, "\t\tInvalidMission = {" );
//            for( int i = 0 ; i < (int)sInfo.m_vecInvalidMissions.size() ; ++i )
//            {
//                fprintf( pFile, "%d,", sInfo.m_vecInvalidMissions[i] );
//            }
//            fprintf( pFile, "},\n" );
//            fprintf( pFile, "\t\tInvalidCompleteMission = {" );
//            for( int i = 0 ; i < (int)sInfo.m_vecInvalidCompletMissions.size() ; ++i )
//            {
//                fprintf( pFile, "%d,", sInfo.m_vecInvalidCompletMissions[i] );
//            }
//            fprintf( pFile, "},\n" );
//            fprintf( pFile, "\t\tUnique = %s,\n", (IsUniqueMission( sInfo.m_dwID )?"TRUE":"FALSE" ) );
//            fprintf( pFile, "\t\t--미션 조건\n" );
//            fprintf( pFile, "\t\tPeriod = %d,\n", sInfo.m_iPeriod );
//            fprintf( pFile, "\t\tSubmissions = {" );
//            for( int i = 0 ; i < (int)sInfo.m_vecSubMissions.size(); ++i )
//            {
//                fprintf( pFile, "%d,", sInfo.m_vecSubMissions[i] );
//            }
//            fprintf( pFile, "},\n" );
//            fprintf( pFile, "\t\t--보상\n" );
//            fprintf( pFile, "\t\tNewChar = %d,\n", sInfo.m_iNewCharacter );
//            fprintf( pFile, "\t\tChangeJob = %d,\n", sInfo.m_iChangeJob );
//            fprintf( pFile, "\t\tGP = %d,\n", sInfo.m_iGP );
//            fprintf( pFile, "\t\tEXP = %d,\n", sInfo.m_iExp );
//            fprintf( pFile, "\t\tRewardItem = {" );
//            for( int i = 0 ; i < (int)sInfo.m_vecRewardItem.size() ; ++i )
//            {
//                fprintf( pFile, "{%d,%d},", sInfo.m_vecRewardItem[i].first, sInfo.m_vecRewardItem[i].second );
//            }
//            fprintf( pFile, "},\n" );
//            fprintf( pFile, "\t\tSelectItem = {" );
//            for( int i = 0 ; i < (int)sInfo.m_vecSelectItem.size() ; ++i )
//            {
//                fprintf( pFile, "{%d,%d},", sInfo.m_vecSelectItem[i].first, sInfo.m_vecSelectItem[i].second );
//            }
//            fprintf( pFile, "},\n" );
//
//            fprintf( pFile, "\t},\n" );
//        }
//    }    
//    fprintf( pFile , "}\n" );
//
//    fclose( pFile );
//
//    return true;
//}

bool KGCCTMissionManager::AddMissionInfo( const SMissionInfo& sInfo )
{
    KGCMission kMission;
    kMission.SetMissionInfo( sInfo );
    return m_mapMissionList.insert( std::make_pair( sInfo.m_dwID, kMission ) ).second;
}

int KGCCTMissionManager::AddSubMissionInfo( const SSubMissionInfo& sInfo )
{
    int iLastIndex = 0;
    std::map< int, KGCSubMission >::iterator mit;
    for( mit = m_mapSubMission.begin() ; mit != m_mapSubMission.end() ; ++mit )
    {
        if( mit->second == sInfo )
        {
            return mit->second.GetSubMissionID();
        }

        if( iLastIndex <= mit->first )
            iLastIndex = mit->first + 1;
    }


    SSubMissionInfo sSubMissionInfo = sInfo;
    sSubMissionInfo.m_iSubMissionID = iLastIndex;
    KGCSubMission kSubMission;
    kSubMission.SetSubMissionInfo( sSubMissionInfo );
    m_mapSubMission.insert( std::make_pair( sSubMissionInfo.m_iSubMissionID, kSubMission ) );
    return sSubMissionInfo.m_iSubMissionID;
}

int KGCCTMissionManager::AddConditionInfo( const SConditionInfo& sInfo )
{
    int iLastIndex = 0;
    std::map< int, KGCCondition > ::iterator mit;
    for( mit = m_mapCondition.begin() ; mit != m_mapCondition.end() ; ++mit )
    {
        if( mit->second == sInfo )
        {
            return mit->second.GetConditionInfo().m_dwID;
        }

        if( iLastIndex <= mit->first )
            iLastIndex = mit->first + 1;
    }

    SConditionInfo sConditionInfo = sInfo;
    sConditionInfo.m_dwID = iLastIndex;
    KGCCondition kCondition;
    kCondition.SetConditionInfo( sConditionInfo );
    m_mapCondition.insert( std::make_pair( sConditionInfo.m_dwID, kCondition ) );
    return sConditionInfo.m_dwID;
}

int KGCCTMissionManager::CheckCompleteMission( DWORD dwMissionID, std::vector< std::pair< int, int > > vecCompletion )
{
    if( IsMissionExist( dwMissionID ) == false )
        return GC_MM_MISSION_NOT_EXIST;

    for( int i = 0 ; i < (int)vecCompletion.size() ; ++i )
    {
		std::map< int, KGCSubMission >::iterator mit = m_mapSubMission.find( vecCompletion[i].first );
        if( mit == m_mapSubMission.end() )
            return GC_MM_SUBMISSION_NOT_EXIST;

        if( mit->second.IsComplete( vecCompletion[i].second ) == false)
            return GC_MM_NOT_ENOUGH;
    }

    return GC_MM_OK;
}

int KGCCTMissionManager::IsAvailableRegister( DWORD dwMissionID, int iChar, int iLevel, int iPromotion )
{
    const SMissionInfo* sInfo = GetMissionInfo( dwMissionID );

    //Character Check
    if( sInfo->m_iCharacter != -1 && sInfo->m_iCharacter != iChar )
        return GC_MM_DIFFERENT_CHAR;

    //Level Check
    if( (sInfo->m_pairLevelLimit.first != -1 && sInfo->m_pairLevelLimit.first > iLevel ) ||
        (sInfo->m_pairLevelLimit.second != -1 && sInfo->m_pairLevelLimit.second < iLevel ) )
        return GC_MM_NOT_ENOUGH_LEVEL;

    //Promotion Check
    if( sInfo->m_iPromotion != -1 &&
        sInfo->m_iPromotion > iPromotion )
        return GC_MM_NOT_ENOUGH_PROMOTION;

    return GC_MM_OK;
}

int KGCCTMissionManager::AddString( std::wstring str )
{
    return m_kStringMgr.AddMissionString( str );
}

void KGCCTMissionManager::EraseString( int iID )
{    
}

std::wstring KGCCTMissionManager::GetString ( int iID )
{
    std::wstring strMissionString;
    m_kStringMgr.GetMissionString( iID , strMissionString );
    return strMissionString;
}

bool KGCCTMissionManager::LoadMissionString( std::string strFileName , FILE* pFile /*= NULL */, bool bForTool /*= false */)
{
    m_kStringMgr.Clear();
    return m_kStringMgr.LoadStringFile( strFileName , pFile , bForTool );
}


bool KGCCTMissionManager::LoadMissionStringFromKOM( std::string strFileName )
{
    m_kStringMgr.Clear();
    return m_kStringMgr.LoadStringFromKOM(strFileName);
}


bool KGCCTMissionManager::LoadMissionString( FILE* pFile /*= NULL*/ )
{
    if( pFile == NULL )
        return false;

    m_kStringMgr.Clear();
    return m_kStringMgr.LoadStringFile( "", pFile );
}

bool KGCCTMissionManager::UpdateSubMissionInfo( const SSubMissionInfo& sInfo )
{
    std::map< int, KGCSubMission > ::iterator mit;
    mit = m_mapSubMission.find( sInfo.m_iSubMissionID );

    if( mit == m_mapSubMission.end() )
        return false;

    mit->second.SetSubMissionInfo( sInfo );
    return true;
}

bool KGCCTMissionManager::UpdateMissionInfo( const SMissionInfo& sInfo )
{
    std::map< DWORD,  KGCMission > ::iterator mit;
    mit = m_mapMissionList.find( sInfo.m_dwID );

    if( mit == m_mapMissionList.end() )
        return false;

    mit->second.SetMissionInfo( sInfo );
    return true;
}

bool KGCCTMissionManager::OutPutMissionString( std::string strFileName )
{
    return m_kStringMgr.OutPutStringFile( strFileName );
}

void KGCCTMissionManager::UpdateUniqueMissionList()
{
    std::map< DWORD,  KGCMission > ::iterator mit;
    for( mit = m_mapMissionList.begin() ; mit != m_mapMissionList.end() ; ++mit )
    {
        const SMissionInfo* sInfo = mit->second.GetMissionInfo();
        for( int i = 0 ; i < (int)sInfo->m_vecInvalidCompletMissions.size() ; ++i )
        {
            m_setUniqueMission.insert( sInfo->m_vecInvalidCompletMissions[i] );
        }
    }
}

bool KGCCTMissionManager::IsUniqueMission( DWORD dwMissionID )
{
    if( m_setUniqueMission.find( dwMissionID ) == m_setUniqueMission.end() )
        return false;
    return true;
}

KGCMission* KGCCTMissionManager::GetMissionWithID( DWORD dwID )
{
    if( m_mapMissionList.find( dwID ) == m_mapMissionList.end() )
        return NULL;
    return &m_mapMissionList[dwID];
}

const SMissionInfo* KGCCTMissionManager::GetMissionInfo( DWORD dwID )
{
    KGCMission* kMission = GetMissionWithID( dwID );
    if( kMission != NULL )
        return kMission->GetMissionInfo();
    return NULL;
}

const SSubMissionInfo* KGCCTMissionManager::GetSubMission( int iID )
{
    std::map< int, KGCSubMission >::iterator mit;
    mit = m_mapSubMission.find( iID );
    if( mit == m_mapSubMission.end() )
        return NULL;
    return mit->second.GetSubMissionInfo();
}

const SConditionInfo* KGCCTMissionManager::GetConditionInfo( int iID )
{
    if( m_mapCondition.find( iID ) == m_mapCondition.end() )
        return NULL;
    return &m_mapCondition[iID].GetConditionInfo();
}

int     KGCCTMissionManager::GetNewSubMissionID()   //새로 생성될 서브 미션이 가질 ID를 리턴합니다. 아직 추가되지 않은 서브미션 ID
{
    int iLastIndex = 0;
    std::map< int, KGCSubMission >::iterator mit;
    for( mit = m_mapSubMission.begin() ; mit != m_mapSubMission.end() ; ++mit )
    {
        if( iLastIndex <= mit->first )
            iLastIndex = mit->first + 1;
    }
    return iLastIndex;
}

int     KGCCTMissionManager::GetNewConditionID()    //새로 생성될 컨디션이 가질 ID를 리턴합니다. 아직 추가되지 않은 컨디션 ID
{
    int iLastIndex = 0;
    std::map< int, KGCCondition > ::iterator mit;
    for( mit = m_mapCondition.begin() ; mit != m_mapCondition.end() ; ++mit )
    {
        if( iLastIndex <= mit->first )
            iLastIndex = mit->first + 1;
    }
    return iLastIndex;
}

bool KGCCTMissionManager::CheckSubMissionCondition( int iSubMissionID, const SConditionInfo& sInfo )
{
    std::map< int, KGCSubMission >::iterator mit;
    mit = m_mapSubMission.find( iSubMissionID );

    if( mit == m_mapSubMission.end() )
        return false;

    const SSubMissionInfo* sSubMissionInfo = mit->second.GetSubMissionInfo();

    for( int i = 0 ; i < (int)sSubMissionInfo->m_vecConditionID.size() ; ++i )
    {
        std::map< int, KGCCondition >::iterator mitCondition;
        mitCondition = m_mapCondition.find( sSubMissionInfo->m_vecConditionID[i] );
        if( mitCondition == m_mapCondition.end() )
            continue;

        if( mitCondition->second.IsSatisfied( sInfo ) )
            return true;
    }

    return false;
}
