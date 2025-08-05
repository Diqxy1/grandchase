#pragma once

#include "FSM/FSMclass.h"
#include "FSM/FSMstate.h"
#include <Thread/Locker.h>


#define DeclareFSM  \
public: \
    void SetFSM( FSMclassPtr& spFSM ) \
    { \
        m_spFSM = spFSM; \
        m_pkCurrentState = m_spFSM->GetState(1); \
    } \
    FSMclassPtr GetFSM() \
    { \
        return m_spFSM; \
    } \
    const wchar_t* GetStateIDString() const \
    { \
        return m_spFSM->GetStateIDString(m_pkCurrentState->GetID()); \
    } \
    int GetStateID() \
    { \
        return m_pkCurrentState->GetID(); \
    } \
    void StateTransition( int nInput ) \
    { \
        m_pkCurrentState = m_spFSM->GetState( m_pkCurrentState->GetOutput( nInput ) ); \
    } \
    void ForceStateTransitionTo( int nStateID ) \
    { \
        m_pkCurrentState = m_spFSM->GetState( nStateID ); \
    } \
protected: \
    FSMstate*                       m_pkCurrentState;   \
    FSMclassPtr                     m_spFSM



#define DeclareFSM_MT  \
public: \
    void SetFSM( FSMclassPtr& spFSM ) \
    { \
        KLocker lock( m_cs ); \
        m_spFSM = spFSM; \
        m_pkCurrentState = m_spFSM->GetState(1); \
    } \
    FSMclassPtr GetFSM() \
    { \
        KLocker lock( m_cs ); \
        return m_spFSM; \
    } \
    const wchar_t* GetStateIDString() const \
    { \
        KLocker lock( m_cs ); \
        return m_spFSM->GetStateIDString(m_pkCurrentState->GetID()); \
    } \
    int GetStateID() \
    { \
        KLocker lock( m_cs ); \
        return m_pkCurrentState->GetID(); \
    } \
    void StateTransition( int nInput ) \
    { \
        KLocker lock( m_cs ); \
        m_pkCurrentState = m_spFSM->GetState( m_pkCurrentState->GetOutput( nInput ) ); \
    } \
    void ForceStateTransitionTo( int nStateID ) \
    { \
        KLocker lock( m_cs ); \
        m_pkCurrentState = m_spFSM->GetState( nStateID ); \
    } \
protected: \
    mutable KncCriticalSection      m_cs;   \
    FSMstate*                       m_pkCurrentState;   \
    FSMclassPtr                     m_spFSM


#define VERIFY_STATE( varg ) \
    if( !m_pkCurrentState->CheckValidState varg ) \
    { \
        START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
            << L"    현재 상태 : " << GetStateIDString() << dbg::endl \
			<< L"    허용 상태 : "L ## #varg << END_LOG; \
        return; \
    }