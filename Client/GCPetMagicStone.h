#pragma once

#include <KNCSingleton.h>
#include "UserPacket.h"

enum class PET_MAGIC_STONE_EFFECT_TYPE
{
	PMSET_INCREASE_PET_DAMAGE = 0,	//
	PMSET_RETAIN_PET_MP = 1,		//Done
	PMSET_PET_MP_RECOVERY = 2,		//Done
	PMSET_INCREASE_MP = 3,			//Done
	PMSET_INCREASE_AP = 4,			//Done
	PMSET_INCREASE_CRITICAL = 5		//
};


class GCPetMagicStone
{
    DeclareSingleton(GCPetMagicStone);

public:

	using PetGlyphMap = std::map<GCITEMID, KPetGlyphData>;


	GCPetMagicStone();
    virtual ~GCPetMagicStone();

	void SetPetGlyphData(KEVENT_PET_MAGIC_STONE_INFO_NOT& kRecv);
	std::vector<GCITEMID> GetSealedPetGlyphID();
	bool IsPetGlyph(GCITEMID dwItemID);
	bool IsSealedPetGlyph(GCITEMID dwItemID);
	KPetGlyphData GetPetGlyph(GCITEMID dwItemID);

	float GetGlyphEffectAmmount(PET_MAGIC_STONE_EFFECT_TYPE eType);

	std::wstring GetEffectDesc(GCITEMID dwItemID);

	PetGlyphMap GetPetGlyphMap() { return m_mapPetGlyph; }

	PetGlyphMap				m_mapPetGlyph;
	std::vector<GCITEMID>	m_vecSealedPetGlyph;
};

DefSingletonInline(GCPetMagicStone);
