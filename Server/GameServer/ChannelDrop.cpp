#include "User.h"
#include ".\channeldrop.h"
#include <KncUtil.h>
#include <KncUtilLua.h> // luabind::object
#include <dbg/dbg.hpp>
#include "UserPacket.h"
#include "Channel.h" //kUser_.GetChannel()->GetUID()
#include "Log4.h"
//FILE_NAME_FOR_LOG

KChannelDrop::KChannelDrop(void)
:uint32( 1, UINT_MAX )
,die(rng, uint32)
{
}

KChannelDrop::~KChannelDrop(void)
{
}

ImplementSingleton( KChannelDrop );
ImplOstmOperatorW2A( KChannelDrop );
ImplToStringW( KChannelDrop )
{
    stm_<< L" " << std::endl;
    return stm_;
}

void KChannelDrop::Clear()
{
    KLocker lock( m_csDropList );
    m_mapDropList.clear();
}

//Channel = { 1, 4, 3, 5, },
//character = { 0, 3, },
//Level = { 0, 75 },
//win = 100,
//lose = 100,
//Items = { {4200, 1}, {4320, -1} , {5810, 5} },

void KChannelDrop::AddChannelDrop( const luabind::object& table_ )
{
    JIF( table_.type() == LUA_TTABLE );
    luabind::object objItem = table_["items"];
    JIF( objItem.type() == LUA_TTABLE );

    std::vector<int>    vecChannels;
    KncUtil::Table2vector( table_["channel"], vecChannels );

    KChanneDropInfo     kDropInfo;
    JIF( KncUtil::Table2vector( table_["character"], kDropInfo.m_vecChar ) );
    JIF( KncUtil::Table2pair( table_["level"], kDropInfo.m_pairLevel ) );
    kDropInfo.m_fWinRatio   = KncUtil::GetLuaData( table_, "win", 0.f );
    kDropInfo.m_fLoseRatio  = KncUtil::GetLuaData( table_, "lose", 0.f );
    kDropInfo.m_bSelect     = KncUtil::GetLuaData( table_, "bselect", false ); 

    kDropInfo.m_fWinRatio *= 0.01f;
    kDropInfo.m_fLoseRatio *= 0.01f;
    
    luabind::object::iterator itItem;
    itItem = objItem.begin();
    while( itItem != objItem.end() )
    {
        std::pair<int,int> pairItem;
        JIF( KncUtil::Table2pair( *itItem, pairItem ) );
        kDropInfo.m_vecItemList.push_back( pairItem );
        ++itItem;
    }

    std::vector<float> vecRatio;
    if( kDropInfo.m_bSelect )
    {
        JIF( KncUtil::Table2vector( table_["ratio"], vecRatio ) );
        for( int i = 0 ; i < static_cast<int>(vecRatio.size()) ; ++i )
        {
            kDropInfo.m_kLottery.AddCase( i, vecRatio[i] );
        }
    }
    

    if( vecChannels.empty() )
    {
        kDropInfo.m_nChannelID = -1;
        KLocker lock( m_csDropList );
        m_mapDropList[-1].push_back( kDropInfo );
    }
    else
    {
        std::vector<int>::const_iterator vit;
        for( vit = vecChannels.begin() ;  vit != vecChannels.end() ; ++vit )
        {
            kDropInfo.m_nChannelID = *vit;
            KLocker lock( m_csDropList );
            m_mapDropList[*vit].push_back( kDropInfo );
        }
    }
}

void KChannelDrop::ProcessDrop( KUser& kUser_, IN const KGameResultIn& kResultIn_, OUT std::vector<KItemUnit>& vecDrop_ )
{
    JIF( kUser_.GetChannel() );
    JIF( kUser_.GetRoom() );
    std::vector<KChanneDropInfo> vecDropInfoList;
    if ( GetdropInfoList( (int)kUser_.GetChannel()->GetUID(), vecDropInfoList ) ) {
        START_LOG( clog, L"해당 사항 없음... ㅋㅋㅋ, Name : " << kUser_.GetName() ) << END_LOG;
        return;
    }
    bool bWin = kResultIn_.m_bWin;
    std::vector< std::pair<char, int> > vecCharLv; // 플레이한 캐릭터와 레벨을 묶는다.
    {
        std::vector< KCharacterExpInfo >::const_iterator mit;
        for ( mit = kResultIn_.m_vecCharExp.begin() ; mit != kResultIn_.m_vecCharExp.end() ; ++mit ) {
            const KCharacterInfo* pkChar = NULL;
            pkChar = kUser_.GetCharacterInfo( mit->m_cCharType );
            
            if( pkChar ) {
                vecCharLv.push_back( std::make_pair(pkChar->m_cCharType, (int)pkChar->m_dwLevel ) );
            }
        }        
    }

    std::vector<KChanneDropInfo>::const_iterator vitDrop;
    for ( vitDrop = vecDropInfoList.begin() ; vitDrop != vecDropInfoList.end() ; ++vitDrop ) {// 현재 채널에서 드랍받을 아이템 이러레이션

        std::vector< std::pair<char, int> >::const_iterator vitChar;
        for ( vitChar = vecCharLv.begin() ; vitChar != vecCharLv.end() ; ++vitChar ) {// 플레이한 캐릭터 이터레이션

            // 올바른 레벨인가?
            if ( vitChar->second < vitDrop->m_pairLevel.first ||
                vitChar->second > vitDrop->m_pairLevel.second ) {
                continue;
            }        

            // 해당 드랍정보에 현재 캐릭터가 존재하는가?
            if ( std::find( vitDrop->m_vecChar.begin() , vitDrop->m_vecChar.begin(), vitChar->first ) == vitDrop->m_vecChar.end() )
                continue;

            // 확률 확인
            float fDice = GetRatio();
            if ( bWin ) {
                if( vitDrop->m_fWinRatio < fDice ) {
                    continue;
            }
            }
            else {
                if ( vitDrop->m_fLoseRatio < fDice ) {
                    continue;
            }
            }
            
            if( vitDrop->m_bSelect ) {
                int iDecision = vitDrop->m_kLottery.Decision();
                KItemUnit kItem;
                kItem.m_iID         = vitDrop->m_vecItemList[iDecision].first;
                kItem.m_iDuration   = vitDrop->m_vecItemList[iDecision].second;
                vecDrop_.push_back( kItem );
            }
            else {
                // 아이템 드랍 목록에 추가
                std::vector<std::pair<int,int> >::const_iterator vitItem;
                for ( vitItem = vitDrop->m_vecItemList.begin() ; vitItem != vitDrop->m_vecItemList.end() ; ++vitItem ) {
                    KItemUnit kItem;
                    kItem.m_iID         = vitItem->first;
                    kItem.m_iDuration   = vitItem->second;
                    vecDrop_.push_back( kItem );
                }
            }            
            }
        }
}

float KChannelDrop::GetRatio()
{
    return die() / (float)(UINT_MAX);
}

bool KChannelDrop::GetdropInfoList( int nChannelID_, std::vector<KChanneDropInfo>& DropInfoList_ )
{
    DropInfoList_.clear();

    std::map<int, std::vector<KChanneDropInfo> >::const_iterator mit;
    KLocker lock( m_csDropList );

    mit = m_mapDropList.find( -1 );
    if( mit != m_mapDropList.end() )
    {
        DropInfoList_ = mit->second;
    }

    mit = m_mapDropList.find( nChannelID_ );
    if( mit != m_mapDropList.end() )
    {
        DropInfoList_.insert( DropInfoList_.end(), mit->second.begin(), mit->second.end() );
    }

    return !DropInfoList_.empty();
}