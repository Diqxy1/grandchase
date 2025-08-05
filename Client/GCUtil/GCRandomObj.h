#pragma once
//#include <boost/random.hpp>
#include <WTypes.h>

// SAFE_DELETE
//
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=NULL; } }
#endif

namespace GCUTIL{

    template< class ParamType, template<class> class RangeType > //타입
    class KGCRandom
    {
    protected:
        DWORD m_dwSeed;
        boost::lagged_fibonacci607 m_kEngine;
        RangeType<ParamType>* m_pkRange;
        boost::variate_generator<boost::lagged_fibonacci607&, RangeType<ParamType> >*  m_pkRandom;
    public:
        KGCRandom() 
            : m_pkRandom( NULL ) 
            , m_pkRange( NULL )
            , m_dwSeed( timeGetTime() )
        {
            SetRange( 0, 100 );
        }
		~KGCRandom() {
			SAFE_DELETE( m_pkRandom );
			SAFE_DELETE( m_pkRange );
		}
		void SetRange( ParamType begin, ParamType end )
		{
			SAFE_DELETE( m_pkRandom );
			SAFE_DELETE( m_pkRange );
			m_pkRange = new RangeType<ParamType>( begin, end );
			m_pkRandom = new boost::variate_generator<boost::lagged_fibonacci607&, RangeType<ParamType> >(m_kEngine, (*m_pkRange));
			m_kEngine.seed( m_dwSeed );
		}
		void SetSeed( DWORD dwSeed )
		{
			if( m_pkRange == NULL )
				return;

			if( dwSeed == 0 )
				dwSeed = 1;

			m_dwSeed = dwSeed;
			m_kEngine.seed( m_dwSeed );
		}
		ParamType rand()
		{
			return (*m_pkRandom)(); 
		}
	};


	typedef KGCRandom< int, boost::uniform_int > GCRand_Int;
	typedef KGCRandom< float, boost::uniform_real > GCRand_float;
}
