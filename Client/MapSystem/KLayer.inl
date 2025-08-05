#include "kLayer.h"

template< class T0 >
class KFindFileName
{
public:
    KFindFileName( std::string strComp ) : m_strComp( strComp ) {}
    
    bool operator()( const CRenderPartPair<T0> comp ) const
    {
        return m_strComp.compare( comp.first ) == 0;
    }

private:
    std::string m_strComp;
};

// T0 : LayerPart, T1 Manager
template <class T0, class T1>
KLayer<T0, T1>::KLayer( T1* pManager ) : m_fLayerSizeX(0), m_fLayerSizeY(0), m_bShow(true),
                        m_iLayerNum(0),m_pLayerManager(NULL)
{
    init();
    m_pLayerManager = pManager;
}

template <class T0, class T1>
KLayer<T0, T1>::~KLayer(void)
{    
    clear();
    m_pLayerManager = NULL;
}

template <class T0, class T1>
void KLayer<T0, T1>::clear()
{
    for( int i = 0; i < (int)m_vecPart.size(); ++i )
        SAFE_DELETE( m_vecPart[i] );

    m_vecPart.clear();
    m_vecRenderPart.clear();
    m_vecDPInfo.clear();
}

template <class T0, class T1>
void KLayer<T0, T1>::init()
{
    m_vecDPInfo.clear();
    m_vecRenderPart.clear();
    m_vecPart.clear();
}

template <class T0, class T1>
void KLayer<T0, T1>::AddPartsErrFunc( std::string partName_ )
{
    ASSERT( partName_.c_str() && !"Invalid Part Add" );
}


template <class T0, class T1>
void KLayer<T0, T1>::SetZoffset()
{
    if( m_vecPart.empty() )
        return;

    float fZterm = (float)( 5.0f / static_cast<float>(m_vecPart.size()) );

    for( int i = 0; i < (int)m_vecPart.size(); ++i )
    {
        float fZoffset = fZterm * ( (int)m_vecPart.size() - i );
        m_vecPart[i]->SetZoffset( fZoffset );
    }
}
