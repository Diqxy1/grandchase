#pragma once
//#include <vector>
#include "GCCondition.h"

struct SMission_Action{
    bool    m_bMonDrop;
    int     m_iCount;
    DWORD   m_dwItemID;
    SMission_Action()
    {
        m_bMonDrop = false;
        m_iCount = 0;
        m_dwItemID = 0;
    }

    SMission_Action& operator=( const SMission_Action& arg )
    {
        m_bMonDrop = arg.m_bMonDrop;
        m_iCount = arg.m_iCount;
        m_dwItemID = arg.m_dwItemID;
        return *this;
    }

    bool operator== ( const SMission_Action& arg )
    {
        return ( m_bMonDrop == arg.m_bMonDrop ) && 
            ( m_iCount == arg.m_iCount ) && 
            ( m_dwItemID == arg.m_dwItemID );
    }
};

struct SMission_Completion{
    int     m_iCount;
    DWORD   m_dwItemID;
    int     m_iCharacterLv;
    SMission_Completion()
    {
        m_iCount = 0;
        m_dwItemID = 0;
        m_iCharacterLv = -1;
    }

    SMission_Completion& operator=( const SMission_Completion& arg )
    {
        m_iCount = arg.m_iCount;
        m_dwItemID = arg.m_dwItemID;
        m_iCharacterLv = arg.m_iCharacterLv;
        return *this;
    }

    bool operator== ( const SMission_Completion& arg )
    {
        return ( m_iCount == arg.m_iCount ) &&
            ( m_dwItemID == arg.m_dwItemID ) &&
            ( m_iCharacterLv == arg.m_iCharacterLv );
    }
};

struct SSubMissionInfo{
    int m_iTitleID;
    int m_iDescID;
    int m_iSubMissionID;
    std::vector< int >          m_vecConditionID;
    SMission_Action             m_sAction;
    SMission_Completion         m_sCompletion;

    SSubMissionInfo()
    {
        m_iTitleID = -1;
        m_iDescID = -1;
        m_iSubMissionID = -1;
    }

    SSubMissionInfo& operator = ( const SSubMissionInfo& arg )
    {
		m_iTitleID = arg.m_iTitleID;
        m_iDescID = arg.m_iDescID;
        m_iSubMissionID = arg.m_iSubMissionID;
        m_vecConditionID = arg.m_vecConditionID;
        m_sAction = arg.m_sAction;
        m_sCompletion = arg.m_sCompletion;
        return *this;
    }

    bool operator== ( const SSubMissionInfo& arg )
    {
        return ( (m_iTitleID == arg.m_iTitleID ) &&
			(m_iDescID == arg.m_iDescID) &&
            (m_vecConditionID == arg.m_vecConditionID )&&
            (m_sAction == arg.m_sAction )&&
            (m_sCompletion == arg.m_sCompletion ) );
    }
};

class KGCSubMission
{
    SSubMissionInfo m_sSubMissionInfo;
public:
    KGCSubMission(void);
    ~KGCSubMission(void);

    void AddCondition( KGCCondition& kCondition );
    void SetAction( int iCount, DWORD dwItemID = -1, bool bMonDrop = false );
    void SetCompletion( int iCount, DWORD dwItemID );
    void SetSubMissionInfo( const SSubMissionInfo& sSubMissionInfo ) { m_sSubMissionInfo = sSubMissionInfo; }    
    int GetSubMissionID()  { return m_sSubMissionInfo.m_iSubMissionID; }
    bool IsComplete( int iCount )    {
        return ( iCount >= m_sSubMissionInfo.m_sCompletion.m_iCount );
    }
    const SSubMissionInfo*    GetSubMissionInfo()   { return &m_sSubMissionInfo; }

    bool operator== ( const SSubMissionInfo& arg )
    {
        return ( m_sSubMissionInfo == arg );
    }
};
