#include "StdAfx.h"
#include ".\monselfunctionmanager.h"
#include "MonsterGenerater.h"
#include "ObjectDataManager.h"

CMonSelFunctionManager::CMonSelFunctionManager(void)
{
    m_pListener = 0;
}

CMonSelFunctionManager::~CMonSelFunctionManager(void)
{
}


void CMonSelFunctionManager::FrameMoveMode(void)
{
    //    FrameMoveMonMode();
}

void CMonSelFunctionManager::DrawMode(void)
{
//    DrawMonMode();
}

void CMonSelFunctionManager::MouseMoveMode(D3DVECTOR pos)
{

}

void CMonSelFunctionManager::MouseLBDownMode(D3DVECTOR pos)
{

}

void CMonSelFunctionManager::MouseLBUpMode(D3DVECTOR pos)
{
    CMonsterGenerater *pMonster = NULL;
    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CMonsterGenerater *pMonsterGen = (*m_pvtObject)[i];
        if( !pMonsterGen ) continue;

        if( IsInLayerPart(pos, pMonsterGen->m_fEntranceX, pMonsterGen->m_fEntranceY
                          , SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[pMonsterGen->m_iMonType]) )
        {
            pMonster = pMonsterGen;
            break;
        }
    }

    if( m_pListener && pMonster )
    {
        m_pListener->SendMessage(WM_MONSEL_FUNCTION, (WPARAM)pMonster, 0);
    }
}