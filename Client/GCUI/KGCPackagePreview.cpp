#include "stdafx.h"
#include "KGCPackagePreview.h"
#include "KGCPackageItemBox.h"

IMPLEMENT_CLASSNAME(KGCPackagePreview);
IMPLEMENT_WND_FACTORY(KGCPackagePreview);
IMPLEMENT_WND_FACTORY_NAME(KGCPackagePreview, "gc_package_preview");


KGCPackagePreview::KGCPackagePreview(void)
    : m_pkBack1(NULL)
    , m_pkBack2(NULL)
    , m_pkScroll(NULL)
    , m_pkClose(NULL)
    , m_pkTitle(NULL)
    , m_iScrollIdx(0)
    , m_iShowItemStartIdx(0)
{
    LINK_CONTROL("background1", m_pkBack1);
    LINK_CONTROL("background2", m_pkBack2);
    LINK_CONTROL("scroll", m_pkScroll);
    LINK_CONTROL("btn_close", m_pkClose);
    LINK_CONTROL("title", m_pkTitle);
}

KGCPackagePreview::~KGCPackagePreview(void)
{
}

bool KGCPackagePreview::CheckCondition(IN const KDialogInfo& kInfo_)
{
    if (kInfo_.m_lParam != 0)
    {
        InitControll();

        bool bPackage = false;

        std::vector< KDropItemInfo > vecItems_;

        g_pItemMgr->GetPackageElements(static_cast<GCITEMID>(kInfo_.m_lParam), vecItems_);
        if (!vecItems_.empty())
        {
            SetItemList(vecItems_);
            bPackage = true;
        }


        if (false == bPackage) {
            OnClickClsoe();
            return false;
        }
    }
    return true;
}

void KGCPackagePreview::InitControll()
{
    if (m_pkBack1)
        m_pkBack1->InitState(false);
    if (m_pkBack2)
        m_pkBack2->InitState(false);
    if (m_pkScroll)
    {
        m_pkScroll->InitState(false, true, this);
    }

    if (m_pkClose)
    {
        m_pkClose->SetHotKey(DIK_ESCAPE);
        m_pkClose->InitState(false, true, this);
        m_pkClose->DisconnectAllEventProcedure();
        m_pkClose->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPackagePreview::OnClickClsoe);
        m_pkClose->ConnectEventProcedure(KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCPackagePreview::OnEnterCursor);
    }
    if (m_pkTitle)
        m_pkTitle->InitState(false);

    m_vecItemBox.clear();
    m_vecItemInfo.clear();
}

void KGCPackagePreview::ActionPerformed(const KActionEvent& event)
{
    if (event.m_pWnd == m_pkScroll)
    {
        int iTemp = m_pkScroll->GetScrollPos();
        if (iTemp != m_iScrollIdx)
        {
            g_KDSound.Play("73");
            m_iScrollIdx = iTemp;
            RefreshItemBox();
        }
    }
}

void KGCPackagePreview::FrameMoveInEnabledState(void)
{
    if (m_pkScroll->IsRenderOn() && g_pkInput->GetZMov())
    {
        int iTemp = m_iScrollIdx;
        if (g_pkInput->GetZMov() > 0)
            m_pkScroll->SetScrollPos((m_pkScroll->GetScrollPos() - 1));
        else if (g_pkInput->GetZMov() < 0)
            m_pkScroll->SetScrollPos((m_pkScroll->GetScrollPos() + 1));

        iTemp = m_pkScroll->GetScrollPos();
        if (iTemp != m_iScrollIdx)
        {
            g_KDSound.Play("73");
            m_iScrollIdx = iTemp;
            RefreshItemBox();
        }
    }

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
    std::vector<KGCPackageItemBox*>::iterator vecIter = m_vecItemBox.begin();
    for (; vecIter != m_vecItemBox.end(); ++vecIter)
    {
        if ((*vecIter) && (*vecIter)->CheckPosInWindowBound(vMousePos))
        {
            (*vecIter)->SetItemInfoBox();
            break;
        }
    }

    if (vecIter == m_vecItemBox.end())
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
}

void KGCPackagePreview::SetItemList(std::vector< KDropItemInfo > vecItemInfo)
{
    int iItemBoxGap = GC_SCREEN_DIV_SIZE_INT(5);

    // 아이템 갯수에 맞게 UI크기 및 위치 조정
    int iItemBoxWidth = GC_SCREEN_DIV_SIZE_INT(75);                                                         // 아이템 박스 넓이
    int iItemBoxHeight = GC_SCREEN_DIV_SIZE_INT(75);                                                        // 아이템 박스 높이

    D3DXVECTOR2 vBGPos;
    vBGPos.x = FLOAT((400.0f * GC_SCREEN_DIV_WIDTH) - (iItemBoxWidth * 2 + iItemBoxGap));
    vBGPos.y = FLOAT((300.0f * GC_SCREEN_DIV_HEIGHT) - iItemBoxHeight);
    m_vOriPos.x = vBGPos.x;                                                         // 초기 윈도우 X축위치 저장
    m_vOriPos.y = vBGPos.y;                                                         // 초기 윈도우 X축위치 저장

    int iLineCnt = ((min(vecItemInfo.size(), SCROLL_LIMIT_CNT) - 1) / 4) + 1;         // 한줄에 4개 몇줄인지 저장
    int iLineWidth = (iItemBoxWidth * 4);                                           // 윈도우 넓이
    int iLineHeight = (iItemBoxHeight * iLineCnt) + (iItemBoxGap * (iLineCnt - 1)); // 윈도우 높이

    if (m_pkBack1)
    {
        m_pkBack1->SetSize((int(vecItemInfo.size()) > SCROLL_LIMIT_CNT) ? iLineWidth + GC_SCREEN_DIV_SIZE_INT(66) : iLineWidth + GC_SCREEN_DIV_SIZE_INT(46), iLineHeight + GC_SCREEN_DIV_SIZE_INT(70));
        m_pkBack1->SetFixedWindowLocalPos(D3DXVECTOR2(vBGPos.x - GC_SCREEN_DIV_SIZE_INT(15), vBGPos.y - GC_SCREEN_DIV_SIZE_INT(50)));
        m_pkBack1->ToggleRender(true);
    }
    if (m_pkBack2)
    {
        m_pkBack2->SetSize((int(vecItemInfo.size()) > SCROLL_LIMIT_CNT) ? iLineWidth + GC_SCREEN_DIV_SIZE_INT(46) : iLineWidth + GC_SCREEN_DIV_SIZE_INT(26), iLineHeight + GC_SCREEN_DIV_SIZE_INT(13));
        m_pkBack2->SetFixedWindowLocalPos(D3DXVECTOR2(vBGPos.x - GC_SCREEN_DIV_SIZE_INT(5), vBGPos.y - GC_SCREEN_DIV_SIZE_INT(5)));
        m_pkBack2->ToggleRender(true);
    }
    if (m_pkClose)
    {
        D3DXVECTOR2 vWindowPos = m_pkBack1->GetFixedWindowLocalPos();
        vWindowPos.x += (m_pkBack1->GetWidth() - m_pkClose->GetWidth()) - (14 / GC_SCREEN_DIV_HEIGHT);
        vWindowPos.y += (14 / GC_SCREEN_DIV_HEIGHT);
        m_pkClose->SetFixedWindowLocalPos(vWindowPos);
        m_pkClose->ToggleRender(true);
    }
    if (m_pkTitle)
    {
        D3DXVECTOR2 vWindowPos = m_pkBack1->GetFixedWindowLocalPos();

        vWindowPos.x += (m_pkBack1->GetWidth() - m_pkTitle->GetWidth()) / 2;

        vWindowPos.y += GC_SCREEN_DIV_SIZE_INT(25);
        m_pkTitle->SetFixedWindowLocalPos(vWindowPos);
        m_pkTitle->ToggleRender(true);
    }
    if (m_pkScroll)
    {
        if (int(vecItemInfo.size()) > SCROLL_LIMIT_CNT)
        {
            m_iScrollIdx = 0;
            D3DXVECTOR2 vWindowPos = m_pkBack1->GetFixedWindowLocalPos();
            vWindowPos.x += iLineWidth + GC_SCREEN_DIV_SIZE_INT(34);
            vWindowPos.y += GC_SCREEN_DIV_SIZE_INT(48);

            int nItemCount = (int)vecItemInfo.size();
            int nScrollMax = ((nItemCount - 1) / EM_INVEN_COLUMN) + 1;

            m_pkScroll->SetFixedWindowLocalPos( vWindowPos );
            m_pkScroll->SetSelfInputCheck( true );
            m_pkScroll->AddActionListener( this );
            m_pkScroll->SetHeight( m_pkBack1->GetHeight() - GC_SCREEN_DIV_SIZE_INT( 64 ) );
            m_pkScroll->SetScrollPos( m_iScrollIdx );
            m_pkScroll->SetScrollGap(1);
            m_pkScroll->SetThumbYGab(0);
            m_pkScroll->SetScrollPageSize( EM_INVEN_ROW );
            m_pkScroll->SetScrollRangeMin( 0 );
            m_pkScroll->SetScrollRangeMax( nScrollMax );
            m_pkScroll->ResizeBtn();
            m_pkScroll->ToggleRender( true );
            m_pkScroll->ToggleRender(true);
        }
        else
        {
            m_iScrollIdx = 0;
            m_pkScroll->SetScrollPos( m_iScrollIdx );
            m_pkScroll->ToggleRender( false );
        }
    }

    // 아이템 박스처리
    m_vecItemBox.clear();
    m_vecItemInfo.clear();
    m_vecItemInfo = vecItemInfo;
    m_iShowItemStartIdx = 0;
    int iCount = 0;
    int iEndLoop = int(vecItemInfo.size());
    while (((iEndLoop % 4) != 0)) ++iEndLoop;
    std::vector< KDropItemInfo >::iterator vecIter = vecItemInfo.begin();
    for (int iLoop = 0; iLoop < min(iEndLoop, SCROLL_LIMIT_CNT); ++iLoop)
    {
        if (vecIter != vecItemInfo.end())
        {
            CreateItemBox(iItemBoxHeight, iItemBoxWidth, vBGPos, iCount, &(*vecIter));
            vecIter++;
        }
        else
            CreateItemBox(iItemBoxHeight, iItemBoxWidth, vBGPos, iCount);
    }

    RefreshItemBox();
}

void KGCPackagePreview::CreateItemBox(int iHeight, int iWidth, D3DXVECTOR2& vPos, int& iCount, KDropItemInfo* paItemInfo)
{
    KGCPackageItemBox* pWnd = (KGCPackageItemBox*)g_pkUIMgr->CreateTemplateUI(this, "gc_package_item_box", "ui_ItemBox.stg", "gc_package_item_box", true, true);
    if (pWnd)
    {
        pWnd->InitState(true, true, this);
        pWnd->SetBoxSize(iHeight, iWidth);
        pWnd->SetFixedWindowLocalPos(vPos);

        if (paItemInfo)
            pWnd->SetItem(paItemInfo);

        vPos.x += iWidth + 5;
        if (0 == (++iCount) % 4)
        {
            vPos.x = m_vOriPos.x;
            vPos.y += iHeight + 5;
        }

        m_vecItemBox.push_back(pWnd);
    }
}

void KGCPackagePreview::RefreshItemBox()
{
    D3DXVECTOR2 vPos = m_vOriPos;
    m_iShowItemStartIdx = (m_iScrollIdx > -1) ? (m_iScrollIdx * 4) : 0;
    std::vector<KGCPackageItemBox*>::iterator vecIter = m_vecItemBox.begin();
    for (int iLoop = m_iShowItemStartIdx; vecIter != m_vecItemBox.end(); ++vecIter, ++iLoop)
    {
        if (iLoop >= int(m_vecItemInfo.size()))
            (*vecIter)->SetItem(NULL);
        else
            (*vecIter)->SetItem(&m_vecItemInfo[iLoop]);
    }
}

void KGCPackagePreview::OnClickClsoe()
{
    std::vector<KGCPackageItemBox*>::iterator vecIter = m_vecItemBox.begin();
    for (; vecIter != m_vecItemBox.end(); ++vecIter)
    {
        if ((*vecIter))
        {
            (*vecIter)->ToggleRender(false);
            (*vecIter)->Destroy();
        }
    }
    Destroy();
}

void KGCPackagePreview::OnEnterCursor()
{
    g_KDSound.Play("30");
}