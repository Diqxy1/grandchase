#include "stdafx.h"
#include ".\decorobjectmanager.h"
#include "MapSystem/KQSLayerPart.h"
#include "DlgObjectGenerate_Popup.h"
#include "DlgTriggerContent.h"
#include "ObjectDataManager.h"


CDecorObjectManager::CDecorObjectManager(void)
{
    m_bDrag         = false;
    m_pDlgTGContent = 0;
    m_pSelectedItem = 0;
    m_pvtObject     = 0;
}

CDecorObjectManager::~CDecorObjectManager(void)
{
}

void CDecorObjectManager::FrameMoveMode(void)
{
    if( !m_pvtObject ) return;

    // 몬스터를 서서히 떨어뜨린다.
    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CObjectGenerater *pObjectGenerater = (*m_pvtObject)[i];
        if( !pObjectGenerater ) continue;

        int m_iMIndexX = (int)(pObjectGenerater->m_fEntranceX / FOOTHOLDSIZE_ON_MAP);
        int m_iMIndexY = (int)(pObjectGenerater->m_fEntranceY / FOOTHOLDSIZE_ON_MAP);

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
            pObjectGenerater->m_fEntranceY -= 3.0f;

            //약 -50픽셀보다 작아지면 자리를 Y값 100으로 재배치한다.
            if( pObjectGenerater->m_fEntranceY < LAST_DEPH )
            {
                pObjectGenerater->m_fEntranceY = 100.0f;
            }
        }
    }
}

void CDecorObjectManager::DrawMode(void)
{
    if( !m_pvtObject ) return;

    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CObjectGenerater *pObjectGenerater = (*m_pvtObject)[i];
        if( !pObjectGenerater ) continue;

        if( pObjectGenerater->m_iType >= (int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size() ) continue;

        CString strObject = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[pObjectGenerater->m_iType];
        CString strMonDesc;
        strMonDesc.Format(L"ObjIndex : %d", pObjectGenerater->m_iSlot);

        DrawPartObject(pObjectGenerater->m_fEntranceX, pObjectGenerater->m_fEntranceY, pObjectGenerater->m_bRight, strObject);
        DrawPartDesc(g_pd3dDevice, pObjectGenerater->m_fEntranceX, pObjectGenerater->m_fEntranceY, strMonDesc, strObject);
    }
}

void CDecorObjectManager::DrawSelectedObject(void)
{
    DrawPartObject((float)m_ptDragItem.x, (float)m_ptDragItem.y, 0, m_strCurObject);
}

void CDecorObjectManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_pSelectedItem )
    {
        // 그놈의 위치를 변경시켜준다.
        m_pSelectedItem->m_fEntranceX = pos.x;
        m_pSelectedItem->m_fEntranceY = pos.y;
    }

    m_ptDragItem.SetPoint((int)pos.x, (int)pos.y);
}

void CDecorObjectManager::MouseLBDownMode(D3DVECTOR pos)
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
                CObjectGenerater *pObjectGenerater = (*m_pvtObject)[i];
                if( !pObjectGenerater ) continue;
                
                if( IsInLayerPart(pos, pObjectGenerater->m_fEntranceX, pObjectGenerater->m_fEntranceY
                                  , SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[pObjectGenerater->m_iType]) )
                {
                    m_bDrag = true;
                    m_pSelectedItem = pObjectGenerater;
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
            CDlgObjectGenerate_Popup dlgObj;

            // dlgObj 창이 뜨기 전에 현재의 위치를 먼저 넘깁니다.
            dlgObj.objectgen.m_fEntranceX = pos.x;
            dlgObj.objectgen.m_fEntranceY = pos.y;

            // 몬스터 현재 선택된 몬스터의 index를 찾아서 먼저 넘깁니다.
            for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT).size();i++)
            {
                if( m_strCurObject == SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_OBJECT)[i] )
                {
                    dlgObj.objectgen.m_iType = i;
                    break;
                }
            }

            // 몬스터 Enum 값과 StageLv의 배열값을 넘깁니다.
            dlgObj.SetMapObjectList(m_pvtObject);

            // 메인 다이얼로그의 포인터를 넘깁니다.
            if( IDOK == dlgObj.DoModal() )
            {
                if( !m_pDlgTGContent ) return;
                if( !m_pDlgTGContent->m_pCurrentTrigger ) return;

                dlgObj.objectgen.m_iSlot = m_pDlgTGContent->GetBlankObjSlotInTrigger();
                CDlgTGObjectGenerater *pDlgTGObjGen = m_pDlgTGContent->m_dlgTGChildGenner.m_pdlgTGObjectGenerater;

                if( pDlgTGObjGen )
                {
                    pDlgTGObjGen->SetValue(&dlgObj.objectgen);
                    pDlgTGObjGen->Refresh();
                    pDlgTGObjGen->CalcResult();
                    pDlgTGObjGen->GetValue(&dlgObj.objectgen);
                }

                CObjectGenerater *pObjGen = new CObjectGenerater;
                *pObjGen = dlgObj.objectgen;                        
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
                std::wstring strDesc = dlgObj.objectgen.m_strDesc;

                pObjGen->m_hItem = m_pDlgTGContent->m_treeTG.AddChild(strDesc.c_str());
                m_pvtObject->push_back(pObjGen);
            }
        } // end if m_strCurObject 길이체크
    }
}

void CDecorObjectManager::MouseLBUpMode(D3DVECTOR pos)
{
    if( m_bDrag && m_pSelectedItem )
    {
        CDlgObjectGenerate_Popup dlgObj;

        // 현재 드래그 중이었던 녀석의 기본적인 정보를 설정해준다.
        dlgObj.objectgen = *m_pSelectedItem;
        dlgObj.SetMapObjectList(m_pvtObject);

        if( IDOK == dlgObj.DoModal() )
        {
            // 확인을 눌렀다면 정보를 갱신해준다.
            *m_pSelectedItem = dlgObj.objectgen;
            CDlgTGObjectGenerater *pDlgTGObjGen = m_pDlgTGContent->m_dlgTGChildGenner.m_pdlgTGObjectGenerater;
            if( pDlgTGObjGen )
            {
                pDlgTGObjGen->SetValue(&dlgObj.objectgen);
                pDlgTGObjGen->Refresh();
                pDlgTGObjGen->CalcResult();
                pDlgTGObjGen->GetValue(&dlgObj.objectgen);
                m_pDlgTGContent->RefreshTreeTitle(dlgObj.objectgen.m_hItem, dlgObj.objectgen.m_strDesc);
            }
        }
        m_bDrag             = false;
        m_pSelectedItem     = 0;
    }
}