#pragma once

//{{ 060128. kkurrung.
/*
{
.INI파일을 읽어들여서 원하는 문자열을 얻어오는것이 목적
약간의 확장성을 위해서 아래의 데이터 구조를 사용하겠다.

map<"section",map >
			  ---
			   |
			   ---> map< key, data >
							  ----
								|
								---> struct
									 {
										std::wstring key;
										std::wstring value;
									 }
strings.ini
------------------
[section1]
key1 = value1
key2 = value2
key3 = value3
	:
	:
[section2]
key1 = value1
key2 = value2
key3 = value3
	:
	:
-----------------

이러한 구조의 파일을 읽을수 있게
만들 예정이다.

protocol Version 과 같이 사용하지 않을 예정..
(참고로 프로토콜 버전은 읽기 쓰기 가능하지만. 이것은 읽기만 가능 하게 할 예정임.
 또, 임의의 섹션을 읽어 들일 예정이기 때문에 껄끄럽게 Int값을 가지는 놈이 있고 수정도 가능해야 하기 때문에
 좀 커지고, 단순해지지 않을것 같아서임.. + 귀찬아서.. )

사용할때는 원한는 섹션과 키를 입력으로 받게 될것이고
그에 해당하는 값을 반환할 예정이다.

그리고 Current Section을 설정해서
임의의 섹션에 항상 접근 해 있는 상대를 유지할수 있게도 할것이다.

섹션내에서 키의 중복은 미리 피해야 한다.(오류를 발생할것이다.)
섹션의 이름은 중복해도 가능하게 할것이다.(되도록 피하는 것이 좋을것이다)
그러나 동일한이름의 섹션내에서 동일한 키는 즐쳐드셈이다.

동일한 섹션과 키가 존재하지 않는다면 여러개의 ini도 읽을수 있게 할까 말까?(보류)
(이유.. 섹션 읽기 제한이 32k characters라서 한번 생각해봤음)

(AddIni()) -- 보류....
LoadIni();
std::wstring GetValue( sectionName, Key );
bool SetCurrentSection( sectionName);
std::wstring GetValue(Key);
void Dump()

이정도의 함수만 노출 할 예정이다.

아마도 SimLayer에 Has a로 .....
}
*/
//}} kkurrung.

#include <map>
#include <string>
#include "../../CommonFiles/CommonPacket.h" // CON_COPYCON_ASSIGNOP
#include <KncUtil.h> // IN, OUT
#include <Thread/Locker.h>

class KStringTable
{
protected:
	struct Key_value
	{
		std::wstring    m_strKey;
		std::wstring    m_strValue;

		CON_COPYCON_ASSIGNOP(Key_value, right)
		{
			m_strKey = right.m_strKey;
			m_strValue = right.m_strValue;
			return *this;
		};
	};

	struct Section
	{
		std::wstring                        m_strSection;
		std::map< std::wstring, Key_value > m_mapKeyValue; //<key, keyValue>

		CON_COPYCON_ASSIGNOP(Section, right)
		{
			m_strSection = right.m_strSection;
			m_mapKeyValue = right.m_mapKeyValue;
			return *this;
		};
	};

	typedef std::map<std::wstring, Section>     MAPSECTION;
	typedef MAPSECTION::iterator                MITSECTION;

	typedef std::map<std::wstring, Key_value>   MAPKEYVALUE;
	typedef MAPKEYVALUE::iterator               MITKEYVALUE;

public:
	KStringTable(void);
	~KStringTable(void);
public:
	bool LoadIni(IN const wchar_t* szFileName);
	bool ReLoadIni();
	std::wstring GetValue(IN const wchar_t* szSectionName, IN const wchar_t* Key);
	std::wstring GetValue(IN const wchar_t* Key);
	bool SetCurrentSection(IN const wchar_t* szSectionName);
	std::wstring GetCurrentSection() { return m_strCurrentSection; };
	void Dump(std::wostream& stm)const;
	std::wstring GetCurrentFile() { return m_strFileName; };
	bool GetAllKeys(IN const wchar_t* szSectionName, OUT std::vector<std::wstring>& vecKey);
protected:
	bool AddAndBuildSection(IN const wchar_t* szSectionName, IN const wchar_t* szFileName, IN DWORD dwFileSize);
	bool AddAndBuildKeyvalue(IN const wchar_t* szKey_value, Section& section);
	bool FindSection(IN const wchar_t* szSectionName, OUT MITSECTION& itSection);
	bool FindKey_value(IN MITSECTION& itSection, IN const wchar_t* szKeyName,
		OUT MITKEYVALUE& keyValue);

protected:
	mutable KncCriticalSection          m_csSectionTable;
	MAPSECTION                          m_mapSectionTable;
	std::wstring                        m_strCurrentSection;
	std::wstring                        m_strFileName;
};
