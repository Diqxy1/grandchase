#include "Room.h" // winsock2
#include "MissionInfo.h"
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include "NetError.h"
sCondition::sCondition()
:m_nID(0)
,m_nSolo(0)
,m_nMonsterKill(0)
,m_nModeID(-1)
,m_nStageID(-1)
,m_nMapID(-1)
,m_bWin(false)
,m_nElapsedTime(0)
,m_nKill(-1)
,m_nDie(-1)
,m_nExLife(-1)
,m_fRatio(1.f)
,m_nType(-1)
,m_nValue(-1)
,m_prValue(-1, -1)
{
    m_prDifficultRage.first     = -1;
    m_prDifficultRage.second    = -1;
}

KSubMission::KSubMission()
:m_dwSubmissionID()
{
    m_setConditions.clear();
    m_sActionInfo.m_bMonDrop    = false;
    m_sActionInfo.m_nCount      = 0;
    m_sActionInfo.m_nItemID     = 0;
    m_sCompletionInfo.m_nCount  = 0;
    m_sCompletionInfo.m_nItemID = 0;
    m_sCompletionInfo.m_nCharLv = -1;

}

ImplToStringW( KSubMission )
{
    START_TOSTRING2W( KSubMission )
        << TOSTRINGW( m_dwSubmissionID )
        << TOSTRINGW( m_setConditions.size() )
        << TOSTRINGWb( m_sActionInfo.m_bMonDrop )
        << TOSTRINGW( m_sActionInfo.m_nItemID )
        << TOSTRINGW( m_sActionInfo.m_nCount )
        << TOSTRINGW( m_sCompletionInfo.m_nCount )
        << TOSTRINGW( m_sCompletionInfo.m_nItemID )
        << TOSTRINGW( m_sCompletionInfo.m_nCharLv );


    if( !m_setConditions.empty() )
    {
        stm_ << L"   --> m_setConditions    : ";
        std::copy( m_setConditions.begin(), m_setConditions.end(), std::ostream_iterator<int,wchar_t>( stm_, L", " ) );
        stm_ << std::endl;
    }
    return stm_;
}

//------------------------------------------------------------------------------------------------------------

KMissionInfo::KMissionInfo()
:m_dwMissionID(0)
,m_cCharType(-1)
,m_cJobLv(-1)
,m_nPeriod(-1)
,m_nCorrectLv(-1)
,m_bUnique(false)
,m_cNewChar(-1)
,m_cChangeJob(-1)
,m_nGp(0)
,m_nExp(0)
,m_dwChainMissionID(0)
,m_cMissionType(0)
,m_bEnableDelete( true )
{
    m_prLevelRage.first = -1;
    m_prLevelRage.second = -1;
    m_vecInvalidMission.clear();
    m_vecInvalidComplete.clear();
    m_vecSubmission.clear();
    m_vecRewardItem.clear();
    m_vecSelectItem.clear();
}

KMissionInfo::~KMissionInfo()
{
}

ImplToStringW( KMissionInfo )
{
    START_TOSTRING2W( m_dwMissionID )
        << TOSTRINGWc( m_cCharType )
        << TOSTRINGWc( m_cJobLv )
        << TOSTRINGW( m_nPeriod )
        << TOSTRINGW( m_prLevelRage.first )
        << TOSTRINGW( m_prLevelRage.second )
        << TOSTRINGW( m_nCorrectLv )
        << TOSTRINGW( m_vecInvalidMission.size() )
        << TOSTRINGW( m_vecInvalidComplete.size() )
        << TOSTRINGW( m_vecSubmission.size() )
        << TOSTRINGWb( m_bUnique )
        << TOSTRINGWc( m_cNewChar )
        << TOSTRINGWc( m_cChangeJob )
        << TOSTRINGW( m_nGp )
        << TOSTRINGW( m_nExp )
        << TOSTRINGW( m_vecRewardItem.size() )
        << TOSTRINGW( m_vecSelectItem.size() )
        << TOSTRINGW( m_dwChainMissionID )
        << TOSTRINGWc( m_cMissionType )
        << TOSTRINGWb( m_bEnableDelete );

    if( !m_vecInvalidMission.empty() )
    {
        stm_ << L"   --> m_vecInvalidMission : ";
        std::copy( m_vecInvalidMission.begin(), m_vecInvalidMission.end(), 
            std::ostream_iterator<DWORD,wchar_t>( stm_, L", " ) );
        stm_ << std::endl;
    }

    if( !m_vecInvalidComplete.empty() )
    {
        stm_ << L"   --> m_vecInvalidComplete : ";
        std::copy( m_vecInvalidComplete.begin(), m_vecInvalidComplete.end(), 
            std::ostream_iterator<DWORD,wchar_t>( stm_, L", " ) );
        stm_ << std::endl;
    }

    if( !m_vecSubmission.empty() )
    {
        stm_ << L"   --> m_vecSubmission : ";
        std::copy( m_vecSubmission.begin(), m_vecSubmission.end(), 
            std::ostream_iterator<int,wchar_t>( stm_, L", " ) );
        stm_ << std::endl;
    }

    if( !m_vecRewardItem.empty() )
    {
        stm_ << L"   --> m_vecRewardItem      : ";
        std::vector<KMissionReward>::const_iterator vit;
        for( vit = m_vecRewardItem.begin() ; vit != m_vecRewardItem.end() ; ++vit )
        {
            stm_ << L"(" << vit->m_ItemID << L"," << vit->m_nCount<< L"," << vit->m_nPeriod << L"," << vit->m_bIsLook << L"), ";
        }
        stm_ << std::endl;
    }
    
    if( !m_vecSelectItem.empty() )
    {
        stm_ << L"   --> m_vecSelectItem      : ";
        std::vector<KMissionReward>::const_iterator vit;
        for( vit = m_vecSelectItem.begin() ; vit != m_vecSelectItem.end() ; ++vit )
        {
            stm_ << L"(" << vit->m_ItemID << L"," << vit->m_nCount<< L"," << vit->m_nPeriod << L"," << vit->m_bIsLook << L"), ";
        }
        stm_ << std::endl;
    }

    return stm_;
}

bool KMissionInfo::CheckValidation()
{

    if( m_dwMissionID % 10  != 0 )
    {
        SET_ERROR( ERR_MISSION_15 );
        goto END_PROC;
    }

    if( m_cCharType < -1 || m_cCharType >= GC_CHAR_NUM )
    {
        SET_ERROR( ERR_MISSION_12 );
        goto END_PROC;
    }

    if( m_prLevelRage.first < -1 || m_prLevelRage.first > 75 ||
        m_prLevelRage.second < -1 || m_prLevelRage.second > 75 )
    {
        SET_ERROR( ERR_MISSION_14 );
        goto END_PROC;
    }

    if( m_cNewChar <= 2 )
    {
        SET_ERROR( ERR_MISSION_16 );
        goto END_PROC;
    }

    return true;
END_PROC:

    START_LOG( cwarn, L"미션 유효성 체크 실패.. : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_dwMissionID )
        << BUILD_LOGc( m_cCharType )
        << BUILD_LOGc( m_cJobLv )
        << BUILD_LOG( m_nPeriod )
        << BUILD_LOG( m_prLevelRage.first )
        << BUILD_LOG( m_prLevelRage.second )
        << BUILD_LOG( m_nCorrectLv )
        << BUILD_LOG( m_vecInvalidMission.size() )
        << BUILD_LOG( m_vecInvalidComplete.size() )
        << BUILD_LOG( m_vecSubmission.size() )
        << BUILD_LOGc( m_cNewChar )
        << BUILD_LOGc( m_cChangeJob )
        << BUILD_LOG( m_nGp )
        << BUILD_LOG( m_nExp )
        << BUILD_LOG( m_vecRewardItem.size() )
        << BUILD_LOG( m_vecSelectItem.size() ) << END_LOG;

    return false;
}

bool KMissionInfo::IsInSelection( GCITEMID itemID_ )
{
    std::vector<KMissionReward>::iterator vit;
    vit = std::find_if( m_vecSelectItem.begin(), m_vecSelectItem.end(),
        boost::bind( &KMissionReward::m_ItemID, _1 ) ==  itemID_ );
    return (vit != m_vecSelectItem.end());
}