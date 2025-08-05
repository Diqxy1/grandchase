#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Lua/KLuaManager.h"
#include "CommonPacket.h"

class KSealedChar
{
    DeclareSingleton( KSealedChar );
    NiDeclareRootRTTI( KSealedChar );
    DeclToStringW;

public:
    struct KSealedCharRawInfo {
        int                     m_nCharType;
        int                     m_nObjectLevel;
        time_t                  m_tmBegin;
        time_t                  m_tmEnd;
    };

    KSealedChar( void );
    ~KSealedChar( void );

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool LoadSealedCharRawInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KSealedCharRawInfo >& mapSealedCharRawInfo_ );
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool IsEventTerm( IN int& nCharType_ );
    bool GetObjectLevel( IN int& nCharType_, OUT int& nObjectLevel_ );
    bool GetObjectTime( IN int& nCharType_, OUT time_t& tmObjectTime_ );

protected:
    mutable KncCriticalSection              m_csSealedChar;

    std::map< int, KSealedCharRawInfo >     m_mapSealedCharRawInfo;
};

DefSingletonInline( KSealedChar );
DeclOstmOperatorA( KSealedChar );
