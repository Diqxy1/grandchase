#pragma once
#include "KNCSingleton.h"
#include "PaletteStruct.h"
#include "stdafx.h"

//#include <vector>
//#include <map>

class CObjectDataManager
{
	DeclareSingleton(CObjectDataManager);

public:
	CObjectDataManager(void);
	~CObjectDataManager(void);

	void AddEnumItem(const OBJECT_TYPE_EACTION enType, const CString strEnum);
	std::vector<CString>& GetEnumItem(const OBJECT_TYPE_EACTION enType);
	void DelEnumItem(const OBJECT_TYPE_EACTION enType, const CString strEnum);
	void ClearEnumItem(void);

	void AddObjImgItem(const OBJECT_TYPE_EACTION enType, const std::pair<CString, CString> prObjImg);
	std::map<CString, CString>& GetObjImgItem(const OBJECT_TYPE_EACTION enType);
	void DelObjImgItem(const OBJECT_TYPE_EACTION enType, const CString strObjEnum);
	void ClearObjImgItem(void);

protected:
	std::map< OBJECT_TYPE_EACTION, std::vector<CString> >       m_mapObjEnum;   // < Type, Enum >
	std::map< OBJECT_TYPE_EACTION, std::map<CString, CString> > m_mapObjImg;   // < Type, <Enum,ImgFilePath>
};

DefSingletonInline(CObjectDataManager);