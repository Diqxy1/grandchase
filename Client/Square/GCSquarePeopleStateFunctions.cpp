#include "stdafx.h"
#include ".\gcsquarepeoplestatefunctions.h"
#include "gcsquarepeoplecommon.h"
#include "gcsquarepeopleelesis.h"
#include "../KGCSocialMotionManager.h"
//

//////////////////////////////////////////////////////////////////////////
//	20091026 광장 세트모션 아이디 enum값 얻어오기 위한 포함 : 이동석
#include "GCGraphicsHelper.h"
//////////////////////////////////////////////////////////////////////////

#define SETFUNC(p,q) do\
                     {\
                        if( ms_mapStateFunction.count(p) != 0 )\
                        {\
                            ASSERT( !"Already Exist State" );\
                        }\
                        ms_mapStateFunction[p] = q;\
                     } while(0)

#define SETFUNCREMOTE(p,q) do\
                             {\
                                if( ms_mapStateFunctionRemote.count(p) != 0 )\
                                {\
                                    ASSERT( !"Already Exist RemoteState" );\
                                }\
                                ms_mapStateFunctionRemote[p] = q;\
                             } while(0)

std::map<MOTIONID, GCSQUARESTATEFUNCTION> GCSquarePeopleStateFunctions::ms_mapStateFunction;
std::map<MOTIONID, GCSQUARESTATEFUNCTIONREMOTE> GCSquarePeopleStateFunctions::ms_mapStateFunctionRemote;
std::map<char, UINT> GCSquarePeopleStateFunctions::ms_mapWaitCharMotion;
std::map<char, UINT> GCSquarePeopleStateFunctions::ms_mapWalkCharMotion;
std::map<char, UINT> GCSquarePeopleStateFunctions::ms_mapJumpReadyCharMotion;
std::map<char, UINT> GCSquarePeopleStateFunctions::ms_mapJumpCharMotion;
std::map<char, UINT> GCSquarePeopleStateFunctions::ms_mapDashCharMotion;

//------------------------------------------------------------------------
// 20091023 광장 세트 아이템 모션 컨테이너 : 이동석
std::map<int, UINT> GCSquarePeopleStateFunctions::ms_mapSquareWaitCharMotion;
std::map<int, UINT> GCSquarePeopleStateFunctions::ms_mapSquareWalkCharMotion;
std::map<int, UINT> GCSquarePeopleStateFunctions::ms_mapSquareJumpReadyCharMotion;
std::map<int, UINT> GCSquarePeopleStateFunctions::ms_mapSquareJumpCharMotion;
std::map<int, UINT> GCSquarePeopleStateFunctions::ms_mapSquareDashCharMotion;

std::map<int, std::set<int>> GCSquarePeopleStateFunctions::ms_mapMotionFunc;

GCSquarePeopleStateFunctions::GCSquarePeopleStateFunctions(void)
{    
}

GCSquarePeopleStateFunctions::~GCSquarePeopleStateFunctions(void)
{
}

void GCSquarePeopleStateFunctions::SetStateFunstions()
{
    ms_mapMotionFunc.clear();

    SetSquarePeppleMotion();

    SetWaitFunc();
    SetWalkFunc();
    SetJumpFunc();
    SetJumpReadyFunc();
    SetDashFunc();

    SetWaitRemoteFunc();
    SetWalkRemoteFunc();
    SetJumpRemoteFunc();
    SetJumpRemoteReadyFunc();
    SetDashRemoteFunc(); 

    SetSocialMotionFunc();

	//------------------------------------------------------------------------
	//	20091026 광장 세트 아이템 모션 : 이동석
	//SetSquareMotion();
}

void GCSquarePeopleStateFunctions::SetSquarePeppleMotion()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr , "MotionEnum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "SquarePeopleStateMotion.stg" ) == false )
        return;

    std::set<int> setMotionIndex;

    LUA_BEGIN_TABLE( "WaitMotion", return )
    {        
        setMotionIndex.clear();
        for( int i = 1 ; ; ++i )
        {
            int iCharClass;
            int iMotionIndex = 0;            
            LUA_BEGIN_TABLE(i, break )
            {
                luaMgr.GetValue( 1, iCharClass );
                luaMgr.GetValue( 2, iMotionIndex );
            }
            LUA_END_TABLE( break );
            ms_mapWaitCharMotion[ iCharClass ] = iMotionIndex;            
            setMotionIndex.insert(iMotionIndex);
        }        
        ms_mapMotionFunc.insert(std::map<int, std::set<int>>::value_type( EWAIT_MOTION, setMotionIndex));
    }
    LUA_END_TABLE( return );    

    LUA_BEGIN_TABLE( "WalkMotion", return )
    {
        setMotionIndex.clear();
        for( int i = 1 ; ; ++i )
        {
            int iCharClass;
            int iMotionIndex = 0;
            LUA_BEGIN_TABLE(i, break )
            {
                luaMgr.GetValue( 1, iCharClass );
                luaMgr.GetValue( 2, iMotionIndex );
            }
            LUA_END_TABLE( break );
            ms_mapWalkCharMotion[ iCharClass ] = iMotionIndex;
            setMotionIndex.insert(iMotionIndex);
        }
        ms_mapMotionFunc.insert(std::map<int, std::set<int>>::value_type( EWALK_MOTION, setMotionIndex));
    }
    LUA_END_TABLE( return );

    LUA_BEGIN_TABLE( "JumpReadyMotion", return )
    {
        setMotionIndex.clear();
        for( int i = 1 ; ; ++i )
        {
            int iCharClass;
            int iMotionIndex = 0;
            LUA_BEGIN_TABLE(i, break )
            {
                luaMgr.GetValue( 1, iCharClass );
                luaMgr.GetValue( 2, iMotionIndex );
            }
            LUA_END_TABLE( break );
            ms_mapJumpReadyCharMotion[ iCharClass ] = iMotionIndex;
            setMotionIndex.insert(iMotionIndex);
        }
        ms_mapMotionFunc.insert(std::map<int, std::set<int>>::value_type( EJUMP_READY_MOTION, setMotionIndex));
    }
    LUA_END_TABLE( return );

    LUA_BEGIN_TABLE( "JumpMotion", return )
    {
        setMotionIndex.clear();
        for( int i = 1 ; ; ++i )
        {
            int iCharClass;
            int iMotionIndex = 0;
            LUA_BEGIN_TABLE(i, break )
            {
                luaMgr.GetValue( 1, iCharClass );
                luaMgr.GetValue( 2, iMotionIndex );
            }
            LUA_END_TABLE( break );
            ms_mapJumpCharMotion[ iCharClass ] = iMotionIndex;
            setMotionIndex.insert(iMotionIndex);
        }
        ms_mapMotionFunc.insert(std::map<int, std::set<int>>::value_type( EJUMP_MOTION, setMotionIndex));
    }
    LUA_END_TABLE( return );

    LUA_BEGIN_TABLE( "DashMotion", return )
    {
        setMotionIndex.clear();
        for( int i = 1 ; ; ++i )
        {
            int iCharClass;
            int iMotionIndex = 0;
            LUA_BEGIN_TABLE(i, break )
            {
                luaMgr.GetValue( 1, iCharClass );
                luaMgr.GetValue( 2, iMotionIndex );
            }
            LUA_END_TABLE( break );
            ms_mapDashCharMotion[ iCharClass ] = iMotionIndex;
            setMotionIndex.insert(iMotionIndex);
        }
        ms_mapMotionFunc.insert(std::map<int, std::set<int>>::value_type( EDASH_MOTION, setMotionIndex));
    }
    LUA_END_TABLE( return );

    setMotionIndex.clear();
    if( SiKGCSocialMotionManager()->GetSocialMotion( setMotionIndex ) ) {
        ms_mapMotionFunc.insert(std::map<int, std::set<int>>::value_type( ESOCIAL_MOTION, setMotionIndex));
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이제 광장모션을 이 밑 함수들에다가 셋팅하지말고 \Stage\Char_Script.kom\SquarePeopleStateMotion.stg 파일안에다가 셋팅하자!! //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCSquarePeopleStateFunctions::SetWaitFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EWAIT_MOTION);
    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch(*sit)
            {
            case MID_ARME_WAIT_AND_FATAL:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme::OnWait );
                continue;
            case MID_ARME2_WAIT_AND_FATAL:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme2::OnWait );
                continue;
            case MID_ARME4_WAIT:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme4::OnWait );
                continue;
            default:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis::OnWait );
                continue;
            }
        }
    }
}

void GCSquarePeopleStateFunctions::SetWalkFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EWALK_MOTION);
    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
            SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis::OnWalk );
    }
}

void GCSquarePeopleStateFunctions::SetJumpFunc()
{             
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EJUMP_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch(*sit)
            {
            case MID_ELESIS_JUMP:
            case MID_ELESIS2_JUMP:
            case MID_LIRE_JUMP:
            case MID_LIRE2_JUMP:
            case MID_ARME_JUMP:
            case MID_ARME2_JUMP:
            case MID_LAS_JUMP:
            case MID_LAS2_JUMP:
            case MID_LAS4_JUMP:
            case MID_RYAN2_JUMP:
            case MID_RONAN3_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis::OnJump );
                continue;
            case MID_ELESIS3_JUMP:
            case MID_ELESIS4_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis3::OnJump );
                continue;
            case MID_LIRE4_JUMP:
            case MID_RYAN3_JUMP:
            case MID_RYAN4_JUMP:
            case MID_RONAN_JUMP:
            case MID_RONAN2_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleLire4::OnJump );
                continue;
            case MID_LIRE3_JUMP:
            case MID_RYAN_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleLire3::OnJump );
                continue;
            case MID_ARME3_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme3::OnJump );
                continue;
            case MID_ARME4_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme4::OnJump );
                continue;
            case MID_LAS3_JUMP:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleLas3::OnJump );
                continue;            
            default:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleCommon::OnJump );
                continue;
            }
        }
    }
}

void GCSquarePeopleStateFunctions::SetJumpReadyFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EJUMP_READY_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch(*sit)
            {
            case MID_ELESIS4_JUMP_READY:
            case MID_ARME3_JUMPREADY:
            case MID_LIRE3_JUMPREADY:
            case MID_LAS3_JUMPREADY:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis4::OnJumpReady );
                continue;
            case MID_ARME_JUMPREADY:
            case MID_ARME4_JUMP_READY:
            case MID_RYAN_JUMP_READY:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme::OnJumpReady );
                continue;
            case MID_LAS_JUMPREADY:
            case MID_LAS4_JUMP_READY:
            case MID_JIN2_JUMP_READY:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleLas::OnJumpReady );
                continue;
            case MID_RYAN3_JUMP_READY:
            case MID_RONAN2_JUMP_READY:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleRyan3::OnJumpReady );
                continue;           
            case MID_ARME2_JUMPREADY:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleArme2::OnJumpReady );
                continue;
            case MID_RYAN2_JUMP_READY:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleRyan2::OnJumpReady );
                continue;
            default:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis::OnJumpReady );
                continue;
            }
        }
    }
}

void GCSquarePeopleStateFunctions::SetDashFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EDASH_MOTION);
    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch(*sit)
            {
            case MID_LEY1_SQUARE_DASH:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleLey1::OnDash );
                continue;
            default:
                SETFUNC( (MOTIONID)(*sit), GCSquarePeopleElesis::OnDash );
                continue;
            }
        }
    }
}

void GCSquarePeopleStateFunctions::SetWaitRemoteFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EWAIT_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch(*sit)
            {
            case MID_ARME_WAIT_AND_FATAL:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleArme::OnWaitRemote );
                continue;
            case MID_ARME2_WAIT_AND_FATAL:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleArme2::OnWaitRemote );
                continue;
            case MID_ARME4_WAIT:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleArme4::OnWaitRemote );
                continue;
            default:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleCommon::OnWaitRemote );
                continue;
            }
        }
    }
}

void GCSquarePeopleStateFunctions::SetWalkRemoteFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EWALK_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
            SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleCommon::OnWalkRemote );
    }
}

void GCSquarePeopleStateFunctions::SetJumpRemoteFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EJUMP_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch(*sit)
            {
            case MID_LIRE4_JUMP:
            case MID_RYAN3_JUMP:
            case MID_RYAN4_JUMP:
            case MID_RONAN_JUMP:
            case MID_RONAN2_JUMP:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleLire4::OnJumpRemote );
                continue;
            case MID_ELESIS3_JUMP:
            case MID_ELESIS4_JUMP:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleElesis3::OnJumpRemote );
                continue;
            case MID_LIRE3_JUMP:
            case MID_RYAN_JUMP:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleLire3::OnJumpRemote );
                continue;
            case MID_ARME3_JUMP:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleArme3::OnJumpRemote );
                continue;
            case MID_ARME4_JUMP:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleArme4::OnJumpRemote );
                continue;
            case MID_LAS3_JUMP:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleLas3::OnJumpRemote );
                continue;
            //case MID_LEY1_JUMP:
            //    SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleLey1::OnJumpRemote );
            //    continue;
            default:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleElesis::OnJumpRemote );
                continue;
            }
        }
    }   
}

void GCSquarePeopleStateFunctions::SetJumpRemoteReadyFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EJUMP_READY_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
            SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleCommon::OnJumpReadyRemote );
    }
}

void GCSquarePeopleStateFunctions::SetDashRemoteFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(EDASH_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit )
        {
            switch( *sit )
            {
            case MID_LEY1_SQUARE_DASH:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleLey1::OnDashRemote );
                continue;
            default:
                SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleElesis::OnDashRemote );
                continue;
            }
        }
    }
}


//------------------------------------------------------------------------
//	20091026 광장 모션은 여기서 일괄 처리
//========================================================================
void GCSquarePeopleStateFunctions::SetSquareMotion()
{
	SETFUNC( MID_SETITEM_JOMBIE_WAIT,				GCSquarePeopleElesis::OnWait );
	SETFUNC( MID_SETITEM_JOMBIE_WALK,				GCSquarePeopleElesis::OnWalk );
	SETFUNC( MID_SETITEM_JOMBIE_JUMP,				GCSquarePeopleElesis::OnJump );
	SETFUNC( MID_SETITEM_JOMBIE_JUMP_READY,			GCSquarePeopleElesis::OnJumpReady );
	SETFUNC( MID_SETITEM_JOMBIE_DASH,				GCSquarePeopleElesis::OnDash );

	SETFUNCREMOTE( MID_SETITEM_JOMBIE_WAIT,			GCSquarePeopleCommon::OnWaitRemote );
	SETFUNCREMOTE( MID_SETITEM_JOMBIE_WALK,			GCSquarePeopleCommon::OnWalkRemote );
	SETFUNCREMOTE( MID_SETITEM_JOMBIE_JUMP,			GCSquarePeopleElesis::OnJumpRemote );
	SETFUNCREMOTE( MID_SETITEM_JOMBIE_JUMP_READY,	GCSquarePeopleCommon::OnJumpReadyRemote );
	SETFUNCREMOTE( MID_SETITEM_JOMBIE_DASH,			GCSquarePeopleElesis::OnDashRemote );
}

void GCSquarePeopleStateFunctions::SetSocialMotionFunc()
{
    std::map<int, std::set<int>>::iterator mit = ms_mapMotionFunc.find(ESOCIAL_MOTION);

    if( mit != ms_mapMotionFunc.end() )
    {
        std::set<int>::iterator sit = mit->second.begin();
        for( sit; sit != mit->second.end(); ++sit ) {
            SETFUNC( (MOTIONID)(*sit), GCSquarePeopleCommon::OnSocialMotion );
            SETFUNCREMOTE( (MOTIONID)(*sit), GCSquarePeopleCommon::OnSocialMotionRemote );
        }
    }    
}