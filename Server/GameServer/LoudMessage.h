#pragma once

#include <Thread/Locker.h>
#include <boost/tuple/tuple.hpp>
#include "KNCSingleton.h"
#include <string>
#include <map>
#include <vector>
#include "PrimitiveTypedef.h"

class KLoudMessage
{
    // 070201. kkurrung. 전체 알림 메세지.. 따로 떼어낼지 깊이 고려해보자.. 젠장..
    //  080702  woosh.  클래스 만드는 중
    DeclareSingleton( KLoudMessage );


public:
    KLoudMessage(void);
    virtual ~KLoudMessage(void);


    bool GetLoudMsg( IN int iItemID, IN char cOffset, IN std::wstring& strNickName, OUT std::wstring& strLoudMsg, IN const int nLanguageType );
    bool GetLoudMsgByComment( IN int nCommoneID, IN std::wstring& strNickName, OUT std::wstring& strLoudMsg, IN int nLanguageCode_ );
    bool GetItemName( IN int iItemID, IN char cOffset, OUT std::wstring& strItemName, IN int nLanguageCode_ = -1 );
    void UpdateMsgNames( std::map< std::pair< int, DWORD >,std::wstring>& mapMessageTable, std::vector< boost::tuple<DWORD,char,std::wstring,DWORD,int> >& vecNameTable );
    bool GetLoudMsg( IN int iItemID_, IN char cOffset_, IN std::wstring& strNickName_, OUT std::wstring& strLoudMsg_, 
                     IN std::wstring& strDungeonName_, IN std::wstring& strGettingMethod_, IN const int nLanguageType_  );
    bool GetCoupleMsg( IN std::wstring& strNick1, IN std::wstring& strNick2, OUT std::wstring& strMgs,  IN const int nLanguageType_ );

    bool GetStrongMsg( IN GCITEMID nItemID, IN std::wstring& strNick, OUT std::wstring& strMgs, IN const int nLanguageType_ );
    bool GetGachaMsg( IN GCITEMID nItemID, IN std::wstring& strNick, OUT std::wstring& strMgs, IN const int nLanguageType_ );
    bool GetMissionMsg( IN DWORD dwTime, OUT std::wstring& strMgs );

    bool GetItemDonationMsg( IN DWORD dwGrade, OUT std::wstring& strMgs );
    bool GetStrengthMsg( IN std::wstring strNick, IN DWORD dwStrengthLevel, OUT std::wstring& strMgs, IN const int nLanguageType_ );
    bool GetGWCEventMsg( IN std::wstring wstrCountryName, IN DWORD dwGrade, OUT std::wstring& strMgs );
    bool GetEclipseMsg( OUT std::wstring& wstrStartEventMgs_, OUT std::wstring& wstrEndEventMgs_, IN const int& nLanguageType_ = 0 );
    bool GetLevelResetMsg( IN std::wstring strNick, IN DWORD dwLevel, OUT std::wstring& strMgs, IN const int nLanguageType_ );
    bool GetGachaLotteryMsg( IN std::wstring strNick, IN std::wstring wstrGrade, OUT std::wstring& strMgs, IN const int nLanguageType );

    bool GetGachaPonMsg(IN std::wstring strNick, OUT std::wstring& strMgs, IN const int nLanguageType);
protected:
    bool GetMsg( DWORD dwKey, std::wstring& strMsg,  IN const int nLanguageType = 0 );
    bool GetItemNickMsg( IN DWORD dwCommentID, IN GCITEMID nItemID, IN std::wstring& strNick, OUT std::wstring& strMgs, IN const int nLanguageType_ );

    KncCriticalSection                                          m_csMsgNames;
    std::map< std::pair< int, DWORD>, std::wstring >                             m_mapMessageTable; // 메세지 테이블 ( <LanguageCode, MsgID>, Msg )
    std::vector< boost::tuple<DWORD,char,std::wstring,DWORD, int> >  m_vecNameTable; // 아이템 이름 테이블 ( ItemID, Offset, Name, MsgID )


};

DefSingletonInline( KLoudMessage );
