#pragma once

#include "CommonPacket.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class KODBC;

class KCommonSP
{
private:
	KODBC& m_kODBC;
	KCommonSP(void);
public:
	KCommonSP(KODBC& kODBC) : m_kODBC(kODBC) {};
	~KCommonSP(void);

public:
	int InsertToWaitItemJob(IN const DWORD dwUID, IN const char cCharType_, IN const GCITEMID itemID, IN const int nCount, IN const int nPeriod, IN const int nGetType, OUT __int64& waitUID, IN int nGrade = -1, IN int nItemLevel = -1, IN const int nEnchantLevel = 0, IN const int nStatus = 2);
	void GetSALT(IN const std::wstring& strLogin_, OUT std::wstring& strSALT_);
	bool UpdateChannelCCU(IN const int nType_, IN const int nChannelType_, IN const int nServerID_, IN const int nCurUserNum_);
	void Merge_AccOnlineTime(IN const DWORD dwLoginUID_, IN const int nTime_);
	void Select_AccOnlineTime(IN const DWORD dwLoginUID_, OUT int& nTime_);

	int GetTodayEventCount(IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_);
	bool SetTodayEventDoneDiff(IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_);
	bool SetTodayEventDone(IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_);

	bool UpdateSellStateFlag_SocialCommerce(IN const DWORD& dwEventUID, IN int& nSuccessFlag);
	void GetGroupPurchaseFlag_SocialCommerce(IN const DWORD& dwEventUID, OUT int& nSuccessFlag);

	bool User_auth_select(IN const DWORD& dwUserUID, OUT char& cLastGrade, OUT char& cCurrentGrade, OUT int& nPeriodType, OUT time_t& tmLimit, OUT char& cCause);
	bool IgnorableErrorProcessingAfterCallSP(IN OUT int& nReturnValue_); // KCommonSP와 KSP 양쪽에 있으므로 수정시 주의가 필요하다
};
