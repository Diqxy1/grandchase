#include "stdafx.h"
// KGCAnimManager.cpp: implementation of the KGCAnimManager class.
//
//////////////////////////////////////////////////////////////////////

//
//
#include "../MyD3D.h"

#include "KGCAnimManager.h"
//
//

#include "GCStateMachine.h"
#include "KGCRoomManager.h"

KGCAnimManager   g_AnimManager;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCAnimManager::KGCAnimManager()
{
    m_pKTDGAnim = NULL;
}

KGCAnimManager::~KGCAnimManager()
{
    DeleteDeviceObjects();
}

void KGCAnimManager::LoadFromLuaScript( char* stringfile, char* filename )
{
    HRESULT     hr;
    int         i;
    std::string temp;
    
	KLuaManager     luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, stringfile ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    if( GCFUNC::LoadLuaScript( luaMgr, filename ) == false )
        return;

    luaMgr.BeginTable( "ITEM_ANIM" );
    {
        for( i=1 ; ; ++i )
        {
            ItemAnim    itemAnim;
            
            if( luaMgr.BeginTable( i ) == E_FAIL )
				break;

			luaMgr.BeginTable( "ITEM_NAME" );
			{
				for ( UINT j=1 ; ; ++j )
				{
					std::string strItemName;
					if ( SUCCEEDED( hr = luaMgr.GetValue( j, strItemName ) ) )
						itemAnim.itemName.push_back( strItemName );
					else 
						break;
				}
			}
			luaMgr.EndTable();

            luaMgr.GetValue( "SEQUENCE_NAME",    itemAnim.sequenceName );
            itemAnim.animSeq = m_pKTDGAnim->GetSequence( itemAnim.sequenceName );
            
            if( FAILED( luaMgr.GetValue( "PLAY_TYPE",        temp ) ))
            {
                itemAnim.playType = CKTDGAnim::PT_LOOP;
            }
            else
            {
                if( temp.compare( "LOOP" ) == 0 )
                    itemAnim.playType = CKTDGAnim::PT_LOOP;
                else if( temp.compare( "COUNTER_LOOP" ) == 0 )
                    itemAnim.playType = CKTDGAnim::PT_COUNTER_LOOP;
                else if( temp.compare( "PT_PING_PONG" ) == 0 )
                    itemAnim.playType = CKTDGAnim::PT_PING_PONG;
                else
                    itemAnim.playType = CKTDGAnim::PT_LOOP;
            }
            LUA_GET_VALUE_DEF( "POSX_MIN",         itemAnim.posX.m_Min,     0.0f );
            LUA_GET_VALUE_DEF( "POSX_MAX",		    itemAnim.posX.m_Max,	0.0f );
            LUA_GET_VALUE_DEF( "POSY_MIN",		    itemAnim.posY.m_Min,	0.0f );
            LUA_GET_VALUE_DEF( "POSY_MAX",		    itemAnim.posY.m_Max,	0.0f );
            LUA_GET_VALUE_DEF( "POSZ_MIN",		    itemAnim.posZ.m_Min,	0.0f );
            LUA_GET_VALUE_DEF( "POSZ_MAX",		    itemAnim.posZ.m_Max,	0.0f );
            LUA_GET_VALUE_DEF( "SCALEX_MIN",		itemAnim.scaleX.m_Min,	0.0018f );
            LUA_GET_VALUE_DEF( "SCALEX_MAX",		itemAnim.scaleX.m_Max,	0.0018f );
            LUA_GET_VALUE_DEF( "SCALEY_MIN",		itemAnim.scaleY.m_Min,	0.0018f );
            LUA_GET_VALUE_DEF( "SCALEY_MAX",		itemAnim.scaleY.m_Max,	0.0018f );
            LUA_GET_VALUE_DEF( "ROTX_MIN",		    itemAnim.rotX.m_Min,	0.0f );
            LUA_GET_VALUE_DEF( "ROTX_MAX",		    itemAnim.rotX.m_Max,	0.0f );	
            LUA_GET_VALUE_DEF( "ROTY_MIN",		    itemAnim.rotY.m_Min,	0.0f );
            LUA_GET_VALUE_DEF( "ROTY_MAX",		    itemAnim.rotY.m_Max,	0.0f );
            LUA_GET_VALUE_DEF( "ROTZ_MIN",		    itemAnim.rotZ.m_Min,	0.0f );
            LUA_GET_VALUE_DEF( "ROTZ_MAX",		    itemAnim.rotZ.m_Max,	0.0f );
            LUA_GET_VALUE_DEF( "POSITION_INDEX",	itemAnim.positionIndex,	0 );

            LUA_GET_VALUE_DEF( "POSITION_TYPE",		temp,	"BODY_CENTER" );
			itemAnim.positionType = ParsePosition(temp);

			int iCharID;
            LUA_GET_VALUE_DEF( "CHAR_IDENTY",      iCharID, (CHAR_IDENTY)CID_COMMON );
			itemAnim.charID = (CHAR_IDENTY)iCharID;

            LUA_GET_VALUE_DEF( "ENCHANT_LEVEL",      itemAnim.enchantLevel, 0 );

            if( FAILED( luaMgr.GetValue( "DEPEND_LOOKAT", temp ) ))
            {
                itemAnim.dependLookAt = false;
            }
            else
            {
                if( temp.compare( "TRUE" ) == 0 )
                    itemAnim.dependLookAt = true;
                else
                    itemAnim.dependLookAt = false;
            }

            LUA_GET_VALUE_DEF( "ANGLE_DIRECTION",   itemAnim.angledirection, false );
			LUA_GET_VALUE_DEF( "LOOKAT_LOATE",      itemAnim.LookAtRotate, false );

			luaMgr.BeginTable( "ITEM_NAME" );
			{
				for ( UINT j=1 ; ; ++j )
				{
					std::string strItemName;
					if ( SUCCEEDED( hr = luaMgr.GetValue( j, strItemName ) ) )
					{
						itemAnim.itemName.push_back( strItemName );
#if defined(RESOURCE_GRAPH)	
						char tmp1[256];				
						sprintf(tmp1,"ITEM_%s_%d",strItemName.c_str(),itemAnim.enchantLevel);								
						g_pGCDeviceManager2->AddRelation("ANIM_"+itemAnim.sequenceName,tmp1);						
#endif
					}
					else 
						break;
				}
			}
			luaMgr.EndTable();
            m_ItemAnimTemplet.push_back( itemAnim );

            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

    luaMgr.BeginTable( "DAMAGE_ANIM" );
    {
        for( i=1 ; ; ++i )
        {
            DamageAnim  damageAnim;
     
            if( luaMgr.BeginTable(i) == E_FAIL )
				break;

            luaMgr.GetValue( "SEQUENCE_NAME_RIGHT",  damageAnim.sequenceNameRight );
            luaMgr.GetValue( "SEQUENCE_NAME_LEFT",   damageAnim.sequenceNameLeft );

            damageAnim.animSeqRight = m_pKTDGAnim->GetSequence( damageAnim.sequenceNameRight );
            damageAnim.animSeqLeft = m_pKTDGAnim->GetSequence( damageAnim.sequenceNameLeft );
            
            if( FAILED( luaMgr.GetValue( "PLAY_TYPE",        temp ) ))
            {
                damageAnim.playType = CKTDGAnim::PT_ONE_PASS_AUTO_DIE;
            }
            else
            {
                if( temp.compare( "ONE_PASS" ) == 0 )
                    damageAnim.playType = CKTDGAnim::PT_ONE_PASS_AUTO_DIE;
                else if( temp.compare( "COUNTER_PASS" ) == 0 )
                    damageAnim.playType = CKTDGAnim::PT_COUNTER_PASS_AUTO_DIE;
            }
            
        	LUA_GET_VALUE_DEF( "POSX_MIN",		damageAnim.posX.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "POSX_MAX",		damageAnim.posX.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "POSY_MIN",		damageAnim.posY.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "POSY_MAX",		damageAnim.posY.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "POSZ_MIN",		damageAnim.posZ.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "POSZ_MAX",		damageAnim.posZ.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "SCALEX_MIN",		damageAnim.scaleX.m_Min,	1.0f );
			LUA_GET_VALUE_DEF( "SCALEX_MAX",		damageAnim.scaleX.m_Max,	1.0f );
			LUA_GET_VALUE_DEF( "SCALEY_MIN",		damageAnim.scaleY.m_Min,	1.0f );
			LUA_GET_VALUE_DEF( "SCALEY_MAX",		damageAnim.scaleY.m_Max,	1.0f );
			LUA_GET_VALUE_DEF( "ROTX_MIN",		damageAnim.rotX.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "ROTX_MAX",		damageAnim.rotX.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "ROTY_MIN",		damageAnim.rotY.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "ROTY_MAX",		damageAnim.rotY.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "ROTZ_MIN",		damageAnim.rotZ.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "ROTZ_MAX",		damageAnim.rotZ.m_Max,	0.0f );

			int iCharID;
			luaMgr.GetValue( "CHAR_IDENTY",      iCharID );
			damageAnim.charID = (CHAR_IDENTY)iCharID;

            luaMgr.GetValue( "ENCHANT_LEVEL",      damageAnim.enchantLevel );
			luaMgr.BeginTable( "ITEM_NAME" );
			{
				for( UINT j=1 ; ; ++j )
				{
					std::string strItemName;
					if ( SUCCEEDED( hr = luaMgr.GetValue( j, strItemName ) ) )
					{
						damageAnim.itemName.push_back( strItemName );

#if defined(RESOURCE_GRAPH)	
						char tmp1[256];				
						sprintf(tmp1,"ITEM_%s_%d",strItemName.c_str(),damageAnim.enchantLevel);								
						g_pGCDeviceManager2->AddRelation("ANIM_"+damageAnim.sequenceNameRight,tmp1);	
						g_pGCDeviceManager2->AddRelation("ANIM_"+damageAnim.sequenceNameLeft,tmp1);	
#endif
					}
					else 
						break;
				}
			}
			luaMgr.EndTable();

            m_DamageAnimTemplet.push_back( damageAnim );

            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

    luaMgr.BeginTable( "SKILL_ANIM" );
    {
        for( i=1 ; ; ++i )
        {
            SkillAnim   skillAnim;

            if( luaMgr.BeginTable(i) == E_FAIL )
				break;

            luaMgr.GetValue( "SEQUENCE_NAME_RIGHT",  skillAnim.sequenceNameRight );
            luaMgr.GetValue( "SEQUENCE_NAME_LEFT",   skillAnim.sequenceNameLeft );

            skillAnim.animSeqRight = m_pKTDGAnim->GetSequence( skillAnim.sequenceNameRight );
            skillAnim.animSeqLeft = m_pKTDGAnim->GetSequence( skillAnim.sequenceNameLeft );

            if( FAILED( luaMgr.GetValue( "PLAY_TYPE",        temp ) ))
            {
                skillAnim.playType = CKTDGAnim::PT_ONE_PASS_AUTO_DIE;
            }
            else
            {
                if( temp.compare( "ONE_PASS" ) == 0 )
                    skillAnim.playType = CKTDGAnim::PT_ONE_PASS_AUTO_DIE;
                else if( temp.compare( "COUNTER_PASS" ) == 0 )
                    skillAnim.playType = CKTDGAnim::PT_COUNTER_PASS_AUTO_DIE;
            }

			LUA_GET_VALUE_DEF( "POSX_MIN",		skillAnim.posX.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "POSX_MAX",		skillAnim.posX.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "POSY_MIN",		skillAnim.posY.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "POSY_MAX",		skillAnim.posY.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "POSZ_MIN",		skillAnim.posZ.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "POSZ_MAX",		skillAnim.posZ.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "SCALEX_MIN",		skillAnim.scaleX.m_Min,	1.0f );
			LUA_GET_VALUE_DEF( "SCALEX_MAX",		skillAnim.scaleX.m_Max,	1.0f );
			LUA_GET_VALUE_DEF( "SCALEY_MIN",		skillAnim.scaleY.m_Min,	1.0f );
			LUA_GET_VALUE_DEF( "SCALEY_MAX",		skillAnim.scaleY.m_Max,	1.0f );
			LUA_GET_VALUE_DEF( "ROTX_MIN",		skillAnim.rotX.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "ROTX_MAX",		skillAnim.rotX.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "ROTY_MIN",		skillAnim.rotY.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "ROTY_MAX",		skillAnim.rotY.m_Max,	0.0f );
			LUA_GET_VALUE_DEF( "ROTZ_MIN",		skillAnim.rotZ.m_Min,	0.0f );
			LUA_GET_VALUE_DEF( "ROTZ_MAX",		skillAnim.rotZ.m_Max,	0.0f );

			luaMgr.GetValue( "POSITION_TYPE",    temp );
            
			skillAnim.positionType = ParsePosition(temp);

            luaMgr.GetValue( "CHAR_IDENTY",      temp );

			int iCharID;
			luaMgr.GetValue( "CHAR_IDENTY",      iCharID );
			skillAnim.charID = (CHAR_IDENTY)iCharID;

            luaMgr.GetValue( "MAX_TIME",         skillAnim.maxTime );
            luaMgr.GetValue( "EMIT_MIN",         skillAnim.emitRate.m_Min );
            luaMgr.GetValue( "EMIT_MAX",         skillAnim.emitRate.m_Min );

            luaMgr.GetValue( "ENCHANT_LEVEL",      skillAnim.enchantLevel );
			luaMgr.BeginTable( "ITEM_NAME" );
			{
				for( UINT j=1 ; ; ++j )
				{
					std::string strItemName;
					if ( SUCCEEDED( hr = luaMgr.GetValue( j, strItemName ) ) )
					{
						skillAnim.itemName.push_back( strItemName );
#if defined(RESOURCE_GRAPH)	
						char tmp1[256];				
						sprintf(tmp1,"ITEM_%s_%d",strItemName.c_str(),skillAnim.enchantLevel);								
						g_pGCDeviceManager2->AddRelation("ANIM_"+skillAnim.sequenceNameRight,tmp1);	
						g_pGCDeviceManager2->AddRelation("ANIM_"+skillAnim.sequenceNameLeft,tmp1);	
#endif
					}
					else 
						break;
				}
			}
			luaMgr.EndTable();

            m_SkillAnimTemplet.push_back( skillAnim );

            luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();
}

void KGCAnimManager::StartGame()
{
    m_pKTDGAnim->ClearInstList();
	m_pKTDGAnim->ReleaseTexture();
    for( std::vector<SkillAnim*>::iterator i = m_SkillAnimInstance.begin(); i != m_SkillAnimInstance.end(); i++ )
        SAFE_DELETE( (*i) );
    m_SkillAnimInstance.clear();
}
void KGCAnimManager::EndGame()
{
    m_pKTDGAnim->ClearInstList();
    for( std::vector<SkillAnim*>::iterator i = m_SkillAnimInstance.begin(); i != m_SkillAnimInstance.end(); i++ )
        SAFE_DELETE( (*i) );
    m_SkillAnimInstance.clear();
}

void KGCAnimManager::DeleteSkillInst( SkillAnim* pInst )
{
	for( int i=0 ; i<(int)m_SkillAnimInstance.size() ; ++i )
	{
		if( m_SkillAnimInstance[i] == pInst )
		{
			SAFE_DELETE( m_SkillAnimInstance[i] );
			m_SkillAnimInstance.erase( m_SkillAnimInstance.begin() + i );
			break;
		}
	}
}

void KGCAnimManager::DeleteInst( CKTDGAnim::CAnimInstance* pInst )
{
	m_pKTDGAnim->DeleteInst( pInst );
}

void KGCAnimManager::FrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCAnimManager::FrameMove");

    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    /*if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL )
        return;*/

    CKTDGAnim::CAnimInstance* pAniminst;

    float fZPos = GetZPos();

    m_pKTDGAnim->FrameMove( fElapsedTime );
    
    for( std::vector<SkillAnim*>::iterator i = m_SkillAnimInstance.begin(); i != m_SkillAnimInstance.end(); i++ )
    {
        if( (*i)->remainTime > 0.0f )
        {
            if( (*i)->emitRemainTime <= 0.0f )
            {
                if( (*i)->isRight == true )
                {
                    pAniminst = m_pKTDGAnim->CreateInst( (*i)->animSeqRight );
                    pAniminst->GetMatrix()->Move( (*i)->pos.x + (*i)->posX.GetRandomNumInRange(), (*i)->pos.y + (*i)->posY.GetRandomNumInRange(), fZPos + (*i)->posZ.GetRandomNumInRange() );
                    pAniminst->GetMatrix()->Scale( (*i)->scaleX.GetRandomNumInRange(), (*i)->scaleY.GetRandomNumInRange(), 1.0f );
                    pAniminst->GetMatrix()->Rotate( D3DXToRadian((*i)->rotX.GetRandomNumInRange()), D3DXToRadian((*i)->rotY.GetRandomNumInRange()), D3DXToRadian((*i)->rotZ.GetRandomNumInRange()) );
                    pAniminst->GetMatrix()->RotateRel( 0.0f, D3DXToRadian(180), 0.0f );

					if( (*i)->animSeqRight->GetName().compare( "SpinCircle" ) == 0 )
					{
						m_pKTDGAnim->LastToFirst();
					}
                }
                else
                {
                    pAniminst = m_pKTDGAnim->CreateInst( (*i)->animSeqLeft );
                    pAniminst->GetMatrix()->Move( (*i)->pos.x + (*i)->posX.GetRandomNumInRange(), (*i)->pos.y + (*i)->posY.GetRandomNumInRange(), fZPos + (*i)->posZ.GetRandomNumInRange() );
                    pAniminst->GetMatrix()->Scale( (*i)->scaleX.GetRandomNumInRange(), (*i)->scaleY.GetRandomNumInRange(), 1.0f );
                    pAniminst->GetMatrix()->Rotate( D3DXToRadian((*i)->rotX.GetRandomNumInRange()), D3DXToRadian((*i)->rotY.GetRandomNumInRange()), D3DXToRadian((*i)->rotZ.GetRandomNumInRange()) );

					if( (*i)->animSeqRight->GetName().compare( "SpinCircle" ) == 0 )
					{
						m_pKTDGAnim->LastToFirst();
					}
                }
                pAniminst->Start( (*i)->playType );
                (*i)->emitRemainTime = 1.0f / (*i)->emitRate.GetRandomNumInRange();
            }
            (*i)->remainTime -= fElapsedTime;
            (*i)->emitRemainTime -= fElapsedTime;
        }
    }
}
void KGCAnimManager::Render()
{
    m_pKTDGAnim->Render();
}

bool KGCAnimManager::IsItemAnim( GCITEMID itemID )
{
    std::ostringstream itemName;
    itemName << itemID;

    ItemAnim anim;
    for( std::vector<ItemAnim>::iterator i = m_ItemAnimTemplet.begin(); i != m_ItemAnimTemplet.end(); i++)
    {
        anim = (*i);
        std::vector<std::string>::iterator itor;
        for( itor = anim.itemName.begin() ; itor != anim.itemName.end() ; ++itor )
        {
            std::string strItemName = *itor;
            if( strItemName.compare(itemName.str()) == 0 )
            {
                return true;
            }
        }
    }
    return false;
}

int KGCAnimManager::GetNewItemAnim( std::string itemName, CHAR_IDENTY charID, std::vector<ItemAnim>& outAnim, int enchantLevel /*=0*/ , bool bSquare_ /* =false */ )
{
    int iRet = 0;
    ItemAnim anim;

    for( std::vector<ItemAnim>::iterator i = m_ItemAnimTemplet.begin(); i != m_ItemAnimTemplet.end(); i++)
    {
        anim = (*i);

        std::vector<ItemAnim>::iterator vit = outAnim.begin();
        for (; vit != outAnim.end() ; ++vit)
        {
            if ( vit->sequenceName.compare(anim.sequenceName) == 0 )
            {
                if( vit->sequenceName == "DarkSword" && vit->positionType == EPT_LASKNIFE_ONE && vit->charID == CID_LAS1 )
                    continue;
                else if( vit->sequenceName == "DarkSword" && vit->positionType == EPT_SWORD_CENTER && vit->charID == CID_ELESIS4 )
                    continue;
                else if( vit->sequenceName == "DarkCB" && vit->positionType == EPT_CROSSBOW_ONE && vit->charID == CID_LIRE2 )
                    continue;
                else if( vit->sequenceName == "DarkSword" && vit->positionType == EPT_SWORD_CENTER && vit->charID == CID_SIEG3 )
                    continue;
                else
                    break;
            }
        }
        if ( vit != outAnim.end() )
            continue;



		bool bEnable = false;

		// 아이템 이름 체크
		std::vector<std::string>::iterator itor;
		for( itor = anim.itemName.begin() ; itor != anim.itemName.end() ; ++itor )
		{
			std::string strItemName = *itor;

			if( strItemName.compare(itemName) == 0 )
			{
				bEnable = true;
				break;
			}
		}

        if( bEnable && anim.enchantLevel == enchantLevel )
        {
			if( anim.charID != CID_COMMON && anim.charID != charID )
				continue;

			anim.animInst = m_pKTDGAnim->CreateInst( anim.animSeq );

			if( anim.animInst == NULL )
				continue;

			//광장에서 제로는 칼을 뽑고있지 않음. 칼에 붙는 애니메이션은 나오면 안된다.
			if (bSquare_)
			{
				if (charID == CID_ZERO1 || charID == CID_ZERO2)
					if (anim.positionType == EPT_SWORD_CENTER || 
						anim.positionType == EPT_SWORD_BODY ||
						anim.positionType == EPT_SWORD_TIP)
						continue;

			}

			

			anim.animInst->GetMatrix()->Scale( anim.scaleX.GetRandomNumInRange(), anim.scaleY.GetRandomNumInRange(), 1.0f );
			anim.animInst->GetMatrix()->Rotate( D3DXToRadian(anim.rotX.GetRandomNumInRange()), D3DXToRadian(anim.rotY.GetRandomNumInRange()), D3DXToRadian(anim.rotZ.GetRandomNumInRange()) );

			anim.animInst->Start( anim.playType );

			if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_CAPTAIN || anim.positionIndex != 1 ) 
			{
				outAnim.push_back( anim );
				iRet++;
			}
        }
    }

    return iRet;
}

void KGCAnimManager::FrameUpdateItemAnim( ItemAnim* anim, PositionDesc* posDesc, int life )
{
    float fZPos = GetZPos();

    if( life <= 0 )
    {
        anim->animInst->SetShow( false );
        return;
    }
    else
    {
        //anim->animInst->SetShow( true );
        anim->animInst->GetMatrix()->Scale( anim->scaleX.GetRandomNumInRange(), anim->scaleY.GetRandomNumInRange(), 1.0f );
        if( posDesc->right == true )
        {
            anim->animInst->GetMatrix()->Rotate( D3DXToRadian(anim->rotX.GetRandomNumInRange()), D3DXToRadian(anim->rotY.GetRandomNumInRange()), D3DXToRadian(-anim->rotZ.GetRandomNumInRange()) );
        }
        else
        {
            if( anim->angledirection )
            {
                anim->animInst->GetMatrix()->Rotate( D3DXToRadian(anim->rotX.GetRandomNumInRange()), D3DXToRadian(anim->rotY.GetRandomNumInRange()), D3DXToRadian(anim->rotZ.GetRandomNumInRange() + 180.0f) );
            }
			else
            {
				if(anim->LookAtRotate)
				{
					anim->animInst->GetMatrix()->Rotate( D3DXToRadian(anim->rotX.GetRandomNumInRange()), D3DXToRadian(anim->rotY.GetRandomNumInRange() + 180.0f), D3DXToRadian(anim->rotZ.GetRandomNumInRange()) );
				}
				else
				{
					anim->animInst->GetMatrix()->Rotate( D3DXToRadian(anim->rotX.GetRandomNumInRange()), D3DXToRadian(anim->rotY.GetRandomNumInRange()), D3DXToRadian(anim->rotZ.GetRandomNumInRange()) );
				}

            }
        }
        
        switch( anim->positionType )
        {
            case EPT_BODY_CENTER:
                if( anim->dependLookAt == true )
                {
                    if( posDesc->right == true )
                        anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + posDesc->body.x, anim->posY.GetRandomNumInRange() + posDesc->body.y, fZPos );
                    else
                        anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->body.x, anim->posY.GetRandomNumInRange() + posDesc->body.y, fZPos );
                }
                else
                {
                    anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->body.x, anim->posY.GetRandomNumInRange() + posDesc->body.y, fZPos );
                }
                break;

            case EPT_SWORD_TIP:
                anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->swordTip.x, anim->posY.GetRandomNumInRange() + posDesc->swordTip.y, fZPos );
                anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->swordAngle );
                break;

            case EPT_SWORD_BODY:
                anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->swordBody.x, anim->posY.GetRandomNumInRange() + posDesc->swordBody.y, fZPos );
                anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->swordAngle );
                break;

            case EPT_SWORD_CENTER:
                {
                    D3DXVECTOR3 pos;
                    
					if( posDesc->charID == CID_ELESIS3 )
					{
						if( posDesc->right == true )
						{   // 맞추다 지쳤음..ㅡㅡ; 나중에 완전히 개편할 각오로 덤벼 들겠음..
							pos = posDesc->swordCenter;// + (posDesc->rightHandBody - posDesc->leftHandBody);
							pos.y -= 0.02f;
							anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->swordAngle );							
						}
						else
						{
							pos = posDesc->swordCenter;
							anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->swordAngle );
						}

					}
					else
					{
						pos = posDesc->swordCenter;
						anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->swordAngle );
					}

                    
					if( anim->dependLookAt == true )
					{
						if( posDesc->right == true )
							anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + pos.x, anim->posY.GetRandomNumInRange() + pos.y, fZPos );
						else
							anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + pos.x, anim->posY.GetRandomNumInRange() + pos.y, fZPos );
					}
					else
						anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + pos.x, anim->posY.GetRandomNumInRange() + pos.y, fZPos );
                }
                break;

            case EPT_LEFT_SWORD_TIP:
                anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->leftSwordTip.x, anim->posY.GetRandomNumInRange() + posDesc->leftSwordTip.y, fZPos );
                anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->leftSwordAngle );
                break;

            case EPT_LEFT_SWORD_BODY:
                anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->leftSwordBody.x, anim->posY.GetRandomNumInRange() + posDesc->leftSwordBody.y, fZPos );
                anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->leftSwordAngle );
                break;

            case EPT_LEFT_SWORD_CENTER:
                {
                    D3DXVECTOR3 pos;

                    pos = posDesc->leftSwordCenter;

                    anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->leftSwordAngle );
                    if( anim->dependLookAt == true )
                    {
                        if( posDesc->right == true )
                            anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + pos.x, anim->posY.GetRandomNumInRange() + pos.y, fZPos );
                        else
                            anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + pos.x, anim->posY.GetRandomNumInRange() + pos.y, fZPos );
                    }
                    else
                        anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + pos.x, anim->posY.GetRandomNumInRange() + pos.y, fZPos );
                }
                break;

            case EPT_BOW:
                anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->bow.x, anim->posY.GetRandomNumInRange() + posDesc->bow.y, fZPos );
                anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->bowAngle );
                break;

            case EPT_CROSSBOW_ONE:
				anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->crossBowLeft.x, anim->posY.GetRandomNumInRange() + posDesc->crossBowLeft.y, fZPos );

				if( posDesc->right == true )
					anim->animInst->GetMatrix()->RotateRel( 0,0, D3DXToRadian(180) );
                break;

            case EPT_CROSSBOW_TWO:
                anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->crossBowRight.x, anim->posY.GetRandomNumInRange() + posDesc->crossBowRight.y, fZPos );
				if( posDesc->right == true )
					anim->animInst->GetMatrix()->RotateRel( 0,0, D3DXToRadian(180) );
				break;

			case EPT_LASKNIFE_ONE:
				anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->LasKnifeOneAngle );
				if( anim->dependLookAt == true )
				{
					if( posDesc->right == true )
						anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + posDesc->LasKnifeOne.x, anim->posY.GetRandomNumInRange() + posDesc->LasKnifeOne.y, fZPos );
					else
						anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->LasKnifeOne.x, anim->posY.GetRandomNumInRange() + posDesc->LasKnifeOne.y, fZPos );
				}
				else
					anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->LasKnifeOne.x, anim->posY.GetRandomNumInRange() + posDesc->LasKnifeOne.y, fZPos );

				break;

			case EPT_LASKNIFE_TWO:
				anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->LasKnifeTwoAngle );
				if( anim->dependLookAt == true )
				{
					if( posDesc->right == true )
						anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + posDesc->LasKnifeTwo.x, anim->posY.GetRandomNumInRange() + posDesc->LasKnifeTwo.y, fZPos );
					else
						anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->LasKnifeTwo.x, anim->posY.GetRandomNumInRange() + posDesc->LasKnifeTwo.y, fZPos );
				}
				else
					anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->LasKnifeTwo.x, anim->posY.GetRandomNumInRange() + posDesc->LasKnifeTwo.y, fZPos );

				break;

			case EPT_AXE:
				anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->RyanAxeAngle );
				if( anim->dependLookAt == true )
				{
					if( posDesc->right == true )
						anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + posDesc->RyanAxe.x, anim->posY.GetRandomNumInRange() + posDesc->RyanAxe.y, fZPos );
					else
						anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->RyanAxe.x, anim->posY.GetRandomNumInRange() + posDesc->RyanAxe.y, fZPos );
				}
				else
					anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->RyanAxe.x, anim->posY.GetRandomNumInRange() + posDesc->RyanAxe.y, fZPos );
				break;

            case EPT_CRICENT:
                anim->animInst->GetMatrix()->RotateRel( 0.0f, 0.0f, posDesc->SentinelCricentAngle );

                if( anim->dependLookAt == true )
                {
                    if( posDesc->right == true )
                        anim->animInst->GetMatrix()->Move( -anim->posX.GetRandomNumInRange() + posDesc->SentinelCricent.x, anim->posY.GetRandomNumInRange() + posDesc->SentinelCricent.y, fZPos );
                    else
                        anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->SentinelCricent.x, anim->posY.GetRandomNumInRange() + posDesc->SentinelCricent.y, fZPos );
                }
                else
                    anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->SentinelCricent.x, anim->posY.GetRandomNumInRange() + posDesc->SentinelCricent.y, fZPos );
                break;
			case EPT_EFFECT_RHAND:			
			case EPT_RHAND:
				anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->rightHandLatancy[10].x, anim->posY.GetRandomNumInRange() + posDesc->rightHandLatancy[10].y, fZPos );
				break;
			case EPT_EFFECT_LHAND:
			case EPT_LHAND:
				anim->animInst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + posDesc->leftHandLatancy[10].x, anim->posY.GetRandomNumInRange() + posDesc->leftHandLatancy[10].y, fZPos );
				break;

				
        }        

    }
}

void KGCAnimManager::GetNewDamageAnim( std::string itemName, CHAR_IDENTY charID, std::vector<DamageAnim>& outAnim, int enchantLevel )
{
    DamageAnim anim;

    for( std::vector<DamageAnim>::iterator i = m_DamageAnimTemplet.begin(); i != m_DamageAnimTemplet.end(); i++)
    {
        anim = (*i);

		bool bEnable = false;

		// 아이템 이름 체크
		std::vector<std::string>::iterator itor;
		for( itor = anim.itemName.begin() ; itor != anim.itemName.end() ; ++itor )
		{
			std::string strItemName = *itor;

			if( strItemName.compare(itemName) == 0 )
			{
				bEnable = true;
				break;
			}
		}

        if( bEnable && anim.enchantLevel == enchantLevel )
        {
			if( anim.charID != CID_COMMON && anim.charID != charID )
				continue;

            if( anim.sequenceNameRight.compare( anim.sequenceNameLeft ) == 0 )
            {
                anim.animSeqRight = m_pKTDGAnim->GetSequence( anim.sequenceNameRight );
                anim.animSeqLeft = anim.animSeqRight;
            }
            else
            {
                anim.animSeqRight = m_pKTDGAnim->GetSequence( anim.sequenceNameRight );
                anim.animSeqLeft = m_pKTDGAnim->GetSequence( anim.sequenceNameLeft );
            }

            if( anim.animSeqLeft != NULL && anim.animSeqRight != NULL )
			{
				CKTDGAnim::CAnimInstance* pAniminst = m_pKTDGAnim->CreateInst( anim.animSeqRight );
				m_pKTDGAnim->DeleteInst( pAniminst );
				pAniminst = m_pKTDGAnim->CreateInst( anim.animSeqLeft );
				m_pKTDGAnim->DeleteInst( pAniminst );
				
                outAnim.push_back( anim );
			}
        }
    }
}

void KGCAnimManager::FrameUpdateDamageAnim( DamageAnim* anim, const D3DXVECTOR3& impactPos, bool right )
{
    CKTDGAnim::CAnimInstance* pAniminst;
    
    if( right == true )
    {
        pAniminst = m_pKTDGAnim->CreateInst( anim->animSeqRight );
        pAniminst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + impactPos.x, anim->posY.GetRandomNumInRange() + impactPos.y, anim->posZ.GetRandomNumInRange() + impactPos.z );
        pAniminst->GetMatrix()->Scale( anim->scaleX.GetRandomNumInRange(), anim->scaleY.GetRandomNumInRange(), 1.0f );
        pAniminst->GetMatrix()->Rotate( D3DXToRadian(anim->rotX.GetRandomNumInRange()), D3DXToRadian(anim->rotY.GetRandomNumInRange()), D3DXToRadian(anim->rotZ.GetRandomNumInRange()) );
        pAniminst->GetMatrix()->RotateRel( 0.0f, D3DXToRadian(180), 0.0f );
    }
    else if( right == false )
    {
        pAniminst = m_pKTDGAnim->CreateInst( anim->animSeqLeft );
        pAniminst->GetMatrix()->Move( anim->posX.GetRandomNumInRange() + impactPos.x, anim->posY.GetRandomNumInRange() + impactPos.y, anim->posZ.GetRandomNumInRange() + impactPos.z );
        pAniminst->GetMatrix()->Scale( anim->scaleX.GetRandomNumInRange(), anim->scaleY.GetRandomNumInRange(), 1.0f );
        pAniminst->GetMatrix()->Rotate( D3DXToRadian(anim->rotX.GetRandomNumInRange()), D3DXToRadian(anim->rotY.GetRandomNumInRange()), D3DXToRadian(anim->rotZ.GetRandomNumInRange()) );
    }
    pAniminst->Start( anim->playType );
}

void KGCAnimManager::GetNewSkillAnim( std::string itemName, CHAR_IDENTY charID, std::vector<SkillAnim*>& outAnim, int enchantLevel )
{
    for( std::vector<SkillAnim>::iterator i = m_SkillAnimTemplet.begin(); i != m_SkillAnimTemplet.end(); i++)
    {
        SkillAnim* anim = new SkillAnim;
        *anim = (*i);

		bool bEnable = false;

		// 아이템 이름 체크
		std::vector<std::string>::iterator itor;
		for( itor = anim->itemName.begin() ; itor != anim->itemName.end() ; ++itor )
		{
			if( (*itor).compare(itemName) == 0 )
			{
				bEnable = true;
				break;
			}
		}

		// 강화레벨 체크
        if( bEnable && anim->enchantLevel == enchantLevel )
		{
			if( anim->charID != CID_COMMON && anim->charID != charID )
			{
				SAFE_DELETE( anim );
				continue;
			}

            if( anim->sequenceNameRight.compare( anim->sequenceNameLeft ) == 0 )
            {
                anim->animSeqRight = m_pKTDGAnim->GetSequence( anim->sequenceNameRight );
                anim->animSeqLeft = anim->animSeqRight;
            }
            else
            {
                anim->animSeqRight = m_pKTDGAnim->GetSequence( anim->sequenceNameRight );
                anim->animSeqLeft = m_pKTDGAnim->GetSequence( anim->sequenceNameLeft );
            }

            if( anim->animSeqRight != NULL && anim->animSeqLeft != NULL )
            {
                outAnim.push_back( anim );
                m_SkillAnimInstance.push_back( anim );
            }
            else
            {
                SAFE_DELETE( anim );
            }
        }
		else
		{
			SAFE_DELETE( anim );
		}
    }
}

void KGCAnimManager::FrameUpdateSkillAnim( SkillAnim* anim, PositionDesc* posDesc )
{
    anim->remainTime = anim->maxTime;
    anim->emitRemainTime = 0.0f;
    anim->isRight = posDesc->right;

    switch( anim->positionType )
    {
        case EPT_BODY_CENTER:
            anim->pos = posDesc->body;
            break;

        case EPT_SWORD_TIP:
            anim->pos = posDesc->swordTip;
            break;

        case EPT_SWORD_BODY:
            anim->pos = posDesc->swordBody;
            break;

        case EPT_SWORD_CENTER:
			anim->pos = posDesc->swordCenter;
            break;

        case EPT_BOW:
            anim->pos = posDesc->bow;
            break;

        case EPT_CROSSBOW_ONE:
            anim->pos = posDesc->crossBowLeft;
            break;

        case EPT_CROSSBOW_TWO:
            anim->pos = posDesc->crossBowRight;
            break;

		case EPT_LASKNIFE_ONE:
			anim->pos = posDesc->LasKnifeOne;
			break;

		case EPT_LASKNIFE_TWO:
			anim->pos = posDesc->LasKnifeTwo;
			break;

		case EPT_AXE:
			anim->pos = posDesc->RyanAxe;
			break;

        case EPT_CRICENT:
            anim->pos = posDesc->SentinelCricent;
            break;

        case EPT_LEFT_SWORD_TIP:
            anim->pos = posDesc->leftSwordTip;
            break;

        case EPT_LEFT_SWORD_BODY:
            anim->pos = posDesc->leftSwordBody;
            break;

        case EPT_LEFT_SWORD_CENTER:
            anim->pos = posDesc->leftSwordCenter;
            break;
    }
	//anim->pos.z = 0.6f;
}

CKTDGAnim::CAnimInstance* KGCAnimManager::GetAnimSequence( char* sequence )
{
    return m_pKTDGAnim->CreateInst( m_pKTDGAnim->GetSequence( sequence ) );
}

void KGCAnimManager::InitDeviceObjects()
{
    m_pKTDGAnim = new CKTDGAnim( g_MyD3D->m_pd3dDevice );

    std::string strAnimScript = "GCAnim.stg";
    m_pKTDGAnim->Compile( (char*)strAnimScript.c_str() );
}
void KGCAnimManager::DeleteDeviceObjects()
{
    SAFE_DELETE( m_pKTDGAnim );

	for( std::vector<ItemAnim>::iterator i = m_ItemAnimTemplet.begin() ; i != m_ItemAnimTemplet.end() ; ++i )
		(*i).itemName.clear();

	for( std::vector<DamageAnim>::iterator j = m_DamageAnimTemplet.begin() ; j != m_DamageAnimTemplet.end() ; ++j )
		(*j).itemName.clear();

	for( std::vector<SkillAnim>::iterator k = m_SkillAnimTemplet.begin() ; k != m_SkillAnimTemplet.end() ; ++k )
		(*k).itemName.clear();

    for( std::vector<SkillAnim*>::iterator l = m_SkillAnimInstance.begin(); l != m_SkillAnimInstance.end(); ++l )
        SAFE_DELETE( (*l) );

	m_ItemAnimTemplet.clear();
	m_DamageAnimTemplet.clear();
	m_SkillAnimTemplet.clear();
    m_SkillAnimInstance.clear();
}

EMIT_POSITION_TYPE KGCAnimManager::ParsePosition( std::string str )
{
	if( str.compare( "BODY_CENTER" ) == 0 )
        return EPT_BODY_CENTER;
    else if( str.compare( "SWORD_TIP" ) == 0 )
        return EPT_SWORD_TIP;
    else if( str.compare( "SWORD_BODY" ) == 0 )
        return EPT_SWORD_BODY;
    else if( str.compare( "SWORD_CENTER" ) == 0 )
        return EPT_SWORD_CENTER;
    else if( str.compare( "BOW" ) == 0 )
        return EPT_BOW;
    else if( str.compare( "CROSSBOW_ONE" ) == 0 )
        return EPT_CROSSBOW_ONE;
    else if( str.compare( "CROSSBOW_TWO" ) == 0 )
        return EPT_CROSSBOW_TWO;
	else if( str.compare( "LASKNIFE_ONE" ) == 0 )
		return EPT_LASKNIFE_ONE;
	else if( str.compare( "LASKNIFE_TWO" ) == 0 )
		return EPT_LASKNIFE_TWO;
	else if( str.compare( "RSHOULDER" ) == 0 )
        return EPT_RSHOULDER;
	else if( str.compare( "LSHOULDER" ) == 0 )
        return EPT_LSHOULDER;
	else if( str.compare( "RHAND" ) == 0 )
        return EPT_RHAND;
	else if( str.compare( "LHAND" ) == 0 )
        return EPT_LHAND;
	else if( str.compare( "RFOOT" ) == 0 )
        return EPT_RFOOT;
	else if( str.compare( "LFOOT" ) == 0 )
        return EPT_LFOOT;
	else if( str.compare( "AXE" ) == 0 )
		return EPT_AXE;
    else if( str.compare( "CRICENT" ) == 0 )
        return EPT_CRICENT;
    else if( str.compare( "LEFT_SWORD_TIP" ) == 0 )
        return EPT_LEFT_SWORD_TIP;
    else if( str.compare( "LEFT_SWORD_BODY" ) == 0 )
        return EPT_LEFT_SWORD_BODY;
    else if( str.compare( "LEFT_SWORD_CENTER" ) == 0 )
        return EPT_LEFT_SWORD_CENTER;
	else if( str.compare( "EFFECT_LHAND" ) == 0 )
		return EPT_EFFECT_LHAND;
	else if( str.compare( "EFFECT_RHAND" ) == 0 )
		return EPT_EFFECT_RHAND;	
    
	return EPT_BODY_CENTER;
}

float KGCAnimManager::GetZPos() const
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SQUARE )
    {
		//return KGCLayerPart::ms_fZOrder[5];
		// 3DOjbect의 위치를 0.5로 고정하면 anim을 0.2로 해주어야 한다.
		return 0.2f;
    }
    else
    {
        return 0.5f;
    }
}

void KGCAnimManager::SetEnchantLevel( std::string strItemName, int iEnchantLevel )
{
    ItemAnim anim;
    int j = 0;

    std::string m_AnimSeqName = "Firesword";

    m_pAnim = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence(m_AnimSeqName ) );

    if ( !m_pAnim )
    {
        m_pAnim = g_AnimManager.GetAnimSequence( (char*) m_AnimSeqName.c_str() );
    }

    if( m_pAnim != NULL )
    {        
        
            m_pAnim->Start( CKTDGAnim::PT_LOOP );
        
    }

    for( std::vector<ItemAnim>::iterator i = m_ItemAnimTemplet.begin(); i != m_ItemAnimTemplet.end(); i++, j++)
    {
        anim = (*i);

        // 아이템 이름 체크
        std::vector<std::string>::iterator itor;
        for( itor = anim.itemName.begin() ; itor != anim.itemName.end() ; ++itor )
        {
            std::string strItemName = *itor;

            if( strItemName.compare(strItemName) == 0 )
            {
                m_ItemAnimTemplet[j].enchantLevel = iEnchantLevel;
            }
            break;
        }            
    }

    m_pAnim->SetShow(true);
}
