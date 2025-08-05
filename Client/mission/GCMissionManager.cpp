#include "stdafx.h"

#include "KGCMultipleLanguages.h"
#include ".\gcmissionmanager.h"
#include "GCUI/KGCWorldMapIcon.h"



ImplementSingleton(KGCMissionManager);

KGCMissionManager::KGCMissionManager(void)
{
}

KGCMissionManager::~KGCMissionManager(void)
{
    m_kStringMgr.Clear();
    Reset();
}

void KGCMissionManager::Reset()
{
    m_mapCondition.clear();
    m_mapSubMission.clear();
    m_mapMissionList.clear();
    m_setUniqueMission.clear();

}

bool KGCMissionManager::LoadMissionFile( std::string strMissionScriptFile , KLuaManager* pkLuaMgr )
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
                    //OKBOX( "Condition Table Error : " << i , "Error");
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
				LUA_GET_VALUE_DEF( "Solo", sCondition.m_nSolo, false );
                LUA_GET_VALUE_DEF( "Time", sCondition.m_iTime, -1 );
                LUA_GET_VALUE_DEF( "Kill", sCondition.m_iKill, -1 );
                LUA_GET_VALUE_DEF( "Die", sCondition.m_iDie, -1 );
                LUA_GET_VALUE_DEF( "ExLife", sCondition.m_iExLife, -1 );
                LUA_GET_VALUE_DEF( "ConditionActiveRatio", sCondition.m_fConditionRatio, 1.0f );

                if( SUCCEEDED( luaMgr.BeginTable( "MonLevelRange", false ) ) )
                {
                    int iBegin, iEnd;
                    LUA_GET_VALUE_DEF( 1, iBegin, -1 );
                    LUA_GET_VALUE_DEF( 2, iEnd, -1 );
                    sCondition.m_pairMonLevelRange = std::make_pair( iBegin, iEnd );
                    luaMgr.EndTable( false );
                }
                else
                {
                    sCondition.m_pairMonLevelRange = std::make_pair( -1, -1 );
                }
                LUA_GET_VALUE_DEF( "Type", sCondition.m_iType, TYPE_NOUSE );
                LUA_GET_VALUE_DEF( "TypeValue", sCondition.m_iValue, -1 );
                if( SUCCEEDED( luaMgr.BeginTable( "TypeValueRange", false ) ) )
                {
                    int iBegin, iEnd;
                    LUA_GET_VALUE_DEF( 1, iBegin, -1 );
                    LUA_GET_VALUE_DEF( 2, iEnd, -1 );
                    sCondition.m_pairUseCount = std::make_pair( iBegin, iEnd );
                    luaMgr.EndTable( false );
                }
                else
                {
                    sCondition.m_pairUseCount = std::make_pair( -1, -1 );
                }

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
                    //OKBOX( "SubMission Table Error : " << i , "Error" );
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
                    //OKBOX( "No Condition Error : " << sSubMissionInfo.m_iSubMissionID , "Error" );
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
                    //OKBOX( "No Action Error : " << sSubMissionInfo.m_iSubMissionID, "Error" );
                    luaMgr.EndTable();
                    break;
                }

                if( SUCCEEDED( luaMgr.BeginTable( "Completion" ) ) )
                {
                    LUA_GET_VALUE_DEF( "ItemID", sSubMissionInfo.m_sCompletion.m_dwItemID, -1 );
                    if( FAILED( luaMgr.GetValue( "Count", sSubMissionInfo.m_sCompletion.m_iCount ) ) )
                    {
                        //OKBOX( "No Completion Count Error : " << sSubMissionInfo.m_iSubMissionID, "Error" );
                    }
                    LUA_GET_VALUE_DEF( "CharacterLv", sSubMissionInfo.m_sCompletion.m_iCharacterLv, -1 );
                    luaMgr.EndTable();
                }
                else
                {
                    //OKBOX( "No Completion Error : " << sSubMissionInfo.m_iSubMissionID, "Error" );
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
        //OKBOX( "Submission read Failed!!!" , "Error");
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
                LUA_GET_VALUE_DEF( "CorrectLevel", sMissionInfo.m_iCorrectLevel, -1 );
                LUA_GET_VALUE_DEF( "MissionTab", sMissionInfo.m_iMissionType, 0 );
                LUA_GET_VALUE_DEF( "TotalChain", sMissionInfo.m_iTotalChain, 0 );
				LUA_GET_VALUE_DEF( "CurrentChain", sMissionInfo.m_iCurrentChain, 0 );
				LUA_GET_VALUE_DEF( "NPCID", sMissionInfo.m_iNPCID, 0 );
                LUA_GET_VALUE_DEF( "EnableDelete", sMissionInfo.m_bEnableDelete, true );

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
                        if( iMissionID < 0 ) continue;
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
                        int iItemID, iCount, iPeriod, nGrade;
                        LUA_GET_VALUE_DEF( 1, iItemID, -2 );
                        LUA_GET_VALUE_DEF( 2, iCount, -2 );
                        LUA_GET_VALUE_DEF( 3, iPeriod, -1 );
                        LUA_GET_VALUE_DEF( 4, nGrade, -1 );
                        if (iItemID != -2 || iCount !=-2)
	                        sMissionInfo.m_vecRewardItem.push_back(  SMissionInfo::SReward( iItemID, iCount, iPeriod, nGrade ) );
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
                        int iItemID, iCount, iPeriod, nGrade;
                        LUA_GET_VALUE_DEF( 1, iItemID, -2 );
                        LUA_GET_VALUE_DEF( 2, iCount, -2 );
                        LUA_GET_VALUE_DEF( 3, iPeriod, -1 );
                        LUA_GET_VALUE_DEF( 4, nGrade, -1 );

                        if (iItemID != -2 || iCount !=-2)
                            sMissionInfo.m_vecSelectItem.push_back(  SMissionInfo::SReward( iItemID, iCount, iPeriod, nGrade ) );
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable();
                }

                int iGameMode;
                LUA_GET_VALUE_DEF( "DungeonID", iGameMode, -1 );

                if ( iGameMode != -1 )
                {
                    std::map<int, std::vector<DWORD>>::iterator mit;
                    std::vector<DWORD> vecMissionList;
                    vecMissionList.clear();

                    if ( sMissionInfo.m_iMissionType == 4 )
                    {
                        mit = m_mapDungeonEpicMissionData.find(iGameMode);
                        if ( mit != m_mapDungeonEpicMissionData.end() )
                            mit->second.push_back( sMissionInfo.m_dwID );
                        else
                        {
                            vecMissionList.push_back( sMissionInfo.m_dwID );
                            m_mapDungeonEpicMissionData.insert( std::make_pair( iGameMode, vecMissionList ) );
                        }
                    }
                    else if ( sMissionInfo.m_iMissionType == 1 )
                    {
                        mit = m_mapDungeonNormalMissionData.find(iGameMode);
                        if ( mit != m_mapDungeonNormalMissionData.end() )
                            mit->second.push_back( sMissionInfo.m_dwID );
                        else
                        {
                            vecMissionList.push_back( sMissionInfo.m_dwID );
                            m_mapDungeonNormalMissionData.insert( std::make_pair( iGameMode, vecMissionList ) );
                        }
                    }
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

//bool KGCMissionManager::OutPutMissionFile( std::string strMissionScriptFile )
//{
//	FILE* pFile = _tfopen( strMissionScriptFile.c_str(), "wc" );
//	if( pFile == NULL )
//	{
//		return false;
//	}
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
//			fprintf( pFile, "\t\tIsSolo = %s", (sInfo.m_bSolo? "TRUE" : "FALSE" ) );
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

bool KGCMissionManager::AddMissionInfo( const SMissionInfo& sInfo )
{
    KGCMission kMission;
    kMission.SetMissionInfo( sInfo );
    return m_mapMissionList.insert( std::make_pair( sInfo.m_dwID, kMission ) ).second;
}

int KGCMissionManager::AddSubMissionInfo( const SSubMissionInfo& sInfo )
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

int KGCMissionManager::AddConditionInfo( const SConditionInfo& sInfo )
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

int KGCMissionManager::CheckCompleteMission( DWORD dwMissionID, std::vector< std::pair< int, int > > vecCompletion )
{
    if( IsMissionExist( dwMissionID ) == false )
        return GC_MM_MISSION_NOT_EXIST;

    for( int i = 0 ; i < (int)vecCompletion.size() ; ++i )
    {
		if( vecCompletion[i].first == -1 )
			continue;
			
		std::map< int, KGCSubMission >::iterator mit = m_mapSubMission.find(vecCompletion[i].first);
        if( mit == m_mapSubMission.end() )
            return GC_MM_SUBMISSION_NOT_EXIST;

        if( mit->second.IsComplete( vecCompletion[i].second ) == false)
            return GC_MM_NOT_ENOUGH;
    }
    
    return GC_MM_OK;
}

int KGCMissionManager::IsAvailableRegister( DWORD dwMissionID, SCharInfo& sCharInfo_ )
{
    const SMissionInfo* sInfo = GetMissionInfo( dwMissionID );

    //Character Check
    if( sInfo->m_iCharacter != -1 && sInfo->m_iCharacter != sCharInfo_.iCharType )
        return GC_MM_DIFFERENT_CHAR;

    //Level Check
    if( (sInfo->m_pairLevelLimit.first != -1 && sInfo->m_pairLevelLimit.first > sCharInfo_.iLevel ) ||
        (sInfo->m_pairLevelLimit.second != -1 && sInfo->m_pairLevelLimit.second < sCharInfo_.iLevel ) )
        return GC_MM_NOT_ENOUGH_LEVEL;

    //Promotion Check
    if( sInfo->m_iPromotion != -1 ) 
        if( sInfo->m_iPromotion > sCharInfo_.iPromotionLevel )
            return GC_MM_NOT_ENOUGH_PROMOTION;

    //이미 한 전직인가?
    if (sInfo->m_iChangeJob > 0 )
        if ( sCharInfo_.iPromotionLevel >=  sInfo->m_iChangeJob )
            return GC_MN_NOT_ALREADY_PROMOTION;

    return GC_MM_OK;
}


int KGCMissionManager::AddString( std::wstring str )
{
    return m_kStringMgr.AddMissionString( str );
}

void KGCMissionManager::EraseString( int iID )
{    
}

std::wstring KGCMissionManager::GetString ( int iID )
{
    std::wstring strMissionString;
    m_kStringMgr.GetMissionString( iID , strMissionString );
    return strMissionString;
}

bool KGCMissionManager::LoadMissionString( std::string strFileName , FILE* pFile /*= NULL */, bool bForTool /*= false */)
{
    m_kStringMgr.Clear();
    return m_kStringMgr.LoadStringFile( strFileName , pFile , bForTool );
}

bool KGCMissionManager::LoadMissionString( FILE* pFile /*= NULL*/ )
{
    if( pFile == NULL )
        return false;

    m_kStringMgr.Clear();
    return m_kStringMgr.LoadStringFile( "", pFile );
}

bool KGCMissionManager::LoadMissionStringFromKOM( std::string strFileName)
{
    m_kStringMgr.Clear();
    return m_kStringMgr.LoadStringFromKOM( strFileName );
}

bool KGCMissionManager::UpdateSubMissionInfo( const SSubMissionInfo& sInfo )
{
    std::map< int, KGCSubMission > ::iterator mit;
    mit = m_mapSubMission.find( sInfo.m_iSubMissionID );

    if( mit == m_mapSubMission.end() )
        return false;

    mit->second.SetSubMissionInfo( sInfo );
    return true;
}

bool KGCMissionManager::UpdateMissionInfo( const SMissionInfo& sInfo )
{
    std::map< DWORD,  KGCMission > ::iterator mit;
    mit = m_mapMissionList.find( sInfo.m_dwID );

    if( mit == m_mapMissionList.end() )
        return false;

    mit->second.SetMissionInfo( sInfo );
    return true;
}

bool KGCMissionManager::OutPutMissionString( std::string strFileName )
{
    return m_kStringMgr.OutPutStringFile( strFileName );
}

void KGCMissionManager::UpdateUniqueMissionList()
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

bool KGCMissionManager::IsUniqueMission( DWORD dwMissionID )
{
    if( m_setUniqueMission.count( dwMissionID ) == 0 )
        return false;
    return true;
}

KGCMission* KGCMissionManager::GetMissionWithID( DWORD dwID )
{
    if( m_mapMissionList.count( dwID ) == 0 )
        return NULL;
    return &m_mapMissionList[dwID];
}

const SMissionInfo* KGCMissionManager::GetMissionInfo( DWORD dwID )
{
    KGCMission* kMission = GetMissionWithID( dwID );
    if( kMission != NULL )
        return kMission->GetMissionInfo();
    return NULL;
}

const SSubMissionInfo* KGCMissionManager::GetSubMission( int iID )
{
    std::map< int, KGCSubMission >::iterator mit;
    mit = m_mapSubMission.find( iID );
    if( mit == m_mapSubMission.end() )
        return NULL;
    return mit->second.GetSubMissionInfo();
}

const SConditionInfo* KGCMissionManager::GetConditionInfo( int iID )
{
    if( m_mapCondition.count( iID ) == 0 )
        return NULL;
    return &m_mapCondition[iID].GetConditionInfo();
}

int     KGCMissionManager::GetNewSubMissionID()   //새로 생성될 서브 미션이 가질 ID를 리턴합니다. 아직 추가되지 않은 서브미션 ID
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

int     KGCMissionManager::GetNewConditionID()    //새로 생성될 컨디션이 가질 ID를 리턴합니다. 아직 추가되지 않은 컨디션 ID
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

bool KGCMissionManager::CheckSubMissionCondition( int iSubMissionID, const SConditionInfo& sInfo, int iMonLevel )
{
    std::map< int, KGCSubMission >::iterator mit;
    mit = m_mapSubMission.find( iSubMissionID );

    if( mit == m_mapSubMission.end() )
        return false;

    const SSubMissionInfo* sSubMissionInfo = mit->second.GetSubMissionInfo();

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    for( int i = 0 ; i < (int)sSubMissionInfo->m_vecConditionID.size() ; ++i )
    {
        std::map< int, KGCCondition >::iterator mitCondition;
        mitCondition = m_mapCondition.find( sSubMissionInfo->m_vecConditionID[i] );
        if( mitCondition == m_mapCondition.end() )
            continue;

        SConditionInfo sCondition = mitCondition->second.GetConditionInfo();

        // 레벨이 조건인 미션이다.(던전이나 몬스터는 지정되지 않음)
        if( pPlayer && (sCondition.m_pairMonLevelRange.first >= 0) || (sCondition.m_pairMonLevelRange.second >= 0) )
        {
            // 몬스터 원정대는 미션처리 안함
            if( sInfo.m_iGameMode == GC_GM_MONSTER_CRUSADER )
                continue;

            // 점령전은 미션처리 안함
            if( sInfo.m_iGameMode == GC_GM_DOTA )
                continue;

            // first가 2고 second가 2면, 내 레벨기준으로 -2 ~ 2사이 몬스터를 잡는게 조건임.
            int iLowLevel, iHighLevel;
            if( (sCondition.m_pairMonLevelRange.first >= 0) )
                iLowLevel = pPlayer->GetCurrentChar().iLevel -  sCondition.m_pairMonLevelRange.first;
            else
                iLowLevel = 0;

            if( (sCondition.m_pairMonLevelRange.second >= 0) )
                iHighLevel = pPlayer->GetCurrentChar().iLevel + sCondition.m_pairMonLevelRange.second;
            else
                iHighLevel = MAX_LEVEL;

            if( !g_MyD3D->m_pkQuestGameMgr->IsQuestLevelFromInputLevel( iLowLevel, iHighLevel, EGCGameMode(-1), -1, iMonLevel ) )
                continue;
        }
        else
        {
            if( sCondition.m_iGameMode != sInfo.m_iGameMode )
                continue;

            if( sCondition.m_iMonsterID != sInfo.m_iMonsterID )
                continue;

            if( sCondition.m_pairDifficultRange.first != -1 &&
                sCondition.m_pairDifficultRange.first > sInfo.m_pairDifficultRange.first )
                continue;
        }

        return true;
    }

    return false;
}

std::wstring KGCMissionManager::GetTitleColor( int iCharLevel, int iMissionLevel )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    std::wstringstream stm;
    stm<<L"#C"<<std::hex<<std::setw(6)<<std::setfill(L'0');
    stm<<lua_tinker::call<int>( L, "GetMissionTitleColor", iCharLevel, iMissionLevel );

    return stm.str();
}

bool KGCMissionManager::LoadScript()
{
    std::string strMissionScript("initMission.lua");
    std::string strMissionString("str_Mission.stg");

    if (SiKGCMultipleLanguages()->UseMultipleLanguages())
    {
        std::string strSkillFileName = SiKGCMultipleLanguages()->ConvertNationStringFileNameA("str_Mission_");
        strMissionString = g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath(strSkillFileName.c_str());
    }

    GCFUNC::LoadLuaScript(KGCLuabinder::getInstance(), "MissionFunc.lua");

    KLuaManager* kMissionLua = new KLuaManager();
    GCFUNC::LoadLuaScript(*kMissionLua, strMissionScript);
    if (SiKGCMissionManager()->LoadMissionFile(strMissionScript, kMissionLua) == false)
    {
        return false;
    }

    if (SiKGCMultipleLanguages()->UseMultipleLanguages())
    {
        strMissionString = SiKGCMultipleLanguages()->ConvertNationStringFileNameA("str_Mission_");
    }

    if (SiKGCMissionManager()->LoadMissionStringFromKOM(strMissionString) == false)
    {
        return false;
    }

    return true;
}

std::wstring KGCMissionManager::GetTitleString( DWORD dwMissionID )
{
    const SMissionInfo* sInfo = GetMissionInfo( dwMissionID );

    std::wstringstream stm;
    if( sInfo->m_iCharacter != -1 )
    {
        SCharInfo& sCharInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar( sInfo->m_iCharacter );
        stm<<GetTitleColor( sCharInfo.iLevel, sInfo->m_iCorrectLevel );
    }

    stm<<GetString( sInfo->m_iTitleID );
    stm<<L"#CX";

    return stm.str();
}

int KGCMissionManager::GetMissionNPCID( DWORD dwMissionID )
{
	const SMissionInfo* sInfo = GetMissionInfo( dwMissionID );

	if(sInfo == NULL || sInfo->m_iNPCID == -1 )
		return 0;
		
	return sInfo->m_iNPCID;
}

bool KGCMissionManager::IsEnableDelete( DWORD dwMissionID )
{
    const SMissionInfo* sInfo = GetMissionInfo( dwMissionID );

    if(sInfo == NULL)
        return false;

    return sInfo->m_bEnableDelete;
}

bool KGCMissionManager::LoadMissionCompleteEff( const char* strFileName_ )
{
    KLuaManager luaMgr;
    
    if( false == GCFUNC::LoadLuaScript( luaMgr, strFileName_ ) ) {
        return false;
    }

    if( FAILED( luaMgr.BeginTable( "MissionCompleteEff" ) ) ) {     // MissionCompleteEff
        return false;
    }

    m_mapCompleteEff.clear();
    int iCount = 1;
    while( true ) { 
        if( FAILED( luaMgr.BeginTable( iCount ) ) ) {       // Begin Info
            break;
        }

        DWORD dwMissionID;
        std::vector< MissionCompleteEff > vecEff;

        luaMgr.GetValue( "MissionID", dwMissionID );

        if( SUCCEEDED( luaMgr.BeginTable( "ParticleList" ) ) ) {    // Begin Particle List
            for( int i=1 ; ; ++i ) { 
                if( SUCCEEDED( luaMgr.BeginTable( i ) ) ) { // Begin ParticleElement
                    MissionCompleteEff tempEff;
                    LUA_GET_VALUE_DEF( 1, tempEff.strParticle, "" );
                    LUA_GET_VALUE_DEF( 2, tempEff.dxvPos.x,   0.0f );
                    LUA_GET_VALUE_DEF( 3, tempEff.dxvPos.y,   0.0f );
                    vecEff.push_back( tempEff );
                    luaMgr.EndTable();  // End ParticleElement
                }
                else { 
                    break;
                }
            }
            luaMgr.EndTable();  // End Particle List
        }

        std::map< DWORD, std::vector< MissionCompleteEff > >::const_iterator mit = m_mapCompleteEff.find( dwMissionID );
        if( mit == m_mapCompleteEff.end() && false == vecEff.empty() ) { 
            m_mapCompleteEff.insert( std::make_pair( dwMissionID, vecEff ) );
        }

        luaMgr.EndTable();  // End Info;
        ++iCount;
    }
    luaMgr.EndTable();  // End MissionCompleteEff

    return true;
}

bool KGCMissionManager::GetMissionCompleteEff( const DWORD dwMissionID_, OUT std::vector< MissionCompleteEff >& vecEff_ )
{
    if( m_mapCompleteEff.empty() ) { 
        return false;
    }

    std::map< DWORD, std::vector< MissionCompleteEff > >::const_iterator mit = m_mapCompleteEff.find( dwMissionID_ );
    if( mit == m_mapCompleteEff.end() ) { 
        return false;
    }

    vecEff_.clear();
    vecEff_ = mit->second;
    return true;
}

bool KGCMissionManager::IsCompleteEff( const DWORD dwMissionID_ )
{
    if( m_mapCompleteEff.empty() ) { 
        return false;
    }

    std::map< DWORD, std::vector< MissionCompleteEff > >::const_iterator mit = m_mapCompleteEff.find( dwMissionID_ );
    if( mit == m_mapCompleteEff.end() ) { 
        return false;
    }

    return true;
}

void KGCMissionManager::SetClearMissionData( std::vector<DWORD> &vecClearMissionData )
{
    m_vecClearMissionData.clear();
    m_vecClearMissionData = vecClearMissionData;
}

bool KGCMissionManager::CheckEpicMission( int iGameMode )
{
    std::map<int, std::vector<DWORD>>::iterator mit;
    mit = m_mapDungeonEpicMissionData.find( iGameMode );
    if ( mit != m_mapDungeonEpicMissionData.end() )
    {
        std::vector<DWORD>::iterator vit;
        for ( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
            if( SiKGCMissionManager()->IsAvailableRegister( *vit, sInfo ) != KGCMissionManager::GC_MM_OK )
                continue;

            //같은 미션은 두개를 중복해서 진행할 수 없음
            if( g_kGlobalValue.m_kUserMission.IsUserMission( *vit ) == true ||
                g_kGlobalValue.m_kUserMission.IsInvalidMission( *vit ) != 0 )
            {
                return true;
            }

            if ( CheckClearCondisionMission( *vit ) )
            {
                std::vector<DWORD>::iterator vit2;
                vit2 = find( m_vecClearMissionData.begin(), m_vecClearMissionData.end(), *vit );
                if ( vit2 == m_vecClearMissionData.end() )
                {
                    KP2P::GetInstance()->Send_DungeonMissionRegistReq(*vit);
                    return true;
                }
            }
            else
                return true;
        }
    }

    return false;
}

bool KGCMissionManager::CheckClearCondisionMission( DWORD dwMissionID )
{
    std::map< DWORD, DWORD >::iterator mit;
    mit = m_mapGettableCondditionInfo.find( dwMissionID );
    if ( mit != m_mapGettableCondditionInfo.end() )
    {
        if ( mit->second == 0 )
            return true;

        std::vector<DWORD>::iterator vit;
        vit = find( m_vecClearMissionData.begin(), m_vecClearMissionData.end(), mit->second );
        if ( vit != m_vecClearMissionData.end() )
            return true;
    }

    return false;
}

DWORD KGCMissionManager::CheckNormalMission( int iGameMode )
{
    std::map<int, std::vector<DWORD>>::iterator mit;
    mit = m_mapDungeonNormalMissionData.find( iGameMode );
    if ( mit != m_mapDungeonNormalMissionData.end() )
    {
        std::vector<DWORD>::iterator vit;
        for ( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
            if( SiKGCMissionManager()->IsAvailableRegister( *vit, sInfo ) != KGCMissionManager::GC_MM_OK )
                continue;

            //같은 미션은 두개를 중복해서 진행할 수 없음
            if( g_kGlobalValue.m_kUserMission.IsUserMission( *vit ) == true ||
                g_kGlobalValue.m_kUserMission.IsInvalidMission( *vit ) != 0 )
            {
                break;
            }

            std::vector<DWORD>::iterator vit2;
            vit2 = find( m_vecClearMissionData.begin(), m_vecClearMissionData.end(), *vit );
            if ( vit2 == m_vecClearMissionData.end() )
            {
                return *vit;
            }
        }
    }

    return 0;
}

int KGCMissionManager::GetEpicMissionIconType( EGCGameMode eGameMode, int iCurrentCharLevel )
{
    std::map<int, std::vector<DWORD>>::iterator mit;
    mit = m_mapDungeonEpicMissionData.find( eGameMode );
    if ( mit != m_mapDungeonEpicMissionData.end() )
    {
        std::vector<DWORD>::iterator vitMissionID;
        for ( vitMissionID = mit->second.begin(); vitMissionID != mit->second.end(); ++vitMissionID )
        {
            SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
            if( SiKGCMissionManager()->IsAvailableRegister( *vitMissionID, sInfo ) != KGCMissionManager::GC_MM_OK )
                continue;

            //같은 미션은 두개를 중복해서 진행할 수 없음
            if( g_kGlobalValue.m_kUserMission.IsUserMission( *vitMissionID ) == true ||
                g_kGlobalValue.m_kUserMission.IsInvalidMission( *vitMissionID ) != 0 )
            {
                break;
            }

            QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eGameMode, 0 );
            std::vector<DWORD>::iterator vit2;
            vit2 = find( m_vecClearMissionData.begin(), m_vecClearMissionData.end(), *vitMissionID );
            if ( vit2 == m_vecClearMissionData.end() )
            {
                if ( kQuestFileInfo.iLowLevel != -1 )
                {
                    int iLevelGap = kQuestFileInfo.iLowLevel - iCurrentCharLevel;
                    if ( iLevelGap <= -3 )
                        return KGCWorldMapIcon::EICON_OVER_LEVEL; //회색아이콘
                    else if ( iLevelGap >= 3 )
                        return KGCWorldMapIcon::EICON_UNDER_LEVEL; //빨간색아이콘
                    else
                        return KGCWorldMapIcon::EICON_NORMAL_LEVEL; //노란색아이콘
                }
            }
        }
    }

    return KGCWorldMapIcon::EICON_MAX;
}

void KGCMissionManager::SetMissionGettableCondditionInfo( std::map< DWORD, DWORD > &mapGettableCondditionInfo )
{
    m_mapGettableCondditionInfo.clear();
    m_mapGettableCondditionInfo = mapGettableCondditionInfo;
}
