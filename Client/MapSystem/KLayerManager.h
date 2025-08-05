#pragma once

//#include <vector>
//#include <map>
//#include <string>
//#include "../GCLuaManager/KLuaManager.h"
//#include "../GCLuaManager/KLuabinder.h"
//#include <d3d9types.h>
#include "KMap.h"
#include "KLayerPart.h"

/************************************************************************/
/*  KGCLayerManager가 해야하는 일들

1. KGCLayer를 가지고 관리. ( Action이 일어나는 Layer는 GCFantasticMap에서 관리하자. ) (O)
2. KGCLayer끼리 Z-Order 관리. (O)

15 ~ 6 까지 그리고
플레이어 그리고
5 ~ 0 까지 그림.
/************************************************************************/

#define PLAYER_FOOTHOLD_LAYER_TERM 5

struct KLayerPartData
{
    std::string PartCategory;
    std::string PartName;
    std::string TexName;
    KLayerRect<float> kLayerRect;
    KLayerRect<int> kTexRect;
    std::vector<KLayerParticleData> vecParticle;

};

// T0 : Part, T1 : Layer, T2 : Map
template <class T0, class T1, class T2>
class KLayerManager
{
private:
    explicit KLayerManager(void) {}
public:
    KLayerManager( T2* pMap );
    virtual ~KLayerManager(void);

    virtual void        FrameMove( float fElapsedTime ) = 0;
    virtual void        RegisterLuabind() = 0;
    virtual void        SetLayer() = 0;
    virtual void        AddLayerPartsErr( std::string partName_ );

    virtual T2*         GetMapPtr() { return m_pMap; }

    virtual void        clear();
    virtual void        UnLoad();

    virtual T0*         CreateClonePart( std::string PartName );
    virtual T0*         GetOriginalPart( std::string PartName );

    virtual void        LoadParts( KLuaManager& luaMgr ) = 0;
    virtual void        AddLayerParts();
    virtual bool        LoadFromLua( KLayerPartData& kData ) = 0;
    virtual void        CreateLayer( KLuaManager& luaMgr );
    virtual void        SetLayerSize( KLuaManager& luaMgr ) = 0;
    virtual bool        LoadScript( std::string Script );
    virtual void        SetPartsCategoryList() = 0;

    virtual int         GetPlayerLayer() { return m_iPlayingLayer; }
    virtual void        SetMaxLayerNum( int iMax ) { m_iPrevLayerNum = m_iMaxLayerNum; m_iMaxLayerNum = iMax; }
    virtual void        SetGamePlayLayer( int iLayer ) { m_iPlayingLayer = iLayer; }

    std::vector< T1* >  GetLayer() { return m_vecLayer; }
    int                 GetMaxLayerNum() { return m_iMaxLayerNum; }

protected:
    std::map< std::string, T0* >    m_mapPartPool;
    std::vector< T1* >              m_vecLayer;
    std::vector< std::string >      m_vecPartCategoryList;

    int                             m_iMaxLayerNum;
    int                             m_iPlayingLayer;
    int                             m_iPrevLayerNum;

    T2*                             m_pMap;
};

#include "KLayerManager.inl"
