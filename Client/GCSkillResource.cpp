#include "stdafx.h"
#include ".\gcskillresource.h"
//

#include "DamageManager.h"



//


GCSkillResource::GCSkillResource(void)
: m_eID((EGCSkillTree)0)
, m_pSkill(NULL)
{
    m_setMotionID.clear();
    m_setFireInfo.clear();
    m_setSoundID.clear();
    m_vecAnimation.clear();
}

GCSkillResource::~GCSkillResource(void)
{
    m_pSkill = NULL;
    m_setMotionID.clear();
    m_setFireInfo.clear();
    m_setSoundID.clear();
}

bool GCSkillResource::Load( KLuaManager& luaMgr )
{
    int nTempVal;
    LUA_GET_VALUE( "ID", nTempVal, return false );
    m_eID = (EGCSkillTree)nTempVal;

	if( SUCCEEDED ( luaMgr.BeginTable( "MotionID" ) ) )    
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setMotionID.insert( (MOTIONID)nTempVal );
        }
    LUA_END_TABLE( return false );
    }    

    if( SUCCEEDED ( luaMgr.BeginTable( "FireInfo" ) ) )    
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setFireInfo.insert( (DAMAGE_TYPE)nTempVal );
        }
		LUA_END_TABLE( return false );
    }
    

	if( SUCCEEDED ( luaMgr.BeginTable( "Sound" ) ) )    
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setSoundID.insert( nTempVal );
        }
		LUA_END_TABLE( return false );
    }
    

    std::string tempStr;
	if( SUCCEEDED ( luaMgr.BeginTable( "Animation" ) ) )
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, tempStr, break );
            m_vecAnimation.push_back( tempStr );
        }
    LUA_END_TABLE( return false );
    }
    return true;
}

void GCSkillResource::LoadRes( DWORD dwUID )
{
    assert( m_pSkill != NULL );

	int iPlayerIndex = g_MyD3D->Get_Player( dwUID );

	char ExtraCharNum;

    std::set< MOTIONID >::iterator sit = m_setMotionID.begin();
    while( sit != m_setMotionID.end() )
    {
		if ( m_pSkill->GetPromotion() == -1 )
		{
			for ( int i = 0; i < (CID_MAX / GC_CHAR_NUM); ++i )
			{
				g_pGraphicsHelper->LoadPlayerActionMotion(  m_pSkill->GetCharType() + i * GC_CHAR_NUM, (*sit) );
			}
		}
		else
		{
			ExtraCharNum = m_pSkill->GetCharType() + m_pSkill->GetPromotion() * GC_CHAR_NUM;
			// [8/1/2008] breadceo. 이렇게 하면 안된다!!! 근데 일정이..
			g_pGraphicsHelper->LoadPlayerActionMotion( ExtraCharNum, (*sit) );
		}

        ++sit;
    }

    if( iPlayerIndex != 7 )
    {
        std::set< DAMAGE_TYPE >::iterator sit = m_setFireInfo.begin();
        while( sit != m_setFireInfo.end() )
        {
            g_MyD3D->m_pDamageManager->CreateMeshAnimationForPrevLoad( (int)(*sit), iPlayerIndex );
            ++sit;
        }
    }

    std::for_each( m_setSoundID.begin(), m_setSoundID.end(),
        boost::bind( &KDSound::PreparetoLoad, boost::ref(g_KDSound), _1 ) );

    PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwUID );
    if( pPlayer != NULL )
    {
        pPlayer->InitSkillAnimation( m_vecAnimation );
    }

    START_LOG( clog, L"스킬 리소스 로드 " << m_eID );
}

void GCSkillResource::ReleaseRes( DWORD dwUID )
{
    UNREFERENCED_PARAMETER(dwUID);

    START_LOG( clog, L"스킬 리소스 해제 " << m_eID );
}

void GCSkillResource::SetSkill( GCSkill* pSkill )
{ 
	m_pSkill = pSkill;
	m_eID = pSkill->GetID();

	if( m_pSkill->m_pHotKeySkill )
	{
		m_setMotionID.insert( (MOTIONID)m_pSkill->m_pHotKeySkill->m_iMotion );
	}
}

void GCSkillResource::GetMotionID( std::set< MOTIONID >& setMotionID_ )
{
    setMotionID_ = m_setMotionID;
}
