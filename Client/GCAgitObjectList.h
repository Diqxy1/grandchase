#pragma once
#include "GCAgitSpecialObject.h"

struct KAgitObjectParticle
{
    std::string strSquence;    // 파티클 이름 
    float       fX;            // 위치 
    float       fY;

    KAgitObjectParticle( void )
    {
        strSquence.clear();
        fX = 0.0f;
        fY = 0.0f;
    }
};

struct KAgit2DSubObject
{
    std::string strTexture;
    float       fX;
    float       fY;

    KAgit2DSubObject( void )
    {
        strTexture.clear();
        fX = 0.0f;
        fY = 0.0f;
    }
};

struct KAgitObjectInfo
{
    DWORD dwObjectID;                           // 오브젝트 ID
    int iObjectType;                            // 오브젝트 타입
    int iCharType;                              // 캐릭터일 경우 캐릭터 타입 
    int iLayerNum;                              // 그려질 순서 
    std::vector< std::string > vecTexture;      // 리소스 리스트 
    std::vector< EXTRA_MESH >  vecMesh;
    std::vector< std::string > vecMotion;
    std::vector< KAgitFootHold > vecFootHold;   // 오브젝트 발판 
    int iStrNameID;                             // 이름의 StringID
    bool bContact;                              // 바닥에 붙을 것인가?
    bool bCanStack;                             // 이 속성이 false 이면 바닥에만 놓을 수 있음.
    bool bOnlyTop;                              // 이 속성이 true 인 경우이 오브젝트 위에는 아무것도 쌓을 수 없음.

    std::map< int, KAgitObjectParticle > mapFlowerPotParticle;  // 화분 파티클 < 화분 상태 , 파티클 > 
    std::vector< KAgitObjectParticle>    vecObjectParticle;     // 오브젝트 일반 파티클 
    std::vector< KAgit2DSubObject >      vec2DSubObject;        // 추가로 붙는 2D 오브젝트 

    bool bEnableSitDown;    // 앉을 수 있는 모브젝트 인가?
    bool bEnableLie;        // 누울 수 있는 오브젝트 인가?
    
    KAgitObjectInfo( void )
    {
        dwObjectID = -1;
        iObjectType = AGIT_INVALID_OBJECT;
        iCharType = GC_CHAR_INVALID;
        iLayerNum = -1;
        vecTexture.clear();
        vecMesh.clear();
        vecMotion.clear();
        vecFootHold.clear();
        iStrNameID = -1;
        bContact = true;
        bCanStack = true;
        bOnlyTop = false;
        mapFlowerPotParticle.clear();
        vecObjectParticle.clear();
        vec2DSubObject.clear();
        bEnableSitDown = false;
        bEnableLie = false;
    }

    KAgitObjectInfo( const KAgitObjectInfo& kInfo_ ) {
        dwObjectID = kInfo_.dwObjectID;
        iObjectType = kInfo_.iObjectType;
        iCharType = kInfo_.iCharType;
        iLayerNum = kInfo_.iLayerNum;
        vecTexture = kInfo_.vecTexture;
        vecMesh = kInfo_.vecMesh;
        vecMotion = kInfo_.vecMotion;
        vecFootHold = kInfo_.vecFootHold;
        iStrNameID = kInfo_.iStrNameID;
        bContact = kInfo_.bContact;
        bCanStack = kInfo_.bCanStack;
        bOnlyTop = kInfo_.bOnlyTop;
        mapFlowerPotParticle = kInfo_.mapFlowerPotParticle;
        vecObjectParticle = kInfo_.vecObjectParticle;
        vec2DSubObject = kInfo_.vec2DSubObject;
        bEnableSitDown = kInfo_.bEnableSitDown;
        bEnableLie = kInfo_.bEnableLie;
    }
};




class KGCAgitObjectAction;

class KGCAgitObjectInfoList
{
public:
    KGCAgitObjectInfoList(void);
    ~KGCAgitObjectInfoList(void);

    typedef const KAgitObjectInfo element_type;

private:
    std::map< GCITEMID, KAgitObjectInfo > m_mapObjectInfo;
    typedef std::map< GCITEMID, KAgitObjectInfo > TObjectInfo;

    MAP_SPECIAL_AGIT_OBJ        m_mapSpecialObjectInfo;
    MAP_AGIT_JUMP_OBJECT        m_mapJumpObject;
    MAP_AGIT_DICE_OBJECT        m_mapDiceObject;
    MAP_AGIT_FLAG_OBJECT        m_mapFlagObject;

private:
    bool InsertSpecialObject( GCITEMID _itemID, EM_AGIT_OBJECT_ACTION emAction );
    bool IsInsertObject( GCITEMID _itemID, EM_AGIT_OBJECT_ACTION emAction );
    bool InsertJumpObject( GCITEMID _itemID, const KAgitJumpObject& kAgitJumpObject );
    bool InsertDiceObject( GCITEMID _itemID, const KAgitDiceObject& kAgitDiceObject );
    bool InsertFlagObject( GCITEMID _itemID, const KAgitFlagObject& kAgitFlagObject );

public:
    bool IsSpecialObject( GCITEMID _itemID );
    KGCAgitObjectAction* GetObjectAction( GCITEMID _itemID, GCITEMUID _itemUID );
    MAP_AGIT_FLAG_OBJECT& GetMapFlagObject() { return m_mapFlagObject; }


public:
    bool LoadObjectInfo( const std::string& strFile );
    bool LoadSpecialObjectInfo( const std::string& strFile );
    bool AddObjectInfo( GCITEMID itemID, const KAgitObjectInfo& kObj );
    const KAgitObjectInfo* GetObjectInfo( GCITEMID itemID ) const;
    bool CheckExistObject( GCITEMID itemID ) const;
    const TObjectInfo& GetObjectInfoList() const { return m_mapObjectInfo; }
    bool IsAgitObject( GCITEMID itemID_ );
};
