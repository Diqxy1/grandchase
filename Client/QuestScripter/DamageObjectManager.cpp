#include "stdafx.h"
#include ".\damageobjectmanager.h"
#include "AddDamage.h"
#include "MapSystem/QSFantasticMap.h"
#include "MapSystem/KQSLayerManager.h"
#include "MapSystem/KQSLayerPart.h"
#include "ObjectDataManager.h"
#include "AddDamage_Popup.h"
#include "DlgTriggerContent.h"
#include "DlgTGAddDamage.h"

CDamageObjectManager::CDamageObjectManager(void)
{
    m_bDrag                 = false;
    m_pSelectedItem         = 0;
    m_pDlgTGContent         = 0;
    m_pvtObject             = 0;
}

CDamageObjectManager::~CDamageObjectManager(void)
{
}

void CDamageObjectManager::FrameMoveMode(void)
{
    //if( !m_pvtObject ) return;
    //// 플레이어를 서서히 떨어뜨린다.
    //for(int i=0;i<(int)m_pvtObject->size();i++)
    //{
    //    CAddDamage *pAddDamage = (*m_pvtObject)[i];
    //    if( !pAddDamage ) continue;

    //    int m_iMIndexX = (int)(pAddDamage->m_iEntranceX / FOOTHOLDSIZE_ON_MAP);
    //    int m_iMIndexY = (int)(pAddDamage->m_iEntranceY / FOOTHOLDSIZE_ON_MAP);

    //    if( m_iMIndexX < 0 )
    //        m_iMIndexX = 0;
    //    if( m_iMIndexX > SiKQSFantasticMap()->GetBlockWidth() - 1 )
    //        m_iMIndexX = SiKQSFantasticMap()->GetBlockWidth() - 1;

    //    if( m_iMIndexY < 0 )
    //        m_iMIndexY = 0;
    //    if( m_iMIndexY > SiKQSFantasticMap()->GetBlockHeight() - 1 )
    //        m_iMIndexY = SiKQSFantasticMap()->GetBlockHeight() - 1;

    //    KQSFootHold *pTemp =  SiKQSFantasticMap()->GetFHWorld()[m_iMIndexX][m_iMIndexY].CrashCheck(true);
    //    DWORD dwFootHoldType = KQSFootHold::FHT_INIT;
    //    if( pTemp ) dwFootHoldType = pTemp->GetFootHoldType();
    //    if( KQSFootHold::FHT_INIT == dwFootHoldType )
    //    {
    //        pAddDamage->m_iEntranceY -= 3.0f;

    //        //약 -50픽셀보다 작아지면 자리를 Y값 100으로 재배치한다.
    //        if( pAddDamage->m_iEntranceY < LAST_DEPH )
    //        {
    //            pAddDamage->m_iEntranceY = 100.0f;
    //        }
    //    }
    //}
}

void CDamageObjectManager::DrawMode(void)
{
    if( !m_pvtObject ) return;

    for(int i=0;i<(int)m_pvtObject->size();i++)
    {
        CAddDamage *pAddDamage = (*m_pvtObject)[i];
        if( !pAddDamage ) continue;

        // 매우 하드한 코딩 -ㅁ-;
        CString strDamageType;
        int iIndex = 0;
        if( 51 == pAddDamage->m_iDamageID )
        {
            iIndex = 0;
            strDamageType = "물기둥";
        }
        else if( 288 == pAddDamage->m_iDamageID )
        {
            iIndex = 1;
            strDamageType = "가스";
        }
        else if( 291 == pAddDamage->m_iDamageID )
        {
            iIndex = 2;
            strDamageType = "큰가스";
        }
		else if( 876 == pAddDamage->m_iDamageID )
		{
			iIndex = 3;
			strDamageType = "불기둥";
		}


        // FireInfo는 Blank 파일로 모두 처리.
        //CString strObject = SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DAMAGE)[iIndex];
        CString strObject = L"BLANK";
        CString strMonDesc;
        strMonDesc.Format(L"DamageID : %s[%d]", strDamageType, pAddDamage->m_iDamageID);

        DrawPartObject(pAddDamage->m_iEntranceX, pAddDamage->m_iEntranceY, 1, strObject);
        DrawPartDesc(g_pd3dDevice, pAddDamage->m_iEntranceX, pAddDamage->m_iEntranceY, strMonDesc, strObject);
    }
}

void CDamageObjectManager::DrawSelectedObject(void)
{
    DrawPartObject((float)m_ptDragItem.x, (float)m_ptDragItem.y, 0, m_strCurObject);
}

void CDamageObjectManager::MouseMoveMode(D3DVECTOR pos)
{
    if( m_pSelectedItem )
    {
        // 선택된 오브젝트가 있으면 위치를 변경시켜준다.
        m_pSelectedItem->m_iEntranceX = pos.x;
        m_pSelectedItem->m_iEntranceY = pos.y;
    }

    m_ptDragItem.SetPoint((int)pos.x, (int)pos.y);
}

void CDamageObjectManager::MouseLBDownMode(D3DVECTOR pos)
{
    if( !m_pvtObject ) return;

    if( GetAsyncKeyState(VK_CONTROL) & 0x8000 )       // 컨트롤 버튼을 눌르고 있나?
    {
        // 한놈이 드래그 되고있다면 위치를 변경시키고 그것이 아니라면 드래그 할 놈을 지정한다.
        if( m_bDrag )
        {
            if( m_pSelectedItem )
            {
                m_pSelectedItem->m_iEntranceX = pos.x;
                m_pSelectedItem->m_iEntranceY = pos.y;
            }
        }
        else
        {
            for(int i=0;i<(int)m_pvtObject->size();i++)
            {
                CAddDamage *pAddDamage = (*m_pvtObject)[i];
                if( !pAddDamage ) continue;

                // 매우 하드한 코딩... 일정이 너무 빡빡해서...
                // 미안... 다음에 손봐주마...
                int iIndex = 0;
                CString strDamageType;
                if( 51 == pAddDamage->m_iDamageID )
                {
                    iIndex = 0;
                    strDamageType = "물기둥";
                }
                else if( 288 == pAddDamage->m_iDamageID )
                {
                    iIndex = 1;
                    strDamageType = "가스";
                }
                else if( 291 == pAddDamage->m_iDamageID )
                {
                    iIndex = 2;
                    strDamageType = "큰가스";
                }
				else if( 876 == pAddDamage->m_iDamageID )
				{
					iIndex = 3;
					strDamageType = "불기둥";
				}
		

                if( IsInLayerPart(pos, pAddDamage->m_iEntranceX, pAddDamage->m_iEntranceY, L"BLANK"/*SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DAMAGE)[iIndex]*/) )
                {
                    m_bDrag = true;
                    m_pSelectedItem = pAddDamage;
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
            CAddDamage_Popup dlgAddDamage_Popup;


           /* for(int i=0;i<(int)SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DAMAGE).size();++i)
            {
                if( SiCObjectDataManager()->GetEnumItem(OBJECTTYPE_EACTION_DAMAGE)[i] == m_strCurObject )
                {*/
                    // 매우 하드한 코딩...
                    // 일정때문에 훗날 되게 만들어 줄게...
                    int iIndex = 0;
                    if( "물기둥" == m_strCurObject )
                    {
                        iIndex = 51;
                    }
                    else if( "가스" == m_strCurObject )
                    {
                        iIndex = 288;
                    }
                    else if( "큰가스" == m_strCurObject )
                    {
                        iIndex = 291;
                    }
					else if( "불기둥" == m_strCurObject )
					{
						iIndex = 876;
					}
		
                    dlgAddDamage_Popup.m_AddDamage.m_iDamageID = iIndex;
             //   }
            //}

            dlgAddDamage_Popup.m_AddDamage.m_iEntranceX = pos.x;
            dlgAddDamage_Popup.m_AddDamage.m_iEntranceY = pos.y;
            dlgAddDamage_Popup.SetMapDamageList(m_pvtObject);

            // 정보를 설정한다.
            if( IDOK == dlgAddDamage_Popup.DoModal() )
            {
                if( !m_pDlgTGContent ) return;
                if( !m_pDlgTGContent->m_pCurrentTrigger ) return;

                // 플레이어 위치 설정 트리거의 설명을 넣어주는 부분이다.
                CDlgTGAddDamage *pDlgTGAddDamage = m_pDlgTGContent->m_dlgTGChildGenner.m_TGAddDamage;
                if( pDlgTGAddDamage )
                {
                    pDlgTGAddDamage->SetValue(&dlgAddDamage_Popup.m_AddDamage);
                    pDlgTGAddDamage->Refresh();
                    pDlgTGAddDamage->CalcResult();
                    pDlgTGAddDamage->GetValue(&dlgAddDamage_Popup.m_AddDamage);
                }

                CAddDamage *pAddDamage = new CAddDamage;
                *pAddDamage = dlgAddDamage_Popup.m_AddDamage;                        
                if( dlgAddDamage_Popup.m_bFnAction )
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hAction, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddAction(pAddDamage);
                }
                else
                {
                    m_pDlgTGContent->m_treeTG.Select(m_pDlgTGContent->m_hEvent, TVGN_CARET, 0);
                    m_pDlgTGContent->m_pCurrentTrigger->AddEvent(pAddDamage);
                }
                std::wstring strDesc = dlgAddDamage_Popup.m_AddDamage.m_strDesc;

                pAddDamage->m_hItem = m_pDlgTGContent->m_treeTG.AddChild(strDesc.c_str());
                m_pvtObject->push_back(pAddDamage);
            }
        } // end if m_strCurObject 길이체크
    }
}

void CDamageObjectManager::MouseLBUpMode(D3DVECTOR pos)
{
    if( m_bDrag && m_pSelectedItem )
    {
        CAddDamage_Popup dlgAddDamage_Popup;
        dlgAddDamage_Popup.SetMapDamageList(m_pvtObject);

        // 현재 드래그 중이었던 녀석의 기본적인 정보를 설정해준다.
        dlgAddDamage_Popup.m_AddDamage = *m_pSelectedItem;

        if( IDOK == dlgAddDamage_Popup.DoModal() )
        {
            // 확인을 눌렀다면 정보를 갱신해준다.
            *m_pSelectedItem = dlgAddDamage_Popup.m_AddDamage;
            CDlgTGAddDamage *pDlgTGAddDamage = m_pDlgTGContent->m_dlgTGChildGenner.m_TGAddDamage;
            if( pDlgTGAddDamage )
            {
                pDlgTGAddDamage->SetValue(&dlgAddDamage_Popup.m_AddDamage);
                pDlgTGAddDamage->Refresh();
                pDlgTGAddDamage->CalcResult();
                pDlgTGAddDamage->GetValue(&dlgAddDamage_Popup.m_AddDamage);
                m_pDlgTGContent->RefreshTreeTitle(dlgAddDamage_Popup.m_AddDamage.m_hItem, dlgAddDamage_Popup.m_AddDamage.m_strDesc);
            }
        }
        m_bDrag        = false;
        m_pSelectedItem     = 0;
    }
}
