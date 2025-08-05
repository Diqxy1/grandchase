#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CommonPacket.h"
#include "CenterPacket.h"

class KLuaManager;

/* 센터 서버에서의 기능은 스크립트 정보를 읽어 게임 서버에 전달해주는 역할만 한다 */
class KCnConnectionGiftBox
{
    DeclareSingleton( KCnConnectionGiftBox );
    NiDeclareRootRTTI( KCnConnectionGiftBox );
    DeclToStringW;

public:
    KCnConnectionGiftBox( void );
    ~KCnConnectionGiftBox( void );

    bool LoadScript();
    bool GetConnectionGiftBoxInfoNot( OUT KECN_CONNECTION_GIFTBOX_INFO_NOT& kPacket_ );

private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadGiftFirstCharacterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KCharGiftItem >& mapGiftFirstCharacter_ );
    bool LoadCharGiftItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KCharGiftItem >& mapCharGiftItem_ );
    bool LoadGiftItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecItems_ );
    bool LoadGiftUserInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KGiftGradeInfo >& mapGiftNewUser_ );

private:
    mutable KncCriticalSection                  m_csCnConnectionGiftBox;

    bool                                        m_bUseGiftFirstCharacter;
    std::map< int, KCharGiftItem >              m_mapGiftFirstCharacter;    // 첫 캐릭터 생성시 지급할 아이템 정보( key: CharType )

    bool                                        m_bUseGiftNewUser;
    time_t                                      m_tmDecideBeginTimeNewUser; // 신규유저 판단용 기준날짜
    int                                         m_nNewUserEffectDuration;   // 신규판정을 받은 이후 신규유저로 취급되는 기간 설정(int)
    CTimeSpan                                   m_kNewUserEffectDuration;   // 신규판정을 받은 이후 신규유저로 취급되는 기간 설정
    std::map< int, KGiftGradeInfo >             m_mapGiftNewUser;           // 신규유저 선물 정보( key: 단계 )

    int                                         m_nGiftEventUserVersion;    // 이벤트 버전
    time_t                                      m_tmGiftEventBegin;         // 이벤트 시작 시간
    time_t                                      m_tmGiftEventEnd;           // 이벤트 종료 시간
    std::map< int, KGiftGradeInfo >             m_mapGiftEventUser;         // 이벤트유저 선물 정보( key: 단계 )

};

DefSingletonInline( KCnConnectionGiftBox );
DeclOstmOperatorA( KCnConnectionGiftBox );
