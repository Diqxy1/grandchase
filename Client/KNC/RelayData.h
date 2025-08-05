#pragma once
//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
////#include <vector>
#include <ObjectPool/BlockAllocator.h>

struct KRelayData;
typedef boost::shared_ptr<KRelayData> KRelayDataPtr;

struct KRelayData : public boost::noncopyable, public BlockAllocator<KRelayData, 5000>
{
    char                            m_cRecvCnt;
    std::vector< DWORD >            m_vecUID;
    std::vector< unsigned char >    m_buff;
};