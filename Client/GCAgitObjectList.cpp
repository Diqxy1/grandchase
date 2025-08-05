#include "stdafx.h"
#include "GCAgitObjectList.h"
#include "GCAgitObjectAction.h"
#include "GCAgitJumpAction.h"
#include "GCAgitDiceAction.h"
#include "GCAgitFlagAction.h"

KGCAgitObjectInfoList::KGCAgitObjectInfoList(void)
{
    m_mapSpecialObjectInfo.clear();
    m_mapJumpObject.clear();
    m_mapDiceObject.clear();
    m_mapFlagObject.clear();
}

KGCAgitObjectInfoList::~KGCAgitObjectInfoList(void)
{
}

bool KGCAgitObjectInfoList::AddObjectInfo( GCITEMID itemID, const KAgitObjectInfo& kObj )
{
    if( m_mapObjectInfo.find( itemID ) != m_mapObjectInfo.end() ) {
        return false;
    }

    m_mapObjectInfo.insert( TObjectInfo::value_type( itemID, kObj ) );
    return true;
}

const KAgitObjectInfo* KGCAgitObjectInfoList::GetObjectInfo( GCITEMID itemID ) const
{
    TObjectInfo::const_iterator itor = m_mapObjectInfo.find( itemID );
    if( itor == m_mapObjectInfo.end() ) {
        return NULL;
    }

    return &itor->second;
}

bool KGCAgitObjectInfoList::CheckExistObject( GCITEMID itemID ) const
{
    if( m_mapObjectInfo.find( itemID ) != m_mapObjectInfo.end() ) {
        return false;
    }
    return true;
}

bool KGCAgitObjectInfoList::InsertSpecialObject( GCITEMID _itemID, EM_AGIT_OBJECT_ACTION emAction )
{
    if ( _itemID == UINT_MAX ) 
        return false;

    if ( m_mapObjectInfo.find( _itemID ) == m_mapObjectInfo.end() )
        return false;

    if ( emAction < EM_AGIT_OBJECT_JUMP_ACTION || emAction >= EM_AGIT_OBJECT_SIZE )
        return false;

    m_mapSpecialObjectInfo.insert( MAP_SPECIAL_AGIT_OBJ::value_type( _itemID, emAction ) );

    return true;
}

bool KGCAgitObjectInfoList::IsInsertObject( GCITEMID _itemID, EM_AGIT_OBJECT_ACTION emAction )
{
    if ( _itemID == UINT_MAX ) 
        return false;

    if ( m_mapObjectInfo.find( _itemID ) == m_mapObjectInfo.end() )
        return false;

    MAP_SPECIAL_AGIT_OBJ_ITER pos = m_mapSpecialObjectInfo.find( _itemID );
    if ( pos == m_mapSpecialObjectInfo.end() ) 
        return false;

    if ( pos->second != emAction ) 
        return false;

    return true;
}

bool KGCAgitObjectInfoList::InsertJumpObject( GCITEMID _itemID, const KAgitJumpObject& kAgitJumpObject )
{
    if ( !IsInsertObject( _itemID, EM_AGIT_OBJECT_JUMP_ACTION ) ) 
        return false;

    m_mapJumpObject.insert( MAP_AGIT_JUMP_OBJECT::value_type( kAgitJumpObject.m_ItemID, kAgitJumpObject ) );

    return true;
}

bool KGCAgitObjectInfoList::InsertDiceObject( GCITEMID _itemID, const KAgitDiceObject& kAgitDiceObject )
{
    if ( !IsInsertObject( _itemID, EM_AGIT_OBJECT_DICE_ACTION ) ) 
        return false;

    m_mapDiceObject.insert( MAP_AGIT_DICE_OBJECT::value_type( kAgitDiceObject.m_ItemID, kAgitDiceObject ) );

    return true;
}

bool KGCAgitObjectInfoList::InsertFlagObject( GCITEMID _itemID, const KAgitFlagObject& kAgitFlagObject )
{
    if ( !IsInsertObject( _itemID, EM_AGIT_OBJECT_FLAG_ACTION ) ) 
        return false;

    m_mapFlagObject.insert( MAP_AGIT_FLAG_OBJECT::value_type( kAgitFlagObject.m_ItemID, kAgitFlagObject ) );

    return true;
}


bool KGCAgitObjectInfoList::LoadSpecialObjectInfo( const std::string& strFile )
{
    // 오브젝트정보 스크립트 로딩하고 저장해 둔다 
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, strFile ) == false )
        return false;

    m_mapSpecialObjectInfo.clear();
    m_mapJumpObject.clear();
    m_mapDiceObject.clear();
    m_mapFlagObject.clear();
    
    if( SUCCEEDED( luaMgr.BeginTable( "SpecialObjectList" ) ) )
    {
        int iObjectIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iObjectIndex++ ) ) ) 
                break;

            DWORD dwID = 0;
            int nObjectType = -1;

            luaMgr.GetValue( "ObjectID",       dwID );
            luaMgr.GetValue( "ObjectType",     nObjectType );

            EM_AGIT_OBJECT_ACTION emAction = static_cast<EM_AGIT_OBJECT_ACTION>(nObjectType);

            InsertSpecialObject( dwID, emAction );

            luaMgr.EndTable();
        }  
        
        luaMgr.EndTable();
    }

    if( SUCCEEDED( luaMgr.BeginTable( "JumpObjectList" ) ) )
    {
        int iObjectIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iObjectIndex++ ) ) ) 
                break;

            KAgitJumpObject kAgitJumpObject;

            luaMgr.GetValue( "ObjectID",  kAgitJumpObject.m_ItemID );
            LUA_GET_VALUE_DEF( "JumpX", kAgitJumpObject.m_fJumpX, 0.0f );
            LUA_GET_VALUE_DEF( "JumpY", kAgitJumpObject.m_fJumpY, 0.0f );
            LUA_GET_VALUE_DEF( "Sound", kAgitJumpObject.m_nSound, -1 );
            LUA_GET_VALUE_DEF( "BasicEffectNum", kAgitJumpObject.m_nBasicEffectNum, 0 );
            

            if( SUCCEEDED( luaMgr.BeginTable( "ActionParticle" ) ) )
            {

                for( int i=1 ; ; ++i )
                {
                    std::string strEffectName;
                    if( FAILED( luaMgr.GetValue( i, strEffectName ) ) )
                        break;

                    kAgitJumpObject.m_vecEffectName.push_back( strEffectName );
                }
                luaMgr.EndTable();       
            }

            InsertJumpObject( kAgitJumpObject.m_ItemID, kAgitJumpObject );
            
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED( luaMgr.BeginTable( "DiceObjectList" ) ) )
    {
        int iObjectIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iObjectIndex++ ) ) ) 
                break;

            KAgitDiceObject kAgitDiceObject;

            luaMgr.GetValue( "ObjectID",  kAgitDiceObject.m_ItemID );
            LUA_GET_VALUE_DEF( "Sound", kAgitDiceObject.m_nSound, -1 );
            LUA_GET_VALUE_DEF( "BasicEffectNum", kAgitDiceObject.m_nBasicEffectNum, 0 );

            if( SUCCEEDED( luaMgr.BeginTable( "ActionParticle" ) ) )
            {

                for( int i=1 ; ; ++i )
                {
                    std::string strEffectName;
                    if( FAILED( luaMgr.GetValue( i, strEffectName ) ) )
                        break;

                    kAgitDiceObject.m_vecEffectName.push_back( strEffectName );
                }
                luaMgr.EndTable();       
            }

            InsertDiceObject( kAgitDiceObject.m_ItemID, kAgitDiceObject );

            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }


    if( SUCCEEDED( luaMgr.BeginTable( "FlagObjectList" ) ) )
    {
        int iObjectIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iObjectIndex++ ) ) ) 
                break;

            KAgitFlagObject kAgitFlagObject;

            luaMgr.GetValue( "ObjectID",  kAgitFlagObject.m_ItemID );
            LUA_GET_VALUE_DEF( "Sound", kAgitFlagObject.m_nSound, -1 );

            if( SUCCEEDED( luaMgr.BeginTable( "ActionParticle" ) ) )
            {

                for( int i=1 ; ; ++i )
                {
                    std::string strEffectName;
                    if( FAILED( luaMgr.GetValue( i, strEffectName ) ) )
                        break;

                    kAgitFlagObject.m_vecEffectName.push_back( strEffectName );
                }
                luaMgr.EndTable();       
            }

            InsertFlagObject( kAgitFlagObject.m_ItemID, kAgitFlagObject );

            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }

    return true;
}

bool KGCAgitObjectInfoList::LoadObjectInfo( const std::string& strFile )
{
    // 오브젝트정보 스크립트 로딩하고 저장해 둔다 
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, strFile ) == false )
        return false;

    m_mapObjectInfo.clear();

    if( SUCCEEDED( luaMgr.BeginTable( "ObjectList" ) ) )
    {
        int iObjectIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iObjectIndex++ ) ) ) 
                break;

            KAgitObjectInfo tempObject;

            luaMgr.GetValue( "ObjectID",       tempObject.dwObjectID );
            luaMgr.GetValue( "ObjectType",     tempObject.iObjectType );
            luaMgr.GetValue( "CharType",       tempObject.iCharType );
            luaMgr.GetValue( "LayerNum",       tempObject.iLayerNum );
            luaMgr.GetValue( "StrNameID",      tempObject.iStrNameID );
            luaMgr.GetValue( "Contact",        tempObject.bContact );
            luaMgr.GetValue( "CanStack",       tempObject.bCanStack );
            luaMgr.GetValue( "OnlyTop",        tempObject.bOnlyTop );
            luaMgr.GetValue( "EnableSitDown",  tempObject.bEnableSitDown );
            luaMgr.GetValue( "EnableLie",      tempObject.bEnableLie );

            if( SUCCEEDED( luaMgr.BeginTable( "Texture" ) ) )
            {
                for( int i=1 ; ; ++i )
                {
                    std::string strTemp;
                    if( FAILED( luaMgr.GetValue( i, strTemp ) ) )
                        break;

                    tempObject.vecTexture.push_back( strTemp );
                }
                luaMgr.EndTable();
            }

            if( SUCCEEDED( luaMgr.BeginTable( "Mesh" ) ) )
            {
                int iMeshIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iMeshIndex ) ) )
                        break;

                    EXTRA_MESH mesh;

                    LUA_GET_VALUE_DEF( 1, mesh.strMeshName, "" );
                    LUA_GET_VALUE_DEF( 2, mesh.strTextureName, "" );
                    LUA_GET_VALUE_DEF( 3, mesh.dwCartoon, 0 );
                    LUA_GET_VALUE_DEF( 4, mesh.dwTechnique, -1 );

                    LUA_GET_VALUE_DEF( "ID", mesh.strID, "" );
                    //모션달린 모델들
                    LUA_GET_VALUE_DEF( "BONEID", mesh.iBoneId, -1 );
                    LUA_GET_VALUE_DEF( "MOTION", mesh.strMotionName, "" );					
                    LUA_GET_VALUE_DEF( "POS_X", mesh.vOptionalPos.x, 0.0f );
                    LUA_GET_VALUE_DEF( "POS_Y", mesh.vOptionalPos.y, 0.0f );
                    LUA_GET_VALUE_DEF( "POS_Z", mesh.vOptionalPos.z, 0.0f );
                    tempObject.vecMesh.push_back( mesh );
                    luaMgr.EndTable();
                    ++iMeshIndex;
                }
                luaMgr.EndTable();          
            }

            if( SUCCEEDED( luaMgr.BeginTable( "Motion" ) ) )
            {
                for( int i=1 ; ; ++i )
                {
                    std::string strTemp;
                    if( FAILED( luaMgr.GetValue( i, strTemp ) ) )
                        break;
                    tempObject.vecMotion.push_back( strTemp );
                }
                luaMgr.EndTable();
            }

            if( SUCCEEDED( luaMgr.BeginTable( "FootHoldData" ) ) )
            {
                int iFootHoldIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iFootHoldIndex ) ) )
                        break;

                    KAgitFootHold tempFootHold;

                    LUA_GET_VALUE_DEF( 1, tempFootHold.iPosX, 0 );
                    LUA_GET_VALUE_DEF( 2, tempFootHold.iPosY, 0 );
                    LUA_GET_VALUE_DEF( 3, tempFootHold.dwProp, 0 );
                    tempObject.vecFootHold.push_back( tempFootHold );
                    luaMgr.EndTable();
                    ++iFootHoldIndex;
                }
                luaMgr.EndTable();          
            }

            if( SUCCEEDED( luaMgr.BeginTable( "FlowerPotParticle" ) ) )
            {
                int iFlowerPotParticleIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iFlowerPotParticleIndex ) ) )
                        break;

                    int iType;
                    KAgitObjectParticle tempParticle;

                    LUA_GET_VALUE_DEF( 1, iType, 0 );
                    LUA_GET_VALUE_DEF( 2, tempParticle.strSquence, "" );
                    LUA_GET_VALUE_DEF( 3, tempParticle.fX, 0.0f );
                    LUA_GET_VALUE_DEF( 4, tempParticle.fY, 0.0f );
                    tempObject.mapFlowerPotParticle[ iType ] = tempParticle;
                    luaMgr.EndTable();
                    ++iFlowerPotParticleIndex;
                }
                luaMgr.EndTable();          
            }

            if( SUCCEEDED( luaMgr.BeginTable( "ObjectParticle" ) ) )
            {
                int iObjectParticleIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( iObjectParticleIndex ) ) )
                        break;

                    KAgitObjectParticle tempParticle;

                    LUA_GET_VALUE_DEF( 1, tempParticle.strSquence, "" );
                    LUA_GET_VALUE_DEF( 2, tempParticle.fX, 0.0f );
                    LUA_GET_VALUE_DEF( 3, tempParticle.fY, 0.0f );
                    tempObject.vecObjectParticle.push_back( tempParticle );
                    luaMgr.EndTable();
                    ++iObjectParticleIndex;
                }
                luaMgr.EndTable();          
            }

            if( SUCCEEDED( luaMgr.BeginTable( "Sub2DObject" ) ) )
            {
                int isubObjIndex = 1;

                while( true )
                {
                    if( FAILED( luaMgr.BeginTable( isubObjIndex ) ) )
                        break;

                    KAgit2DSubObject tempObj;

                    LUA_GET_VALUE_DEF( 1, tempObj.strTexture, "" );
                    LUA_GET_VALUE_DEF( 2, tempObj.fX, 0.0f );
                    LUA_GET_VALUE_DEF( 3, tempObj.fY, 0.0f );
                    tempObject.vec2DSubObject.push_back( tempObj );
                    luaMgr.EndTable();
                    ++isubObjIndex;
                }
                luaMgr.EndTable();          
            }

            AddObjectInfo( tempObject.dwObjectID, tempObject );
    luaMgr.EndTable();
        }
    }
    luaMgr.EndTable();

    return true;
}

bool KGCAgitObjectInfoList::IsSpecialObject( GCITEMID _itemID )
{
    if ( m_mapSpecialObjectInfo.find( _itemID ) == m_mapSpecialObjectInfo.end() )
        return false;

    return true;
}

KGCAgitObjectAction* KGCAgitObjectInfoList::GetObjectAction( GCITEMID _itemID, GCITEMUID _itemUID )
{
    MAP_SPECIAL_AGIT_OBJ_ITER pos = m_mapSpecialObjectInfo.find( _itemID );

    if ( pos == m_mapSpecialObjectInfo.end() ) 
        return NULL;

    EM_AGIT_OBJECT_ACTION emAction = pos->second;

    KGCAgitObjectAction* pAction = NULL;

    switch ( emAction ) 
    {
    case EM_AGIT_OBJECT_JUMP_ACTION:
        {
            MAP_AGIT_JUMP_OBJECT_ITER pos_obj = m_mapJumpObject.find( _itemID );
            
            if ( pos_obj == m_mapJumpObject.end() )
                break;

            const KAgitJumpObject& kAgitJumpObject = pos_obj->second;

            KGCAgitJumpAction* pJumpAction = new KGCAgitJumpAction;

            pJumpAction->Create();			
            pJumpAction->SetJump( kAgitJumpObject.m_fJumpX, kAgitJumpObject.m_fJumpY ); 
            pJumpAction->SetEffectName( kAgitJumpObject.m_vecEffectName );
            pJumpAction->SetSound( kAgitJumpObject.m_nSound );
            pJumpAction->SetItemUID( _itemUID );
            pJumpAction->SetEffctNum ( kAgitJumpObject.m_nBasicEffectNum );

            pAction = static_cast<KGCAgitObjectAction*>(pJumpAction);
        }
        break;
    case EM_AGIT_OBJECT_DICE_ACTION:
        {
            MAP_AGIT_DICE_OBJECT_ITER pos_obj = m_mapDiceObject.find( _itemID );

            if ( pos_obj == m_mapDiceObject.end() )
                break;

            const KAgitDiceObject& kAgitDiceObject = pos_obj->second;

            KGCAgitDiceAction* pDiceAction = new KGCAgitDiceAction;

            pDiceAction->Create();			
            pDiceAction->SetEffectName( kAgitDiceObject.m_vecEffectName );
            pDiceAction->SetSound( kAgitDiceObject.m_nSound );
            pDiceAction->SetItemUID( _itemUID );
            pDiceAction->SetEffctNum ( kAgitDiceObject.m_nBasicEffectNum );

            pAction = static_cast<KGCAgitObjectAction*>(pDiceAction);
        }
        break;
    case EM_AGIT_OBJECT_FLAG_ACTION:
        {
            MAP_AGIT_FLAG_OBJECT_ITER pos_obj = m_mapFlagObject.find( _itemID );

            if ( pos_obj == m_mapFlagObject.end() )
                break;

            const KAgitFlagObject& kAgitFlagObject = pos_obj->second;

            KGCAgitFlagAction* pFlagAction = new KGCAgitFlagAction;

            pFlagAction->Create();			
            pFlagAction->SetEffectName( kAgitFlagObject.m_vecEffectName );
            pFlagAction->SetSound( kAgitFlagObject.m_nSound );
            pFlagAction->SetItemUID( _itemUID );

            pAction = static_cast<KGCAgitObjectAction*>(pFlagAction);
        }
        break;
    default:
        break;
    }

    return pAction;
}

bool KGCAgitObjectInfoList::IsAgitObject( GCITEMID itemID_ )
{
    std::map< GCITEMID, KAgitObjectInfo >::iterator mit = m_mapObjectInfo.find( itemID_ * 10 );
    if( mit == m_mapObjectInfo.end() ) {
        return false;
    }

    return true;
}