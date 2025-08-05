#ifndef _GCParticleChangeSystem_H_
#define _GCParticleChangeSystem_H_
#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning(disable:4786)

#include "GCItemEnum.h"
#include "ProtectedType.h"
#include <atltime.h>
#include "KGCStringID.h"

struct GCParticleChangeString
{
    std::string m_sCurrentParticle;
    std::string m_sCustomParticle;
};

struct GCParticleChangeDefinition
{
    GCITEMID m_iItemID;
    std::vector< GCParticleChangeString > m_vecParticleVector;
};

class KGCParticleChangeSystem
{
public:
    KGCParticleChangeSystem();
    ~KGCParticleChangeSystem();

private:
    std::vector< GCParticleChangeDefinition > m_vecParticleChange;

public:
    void SetParticleChangeList( void );
    std::string GetParticleChageString( const int iPlayerID, const std::string strParticleDefault );
};

extern KGCParticleChangeSystem* g_pParticleChangeSys;

#endif