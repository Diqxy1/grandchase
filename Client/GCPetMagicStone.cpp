#include "stdafx.h"
//
#include "GCPetMagicStone.h"

ImplementSingleton( GCPetMagicStone );



GCPetMagicStone::GCPetMagicStone()
{
 
}

GCPetMagicStone::~GCPetMagicStone()
{
}


void GCPetMagicStone::SetPetGlyphData(KEVENT_PET_MAGIC_STONE_INFO_NOT& kRecv)
{
	m_mapPetGlyph.clear();
	m_vecSealedPetGlyph.clear();

	m_mapPetGlyph.swap(kRecv.m_mapPetGlyphData);
	m_vecSealedPetGlyph.swap(kRecv.m_vecPetSealedGlyphData);
}

std::vector<GCITEMID> GCPetMagicStone::GetSealedPetGlyphID()
{
	return m_vecSealedPetGlyph;
}

bool GCPetMagicStone::IsPetGlyph(GCITEMID dwItemID)
{
	dwItemID *= 10;

	return m_mapPetGlyph.find(dwItemID) != m_mapPetGlyph.end();
}

bool GCPetMagicStone::IsSealedPetGlyph(GCITEMID dwItemID)
{
	dwItemID *= 10;

	for (auto dwPetGlyphID : m_vecSealedPetGlyph)
	{
		if (dwPetGlyphID == dwItemID)
		{
			return true;
		}
	}

	return false;
}

KPetGlyphData GCPetMagicStone::GetPetGlyph(GCITEMID dwItemID)
{
	auto it = m_mapPetGlyph.find(dwItemID);

	if (it != m_mapPetGlyph.end())
	{
		return it->second;
	}

	return KPetGlyphData();
}

float GCPetMagicStone::GetGlyphEffectAmmount(PET_MAGIC_STONE_EFFECT_TYPE eType)
{
	for (auto glyphInfo : g_kGlobalValue.m_kUserInfo.GetCurrentChar().vecPetGlyphInfo)
	{
		auto petGlyph = GetPetGlyph(glyphInfo.m_dwID);

		if (petGlyph.m_iGlyphEffect == static_cast<int>(eType))
		{
			return static_cast<float>(petGlyph.m_iGlyphEffectAmmount) / 100.0f;
		}
	}

	return 0.0f;
}

std::wstring GCPetMagicStone::GetEffectDesc(GCITEMID dwItemID)
{
	auto petGlyph = GetPetGlyph(dwItemID*10);
	return g_pkStrLoader->GetReplacedString(STR_ID_PET_MAGIC_STONE_DAMAGE_EFFECT + petGlyph.m_iGlyphEffect, "i", petGlyph.m_iGlyphEffectAmmount);
}