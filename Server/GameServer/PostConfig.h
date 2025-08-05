#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "PrimitiveTypedef.h"
#include <set>

class KLuaManager;
class KPostConfig
{
    DeclareSingleton( KPostConfig );
    NiDeclareRootRTTI( KPostConfig );
    DeclToStringW;
public:
    KPostConfig(void);
    ~KPostConfig(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsPostEnable()             { KLocker lock(m_csPostEnable); return m_bPostEnable; }

    bool IsSantaPostEnable()        { KLocker lock(m_csPostConfig); return m_bSantaPost; }
    bool IsNewPostAlarmEnable()     { KLocker lock(m_csPostConfig); return m_bNewPostAlarm; }
    bool IsUserSendPostEnable()     { KLocker lock(m_csPostConfig); return m_bUserSendPost; }
    bool IsUserPostItemEnable()     { KLocker lock(m_csPostConfig); return m_bUserPostItem; }
    bool IsUserPostGPEnable()       { KLocker lock(m_csPostConfig); return m_bUserPostGP; }

    DWORD GetCheckNewPostTickGap()  { KLocker lock(m_csSystemConfig); return m_dwCheckNewPostTickGap; }
    USHORT GetOnePageLetterCount()  { KLocker lock(m_csSystemConfig); return m_usOnePageLetterCount; }
    USHORT GetNewLetterTitleCount() { KLocker lock(m_csSystemConfig); return m_usNewLetterTitleCount; }

    USHORT GetTitleLength()         { KLocker lock(m_csLetterConfig); return m_usTitleLength; }
    USHORT GetMessageLength()       { KLocker lock(m_csLetterConfig); return m_usMessageLength; }
    int GetnLetterCost()            { KLocker lock(m_csLetterConfig); return m_nLetterCost; }
    float GetGPCostRatio()          { KLocker lock(m_csLetterConfig); return m_fGPCostRatio; }
    float GetItemCostRatio()        { KLocker lock(m_csLetterConfig); return m_fItemCostRatio; }
    int GetSystemPostStorageTime()  { KLocker lock(m_csLetterConfig); return m_nSystemPostStorageTime; }
    int GetUserPostStorageTime()    { KLocker lock(m_csLetterConfig); return m_nUserPostStorageTime; }

    // 아이템 첨부할 수 없는 ItemID인가?
    bool IsNotPostItemID( IN const GCITEMID& ItemID ) { KLocker lock(m_csLetterConfig); return m_setNotPostItemID.find( ItemID ) != m_setNotPostItemID.end(); }
    std::wstring GetPostString( IN std::wstring strKey );
    void GetPostPresentString( IN std::wstring strMsgID, IN std::wstring wstrNickName, OUT std::wstring& wstrMsg );

private:
    mutable KncCriticalSection  m_csPostEnable;
    bool                        m_bPostEnable; // 우편기능 전체 사용할지 여부

    // 기능제한
    mutable KncCriticalSection  m_csPostConfig;
    bool                        m_bSantaPost; // 산타시스템 우편으로 사용하는가?
    bool                        m_bNewPostAlarm; // 우편 도착시, 실시간으로 알려줄 것인가?
    bool                        m_bUserSendPost; // 유저 우편보내기 기능 사용할 것인가?
    bool                        m_bUserPostItem; // 유저 우편보내기시 아이템 첨부 사용할 것인가?
    bool                        m_bUserPostGP; // 유저 우편보내기시 GP 첨부 사용할 것인가?

    // 시스템설정
    mutable KncCriticalSection  m_csSystemConfig;
    DWORD                       m_dwCheckNewPostTickGap; // 접속중인 유저의 새 우편 DB에서 받아오는 주기(ms단위)
    USHORT                      m_usOnePageLetterCount; // 1페이지당 보여주는 우편 개수
    USHORT                      m_usNewLetterTitleCount; // 한번에 보여주는 신규우편 개수

    // 우편설정
    mutable KncCriticalSection  m_csLetterConfig;
    USHORT                      m_usTitleLength; // 우편 제목 길이제한
    USHORT                      m_usMessageLength; // 우편 내용 길이제한
    int                         m_nLetterCost; // 우편 기본 수수료(GP)
    float                       m_fGPCostRatio; // GP첨부시 첨부GP에 대한 수수료 비율
    float                       m_fItemCostRatio; // 아이템첨부시 첨부아이템에 대한 수수료 비율
    int                         m_nSystemPostStorageTime; // 시스템우편 유통기한 (일단위,-1:영구보관)
    int                         m_nUserPostStorageTime; // 유저우편 유통기한 (일단위,-1:영구보관)
    std::set<GCITEMID>          m_setNotPostItemID; // 첨부할 수 없는 ItemID 리스트
};
DefSingletonInline( KPostConfig );
DeclOstmOperatorA( KPostConfig );