#pragma once
#include <Thread/Locker.h>
#include <Lottery.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <set>
#include "PrimitiveTypedef.h"

namespace luabind { class object; } // 전방선언

class KEnchantStrong
{
    DeclareSingleton( KEnchantStrong );
    DeclToStringW;
public:
    KEnchantStrong(void);
    ~KEnchantStrong(void);

    enum ENCHANT_ACTION
    {
        EA_SUCCESS      = 0, // 성공
        EA_WEAKNESS     = 1, // 하락
        EA_BREAK        = 2, // 깨짐
        EA_NONE         = 3, // 제자리
        EA_DROP         = 4, // 초기화
        EA_SPECIFIC     = 5, // 특정 레벨 변화  
        EA_MAX,
    };
    enum ENCHANT_GRADE
    {
        EG_FIRST        = 0,
        EG_SECOND       = 1,
        EG_THIRD        = 2,
        EG_MAX,
    };

    //enum ENCHANT_SLOT
    //{
    //    ES_WEAPON       = 0,
    //    ES_ARMOR        = 1,
    //    ES_HELMET       = 2,
    //    ES_MAX,
    //};

    enum ENCHANT_KIND
    {
        EK_FIRE             = 0,    // 불 강화
        EK_ICE              = 1,    // 얼음 강화
        EK_LIGHT            = 2,    // 번개 강화 (Lightning)
        EK_DARK             = 3,    // 암흑 강화
        EK_WIND             = 4,    // 바람 강화
        EK_CASH_SHIELD      = 5,    // 캐쉬 방어 발찌
        EK_GP_SHIELD        = 6,    // GP 방어 발찌
        EK_SHINE            = 7,    // 빛 강화
        EK_LOVE             = 8,    // 하트 강화
        EK_ROSE             = 9,    // 장미 강화
        EK_DEATH            = 10,   // 사신 강화
        EK_TIME             = 11,   // 아트로포스 강화
        EK_LIFE             = 12,   // 생명 강화
        EK_DARK_DRAGON_RED  = 13,   // 흑룡 레드 강화
        EK_DARK_DRAGON_BLUE = 14,   // 흑룡 블루 강화
        EK_FOOLS_DAY        = 15,   //똥목걸이
        EK_ARIES_SHIELD     = 16,   // 염소 자리 발찌
        EK_TAURUS_SHIELD    = 17,   // 황소 자리 발찌
        EK_ZORRO_SHIELD     = 18,   // 조로냥이 발찌
        EK_GEMINI_SHIELD    = 19, 
        EK_CANCER_SHIELD        = 20,   // 게자리         
        EK_LEO_SHIELD           = 21,   // 사자자리
        EK_VIRGO_SHIELD         = 22,   // 처녀 자리
        EK_LIBRA_SHIELD         = 23,
        EK_SCORPIO_SHIELD       = 24,
        EK_SAGITTARIUS_SHIELD   = 25, 
        EK_CAPRICORN_SHIELD     = 26,
        EK_AQUARIUS_SHIELD      = 27,
        EK_PISCES_SHIELD        = 28,
        EK_HALLOWEEN            = 29,
        EK_ELIA                 = 30,   // 엘리아 목걸이
        EK_ELIA_ANKLE           = 31,   // 엘리아 발찌
        EK_ARIES                = 32, //    양자리 목걸이
        EK_TAURUS               = 33, //    황소 목걸이
        EK_GEMINI               = 34, //    쌍둥이 목걸이
        EK_CANCER               = 35, //    사자 목걸이
        EK_LEO                  = 36, //    게 목걸이
        EK_VIRGO                = 37, //    처녀 목걸이
        EK_LIBRA                = 38, //    천칭 목걸이
        EK_SCORPIO              = 39, //    전갈 목걸이
        EK_SAGITTARIUS          = 40, //    사수 목걸이
        EK_CAPRICORN            = 41, //    염소 목걸이
        EK_AQUARIUS             = 42, //    물병 목걸이
        EK_PISCES               = 43, //    물고기 목걸이
        EK_10TH_ANKLE           = 44, // 10주년 발찌
        EK_DIMENSION_ANKLE      = 45, // 차원의 문 발찌
        EK_PEGASUS_ANKLE        = 46, // 페가수스 발찌
        EK_PEGASUS_NECK         = 47, // 페가수스 목걸이
        EK_MAX,
    };

    class KEnchant
    {
    public:
        KEnchant()
        {
            ZeroMemory( m_afRatio, sizeof(m_afRatio) );
            m_uiStartLv = 0;
            m_uiEndLv   = 0;
            m_uiInc     = 0;
        }
        virtual ~KEnchant(){}
        // 함수
        //bool CheckGrade( const UINT uiLv_ )
        //{
        //    ( m_uiStartLv <= uiLv_ && uiLv_ <= m_uiEndLv ) ? return true : return false;
        //}
        UINT GetNeedCoreCount( const UINT uiEnchantLv_ )
        {
            return ( uiEnchantLv_ + 1 ) * m_uiInc;
        }
        // 변수 
        UINT  m_uiStartLv;
        UINT  m_uiEndLv;
        float m_afRatio[EA_MAX];
        UINT  m_uiInc;
    };

    void ClearEnchantInfo();
    void ClearMaterialCount();
    void ClearAssistInfoTable();
    void ClearEnchantLimitLv();
    void ClearEquipItemID();
    void ClearEnchantAssist();

    void SetEnchantAction(  const int nEnchantKind, const int nStrongLV, const int nStrongMaterialIdx,
        const float fSuccess, const float fWeakness, const float fBreak );
    void SetEnchantFailResult( const UINT uiLv_, const UINT uiResult_ );
    void SetEnchantRatio( const UINT uiGrade_, const UINT uiStartLv_, const UINT uiEndLv_, 
        const float fWeaknessRatio_, const float fDropRatio_, const float fBreakRatio_, const UINT uiInc_ );
    //ENCHANT_GRADE GetGrade( const UINT uiLv_ );
    void SetEnchantMaterialCount( const luabind::object& table );
    void SetCoreItemID( const GCITEMID dwItemID_ );
    GCITEMID GetCoreItemID(){ return m_dwCoreItemID; }
    void AddEnchantEnableItemID( const GCITEMID dwItemID_, KEnchantStrong::ENCHANT_GRADE eGrade_ );
    void AddEnchantAssistItem( const GCITEMID dwItemID_, const float fIncreaseRate_ );
    UINT GetNeedCoreCount( const GCITEMID dwItemID_, const UINT uiEnchantLv_, const float fScale );
    ENCHANT_ACTION GetEnchantFailResult( const UINT uiEnchantLv_ );
    ENCHANT_ACTION Enchant( IN const GCITEMID dwItemID_, IN const UINT uiEnchantLv_, IN const GCITEMID dwAssistItemID_, IN const float fScale, IN const bool bPCBang, OUT float& fFailRatio, OUT float& fAddRatio, OUT int& nLevel );

    bool IsEnchantEnableItem( const GCITEMID dwItemID_ );
    bool IsEnchantAssistItem( const GCITEMID dwItemID_ );

    bool GetEnchantUnit( const int nEnchantKind, const int nStrongLV, const int nStrongMaterialIdx, KLottery& kEnchantUnit );

    void MaterialCountList( std::map<DWORD,int>& mapCount );
    bool GetNeedMaterialCount( IN const int nStrongLV, OUT DWORD& dwNeedCount );

    void AddStronAssist( const GCITEMID dwAssistItemID, const int nStrongLV, const luabind::object& table );
    bool GetStrongAssist( const GCITEMID dwAssistItemID, const int nStrongLV, std::vector< std::pair<int,float> >& vecRatio );

    void SetEnchantBonusRate( IN float fBonusRate );
    float GetEnchantBonusRate();

    void AddStrongAssistByCharacter( const int nChar_, const int nStrongLV_, const luabind::object& table_ );
    bool GetStrongAssistByCharacter( const int nCharacter_, const int nStrongLV_, std::vector< std::pair<int,float> >& vecRatio_ );
    void EnchantTest( GCITEMID nItemID, int nBeginLevel, int nGoalLevel, GCITEMID nAssistItemID, int nLoopCount );

    int GetMaxMaterialCount();

    void SetStrongMaterial( IN const int nIdx, IN const GCITEMID dwItemID );
    bool GetStrongMaterialIndex( IN const GCITEMID dwItemID, OUT int& nIdx );
    void ClearStrongMaterial();

    void SetStrongAssistWeak( IN const GCITEMID dwItemID );
    bool IsStrongAssistWeak( IN const GCITEMID dwItemID );
    void ClearStrongAssistWeak();

    void SetStrongLimitLevel( IN const DWORD dwLevel_ ) { KLocker lock(m_csEnchant); m_dwLimitEnchantLevel = dwLevel_; }
    DWORD GetStrongLimitLevel() { KLocker lock(m_csEnchant); return m_dwLimitEnchantLevel; }
    void ClearStrongLimitLevel() { KLocker lock(m_csEnchant); m_dwLimitEnchantLevel = 0; }

    void SetSpecificLevelEnchant( IN const GCITEMID ItemID, IN const int nLevel );
    bool GetSpecificLevelEnchant( IN const GCITEMID ItemID, OUT int& nLevel );
    void ClearSpecificLevelEnchant() { KLocker lock(m_csEnchant); m_mapSpecificLevelEnchant.clear(); }
    bool CheckSpecificLevelItem( IN const GCITEMID ItemID );
    bool CheckEnchantLevel( IN const GCITEMID ItemID, IN const int nLevel );

protected:
    ENCHANT_GRADE GetEnchantGrade( const GCITEMID dwItemID );
    float GetEnchantAssistRatio( const GCITEMID dwAssistItemID );
    bool ChkGrade_Action( int nGrade, int nAction );


protected:
    KncCriticalSection                                                      m_csEnchant;
    std::map<int, KLottery>                                                 m_mapEnchantInfo;
    std::map<DWORD,int>                                                     m_mapNeedMaterialCount;
    std::map<UINT, UINT>                                                    m_mapEnchantLimitLv;    
    std::map< std::pair<DWORD,int>, std::vector< std::pair<int,float> > >   m_mapAssistInfoTable; // <ItemID,StrongLv>, <EnchantAction, fAssistRatio>
    KEnchant                                                                m_akEnchant[EA_MAX];
    GCITEMID                                                                   m_dwCoreItemID;
    std::map<DWORD, UINT>                                                   m_mapEnchantEnableItem;
    //====================================================================================
    // 2007-10-15 오후 10:36:08 : Asirion 
    // GoodsID, 상승률
    std::map< DWORD, float >                                                m_mapEnchantAssist;

    float                                                                   m_fEnchantBonusRate; // 강화시 보너스 확률(이벤트시 사용)

    std::map< std::pair<int,int>, std::vector< std::pair<int,float> > >     m_mapEnchantBonusRateByChar; // <character,StrongLv>, <EnchantAction, fAssistRatio>

    std::map<int,DWORD>                                                     m_mapStrongMaterial; // 강화 재료(Gem) <Index,ItemID>
    DWORD                                                                   m_dwStrongAssistWeak; // 강화하락방지 아이템
    DWORD                                                                   m_dwLimitEnchantLevel; // 강화 제한 레벨.

    std::map<DWORD, int>                                                    m_mapSpecificLevelEnchant; // 특정 강화 레벨 변경.
};

DefSingletonInline( KEnchantStrong );
DeclOstmOperatorA( KEnchantStrong );