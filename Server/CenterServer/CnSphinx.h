#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>

class KLuaManager;
struct KSphinxQuestionInfo;
struct KECN_SPHINX_DATA_NOT;
class KSerBuffer;

class KCnSphinx
{
    DeclareSingleton( KCnSphinx );
    NiDeclareRootRTTI( KCnSphinx );
    DeclToStringW;

public:
    KCnSphinx(void);
    ~KCnSphinx(void);

    bool LoadScript();
    void Tick();
    void UpdateSphinxData( IN std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo );
    bool VerifySphinxData( IN const std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo );
    void GetSphinxData( OUT KECN_SPHINX_DATA_NOT& kPacket );
    int GetSphinxVersion() { KLocker lock( m_csSphinx ); return m_nVersion; };

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    void CompressSphinxQuestions( IN const std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo );
    bool CheckTick();
    bool CheckChange();

protected:
    mutable KncCriticalSection  m_csSphinx;
    time_t                      m_tmBegin;
    time_t                      m_tmEnd;
    int                         m_nVersion;
    DWORD                       m_dwTickGap;
    DWORD                       m_dwLastTick;
    bool                        m_bChanged;

    std::map<int,KSphinxQuestionInfo>   m_mapSphinxQuestionInfo;
    std::map<int,KSerBuffer>            m_mapCompressedSphinxQuestionInfo;
};

DefSingletonInline( KCnSphinx );
DeclOstmOperatorA( KCnSphinx );