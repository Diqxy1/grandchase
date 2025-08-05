#pragma once

#include "D3DWndClass.h"
#include "atlstr.h"
//#include <map>
//#include <vector>

enum DataType  {IDE_UNDEFINE, IDE_STRING, IDE_INT, IDE_FLOAT};

class KWndDataPackage
{
	class WndData
	{
	public:
		WndData(void)
		{
			m_eDataType = IDE_UNDEFINE;
			m_strWndName.clear();
			m_pRealData = NULL;
		}
		WndData( const WndData& src )
		{
			m_eDataType = src.m_eDataType;
			m_strWndName = src.m_strWndName;
			m_pRealData = src.m_pRealData;
		}
		~WndData()
		{}
		
		
		std::string ToString( UINT uiTab );
		void Init(std::string , float*);
		void Init(std::string , int*);
		void Init(std::string , std::string*);
		
		void SetValue(char*);
		CString GetValue();
		CString GetName();

		DataType m_eDataType;
		std::string m_strWndName;
		
		void*	m_pRealData;
        std::string m_strTextureFileName;
        void SetTexFileString(std::vector<std::string>& pVecString);

	};

public:
	KWndDataPackage();
	~KWndDataPackage();
	
	void SetValue(std::string strMsg, char* Value);
	
	void PushWnd(KD3DWndClass* Wnd);
    void PushWndSimple(KD3DWndClass* Wnd);
    void CheckTexData(KD3DWndClass* Wnd);

	void PushData(std::string strName, std::string* pValue);
	void PushData(std::string strName, int* pValue);
	void PushData(std::string strName, float* pValue);

	bool GetData(int index, std::string* pName, std::string* pValue);
	void Clear();

	std::string	ToString( UINT uiTab );

	std::map<std::string , WndData> m_mapData;
	std::vector<WndData>	m_vecData;
	std::vector<std::string> m_vecChildList;
    void SetTexFileString(std::vector<std::string>& pVecString);
    std::vector<std::string> m_vecTextureFileName;

};