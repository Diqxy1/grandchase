#include "LookEquipment.h"
#include <dbg/dbg.hpp>
#include "UserPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
KLookEquipChar::KLookEquipChar(void)
:m_cCharType(-1)
{
}

KLookEquipChar::~KLookEquipChar(void)
{
}

bool KLookEquipChar::VerifyCoordiID( IN const int nCoordiID_ )
{
    return (LC_COORDI_0 >= nCoordiID_ && LC_COORDI_MAX > nCoordiID_);
}

void KLookEquipChar::Clear()
{
    m_cCharType = -1;
    m_mapCoordiInfo.clear();
}

bool KLookEquipChar::EquipItem( IN const int nCoordiID_, IN const GCITEMUID ItemUID_ )
{
    _JIF( VerifyCoordiID( nCoordiID_ ), return false );

    std::map<int,KCoordiInfo>::iterator mit;
    mit = m_mapCoordiInfo.find( nCoordiID_ );
    if ( mit == m_mapCoordiInfo.end() ) {
        KCoordiInfo kInfo;
        kInfo.m_nCoordiID = nCoordiID_;
        kInfo.m_setEquip.insert( ItemUID_ );
        LIF( m_mapCoordiInfo.insert(std::map<int,KCoordiInfo>::value_type( nCoordiID_, kInfo ) ).second );
    }
    else
    {
        mit->second.m_setEquip.insert( ItemUID_ );
    }
    return true;
}

bool KLookEquipChar::EquipItem( IN const int nCoordiID_, IN const std::vector<GCITEMUID>& vecItemUID_ )
{
    if ( vecItemUID_.empty() )
        return false;

    _JIF( VerifyCoordiID( nCoordiID_ ), return false );

    std::vector<GCITEMUID>::const_iterator vit;
    for ( vit = vecItemUID_.begin() ; vit != vecItemUID_.end() ; ++vit ) {
        LIF( EquipItem( nCoordiID_, *vit ) );
    }
    return true;
}

bool KLookEquipChar::EquipItem( IN const int nCoordiID_, IN const std::set<GCITEMUID>& setItemUID_ )
{

    if ( setItemUID_.empty() )
        return true;

    _JIF( VerifyCoordiID( nCoordiID_ ), return false );

    std::set<GCITEMUID>::const_iterator sit;
    for ( sit = setItemUID_.begin() ; sit != setItemUID_.end() ; ++sit ) {
        LIF( EquipItem( nCoordiID_, *sit ) );
    }
    return true;
}

bool KLookEquipChar::UnEquipItem( IN const int nCoordiID_, IN const GCITEMUID ItemUID_ )
{
    _JIF( VerifyCoordiID( nCoordiID_ ), return false );

    std::map<int,KCoordiInfo>::iterator mit;
    mit = m_mapCoordiInfo.find( nCoordiID_ );
    if ( mit == m_mapCoordiInfo.end() )
        return false;

    return UnEquipItem( mit->second.m_setEquip, ItemUID_ );
}

bool KLookEquipChar::UnEquipItem( IN const int nCoordiID_, IN const std::vector<GCITEMUID>& vecItemUID_ )
{
    if ( vecItemUID_.empty() )
        return true;

    if ( !VerifyCoordiID( nCoordiID_ ) )
        return false;

    std::vector<GCITEMUID>::const_iterator vit;
    for ( vit = vecItemUID_.begin() ; vit != vecItemUID_.end() ; ++vit ) {
        LIF( UnEquipItem( nCoordiID_, *vit ) );
    }

    return true;
}

bool KLookEquipChar::UnEquipItem( IN OUT std::set<GCITEMUID>& setEquip_, IN const GCITEMUID ItemUID_ )
{
    std::set<GCITEMUID>::iterator sit;
    sit = setEquip_.find( ItemUID_ );
    if ( sit == setEquip_.end() )
        return false;

    setEquip_.erase( sit );
    return true;
}

bool KLookEquipChar::UnAllEquipItem()
{
    if ( m_mapCoordiInfo.empty() )
    {
        return false;
    }

    std::map<int,KCoordiInfo>::iterator mit = m_mapCoordiInfo.begin();
    std::map<int,KCoordiInfo>::iterator mit_end = m_mapCoordiInfo.end();

    for ( ; mit != mit_end; ++mit )
    {
        mit->second.m_setEquip.clear();
    }

    return true;
}

bool KLookEquipChar::UnEquipItem( IN const int nCoordiID_, IN const std::set<GCITEMUID>& setItemUID_ )
{
    if ( setItemUID_.empty() )
        return true;

    if ( !VerifyCoordiID( nCoordiID_ ) )
        return false;

    std::set<GCITEMUID>::const_iterator sit;
    for ( sit = setItemUID_.begin() ; sit != setItemUID_.end() ; ++sit ) {
        LIF( UnEquipItem( nCoordiID_, *sit ) );
    }

    return true;
}

bool KLookEquipChar::ForceUnEquipItem( IN const GCITEMUID ItemUID_ )
{
    bool bRet = true;
    std::map<int,KCoordiInfo>::iterator mit;
    for ( mit = m_mapCoordiInfo.begin() ; mit != m_mapCoordiInfo.end() ; ++mit ) {

        if ( !UnEquipItem( mit->second.m_setEquip, ItemUID_ ) ) {
            bRet = false;
        }
    }
    return bRet;
}

bool KLookEquipChar::ForceUnEquipItem( IN const std::vector<GCITEMUID>& vecItemUID_ )
{
    bool bRet = true;
    std::map<int,KCoordiInfo>::iterator mit;
    for ( mit = m_mapCoordiInfo.begin() ; mit != m_mapCoordiInfo.end() ; ++mit ) {

        std::vector<GCITEMUID>::const_iterator vit;
        for ( vit = vecItemUID_.begin() ; vit != vecItemUID_.end() ; ++vit ) {

            if ( !UnEquipItem( mit->second.m_setEquip, *vit ) ) {
                bRet = false;
            }
        }
    }
    return bRet;
}

void KLookEquipChar::SetCharacter( IN const char cCharType_ )
{
    m_cCharType = cCharType_;
}

char KLookEquipChar::GetCharacter()
{
    return m_cCharType;
}

bool KLookEquipChar::GetCoordiInfo( IN const int nCoordiID_, OUT OUT KCoordiInfo& kCoordi_ )
{
    _JIF( VerifyCoordiID( nCoordiID_ ), return false );
    std::map<int,KCoordiInfo>::iterator mit;
    mit = m_mapCoordiInfo.find( nCoordiID_ );
    if ( mit == m_mapCoordiInfo.end() )
        return false;
    kCoordi_ = mit->second;
    return true;

}

bool KLookEquipChar::GetCurrentCoordiInfo( OUT KCoordiInfo& kCoordi_ )
{
    return GetCoordiInfo( LC_COORDI_0, kCoordi_ );
}

bool KLookEquipChar::GetFullCoordiInfo( OUT std::map<int,KCoordiInfo>& mapFullCoordi_ )
{
    mapFullCoordi_.clear();
    if ( m_mapCoordiInfo.empty() )
        return false;

    mapFullCoordi_ = m_mapCoordiInfo;
    return true;
}

void KLookEquipChar::SetFullCoordiInfo( IN const std::map<int,KCoordiInfo>& mapFullCoordiInfo_ )
{
    m_mapCoordiInfo = mapFullCoordiInfo_;
    m_mapInitCoordiInfo = mapFullCoordiInfo_;
}

void KLookEquipChar::SetCoordiInfo( IN const int nCoordiID_, IN const KCoordiInfo& kCoordiInfo_ )
{
    JIF( VerifyCoordiID( nCoordiID_ ) );
    std::map<int,KCoordiInfo>::iterator mit;
    mit = m_mapCoordiInfo.find( nCoordiID_ );

    if ( mit == m_mapCoordiInfo.end() ) {
        m_mapCoordiInfo.insert( std::map<int,KCoordiInfo>::value_type(nCoordiID_,kCoordiInfo_) );
    }
    else {
        mit->second = kCoordiInfo_;
    }
}

void KLookEquipChar::GetDiffCoordiInfo( OUT std::map<int,KCoordiDiffInfo>& mapCoordiInfo_ )
{
    std::map<int,KCoordiInfo>::iterator mitCurr;
    for ( mitCurr = m_mapCoordiInfo.begin() ; mitCurr != m_mapCoordiInfo.end() ; ++mitCurr ) {
        std::map<int,KCoordiInfo>::iterator mitInit;
        mitInit = m_mapInitCoordiInfo.find( mitCurr->first );
        KCoordiDiffInfo kDiffInfo;

        if ( mitInit == m_mapInitCoordiInfo.end() ) {
            GetDiffCoordiInfo( KCoordiInfo(), mitCurr->second, kDiffInfo );
        }
        else {
            GetDiffCoordiInfo( mitInit->second, mitCurr->second, kDiffInfo );
        }

        mapCoordiInfo_.insert( std::map<int,KCoordiDiffInfo>::value_type(kDiffInfo.m_nCoordiID, kDiffInfo) );
    }

    m_mapInitCoordiInfo = m_mapCoordiInfo;
}

void KLookEquipChar::GetDiffCoordiInfo( IN const KCoordiInfo& kInitCoordi_, IN const KCoordiInfo& kCurrentCoordi_, OUT KCoordiDiffInfo& kDiffInfo_ )
{
    kDiffInfo_.m_nCoordiID = kCurrentCoordi_.m_nCoordiID;
    kDiffInfo_.m_strCoordiName = kCurrentCoordi_.m_strCoordiName;
    ExtractNotExistLeft( kInitCoordi_.m_setEquip, kCurrentCoordi_.m_setEquip, kDiffInfo_.m_setEquip );
    ExtractNotExistLeft( kCurrentCoordi_.m_setEquip, kInitCoordi_.m_setEquip, kDiffInfo_.m_setRelease );
}

void KLookEquipChar::ExtractNotExistLeft( IN const std::set<GCITEMUID>& setLeft_, IN const std::set<GCITEMUID>& setRight, OUT std::set<GCITEMUID>& setExtract_ )
{
    setExtract_.clear();
    std::set<GCITEMUID>::const_iterator sit;
    for ( sit = setRight.begin() ; sit != setRight.end() ; ++sit ) {
        if ( setLeft_.find( *sit ) == setLeft_.end() ) {
            setExtract_.insert( *sit );
        }
    }
}

bool KLookEquipChar::IsEquiped( IN const GCITEMUID itemUID_ )
{
    std::map<int,KCoordiInfo>::iterator mit;
    for ( mit = m_mapCoordiInfo.begin() ; mit != m_mapCoordiInfo.end() ; ++mit ) {
        if ( IsEquiped( mit->second.m_setEquip, itemUID_ ) ) {
            return true;
        }
    }

    return false;
}

bool KLookEquipChar::IsEquiped( IN const int nCoordiID_, IN const GCITEMUID itemUID_ )
{
    std::map<int,KCoordiInfo>::iterator mit;
    mit = m_mapCoordiInfo.find( nCoordiID_ );
    if ( mit == m_mapCoordiInfo.end() )
        return false;

    return IsEquiped( mit->second.m_setEquip, itemUID_ );
}

bool KLookEquipChar::IsEquiped( IN const std::set<GCITEMUID>& setEquips_, IN const GCITEMUID itemUID_ )
{
    return (setEquips_.find( itemUID_ ) != setEquips_.end() );
}

bool KLookEquipChar::VerifySaveCoordiID( IN const int nCoordiID_ )
{
    return (LC_COORDI_1 >= nCoordiID_ && LC_COORDI_MAX > nCoordiID_);
}

bool KLookEquipChar::SaveCoordi( IN const KCoordiInfo& kCoordiInfo_ )
{
    std::map<int,KCoordiInfo>::iterator mit;
    mit = m_mapCoordiInfo.find( kCoordiInfo_.m_nCoordiID );
    if ( mit == m_mapCoordiInfo.end() ) {
        return m_mapCoordiInfo.insert( std::map<int,KCoordiInfo>::value_type( kCoordiInfo_.m_nCoordiID, kCoordiInfo_ ) ).second;
    }
    else {
        mit->second = kCoordiInfo_;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------//
#include "Lua/KLuaManager.h"

ImplementSingleton( KLookEquipment );
ImplOstmOperatorW2A( KLookEquipment );
NiImplementRootRTTI( KLookEquipment );

KLookEquipment::KLookEquipment(void)
:m_lookChangeItemID(8880)
,m_nCoordiNameLength(6)
,m_nMaxLookInvenSize(100)
,m_bUseAvatarSystem( false )
,m_CoordiComposeItemID(8880)
{
    m_mapLookExtendItemInfo.clear();
}

KLookEquipment::~KLookEquipment(void)
{
}

bool KLookEquipment::LoadScript()
{
    return _LoadScript(std::string());
}

bool KLookEquipment::_LoadScript( OUT std::string& strScript_ )
{
    GCITEMID changeItemID;
    int nCoordiNameLength;
    int nMaxLookInvenSize;
    bool bUseAvatarSystem;
    GCITEMID CoordiComposeItemID;
    std::set<GCITEMID> setLookBan;
    std::set<GCITEMID> setNormalBan;
    std::set<GCITEMID> setLookItemList;
    std::map<GCITEMID, int> mapLookExtendItemInfo;
    KLuaManager kLuaNmg;

    const std::string strScriptName = "InitLookEquip.lua";
    strScript_ = strScriptName;
    _JIF( kLuaNmg.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaNmg.GetValue( "LookChangeItemID", changeItemID ) == S_OK, return false );
    _JIF( kLuaNmg.GetValue( "CoordiNameLength", nCoordiNameLength ) == S_OK, return false );
    _JIF( kLuaNmg.GetValue( "MaxLookInvenSize", nMaxLookInvenSize ) == S_OK, return false );
    _JIF( kLuaNmg.GetValue( "UseAvatarSystem", bUseAvatarSystem ) == S_OK, return false );

    _JIF( LoadItemIDList( kLuaNmg, "LookBan", setLookBan ), return false );
    _JIF( LoadItemIDList( kLuaNmg, "NormalBan", setNormalBan ), return false );
    _JIF( LoadItemIDList( kLuaNmg, "LookItemList", setLookItemList ), return false );
    _JIF( LoadLookExtendItemInfo( kLuaNmg, std::string("LookExtendItem"), mapLookExtendItemInfo ), return false );
    _JIF( kLuaNmg.GetValue( "CoordiComposeItemID", CoordiComposeItemID ) == S_OK, return false );

    START_LOG( cerr, L"룩 변환 설정 읽기 완료" )
        << BUILD_LOG( changeItemID )
        << BUILD_LOG( nCoordiNameLength )
        << BUILD_LOG( nMaxLookInvenSize )
        << BUILD_LOG( CoordiComposeItemID )
        << BUILD_LOG( setLookBan.size() )
        << BUILD_LOG( setNormalBan.size() )
        << BUILD_LOG( setLookItemList.size() )
        << BUILD_LOG( mapLookExtendItemInfo.size() ) << END_LOG;

    {
        KLocker lock( m_csLookEquip );
        m_lookChangeItemID = changeItemID;
        m_nCoordiNameLength = nCoordiNameLength;
        m_nMaxLookInvenSize = nMaxLookInvenSize;
        m_CoordiComposeItemID = CoordiComposeItemID;
        m_setLookBanItemID.swap( setLookBan );
        m_setNormalBanItemID.swap( setNormalBan );
        m_setLookItemList.swap( setLookItemList );
        m_bUseAvatarSystem = bUseAvatarSystem;
        m_mapLookExtendItemInfo.swap( mapLookExtendItemInfo );
    }

    return true;
}

bool KLookEquipment::LoadItemIDList( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::set<GCITEMID>& setItemID_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        GCITEMID ItemID = 0;
        if ( kLuaMng_.GetValue( i, ItemID ) != S_OK ) break;
        setItemID_.insert( ItemID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

ImplToStringW( KLookEquipment )
{
    KLocker lock( m_csLookEquip );
    return START_TOSTRINGW
        << TOSTRINGW( m_nCoordiNameLength )
        << TOSTRINGW( m_lookChangeItemID )
        << TOSTRINGW( m_nMaxLookInvenSize )
        << TOSTRINGW( m_setLookBanItemID.size() )
        << TOSTRINGW( m_setNormalBanItemID.size() );
}

GCITEMID KLookEquipment::GetChangeItemID()
{
    KLocker lock( m_csLookEquip );
    return m_lookChangeItemID;
}

int KLookEquipment::GetCoordiNameLength()
{
    KLocker lock( m_csLookEquip );
    return m_nCoordiNameLength;
}

int KLookEquipment::GetMaxInvenSize()
{
    KLocker lock( m_csLookEquip );

    return m_nMaxLookInvenSize;
}

bool KLookEquipment::IsLookBanItem( IN const GCITEMID& ItemID_ )
{
    KLocker lock( m_csLookEquip );
    return (m_setLookBanItemID.find( ItemID_ ) != m_setLookBanItemID.end() );
}

bool KLookEquipment::IsNormalBanItem( IN const GCITEMID& ItemID_ )
{
    KLocker lock( m_csLookEquip );
    return (m_setNormalBanItemID.find( ItemID_ ) != m_setNormalBanItemID.end() );
}

bool KLookEquipment::IsLookItemID( IN const GCITEMID& ItemID_ )
{
    KLocker lock( m_csLookEquip );
    return (m_setLookItemList.find( ItemID_ ) != m_setLookItemList.end() );
}

bool KLookEquipment::GetEnableAvatar()
{
    KLocker lock( m_csLookEquip );
    return m_bUseAvatarSystem;
}

bool KLookEquipment::LoadLookExtendItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<GCITEMID, int>& mapLookExtendItemInfo_ )
{
    mapLookExtendItemInfo_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nItemID = 0;
        int nExtendSize = 0;

        _JIF( kLuaMng_.GetValue( 1, nItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nExtendSize ) == S_OK, return false );

        if( mapLookExtendItemInfo_.insert( std::make_pair( nItemID, nExtendSize ) ).second == false ) {
            START_LOG( cerr, L" 코디 인벤 늘리는 아이템 체크중 중복 ItemID :" << nItemID ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KLookEquipment::GetItemExtendSize( IN const GCITEMID& ItemID_, OUT int& nExtendSize_ )
{
    std::map<GCITEMID, int>::iterator mit;

    KLocker lock( m_csLookEquip );
    mit = m_mapLookExtendItemInfo.find( ItemID_ );
    if( mit == m_mapLookExtendItemInfo.end() ) {
        return false;
    }
    nExtendSize_ = mit->second;
    return true;
}