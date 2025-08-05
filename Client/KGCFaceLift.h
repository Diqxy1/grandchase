#pragma once

#include "KNCSingleton.h"
class KLuaManager;

class KGCFaceLift;
struct KGCFaceLiftState;

struct FACE_FRAME
{
    int iMinFrame;
    int iMaxFrame;

    FACE_FRAME()
    {
        iMinFrame = 0;
        iMaxFrame = 0;
    }

public:
    bool operator < ( const FACE_FRAME& right ) const
    {
        if( iMinFrame < right.iMinFrame )       return true;
        return false;
    }

private:
    bool operator == ( const FACE_FRAME& right ) const;
    bool operator >= ( const FACE_FRAME& right ) const;
    bool operator <= ( const FACE_FRAME& right ) const;
    bool operator >  ( const FACE_FRAME& right ) const;
};

struct FACE_INFO
{
    int iFaceType;
    int iDelayCount;
    int iDelayFrame;
    std::string sExtraMeshName;

    FACE_INFO()
    {
        iFaceType = -1;
        iDelayCount = -1;
        iDelayFrame = 0;
        sExtraMeshName = "";
    }
};

class KGCFaceLift
{
	DeclareSingleton( KGCFaceLift );
public:
	KGCFaceLift();
	~KGCFaceLift();
	void LoadFromLuaScript( char* strFilename_ );
    std::map<FACE_FRAME, FACE_INFO> LoadScriptFrameInfo( KLuaManager &luaMgr );

public:
    void SetFaceLiftByMotionName( PLAYER* pPlayer, std::string strFileaName, int iFrame = -1 );
    void SetFaceLiftByMotionEnum( PLAYER* pPlayer, int iMotionEnum, int iFrame = -1 );
    void SetCurMotionName( std::string strName ) { m_strCurMotionName = strName; }
    std::string GetCurMotionName() { return m_strCurMotionName; }

public:
    void ProcessingDelay( PLAYER *pPlayer );
    void ProcessingChangeFace( std::map< FACE_FRAME, FACE_INFO > &mapFaceLiftInfo, PLAYER *pPlayer, int iCurFrame );

public:
#if !defined( __PATH__ )
    void CheckFileMonitor();
    void RegisterFileMoniter();
#endif

private:
    // Key : ( 모션Enum ), Date : map( 모션프레임, 얼굴타입 )
    std::map< int, map< FACE_FRAME, FACE_INFO > > m_mapCommonFaceLiftInfo;
    // Key : ( 캐릭터타입, 모션Enum ), Date : map( 모션프레임, 얼굴타입 )
    std::map< std::pair< int, int >, map< FACE_FRAME, FACE_INFO > > m_mapPlayFaceLiftInfo;
    // Key : ( 캐릭터타입, 모션파일이름 ), Date : map( 모션프레임, 얼굴타입 )
    std::map< std::pair< int, std::string >, map< FACE_FRAME, FACE_INFO > > m_mapFaceLiftInfo; 
    std::string m_strCurMotionName;
};

DefSingletonInline( KGCFaceLift );