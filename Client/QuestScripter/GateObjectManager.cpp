#include "StdAfx.h"
#include ".\gateobjectmanager.h"
#include "GateObjectGenerater.h"
#include "DlgTGGateObjectGenerater.h"
#include "MapSystem/QSFantasticMap.h"
#include "MapSystem/KQSLayerManager.h"
#include "MapSystem/KQSLayerPart.h"
#include "DlgGateObject_Popup.h"
#include "DlgTriggerContent.h"
#include "ObjectDataManager.h"

CGateObjectManager::CGateObjectManager(void)
{
    m_bDrag                 = false;
    m_pSelectedItem         = 0;
    m_pDlgTGContent         = 0;
    m_pvtGateObject         = 0;
}

CGateObjectManager::~CGateObjectManager(void)
{
}

void CGateObjectManager::FrameMoveMode(void)
{
    if( !m_pvtGateObject ) return;
    // 플레이어를 서서히 떨어뜨린다.
    for(int i=0;i<(int)m_pvtGateObject->size();i++)
    {
        CGateObjectGenerater *pGateObject = (*m_pvtGateObject)[i];
        if( !pGateObject ) continue;

        int m_iMIndexX = (int)(pGateObject->m_fEntranceX / FOOTHOLDSIZE_ON_MAP);
        int m_iMIndexY = (int)(pGateObject->m_fEntranceY / FOOTHOLDSIZE_ON_MAP);

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
            pGateObject->m_fEntranceY -= 3.0f;

            //약 -50픽셀보다 작아지면 자리를 Y값 100으로 재배치한다.
            if( pGateObject->m_fEntranceY < LAST_DEPH )
            {
                pGateObject->m_fEntranceY = 100.0f;
            }
        }
    }
}

void CGateObjectManager::DrawMode(void)
{
    if( !m_pvtGateObject ) return;

    for(int i=0;i<(int)m_pvtGateObject->size();i++)
    {
        CGateObjectGenerater *pGateObject = (*m_pvtGateObject)[i];
        if( !pGateObject ) continue;
        if( -1 == pGateObject->m_iType ) continue;

        CString strObject = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DOOR)[pGateObject->m_iType];
        CString strMonDesc;
        strMonDesc.Format(L"<Gate>\nRight : %d", pGateObject->m_bRight);

        DrawPartObject(pGateObject->m_fEntranceX, pGateObject->m_fEntranceY, pGateObject->m_bRight, strObject);
        DrawPartDesc(g_pd3dDevice, pGateObject->m_fEntranceX, pGateObject->m_fEntranceY, strMonDesc, strObject);
    }
}

void CGateObjectManager::DrawSelectedObject(void)
{
    DrawPartObject((float)m_ptDragItem.x, (float)m_ptDragItem.y, 0, m_strCurObject);
}

void CGateObjectManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_pSelectedItem )
    {
        // 선택된 오브젝트가 있으면 위치를 변경시켜준다.
        m_pSelectedItem->m_fEntranceX = pos.x;
        m_pSelectedItem->m_fEntranceY = pos.y;
    }

    m_ptDragItem.SetPoint((int)pos.x, (int)pos.y);
}

void CGateObjectManager::MouseLBDownMode(D3DVECTOR pos)
{
    if( !m_pvtGateObject ) return;

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
            for(int i=0;i<(int)m_pvtGateObject->size();i++)
            {
                CGateObjectGenerater *pGateObject = (*m_pvtGateObject)[i];
                if( !pGateObject ) continue;
                if( -1 == pGateObject->m_iType ) continue;

                if( IsInLayerPart(pos, pGateObject->m_fEntranceX, pGateObject->m_fEntranceY
                                  , SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DOOR)[pGateObject->m_iType]) )
                {
                    m_bDrag = true;
                    m_pSelectedItem = pGateObject;
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
            // 플레이어 위치 추가 하는 다이얼로그를 띄운다.
            CDlgGateObject_Popup dlgObj;
            // dlgObj 창이 뜨기 전에 현재의 위치를 먼저 넘깁니다.
            dlgObj.m_GateObject.m_fEntranceX = pos.x;
            dlgObj.m_GateObject.m_fEntranceY = pos.y;

            // 몬스터 현재 선택된 몬스터의 index를 찾아서 먼저 넘깁니다.
            for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DOOR).size();i++)
            {
                if( m_strCurObject == SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DOOR)[i] )
                {
                    dlgObj.m_GateObject.m_iType = i;
                    break;
                }
            }

            // 몬스터 Enum 값과 StageLv의 배열값을 넘깁니다.
            dlgObj.SetMapObjectList(m_pvtGateObject);

            // 메인 다이얼로그의 포인터를 넘깁니다.
            if( IDOK == dlgObj.DoModal() )
            {
                if( !m_pDlgTGContent ) return;
                if( !m_pDlgTGContent->m_pCurrentTrigger ) return;

                dlgObj.m_GateObject.m_iSlot = m_pDlgTGContent->GetBlankGateObjSlotInTrigger();
                CDlgTGGateObjectGenerater *pDlgTGObjGen = m_pDlgTGContent->m_dlgTGChildGenner.m_TGGateObjectGenerater;

                if( pDlgTGObjGen )
                {
                    pDlgTGObjGen->SetValue(&dlgObj.m_GateObject);
                    pDlgTGObjGen->Refresh();
                    pDlgTGObjGen->CalcResult();
                    pDlgTGObjGen->GetValue(&dlgObj.m_GateObject);
                }

                CGateObjectGenerater *pObjGen = new CGateObjectGenerater;
                *pObjGen = dlgObj.m_GateObject;                        
                if( dlgObj.m_bFnAction )
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hAction, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddAction(pObjGen);
                }
                else
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hEvent, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddEvent(pObjGen);
                }
                std::wstring strDesc = dlgObj.m_GateObject.m_strDesc;

                pObjGen->m_hItem = m_pDlgTGContent->m_treeTG.AddChild( strDesc.c_str() );
                m_pvtGateObject->push_back(pObjGen);
            }
        } // end if m_strCurObject 길이체크
    }
}

void CGateObjectManager::MouseLBUpMode(D3DVECTOR pos)
{
    if( m_bDrag && m_pSelectedItem )
    {
        CDlgGateObject_Popup dlgObj;

        // 현재 드래그 중이었던 녀석의 기본적인 정보를 설정해준다.
        dlgObj.m_GateObject = *m_pSelectedItem;
        dlgObj.SetMapObjectList(m_pvtGateObject);

        if( IDOK == dlgObj.DoModal() )
        {
            // 확인을 눌렀다면 정보를 갱신해준다.
            *m_pSelectedItem = dlgObj.m_GateObject;
            CDlgTGGateObjectGenerater *pDlgTGObjGen = m_pDlgTGContent->m_dlgTGChildGenner.m_TGGateObjectGenerater;
            if( pDlgTGObjGen )
            {
                pDlgTGObjGen->SetValue(&dlgObj.m_GateObject);
                pDlgTGObjGen->Refresh();
                pDlgTGObjGen->CalcResult();
                pDlgTGObjGen->GetValue(&dlgObj.m_GateObject);
                m_pDlgTGContent->RefreshTreeTitle(dlgObj.m_GateObject.m_hItem, dlgObj.m_GateObject.m_strDesc);
            }
        }
        m_bDrag             = false;
        m_pSelectedItem     = 0;
    }
}
