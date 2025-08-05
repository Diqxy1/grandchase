#include "StdAfx.h"
#include "KGCFaceLift.h"
#include "GCObjectAndMotion.h"

ImplementSingleton( KGCFaceLift )

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스크립트 파일모니터 등록( SetFaceLift.lua )
////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( __PATH__ )

#include "./GCUtil/GCFileMoniter.h"
static bool bReLoadFaceLift = false;
void ReLoadFaceLift()
{
    bReLoadFaceLift	= true;
}
void KGCFaceLift::CheckFileMonitor()
{
    if( bReLoadFaceLift )
    {
        LoadFromLuaScript( "SetFaceLift.stg" );
        bReLoadFaceLift = false;
    }
}
void KGCFaceLift::RegisterFileMoniter()
{
    SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( "SetFaceLift.stg" ), ReLoadFaceLift );
}

#endif

KGCFaceLift::KGCFaceLift()
{
    m_mapFaceLiftInfo.clear();
    m_mapPlayFaceLiftInfo.clear();
    m_mapCommonFaceLiftInfo.clear();
    m_strCurMotionName = "";

#if !defined( __PATH__ )
    RegisterFileMoniter();
#endif
}

KGCFaceLift::~KGCFaceLift()
{
}

void KGCFaceLift::LoadFromLuaScript( char* strFilename_ )
{
    KLuaManager luaMgr;
    if ( !GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) )
    {
        return;
    }
    if ( !GCFUNC::LoadLuaScript( luaMgr, "MotionEnum.stg" ) )
    {
        return;
    }

	if ( !GCFUNC::LoadLuaScript( luaMgr, strFilename_ ) )
	{
		return;
	}

    // 캐릭터별 모션정보
    m_mapFaceLiftInfo.clear();
    m_mapPlayFaceLiftInfo.clear();
    for( int i = 0; i < GC_CHAR_NUM; i++ ) 
    {
        const char* strTableName;
        std::wstringstream wstrmTableName;
        wstrmTableName << L"Face_" << i;
        strTableName = GCUTIL_STR::GCStrWideToChar( wstrmTableName.str().c_str() );

        LUA_BEGIN_TABLE_RAII( strTableName )
        {
            for( int j = 1; luaMgr.BeginTable( j ) == S_OK; ++j )
            {
                int iMotionID;
                std::string strFileName;
                LUA_GET_VALUE_DEF( "MOTION_ID", iMotionID, -1);
                LUA_GET_VALUE_DEF( "MOTION_FILE_NAME", strFileName, "");

                map<FACE_FRAME, FACE_INFO> mapFaceInfo = LoadScriptFrameInfo( luaMgr );

                if( -1 == iMotionID )
                {
                    m_mapFaceLiftInfo.insert( std::make_pair( std::make_pair( i, strFileName ), mapFaceInfo ) );
                }
                else
                {
                    m_mapPlayFaceLiftInfo.insert( std::make_pair( std::make_pair( i, iMotionID ), mapFaceInfo ) );
                }

                luaMgr.EndTable();
            }
        }
    }

    // 공용모션정보
    m_mapCommonFaceLiftInfo.clear();
    LUA_BEGIN_TABLE_RAII( "Face_Common" )
    {
        for( int j = 1; luaMgr.BeginTable( j ) == S_OK; ++j )
        {
            int iMotionID;
            LUA_GET_VALUE_DEF( "MOTION_ID", iMotionID, -1);
            if( -1 == iMotionID )
                continue;

            map<FACE_FRAME, FACE_INFO> mapFaceInfo = LoadScriptFrameInfo( luaMgr );
            m_mapCommonFaceLiftInfo.insert( std::make_pair(iMotionID, mapFaceInfo) );
            luaMgr.EndTable();
        }
    }
}

std::map<FACE_FRAME, FACE_INFO> KGCFaceLift::LoadScriptFrameInfo( KLuaManager &luaMgr ) 
{
    FACE_INFO kFaceInfo;
    FACE_FRAME kFaceFrame;
    std::map< FACE_FRAME, FACE_INFO > mapFaceInfo;
    for( int k = 1; luaMgr.BeginTable( k ) == S_OK; ++k )
    {
        LUA_GET_VALUE_DEF( "MIN_FRAME", kFaceFrame.iMinFrame, 0 );
        LUA_GET_VALUE_DEF( "MAX_FRAME", kFaceFrame.iMaxFrame, 0 );
        LUA_GET_VALUE_DEF( "FACE_TYPE", kFaceInfo.iFaceType, -1 );
        LUA_GET_VALUE_DEF( "DELAY_COUNT", kFaceInfo.iDelayCount, -1 );
        LUA_GET_VALUE_DEF( "DELAY_FRAME", kFaceInfo.iDelayFrame, 0 );
        LUA_GET_VALUE_DEF( "EXTRA_MESH_NAME", kFaceInfo.sExtraMeshName, "" );

        mapFaceInfo.insert( std::make_pair( kFaceFrame, kFaceInfo ) );
        luaMgr.EndTable();
    }

    return mapFaceInfo;
}

void KGCFaceLift::SetFaceLiftByMotionName( PLAYER* pPlayer, std::string strFileaName, int iFrame )
{
#if !defined( __PATH__ )
    CheckFileMonitor();
#endif

    if( pPlayer ) {
        // 빅헤드는 표정변화없음
        if( pPlayer->m_bEquipBigHead )
            return;

        int iCurFrame = pPlayer->GetFrameIndex();
        if( iFrame != -1 ) {
            iCurFrame = iFrame;
        }

        // 딜레이 처리
        ProcessingDelay( pPlayer );

        // 표정변경
        std::map< std::pair< int, std::string >, map< FACE_FRAME, FACE_INFO > >::iterator mitFaceInfo;
        mitFaceInfo = m_mapFaceLiftInfo.find( std::make_pair( pPlayer->GetCurrentChar().iCharType, strFileaName ) );
        if( mitFaceInfo != m_mapFaceLiftInfo.end() ) {
            ProcessingChangeFace( mitFaceInfo->second, pPlayer, iCurFrame );
        } else {
            pPlayer->SetRenderFaceLift( pPlayer->GetDefaultFaceLiftType() );
        }
    }
}
 
void KGCFaceLift::SetFaceLiftByMotionEnum( PLAYER* pPlayer, int iMotionEnum, int iFrame )
{
#if !defined( __PATH__ )
    CheckFileMonitor();
#endif

    if( pPlayer ) {
        // 빅헤드는 표정변화없음
        if( pPlayer->m_bEquipBigHead )
            return;

        int iCurFrame = pPlayer->GetFrameIndex();
        if( iFrame != -1 ) {
            iCurFrame = iFrame;
        }

        // 딜레이 처리
        ProcessingDelay( pPlayer );

        // 공용모션처리
        std::map< int, map< FACE_FRAME, FACE_INFO > >::iterator mitCommonFaceInfo = m_mapCommonFaceLiftInfo.find( iMotionEnum );
        if( mitCommonFaceInfo != m_mapCommonFaceLiftInfo.end() )
        {
            ProcessingChangeFace( mitCommonFaceInfo->second, pPlayer, iCurFrame );
            return;
        }

        // 캐릭터별 표정 변경
        std::map< std::pair< int, int >, map< FACE_FRAME, FACE_INFO > >::iterator mitFaceInfo;
        mitFaceInfo = m_mapPlayFaceLiftInfo.find( std::make_pair( pPlayer->GetCurrentChar().iCharType, iMotionEnum ) );
        if( mitFaceInfo != m_mapPlayFaceLiftInfo.end() ) {
            ProcessingChangeFace( mitFaceInfo->second, pPlayer, iCurFrame );
        }
    }
}

void KGCFaceLift::ProcessingDelay( PLAYER *pPlayer )
{
    if( NULL == pPlayer )
        return;

    std::map< int, std::pair<int, int> >::iterator mapDelayIter = pPlayer->m_mapFaceLiftDelayInfo.begin();
    while( mapDelayIter != pPlayer->m_mapFaceLiftDelayInfo.end() )
    {
        // 딜레이 카운팅이 남아있으면 아직은 딜레이를 걸지 않는다.
        if( 0 < mapDelayIter->second.first )
        {
            ++mapDelayIter;
            continue;
        }

        // 딜레이 감소 중
        --mapDelayIter->second.second;
        if( 0 >= mapDelayIter->second.second )
            mapDelayIter = pPlayer->m_mapFaceLiftDelayInfo.erase( mapDelayIter );
        else
            ++mapDelayIter;
    }
}

void KGCFaceLift::ProcessingChangeFace( std::map< FACE_FRAME, FACE_INFO > &mapFaceLiftInfo, PLAYER *pPlayer, int iCurFrame )
{
    if( NULL == pPlayer )
        return;

    std::map< FACE_FRAME, FACE_INFO >::iterator mitFrameInfo = mapFaceLiftInfo.begin();
    for(; mitFrameInfo != mapFaceLiftInfo.end(); ++mitFrameInfo)
    {
        FACE_INFO tempFaceInfo = mitFrameInfo->second;

        if( tempFaceInfo.sExtraMeshName != "" ) {
            std::stringstream stmString;
            stmString << tempFaceInfo.sExtraMeshName;
            pPlayer->ToggleExtraMesh( stmString.str().c_str(), false );
        }

        if( mitFrameInfo->first.iMinFrame <= iCurFrame && iCurFrame <= mitFrameInfo->first.iMaxFrame )
        {
            if( tempFaceInfo.sExtraMeshName == "" ) {
                bool bChangeFace = false;
                std::map< int, std::pair<int, int> >::iterator mapDelayIter = pPlayer->m_mapFaceLiftDelayInfo.find( tempFaceInfo.iFaceType );
                if( mapDelayIter == pPlayer->m_mapFaceLiftDelayInfo.end() )
                {
                    // 스크립트에 딜레이 정보가 있으면 딜레이를 걸자
                    if( ((0 < tempFaceInfo.iDelayCount) || (0 < tempFaceInfo.iDelayFrame)) )
                    {
                        pPlayer->m_mapFaceLiftDelayInfo.insert( std::make_pair(tempFaceInfo.iFaceType, std::make_pair(tempFaceInfo.iDelayCount, tempFaceInfo.iDelayFrame)) );
                    }
                    bChangeFace = true;
                }
                else
                {
                    // 딜레이 카운터처리 : 1회 교체시 1씩 감소
                    if( 0 < mapDelayIter->second.first )
                    {
                        --mapDelayIter->second.first;
                        bChangeFace = true;
                    }
                }

                if( bChangeFace )
                    pPlayer->SetRenderFaceLift(tempFaceInfo.iFaceType );
            } else {
                std::stringstream stmString;
                stmString << tempFaceInfo.sExtraMeshName;
                pPlayer->ToggleExtraMesh( stmString.str().c_str(), true );
            } 
        }
    }
}