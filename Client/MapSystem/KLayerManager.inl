//#include "../GCLuaManager/KLuabinder.h"
//#include "../GCLuaManager/KLuaManager.h"
//#include "../ParticleLib/GCEffectManager.h"
#include "../GCDeviceLib/GCDeviceShader.h"

// T0 : Part, T1 : Layer, T2 : Map
template <class T0, class T1, class T2>
KLayerManager<T0, T1, T2>::KLayerManager( T2* pMap ) : m_iMaxLayerNum(0), m_iPlayingLayer(0), m_iPrevLayerNum(0), m_pMap(NULL)
{
    m_vecLayer.clear();
    m_mapPartPool.clear();

    m_pMap = pMap;
}

template <class T0, class T1, class T2>
KLayerManager<T0, T1, T2>::~KLayerManager(void)
{
    clear();
}

template <class T0, class T1, class T2>
void KLayerManager<T0, T1, T2>::clear()
{
    std::map< std::string, T0* >::iterator itor;
    for( itor = m_mapPartPool.begin(); itor != m_mapPartPool.end(); ++itor )
    {
        T0* pPart = itor->second;
        SAFE_DELETE( pPart );
    }

    UnLoad();

    m_mapPartPool.clear();
    m_vecPartCategoryList.clear();
}

template <class T0, class T1, class T2>
void KLayerManager<T0, T1, T2>::UnLoad()
{
    // 이전 데이터들 삭제
    if( !m_vecLayer.empty() )
    {
        for( int i=0 ; i< (int)m_vecLayer.size(); ++i )
        {
            m_vecLayer[i]->clear();
            SAFE_DELETE( m_vecLayer[i] );
        }
        m_vecLayer.clear();
    }
}

template <class T0, class T1, class T2>
void KLayerManager<T0, T1, T2>::AddLayerParts()
{
    KLayerPartData kData;
    if( LoadFromLua( kData ) == false )
        return;

    T0* pPart = new T0();
    pPart->SetPartCategory( kData.PartCategory );
    pPart->SetPartName( kData.PartName );
    pPart->SetTexturePart( kData.TexName, kData.kTexRect );
    pPart->SetLayerPos( kData.kLayerRect );
    pPart->SetParticle( kData.vecParticle );

    if( !m_mapPartPool.count(pPart->GetPartName()) )
        m_mapPartPool[pPart->GetPartName()] = pPart;
}

template <class T0, class T1, class T2>
void KLayerManager<T0, T1, T2>::AddLayerPartsErr( std::string partName_ )
{
    ASSERT( partName_.c_str() && !"Invalid Part Add" );
}

template <class T0, class T1, class T2>
void KLayerManager<T0, T1, T2>::CreateLayer( KLuaManager& luaMgr )
{
    std::stringstream stm;
    LUA_BEGIN_TABLE("LAYER_DATA", return)
    {
        for( int i = 0; i < (int)m_vecLayer.size(); ++i )
        {
            stm.str("");
            stm << "LAYER" << i;

            LUA_BEGIN_TABLE( stm.str().c_str(), break )
            {
                for( int j = 1; ; ++j )
                {
                    LUA_BEGIN_TABLE(j, break)
                    {
                        m_vecLayer[i]->AddParts( luaMgr );
                    }
                    LUA_END_TABLE(break);
                }
            }
            LUA_END_TABLE( break );
        }
    }
    LUA_END_TABLE(return);

    for( int i = 0; i < (int)m_vecLayer.size(); ++i )
    {
        m_vecLayer[i]->CreateBuffer();
    }
}

template <class T0, class T1, class T2>
T0* KLayerManager<T0, T1, T2>::CreateClonePart( std::string PartName )
{
    if( m_mapPartPool[PartName] == NULL )
        return NULL;
    else
        return m_mapPartPool[PartName]->Clone();
}

template <class T0, class T1, class T2>
T0* KLayerManager<T0, T1, T2>::GetOriginalPart( std::string PartName )
{
    if( m_mapPartPool[PartName] == NULL )
        return NULL;
    else
        return m_mapPartPool[PartName];
}

template <class T0, class T1, class T2>
bool KLayerManager<T0, T1, T2>::LoadScript( std::string Script )
{
    m_pMap->RegisterLuabind();

    KLuaManager luaMgr;
    if( luaMgr.DoFile( Script.c_str() ) == E_FAIL )
    {
        ASSERT( !"Load Failed!" && Script.c_str() );
        return false;
    }

    m_pMap->LoadBaseData( luaMgr );
    CreateLayer( luaMgr);
    SetLayerSize( luaMgr );
    m_pMap->LoadFootHoldData( luaMgr );
    return true;
}
