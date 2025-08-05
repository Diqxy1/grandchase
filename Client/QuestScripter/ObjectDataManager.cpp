#include "stdafx.h"
#include ".\objectdatamanager.h"

ImplementSingleton(CObjectDataManager);

CObjectDataManager::CObjectDataManager(void)
{
}

CObjectDataManager::~CObjectDataManager(void)
{
}

void CObjectDataManager::AddEnumItem(const OBJECT_TYPE_EACTION enType, const CString strEnum)
{
	m_mapObjEnum[enType].push_back(strEnum);
}

std::vector<CString>& CObjectDataManager::GetEnumItem(const OBJECT_TYPE_EACTION enType)
{
	return m_mapObjEnum[enType];
}

void CObjectDataManager::DelEnumItem(const OBJECT_TYPE_EACTION enType, const CString strEnum)
{
	if (!m_mapObjEnum.count(enType)) return;

	for (int i = 0; i < (int)m_mapObjEnum[enType].size(); ++i)
	{
		if (strEnum == m_mapObjEnum[enType][i])
		{
			m_mapObjEnum[enType].erase(m_mapObjEnum[enType].begin() + i);
			break;
		}
	}
}

void CObjectDataManager::ClearEnumItem(void)
{
	m_mapObjEnum.clear();
}

void CObjectDataManager::AddObjImgItem(const OBJECT_TYPE_EACTION enType, const std::pair<CString, CString> prObjImg)
{
	m_mapObjImg[enType][prObjImg.first] = prObjImg.second;
}

std::map<CString, CString>& CObjectDataManager::GetObjImgItem(const OBJECT_TYPE_EACTION enType)
{
	return m_mapObjImg[enType];
}

void CObjectDataManager::DelObjImgItem(const OBJECT_TYPE_EACTION enType, const CString strObjEnum)
{
	std::map<CString, CString>::iterator itImg = m_mapObjImg[enType].find(strObjEnum);
	if (itImg != m_mapObjImg[enType].end())
	{
		m_mapObjImg[enType].erase(itImg);
	}
}

void CObjectDataManager::ClearObjImgItem(void)
{
	m_mapObjImg.clear();
}