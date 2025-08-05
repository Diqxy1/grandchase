#pragma once
#include "GCAgitSpecialObject.h"

struct KAgitObjectParticle
{
    std::string strSquence;    // ��ƼŬ �̸� 
    float       fX;            // ��ġ 
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
    DWORD dwObjectID;                           // ������Ʈ ID
    int iObjectType;                            // ������Ʈ Ÿ��
    int iCharType;                              // ĳ������ ��� ĳ���� Ÿ�� 
    int iLayerNum;                              // �׷��� ���� 
    std::vector< std::string > vecTexture;      // ���ҽ� ����Ʈ 
    std::vector< EXTRA_MESH >  vecMesh;
    std::vector< std::string > vecMotion;
    std::vector< KAgitFootHold > vecFootHold;   // ������Ʈ ���� 
    int iStrNameID;                             // �̸��� StringID
    bool bContact;                              // �ٴڿ� ���� ���ΰ�?
    bool bCanStack;                             // �� �Ӽ��� false �̸� �ٴڿ��� ���� �� ����.
    bool bOnlyTop;                              // �� �Ӽ��� true �� ����� ������Ʈ ������ �ƹ��͵� ���� �� ����.

    std::map< int, KAgitObjectParticle > mapFlowerPotParticle;  // ȭ�� ��ƼŬ < ȭ�� ���� , ��ƼŬ > 
    std::vector< KAgitObjectParticle>    vecObjectParticle;     // ������Ʈ �Ϲ� ��ƼŬ 
    std::vector< KAgit2DSubObject >      vec2DSubObject;        // �߰��� �ٴ� 2D ������Ʈ 

    bool bEnableSitDown;    // ���� �� �ִ� �����Ʈ �ΰ�?
    bool bEnableLie;        // ���� �� �ִ� ������Ʈ �ΰ�?
    
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
