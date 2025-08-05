#pragma once

enum
{
    JIT_NO_MORE_EVENT = 0,
    JIT_CLICK_ON_TO_GET_REWARD,
    JIT_WAIT_REWARD,
    JIT_WAIT_NEXT_TIME,
};

enum // tooltip string 
{
    JIT_STRING_TYPE_NORMAL = 0,
    JIT_STRING_TYPE_NOT_PCBANG_USER,
    JIT_STRING_TYPE_NOT_NORMAL_USER,
};

extern KGCJustInTime    g_kJustInTime;

class KGCJustInTime
{
private:
    time_t  m_tmJITNextEventTime;
    int     m_nState;
    std::wstring m_strToolTip;
	std::vector< KDropItemInfo > m_vecItemList;

public:
    KGCJustInTime( void );
    ~KGCJustInTime( void );

    KGCJustInTime( const KGCJustInTime& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    const KGCJustInTime& operator=( const KGCJustInTime& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void SetRewardList( const std::vector< KDropItemInfo >& _vecItemList ) { m_vecItemList = _vecItemList; }
	std::vector< KDropItemInfo >& GetRewardList() { return m_vecItemList; } 

    void SetJustInTimeEventTime( time_t tmTime ) { m_tmJITNextEventTime = tmTime; }
    time_t GetJustInTimeNextEventTime() { return m_tmJITNextEventTime; }
    
	void SetJustInTimeState( int nState ) { m_nState = nState; }
    int GetJustInTimeState() { return m_nState; }
    void CreateJustInTimeToolTip( int nStringType = JIT_STRING_TYPE_NORMAL );
    const std::wstring& GetJustInTimeToolTip() { return m_strToolTip; }

};

