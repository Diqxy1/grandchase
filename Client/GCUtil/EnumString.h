#pragma once
//#include <string>
//#include <map>

class EnumString
{
public:
	EnumString() {}

	// string 에서 enum으로 변환
	template < typename _Ty > _Ty GetEnum(const std::wstring name) const
	{ 
		std::map< std::wstring, int >::iterator itor = E2S<_Ty>::eEnum.find(name);
		if( itor == E2S<_Ty>::eEnum.end() )
        {
            //assert(!"Error ***String->Enum***");
            return L"Error ***String->Enum***";
        }
		return itor->second;
	} 

	// enum 에서 string으로 변환
	template < typename _Ty > std::wstring GetName(const _Ty eEnum) const
	{ 
		std::map< int, std::wstring >::iterator itor = E2S<_Ty>::strName.find(eEnum);
		if( itor == E2S<_Ty>::strName.end() )
        {
            //assert(!"Error ***Enum->String***");
            return L"Error ***Enum->String***";
        }
		return itor->second;
	}

	// 새로운 enum/string 쌍을 추가하는 메소드
	template < typename _Ty > void SetEnumName(const _Ty eNum, const std::wstring name)
	{
		E2S<_Ty>::strName[eNum] = name;
		E2S<_Ty>::eEnum[name] = eNum;		
	}

private:
	template < typename _Ty >
	struct E2S
	{
		static std::map<int, std::wstring> strName; // static 임이 중요
		static std::map< std::wstring, int > eEnum; // static임이 중요
	};
};

template <typename _Ty>
std::map<int, std::wstring> EnumString::E2S<_Ty>::strName;

template <typename _Ty>
std::map< std::wstring, int > EnumString::E2S<_Ty>::eEnum;

#define ENUM_STR_GETNAME( ID )  KSingleton<EnumString>::GetInstance()->GetName( ID )
#define ENUM_STR_GETENUM( str ) KSingleton<EnumString>::GetInstance()->GetEnum( str )