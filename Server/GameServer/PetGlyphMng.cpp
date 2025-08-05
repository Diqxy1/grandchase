#include "CnConnector.h"
#include "PetGlyphMng.h"
#include "userevent.h"
#include "UserPacket.h"
#include <dbg/dbg.hpp >
#include "NetError.h"
#include "Lua/KLuaManager.h"

ImplementSingleton( KPetGlyphMng );
ImplOstmOperatorW2A( KPetGlyphMng );
NiImplementRootRTTI( KPetGlyphMng );


KPetGlyphMng::KPetGlyphMng()
{
}

KPetGlyphMng::~KPetGlyphMng()
{
}

ImplToStringW( KPetGlyphMng )
{
	KLocker lock(m_csPetGlyphData);
	START_TOSTRINGW
		<< TOSTRINGW(m_mapPetGlyphData.size() );

	return stm_;
}

bool KPetGlyphMng::LoadScript()
{
	return _LoadScript();
}

bool KPetGlyphMng::_LoadScript( std::string strScript )
{
	KLuaManager kLuaMng;

	std::map<GCITEMID, KPetGlyphData> mapGlyphData;

	_JIF( kLuaMng.DoFile( strScript.c_str() ) == S_OK, return false );
	_JIF( LoadGlyphInfo( kLuaMng, std::string("Glyphs"), mapGlyphData ), return false );

	START_LOG(clog, "Pet Magic Stone info loaded with success") << END_LOG;
	{
		m_mapPetGlyphData.swap(mapGlyphData);
	}

	return true;
}

bool KPetGlyphMng::LoadGlyphInfo(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<GCITEMID, KPetGlyphData>& mapGlyphData)
{
	_JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
	
	for( int i = 1 ; ; ++i )
	{
		KPetGlyphData petGlyphData;

		if( kLuaMng_.BeginTable( i ) != S_OK ) break;
			_JIF( kLuaMng_.GetValue( 1, petGlyphData.m_dwGlyphID ) == S_OK, return false );
			_JIF( kLuaMng_.GetValue( 2, petGlyphData.m_iGlyphType ) == S_OK, return false );
			_JIF( kLuaMng_.GetValue( 3, petGlyphData.m_iGlyphEffect ) == S_OK, return false );
			_JIF( kLuaMng_.GetValue( 4, petGlyphData.m_iGlyphEffectAmmount ) == S_OK, return false );
		_JIF( kLuaMng_.EndTable() == S_OK, return false );

		std::map<GCITEMID, KPetGlyphData>::iterator it = mapGlyphData.find(petGlyphData.m_dwGlyphID);
		if( it == mapGlyphData.end() )
		{
			mapGlyphData.insert(std::make_pair(petGlyphData.m_dwGlyphID, petGlyphData ));
		}
	}

	_JIF( kLuaMng_.EndTable() == S_OK, return false );
	return true;
}