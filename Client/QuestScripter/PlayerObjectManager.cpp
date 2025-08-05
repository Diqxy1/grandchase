#include "stdafx.h"
#include ".\playerobjectmanager.h"
#include "PlayerPosition.h"
#include "DlgTGPlayerPosition.h"
#include "MapSystem/QSFantasticMap.h"
#include "MapSystem/KQSLayerManager.h"
#include "MapSystem/KQSLayerPart.h"
#include "DlgPlayerPosition_Popup.h"
#include "DlgTriggerContent.h"
#include "ObjectDataManager.h"

CPlayerObjectManager::CPlayerObjectManager(void)
{
    m_bDrag                 = false;
    m_pSelectedItem         = 0;
    m_pDlgTGContent         = 0;
    m_pvtObject             = 0;
}

CPlayerObjectManager::~CPlayerObjectManager(void)
{
}

void CPlayerObjectManager::FrameMoveMode(void)
{
    if( !m_pvtObject ) return;
    // 플레이어를 서서히 떨어뜨린다.
    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CPlayerPosition *pPlayerPosition = (*m_pvtObject)[i];
        if( !pPlayerPosition ) continue;
        if( -1 == pPlayerPosition->m_iPlayerIndex ) continue;   // -1은 위치 선택 지정이 안되었단 말.

        int m_iMIndexX = (int)(pPlayerPosition->m_fEntranceX / FOOTHOLDSIZE_ON_MAP);
        int m_iMIndexY = (int)(pPlayerPosition->m_fEntranceY / FOOTHOLDSIZE_ON_MAP);

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
            pPlayerPosition->m_fEntranceY -= 3.0f;

            //약 -50픽셀보다 작아지면 자리를 Y값 100으로 재배치한다.
            if( pPlayerPosition->m_fEntranceY < LAST_DEPH )
            {
                pPlayerPosition->m_fEntranceY = 100.0f;
            }
        }
    }
}

void CPlayerObjectManager::DrawMode(void)
{
    if( !m_pvtObject ) return;

    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CPlayerPosition *pPlayerPosition = (*m_pvtObject)[i];
        if( !pPlayerPosition ) continue;
        if( -1 == pPlayerPosition->m_iPlayerIndex ) continue;

        CString strObject = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_PLAYER)[pPlayerPosition->m_iPlayerIndex];
        CString strMonDesc;
        strMonDesc.Format(L"PlayerIndex : %d\n Right : %d", pPlayerPosition->m_iPlayerIndex, pPlayerPosition->m_bRight);

        DrawPartObject(pPlayerPosition->m_fEntranceX, pPlayerPosition->m_fEntranceY, pPlayerPosition->m_bRight, strObject);
        DrawPartDesc(g_pd3dDevice, pPlayerPosition->m_fEntranceX, pPlayerPosition->m_fEntranceY, strMonDesc, strObject);
    }
}

void CPlayerObjectManager::DrawSelectedObject(void)
{
    DrawPartObject((float)m_ptDragItem.x, (float)m_ptDragItem.y, 0, m_strCurObject);
}

void CPlayerObjectManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_pSelectedItem )
    {
        // 선택된 오브젝트가 있으면 위치를 변경시켜준다.
        m_pSelectedItem->m_fEntranceX = pos.x;
        m_pSelectedItem->m_fEntranceY = pos.y;
    }

    m_ptDragItem.SetPoint((int)pos.x, (int)pos.y);
}

void CPlayerObjectManager::MouseLBDownMode(D3DVECTOR pos)
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
                CPlayerPosition *pPosition = (*m_pvtObject)[i];
                if( !pPosition ) continue;
                if( -1 == pPosition->m_iPlayerIndex ) continue;

                if( IsInLayerPart(pos, pPosition->m_fEntranceX, pPosition->m_fEntranceY, SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_PLAYER)[pPosition->m_iPlayerIndex]) )
                {
                    m_bDrag = true;
                    m_pSelectedItem = pPosition;
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
            CDlgPlayerPosition_Popup dlgPlayerPosition_Popup;
            for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_PLAYER).size();++i)
            {
                if( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_PLAYER)[i] == m_strCurObject )
                    dlgPlayerPosition_Popup.m_PlayerPosition.m_iPlayerIndex = i;
            }

            dlgPlayerPosition_Popup.m_PlayerPosition.m_fEntranceX = pos.x;
            dlgPlayerPosition_Popup.m_PlayerPosition.m_fEntranceY = pos.y;
            dlgPlayerPosition_Popup.SetPlayerListValue(m_pvtObject);

            // 정보를 설정한다.
            if( IDOK == dlgPlayerPosition_Popup.DoModal() )
            {
                if( !m_pDlgTGContent ) return;
                if( !m_pDlgTGContent->m_pCurrentTrigger ) return;

                // 플레이어 위치 설정 트리거의 설명을 넣어주는 부분이다.
                CDlgTGPlayerPosition *pDlgTGPlayerPosition = m_pDlgTGContent->m_dlgTGChildGenner.m_pdlgTGPlayerPosition;
                if( pDlgTGPlayerPosition )
                {
                    pDlgTGPlayerPosition->SetValue(&dlgPlayerPosition_Popup.m_PlayerPosition);
                    pDlgTGPlayerPosition->Refresh();
                    pDlgTGPlayerPosition->CalcResult();
                    pDlgTGPlayerPosition->GetValue(&dlgPlayerPosition_Popup.m_PlayerPosition);
                }

                CPlayerPosition *pPlayerPosition = new CPlayerPosition;
                *pPlayerPosition = dlgPlayerPosition_Popup.m_PlayerPosition;                        
                if( dlgPlayerPosition_Popup.m_bFnAction )
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hAction, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddAction(pPlayerPosition);
                }
                else
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hEvent, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddEvent(pPlayerPosition);
                }

                std::wstring strDesc = dlgPlayerPosition_Popup.m_PlayerPosition.m_strDesc;

                pPlayerPosition->m_hItem = m_pDlgTGContent->m_treeTG.AddChild( strDesc.c_str() );
                m_pvtObject->push_back(pPlayerPosition);
            }
        } // end if m_strCurObject 길이체크
    }
}

void CPlayerObjectManager::MouseLBUpMode(D3DVECTOR pos)
{
    if( m_bDrag && m_pSelectedItem )
    {
        CDlgPlayerPosition_Popup dlgPlayerPosition_Popup;
        dlgPlayerPosition_Popup.SetPlayerListValue(m_pvtObject);

        // 현재 드래그 중이었던 녀석의 기본적인 정보를 설정해준다.
        dlgPlayerPosition_Popup.m_PlayerPosition = *m_pSelectedItem;

        if( IDOK == dlgPlayerPosition_Popup.DoModal() )
        {
            // 확인을 눌렀다면 정보를 갱신해준다.
            *m_pSelectedItem = dlgPlayerPosition_Popup.m_PlayerPosition;
            CDlgTGPlayerPosition *pDlgTGPlayerPosition = m_pDlgTGContent->m_dlgTGChildGenner.m_pdlgTGPlayerPosition;
            if( pDlgTGPlayerPosition )
            {
                pDlgTGPlayerPosition->SetValue(&dlgPlayerPosition_Popup.m_PlayerPosition);
                pDlgTGPlayerPosition->Refresh();
                pDlgTGPlayerPosition->CalcResult();
                pDlgTGPlayerPosition->GetValue(&dlgPlayerPosition_Popup.m_PlayerPosition);
                m_pDlgTGContent->RefreshTreeTitle(dlgPlayerPosition_Popup.m_PlayerPosition.m_hItem, dlgPlayerPosition_Popup.m_PlayerPosition.m_strDesc);
            }
        }
        m_bDrag        = false;
        m_pSelectedItem     = 0;
    }
}
