#ifndef _KNCPEER_H_
#define _KNCPEER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KncCommonHeader.h"
#include "./RakNetSrc/BitStream.h"
#include "./RakNetSrc/NetworkTypes.h"

class KncPeer
{
    enum 
    {
        INIT_MAX_PING = 10000, // ping 초기값,
    };

public:
    //변수
    bool            m_bLocal;
    bool            m_bRelay;
    PlayerID        m_kRakPeerID;
    DWORD           m_dwUID;
    std::wstring    m_strDesc;   


    //함수
    KncPeer(void);
    KncPeer( DWORD dwUID );
    KncPeer( DWORD dwUID, PlayerID rakPeerID_ );
    virtual ~KncPeer(void);

    virtual void SetUID( DWORD dwUID_ );
    virtual void SetRakPeerID( PlayerID rakPeerID_ );
    virtual void SetRakPeerID( DWORD dwIP_, unsigned short usPort_ );
    virtual void SetRelay( bool bRelay_ ){ m_bRelay = bRelay_; }

    virtual inline DWORD    GetUID() { return m_dwUID; }
    virtual inline PlayerID GetRakPeerID() { return m_kRakPeerID; }
    virtual inline USHORT   GetPort(){ return m_kRakPeerID.port; }
    virtual inline DWORD    GetIP(){ return m_kRakPeerID.binaryAddress; }
    virtual inline bool     IsRelay(){ return m_bRelay; }
    virtual inline bool     IsLocal(){ return m_bLocal; }

    virtual bool operator==( const KncPeer* pPeer );
    virtual bool operator!=( const KncPeer* pPeer );
    virtual const WCHAR* ToString();
    //====================================================================================
    // 2006.11.15 : Asirion
    // 에러 통계및 Ping통계를 위해 필요.
    virtual void SetPing( const DWORD& dwPing_ );
	virtual DWORD GetPing();
    virtual void SetPartPing( const DWORD& dwPing_ );
    virtual DWORD GetPartPing();

    virtual void ClearPingTime();
    virtual void ClearPartPingTime();




	void SetSendPingTime( const DWORD dwTime );
	DWORD GetLastSendPingTime()	{ return m_dwLastPingSendTime;}
	void SetRecvPingTime( const DWORD dwTime );
	DWORD GetLastRecvPingTime() { return m_dwLastPingRecvTime;}

	void PingPacketLost()
	{
		++m_iPingLossCount;
	}
	
    void ResetLossPingCount(){ m_iPingLossCount = 0; }
	int	GetPingLossCount()	{ return m_iPingLossCount; }



private:
    virtual void InitPeer();

private:
    DWORD           m_dwAvgPing;
    DWORD           m_dwAvgPartPing;

	DWORD			m_dwLastPingSendTime;
	DWORD			m_dwLastPingRecvTime;
	int				m_iPingLossCount;
    
    int             m_nPingCount;
    int             m_nPartPingCount;

};

#endif // _KNCPEER_H_