#pragma once
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <ToString.h>
#include "Lua/KLuaManager.h"
#include <vector>
#include <map>

struct sRecipe
{
    DWORD                               m_dwRecipeID;
    DWORD                               m_dwDestItem;
    int                                 m_nFactor;
    std::vector< std::pair<DWORD,int> > m_vecMaterial;

    sRecipe():m_dwRecipeID(0),m_dwDestItem(0),m_nFactor(0){};
    sRecipe(const sRecipe& t) { *this = t; };
    sRecipe& operator=(const sRecipe& right )
    {
        m_dwRecipeID    = right.m_dwRecipeID;
        m_dwDestItem    = right.m_dwDestItem;
        m_nFactor       = right.m_nFactor;
        m_vecMaterial   = right.m_vecMaterial;
        return *this;
    }
};

class KManufacture
{
    DeclareSingleton( KManufacture );
    DeclToStringW;

public:
    KManufacture(void);
    ~KManufacture(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetRecipe( IN DWORD dwRecipeID, OUT sRecipe& kRecipe );

protected:
    bool LoadRecipe( IN OUT KLuaManager& kLuaMng, IN sRecipe& recipeInfo );

    mutable KncCriticalSection      m_csRecipe;
    std::map< DWORD, sRecipe >      m_mapRecipe;
};

DefSingletonInline( KManufacture );
DeclOstmOperatorA( KManufacture );