#include "stdafx.h"
#include ".\monsterobjectmanager.h"
#include "MonsterGenerater.h"
#include "MapSystem/QSFantasticMap.h"
#include "ObjectDataManager.h"
#include "DlgMapToObject.h"
#include "DlgStageMonster_Popup.h"
#include "DlgTGMonsterGenerater.h"

//#include "MapSystem/KQSLayerManager.h"
//#include "MapSystem/KQSLayerPart.h"
//#include "DlgTriggerContent.h"


CMonsterObjectManager::CMonsterObjectManager(void)
{
    m_bDrag         = false;
    m_pDlgTGContent = 0;
    m_pSelectedItem = 0;
    m_pvtObject     = 0;
}

CMonsterObjectManager::~CMonsterObjectManager(void)
{
}

void CMonsterObjectManager::FrameMoveMode(void)
{
    if( !m_pvtObject ) return;

    // 몬스터를 서서히 떨어뜨린다.
    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CMonsterGenerater *pMonsterGen = (*m_pvtObject)[i];
        if( !pMonsterGen ) continue;

        int m_iMIndexX = (int)(pMonsterGen->m_fEntranceX / FOOTHOLDSIZE_ON_MAP);
        int m_iMIndexY = (int)(pMonsterGen->m_fEntranceY / FOOTHOLDSIZE_ON_MAP);

        if( m_iMIndexX < 0 )
            m_iMIndexX = 0;
        if( m_iMIndexX > SiKQSFantasticMap()->GetBlockWidth() - 1 )
            m_iMIndexX = SiKQSFantasticMap()->GetBlockWidth() - 1;

        if( m_iMIndexY < 0 )
            m_iMIndexY = 0;
        if( m_iMIndexY > SiKQSFantasticMap()->GetBlockHeight() - 1 )
            m_iMIndexY = SiKQSFantasticMap()->GetBlockHeight() - 1;

        DWORD dwFootHoldType = SiKQSFantasticMap()->GetFHWorld()[m_iMIndexX][m_iMIndexY].GetFootHoldType();
        if( KQSFootHold::FHT_INIT == dwFootHoldType )
        {
            pMonsterGen->m_fEntranceY -= 3.0f;

            //약 -50픽셀보다 작아지면 자리를 Y값 100으로 재배치한다.
            if( pMonsterGen->m_fEntranceY < LAST_DEPH )
            {
                pMonsterGen->m_fEntranceY = 100.0f;
            }
        }
    }
}

void CMonsterObjectManager::DrawMode(void)
{
    if( !m_pvtObject ) return;

    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CMonsterGenerater *pMonsterGen = (*m_pvtObject)[i];
        if( !pMonsterGen ) continue;

        if( pMonsterGen->m_iMonType >= (int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size() ) continue;

        CString strObject = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[pMonsterGen->m_iMonType];
        CString strMonDesc;
        strMonDesc.Format(L"Right : %d\nBoss : %d\nMonIndex : %d", pMonsterGen->m_bRight, pMonsterGen->m_iBoss, pMonsterGen->m_iMonIndex);

        DrawPartObject(pMonsterGen->m_fEntranceX, pMonsterGen->m_fEntranceY, pMonsterGen->m_bRight, strObject);
        DrawPartDesc(g_pd3dDevice, pMonsterGen->m_fEntranceX, pMonsterGen->m_fEntranceY, strMonDesc, strObject);
    }
}

void CMonsterObjectManager::DrawSelectedObject(void)
{
    // 몬스터를 드래그 하고 있으면, 블랜딩된 몬스터를 찍지 않는다.
    DrawPartObject((float)m_ptDragItem.x, (float)m_ptDragItem.y, 0, m_strCurObject);
}

void CMonsterObjectManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_pSelectedItem )
    {
        // 선택된 오브젝트가 있으면 위치를 변경시켜준다.
        m_pSelectedItem->m_fEntranceX = pos.x;
        m_pSelectedItem->m_fEntranceY = pos.y;
    }

    m_ptDragItem.SetPoint((int)pos.x, (int)pos.y);
}

void CMonsterObjectManager::MouseLBDownMode(D3DVECTOR pos)
{
    if( !m_pvtObject ) return;

    if( GetAsyncKeyState(VK_CONTROL) & 0x8000 )       // 컨트롤 버튼을 눌르고 있나?
    {
        // 한놈이 드래그 되고있다면 위치를 변경시키고 그것이 아니라면 드래그 할 놈을 지정한다.
        if( m_bDrag )
        {
            if( m_pSelectedItem )
            {
                m_pSelectedItem->m_fEntranceX = pos.x;
                m_pSelectedItem->m_fEntranceY = pos.y;
            }
        }
        else
        {
            for(int i=0;i<(int)m_pvtObject->size();i++)
            {
                CMonsterGenerater *pMonsterGen = (*m_pvtObject)[i];
                if( !pMonsterGen ) continue;

                if( IsInLayerPart(pos, pMonsterGen->m_fEntranceX, pMonsterGen->m_fEntranceY
                                  , SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[pMonsterGen->m_iMonType]) )
                {
                    m_bDrag = true;
                    m_pSelectedItem = pMonsterGen;
                    break;
                }
            }
        }
    }
    else
    {
        // m_strCurObject에 Enum 값이 있어야지 추가되는 오브젝트의 종류를 확인할 수 있습니다.
        if( m_strCurObject.GetLength() )
        {
            CDlgStageMonster_Popup dlgMon;

            // dlgMon 창이 뜨기 전에 현재의 위치를 먼저 넘깁니다.
            dlgMon.monstergen.m_fEntranceX = pos.x;
            dlgMon.monstergen.m_fEntranceY = pos.y;

            // 몬스터 현재 선택된 몬스터의 index를 찾아서 먼저 넘깁니다.
            for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON).size();i++)
            {
                if( m_strCurObject == SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_MON)[i] )
                {
                    dlgMon.monstergen.m_iMonType = i;
                    break;
                }
            }

            // 몬스터 Enum 값과 StageLv의 배열값을 넘깁니다.
            dlgMon.SetMapObjectList(m_pvtObject);

            // 메인 다이얼로그의 포인터를 넘깁니다.
            if( IDOK == dlgMon.DoModal() )
            {
                if( !m_pDlgTGContent ) return;
                if( !m_pDlgTGContent->m_pCurrentTrigger ) return;

                dlgMon.monstergen.m_iMonIndex = m_pDlgTGContent->GetBlankMonsterSlotInTrigger();
                CDlgTGMonsterGenerater *pMonGen = m_pDlgTGContent->m_dlgTGChildGenner.m_pdlgTGMonGenerater;

                if( pMonGen )
                {
                    pMonGen->SetValue(&dlgMon.monstergen);
                    pMonGen->Refresh();
                    pMonGen->CalcResult();
                    pMonGen->GetValue(&dlgMon.monstergen);
                }

                CMonsterGenerater *pMonsterGen = new CMonsterGenerater;
                *pMonsterGen = dlgMon.monstergen;                        
                if( dlgMon.m_bFnAction )
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hAction, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddAction(pMonsterGen);
                }
                else
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hEvent, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddEvent(pMonsterGen);
                }
                
                std::wstring strDesc = dlgMon.monstergen.m_strDesc.c_str();
                pMonsterGen->m_hItem = m_pDlgTGContent->m_treeTG.AddChild( strDesc.c_str() );
                m_pvtObject->push_back(pMonsterGen);
            }
        } // end if m_strCurMon 길이체크
    }
}

void CMonsterObjectManager::MouseLBUpMode(D3DVECTOR pos)
{
    if( m_bDrag && m_pSelectedItem )
    {
        CDlgStageMonster_Popup dlgMon;

        // 현재 드래그 중이었던 녀석의 기본적인 정보를 설정해준다.
        dlgMon.monstergen = *m_pSelectedItem;
        dlgMon.SetMapObjectList(m_pvtObject);

        if( IDOK == dlgMon.DoModal() )
        {
            // 확인을 눌렀다면 정보를 갱신해준다.
            *m_pSelectedItem = dlgMon.monstergen;
            CDlgTGMonsterGenerater *pDlgTGObjGen = m_pDlgTGContent->m_dlgTGChildGenner.m_pdlgTGMonGenerater;
            if( pDlgTGObjGen )
            {
                pDlgTGObjGen->SetValue(&dlgMon.monstergen);
                pDlgTGObjGen->Refresh();
                pDlgTGObjGen->CalcResult();
                pDlgTGObjGen->GetValue(&dlgMon.monstergen);
                m_pDlgTGContent->RefreshTreeTitle(dlgMon.monstergen.m_hItem, dlgMon.monstergen.m_strDesc);
            }
        }
        m_bDrag             = false;
        m_pSelectedItem     = 0;
    }
}
