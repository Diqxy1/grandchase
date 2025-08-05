#include "TRPacket.h"

SERIALIZE_DEFINE_PUT(KDB_ETR_TYPE_COUNT_STAT, obj, ks)
{
	return  PUT(m_nSID) &&
		PUT(m_mapTypeStat);
};

SERIALIZE_DEFINE_GET(KDB_ETR_TYPE_COUNT_STAT, obj, ks)
{
	return  GET(m_nSID) &&
		GET(m_mapTypeStat);
};