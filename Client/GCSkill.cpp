#include "stdafx.h"
#include ".\gcskill.h"
#include "Player.h"
//

GCSkill::GCSkill(void)
: m_eID((EGCSkillTree)-1)
, m_eGroup(SG_MAX)
, m_nCharLv(0)
, m_eCharType(GC_CHAR_NUM)
, m_nPromotion(-1)
, m_bEnable(false)
, m_strTexName("")
, m_strAddMotionTexName("")
, m_nTitleID(-1)
, m_nDescID(-1)
, m_nControlDescID( -1 )
, m_nLearnSP(1)
, m_bBase(false)
, m_bCash(false)
, m_nLockGroup(-1)
, m_pHotKeySkill( NULL )
, m_bAccSkill(false)
, m_bPassive(false)
, m_bSpecialTypeB(false)
, m_iCurSkillLv( -1 )
, m_iMaxSkillLv( -1 )
, m_iAwakeningLv( -1 )
{
    clear();
}

GCSkill::~GCSkill(void)
{
    clear();
}

bool GCSkill::Load( KLuaManager& luaMgr )
{
    int nTempVal;
    LUA_GET_VALUE( "ID", nTempVal, return false );
    m_eID = (EGCSkillTree)nTempVal;
    LUA_GET_VALUE( "GroupID", nTempVal, return false );
    m_eGroup = (EGCSkillGroup)nTempVal;

    LUA_BEGIN_TABLE( "PreList", return false )
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setPrevID.insert( (EGCSkillTree)nTempVal );
        }
    }
    LUA_END_TABLE( return false );

    LUA_GET_VALUE_DEF( "OrCheck_PreList", m_bOrCheckPreList, false );

    LUA_BEGIN_TABLE( "NextList", return false )
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setNextID.insert( (EGCSkillTree)nTempVal );
        }
    }
    LUA_END_TABLE( return false );

    LUA_BEGIN_TABLE_RAII( "LimitList" )
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setLimitID.insert( (EGCSkillTree)nTempVal );
        }
    }

    LUA_BEGIN_TABLE( "GoodsID", return false )
    {
        for( int i = 1; ; ++i )
        {
            LUA_GET_VALUE_NOASSERT( i, nTempVal, break );
            m_setGoodsID.insert( nTempVal );
        }
    }
    LUA_END_TABLE( return false );

    LUA_GET_VALUE( "Base", m_bBase, return false );
    LUA_GET_VALUE( "CharLv", m_nCharLv, return false );
    LUA_GET_VALUE( "CharType", nTempVal, return false );
    m_eCharType = (EGCCharType)nTempVal;
    LUA_GET_VALUE( "Promotion", m_nPromotion, return false );
    LUA_GET_VALUE( "Enable", m_bEnable, return false );
    
    char Skillimg[MAX_PATH];
    sprintf( Skillimg, "SkillIcon%d.dds", (int)m_eID );
    m_strTexName = Skillimg;
    luaMgr.GetValue( "Img", m_strTexName );
    luaMgr.GetValue( "AddMotionImg", m_strAddMotionTexName );
    if ( m_strAddMotionTexName == "")
        m_strAddMotionTexName = m_strTexName;
    LUA_GET_VALUE( "TitleID", m_nTitleID, return false );
    LUA_GET_VALUE( "DescID", m_nDescID, return false );
	LUA_GET_VALUE( "ControlDesc", m_nControlDescID, return false );
	LUA_GET_VALUE( "LearnSP", m_nLearnSP, return false );
	LUA_GET_VALUE_DEF( "Cash", m_bCash, false );

	luaMgr.GetValue( "UI_Pos_x", m_sUIinfo.x );
	luaMgr.GetValue( "UI_Pos_y", m_sUIinfo.y );
	int line;
	m_sUIinfo.setLineType.clear();
	if( luaMgr.BeginTable( "UI_LineType" ) == S_OK )
	{		
		for ( int i = 1; luaMgr.GetValue( i++, line ) == S_OK; m_sUIinfo.setLineType.insert( line ) );		
		luaMgr.EndTable();
	}
	else
	{
		if( luaMgr.GetValue( "UI_LineType", line ) == S_OK )
			m_sUIinfo.setLineType.insert( line );
	}	


    luaMgr.GetValue( "LockGroup", m_nLockGroup );

	luaMgr.GetValue( "Overlapped", m_bAccSkill );
	luaMgr.GetValue( "Passive", m_bPassive );

    luaMgr.GetValue( "SpecialTypeB", m_bSpecialTypeB );
    luaMgr.GetValue( "SkillLv", m_iCurSkillLv );
    luaMgr.GetValue( "MaxSkillLv", m_iMaxSkillLv );

    luaMgr.GetValue( "AwakeningLv", m_iAwakeningLv );
	

	if( luaMgr.BeginTable( "SlotSkill" ) == S_OK )
	{
		delete m_pHotKeySkill;
		m_pHotKeySkill = new GCHotKeySkillInfo();
		luaMgr.GetValue( "Mana", m_pHotKeySkill->m_fMana );
		luaMgr.GetValue( "Motion", m_pHotKeySkill->m_iMotion );
        luaMgr.GetValue("MonsterModeMotion", m_pHotKeySkill->m_iMonsterModeMotion);
        luaMgr.GetValue( "AddMotion", m_pHotKeySkill->m_iAddMotion );
        luaMgr.GetValue( "MotionWhenCooltime", m_pHotKeySkill->m_iMotionWhenCooltime );
        luaMgr.GetValue( "ManaMotionWhenCooltime", m_pHotKeySkill->m_fManaMotionWhenCooltime );
		luaMgr.GetValue( "CoolTime", m_pHotKeySkill->m_fCoolTime );
		luaMgr.GetValue( "MonsterModeMana", m_pHotKeySkill->m_fMonsterModeMana );
        luaMgr.GetValue( "MonsterModeCoolTime", m_pHotKeySkill->m_fMonsterModeCoolTime );
        luaMgr.GetValue( "ForceUseSkill", m_pHotKeySkill->m_bForceUseSkill );        
        luaMgr.GetValue( "IsSpecialSkill", m_pHotKeySkill->m_bSpecialSkill );

        LUA_GET_VALUE_DEF( "ChargingSkillLV", m_pHotKeySkill->m_iChargingSkillLV, 0 );   
       
        luaMgr.GetValue( "AuraSizeWidht", m_pHotKeySkill->m_fAuraSizeWidth );
        luaMgr.GetValue( "AuraSizeHeight", m_pHotKeySkill->m_fAuraSizeHeight );

        luaMgr.GetValue( "BuffIndex", m_pHotKeySkill->m_iBuffIndex );
        luaMgr.GetValue( "BuffTime", m_pHotKeySkill->m_fBuffTime );

        luaMgr.GetValue( "NoEquipSlot", m_pHotKeySkill->m_bNoEquipSlot );

        luaMgr.GetValue( "CheckCoolTimeBuff", m_pHotKeySkill->m_iCheckCoolTimeBuff );

        luaMgr.GetValue( "CheckSpecialMotion", m_pHotKeySkill->m_bCheckSpecialMotion );

        luaMgr.GetValue( "NoUseFatal", m_pHotKeySkill->m_bNoUseFatal );

        m_pHotKeySkill->m_vecAuraBuff.clear();
        
        LUA_BEGIN_TABLE_RAII( "PossibleUseJump" )
        {
            luaMgr.GetValue( "IsPossible", m_pHotKeySkill->m_kPossibleUseJump.m_bPossible );
            luaMgr.GetValue( "PrecedingSkillID", m_pHotKeySkill->m_kPossibleUseJump.m_bPrecedingSID );
        }

        LUA_BEGIN_TABLE_RAII( "AuraBuff" ) 
        {                    
            for( int j = 1; ; ++j ) {
                int iValue = 0;
                if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                    break;
                }
                m_pHotKeySkill->m_vecAuraBuff.push_back( iValue );
            }
        }         

        m_pHotKeySkill->m_vecAuraDeBuff.clear();
        LUA_BEGIN_TABLE_RAII( "AuraDeBuff" ) 
        {                    
            for( int j = 1; ; ++j ) {
                int iValue = 0;
                if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                    break;
                }
                m_pHotKeySkill->m_vecAuraDeBuff.push_back( iValue );
            }
        }         

        m_pHotKeySkill->m_vecComboSkill.clear();
        LUA_BEGIN_TABLE_RAII( "ComboSkill" )
        {
            int iLoop = 1;
            GCComboSkill kComboSkillInfo;
            while(1)
            {
                LUA_BEGIN_TABLE_RAII( iLoop++ )
                {
                    luaMgr.GetValue( 1, kComboSkillInfo.m_iSID );
                    luaMgr.GetValue( 2, kComboSkillInfo.m_iStartKeyCoolTime );
                    luaMgr.GetValue( 3, kComboSkillInfo.m_iEndKeyCoolTime );
                    luaMgr.GetValue( 4, kComboSkillInfo.m_iMotionChangeFrame );
                    m_pHotKeySkill->m_vecComboSkill.push_back( kComboSkillInfo );
                }
                else break;
            }
        }

		luaMgr.EndTable();
	}
    return true;
}

void GCSkill::clear()
{
    m_lPrev.clear();
    m_lNext.clear();
    m_setPrevID.clear();
    m_setNextID.clear();
    m_setLimitID.clear();
    m_setGoodsID.clear();

	delete m_pHotKeySkill;
	m_pHotKeySkill = NULL;
}

namespace 
{
    class IsEquipItem
    {
    public:
        IsEquipItem( SCharInfo& kCharInfo )
        : m_kCharInfo(kCharInfo)
        , m_bFind(false) {}
        ~IsEquipItem() {}

        bool operator() ( const int& goodsID )
        {
            if( true == m_bFind )
                return m_bFind;

            m_bFind = m_kCharInfo.vecItemInfo.end() != std::find_if( m_kCharInfo.vecItemInfo.begin(), 
                m_kCharInfo.vecItemInfo.end(), boost::bind( &SUserItem::iItemID, _1 ) == goodsID );
            return m_bFind;
        }

        SCharInfo& m_kCharInfo;
        bool m_bFind;
    };
}

bool GCSkill::CheckItem( PLAYER* pPlayer )
{
    //if( !pPlayer->IsLocalPlayer() )
    //{
    //    return false;
    //}

    if( m_setGoodsID.empty() )
    {
        return true;
    }

    return std::for_each( m_setGoodsID.begin(), m_setGoodsID.end(), IsEquipItem( pPlayer->GetCurrentChar() ) ).m_bFind;
}


#ifndef __PATH__
bool GCSkill::LoadUI( KLuaManager& luaMgr )
{
	LUA_GET_VALUE( "UI_Pos_x", m_sUIinfo.x, return false );
	LUA_GET_VALUE( "UI_Pos_y", m_sUIinfo.y, return false );
	int line;
	m_sUIinfo.setLineType.clear();
	if( luaMgr.BeginTable( "UI_LineType" ) == S_OK )
	{		
		for ( int i = 1; luaMgr.GetValue( i++, line ) == S_OK; m_sUIinfo.setLineType.insert( line ) );		
		luaMgr.EndTable();
	}
	else
	{
		if( luaMgr.GetValue( "UI_LineType", line ) == S_OK )
			m_sUIinfo.setLineType.insert( line );
	}	

	return true;
}
#endif
