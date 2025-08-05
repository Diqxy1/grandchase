#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "PrimitiveTypedef.h"
#include "UserPacket.h"

class KLuaManager;

class KPetGlyphMng
{
	DeclareSingleton( KPetGlyphMng );
	NiDeclareRootRTTI( KPetGlyphMng );
	DeclToStringW;

public:

	KPetGlyphMng(void);
	~KPetGlyphMng(void);

	bool LoadScript();
	bool _LoadScript( OUT std::string strScript = "InitPetGlyph.lua");
	bool LoadGlyphInfo(IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<GCITEMID, KPetGlyphData>& vecGlyphData);

	void GetPetGlyphData(OUT std::map<GCITEMID, KPetGlyphData>& mapGlyphData)
	{
		KLocker lock(m_csPetGlyphData);
		mapGlyphData.insert(m_mapPetGlyphData.begin(), m_mapPetGlyphData.end());
	};

	int GetPetGlyphType( IN const GCITEMID dwItemID )
	{
		std::map<GCITEMID, KPetGlyphData>::iterator it = m_mapPetGlyphData.find(dwItemID);

		if(it != m_mapPetGlyphData.end())
		{
			return it->second.m_iGlyphEffect;
		}

		return -1;
	}

protected:
	mutable KncCriticalSection			m_csPetGlyphData;
	std::map<GCITEMID, KPetGlyphData>	m_mapPetGlyphData;	
};

DefSingletonInline( KPetGlyphMng );
DeclOstmOperatorA( KPetGlyphMng );
