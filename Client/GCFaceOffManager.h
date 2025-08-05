#pragma once
#include "KNCSingleton.h"

//KGCFaceOffManager
class GCFaceOffManager
{
    DeclareSingleton( GCFaceOffManager );

private:
    bool m_bIsFaceOffItemList;
    bool m_bUseThisScript;
    std::set<DWORD> m_setCharList;
    std::set<GCITEMID> m_setItemList;
    std::set<GCITEMID> m_setEmotionItemList;

    //
    void LoadFaceOffItemList();

public:
    GCFaceOffManager(void);
    ~GCFaceOffManager(void);

    bool IsFaceOffItem( GCITEMID dwItemID_ );
    bool IsEmotionItem( GCITEMID dwItemID_ );
    bool IsFaceOffChar( DWORD dwChar );
    bool IsUseThisScript() { return m_bUseThisScript; };

};

DefSingletonInline( GCFaceOffManager );




