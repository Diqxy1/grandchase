#pragma once
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <boost/random.hpp>
#include <KNCSingleton.h>
#include <ToString.h>
#include "Lottery.h"

namespace luabind { class object; } // 전방선언

struct KChanneDropInfo
{
    int                                 m_nChannelID;
    std::vector<char>                   m_vecChar;
    std::pair<char,char>                m_pairLevel;
    float                               m_fWinRatio;
    float                               m_fLoseRatio;
    bool                                m_bSelect;
    std::vector<std::pair<int,int> >    m_vecItemList;
    KLottery                            m_kLottery;

    KChanneDropInfo():m_nChannelID(0),m_fWinRatio(0.f),m_fLoseRatio(0.f),m_bSelect(false){};
    KChanneDropInfo(const KChanneDropInfo& t) { *this = t; };
    KChanneDropInfo& operator=( const KChanneDropInfo& right )
    {
        m_nChannelID        = right.m_nChannelID;
        m_vecChar           = right.m_vecChar;
        m_pairLevel         = right.m_pairLevel;
        m_fWinRatio         = right.m_fWinRatio;
        m_fLoseRatio        = right.m_fLoseRatio;
        m_bSelect           = right.m_bSelect;
        m_vecItemList       = right.m_vecItemList;
        m_kLottery          = right.m_kLottery;

        return *this;
    }

};

class KUser;
struct KGameResultIn;
struct KItemUnit;

class KChannelDrop
{
    DeclareSingleton( KChannelDrop );
    DeclToStringW;

public:
    KChannelDrop(void);
    ~KChannelDrop(void);

    void Clear();
    void AddChannelDrop( const luabind::object& table );
    void ProcessDrop( KUser& kUser, IN const KGameResultIn& kResultIn, OUT std::vector<KItemUnit>& vecDrop );

protected:
    bool GetdropInfoList( int nChannelID, std::vector<KChanneDropInfo>& DropInfoList );
    float GetRatio();


protected:
    KncCriticalSection                              m_csDropList;
    std::map<int, std::vector<KChanneDropInfo> >    m_mapDropList;

    boost::mt19937 rng;
    boost::uniform_int<DWORD> uint32;
    boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die;
};
DefSingletonInline( KChannelDrop );
DeclOstmOperatorA( KChannelDrop );
