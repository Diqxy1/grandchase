#pragma once
//#include "KGCLuabinder.h"
//#include "UserPacket.h"
#include "Packet.h"
//#include <string>
//#include <vector>
//#include <map>
#include "Particle/KGCParticleManager.h"

#define CALL_LUA_FUNC( func )   \
{\
    std::stringstream strFunc;\
    strFunc<<func;\
    GCFUNC::CallLuaFunction( KGCLuabinder::getInstance(), strFunc.str() );\
}

class GCDeviceFont;
class KD3DWnd;

//객체의 성격에는 맞지않지만, 여러 객체에 걸쳐서 사용해야 하는 일반적인 함수들을 가지는 객체입니다.
//일종의 글로벌 함수들의 모음이라고 할 수 있습니다.
namespace GCFUNC
{
    bool CompressStream( void* pSrc_, void*& pDest_, int iByteNum_, unsigned long& ulBound_  ); //BitStream을 넘겨주면 압축해서 출력합니다.
    bool LoadLuaScript( KLuaManager& kLuaMgr_, std::string strFileName_, bool bAssert = true );
    bool LoadLuaScript( KGCLuabinder& kLuaBinder_, std::string strFileName_, bool bAssert = true );
    bool CallLuaFunction( KGCLuabinder& kLuaBinder_, std::string strFunc_ );
	std::wstring GetCharName( int iCharType );
	bool HadChar(const int& iCharType );
    std::wstring GetPromotionName( int iCharType, char cPromotion );
    void ConvertCharInfo( OUT SCharInfo& Info, IN const KCharacterInfo& kServerInfo );
    void ConvertPetInfo( OUT std::map< GCITEMUID, KPetInfo >& mapPetInfo, IN std::map< GCITEMUID, KPetInfo >& mapServerPetInfo );
	bool GetValueFromURL( const char* strURL, int& iValue, std::vector< std::pair<std::string, std::string> > vecPostArg );
    GCDeviceFont* GetFontManager( int iSize, bool bOutLine );
	CParticleEventSeqPTR CreateSequenceToUI( KD3DWnd* pWnd, const std::string& name, float xoffset = 0.0f, float yoffset = 0.0f, int iLayer = GC_LAYER_UI );
	DWORD GetRealFileSize( std::string strPath_ );
    D3DXVECTOR2 UICoordinateConvertToParticleCoordinate( D3DXVECTOR2 vUICoordinate_ );
	int GetStatusGradeByTotalAtk( IN const int iTotalAtk_ );
	std::wstring GetStatusGrade( IN int iGrade );
	std::map<int,DWORD> GetCharListWithoutHave();
	void CharGainParticle(const int iCharType_ );
	bool CheckInEllips(int iStandardX_ , int iStandardY_ , int iRangeX_ , int iRangeY_ , POINT ptPoint_ );
	void DrawDotEllips(DWORD* dwTexAccess_, D3DSURFACE_DESC TexDesc_ , int iStartCol_ ,int iEndCol_ , int iStartRow_ ,int iEndRow_, POINT ptCenter_ ,D3DCOLOR col_, float fPercent_ = 1.0f );
	bool CheckColInRect(DWORD* dwTexAccess_, D3DSURFACE_DESC TexDesc_ , int iStartCol_ ,int iEndCol_ , int iStartRow_ ,int iEndRow_,D3DCOLOR col_, float fPercent_ = 1.0f);
	void FillTheCol(DWORD* dwTexAccess_, D3DSURFACE_DESC TexDesc_ , D3DCOLOR col_ );
	bool Is4SpSkill(int iCharType_ , EGCSkillTree eSkillID_ );
    void CreateCharViewMotion( int iExtraCharNum, GCDeviceMotionFRM** pWaitMotion, GCDeviceMotionFRM** pActionMotion );
    void GetRankGradeText(IN int iRankGrade, OUT std::wstring& wstrRankGrade );
	void CreateCharViewMotionForMenu(int iExtraCharNum, GCDeviceMotionFRM** pWaitMotion, GCDeviceMotionFRM** pActionMotion);
	std::string Base64Encode(std::string in);
	std::string Base64Decode(std::string in);
	bool __forceinline IsHookAPI(BYTE* pbFunction);
};


namespace GCFUNCTOR
{
	class ItemID_S2C
	{
	public:
		ItemID_S2C() {}
		~ItemID_S2C() {}

		template<typename T>
		void operator()(T& dwItemID) //const
		{
			//const DWORD& di = (const DWORD&)(dwItemID);
			//const_cast<DWORD &>(di) /= 10;

			//*p /= 10;
		}
	};
}
