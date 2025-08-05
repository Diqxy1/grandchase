#include "stdafx.h"
#include "../MyD3D.h"
#include "GCUserEquipBox.h"
#include "GCUI/GCMyInfoScene.h"
#include "GCUI/GCPetWnd.h"
#include "GCUI/GCPetInfoBox.h"
#include "GCUI/GCPetItembox.h"
#include "GCUI/GCUserCoreEquipWnd.h"
#include "GCUI/GCLookSavePanel.h"
#include "GCUI/GCZeroMaskDlg.h"

IMPLEMENT_CLASSNAME(KGCUserEquipBox);
IMPLEMENT_WND_FACTORY(KGCUserEquipBox);
IMPLEMENT_WND_FACTORY_NAME(KGCUserEquipBox, "gc_user_equip_box");

static DWORD s_aItemCategory[UEC_NUM][KGCUserEquipBox::NUM_LOOK_CATEGORY] =
{
    {
        ESP_HELMET,
        ESP_JACKET,
        ESP_PANTS,
        ESP_GLOVES,
        ESP_SHOES,
        ESP_MANTLE,
        ESP_WEAPON,
        ESP_PET,
        ESP_HELMET_SOCKET,
        ESP_WEAPON_SOCKET,
        ESP_JACKET_SOCKET,
        0,
        0,
        0,
        0,
    },
    {
        ESP_A_UPPER_HELMET,
        ESP_A_DOWN_HELMET,
        ESP_A_JACKET,
        ESP_A_PANTS,
        ESP_A_ARM,
        ESP_A_FINGER,
        ESP_A_NECKLET,
        ESP_A_SHOES,
        ESP_A_EARRING,
        ESP_A_EARRING,
        ESP_A_HELMET_SOCKET,
        0,
        0,
        0,
        0,
    },
    {
        ESP_SLOT_ITEM,
        ESP_SLOT_EMOT,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    },
    {
        EPET_EQUIP_ITEM_SLOT_NORMAL_ITEM,
        EPET_EQUIP_ITEM_SLOT_SKILL,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    },
    {
        ESP_SKC_WHITE_ONE,
        ESP_SKC_WHITE_TWO,
        ESP_SKC_WHITE_THREE,
        0,
        ESP_SKC_WHITE_CHANGEJOB_ONE,
        ESP_SKC_WHITE_CHANGEJOB_TWO,
        ESP_SKC_WHITE_CHANGEJOB_THREE,
        0,
        ESP_SKC_WHITE_CHANGEJOB2_ONE,
        ESP_SKC_WHITE_CHANGEJOB2_TWO,
        ESP_SKC_WHITE_CHANGEJOB2_THREE,
        0,
        0,
        0,
        0,
    },
    {
        ESP_SKC_WHITE_ONE,
        ESP_SKC_WHITE_TWO,
        ESP_SKC_WHITE_THREE,
        0,
        ESP_SKC_WHITE_CHANGEJOB_ONE,
        ESP_SKC_WHITE_CHANGEJOB_TWO,
        ESP_SKC_WHITE_CHANGEJOB_THREE,
        0,
        ESP_SKC_WHITE_CHANGEJOB2_ONE,
        ESP_SKC_WHITE_CHANGEJOB2_TWO,
        ESP_SKC_WHITE_CHANGEJOB2_THREE,
        0,
        0,
        0,
        0,
    },
    {
        ESP_SKC_DARK_ONE,
        ESP_SKC_DARK_TWO,
        ESP_SKC_DARK_THREE,
        0,
        ESP_SKC_DARK_CHANGEJOB_ONE,
        ESP_SKC_DARK_CHANGEJOB_TWO,
        ESP_SKC_DARK_CHANGEJOB_THREE,
        0,
        ESP_SKC_DARK_CHANGEJOB2_ONE,
        ESP_SKC_DARK_CHANGEJOB2_TWO,
        ESP_SKC_DARK_CHANGEJOB2_THREE,
        0,
        0,
        0,
        0,
    },
    {
        ESP_HELMET,
        ESP_JACKET,
        ESP_PANTS,
        ESP_GLOVES,
        ESP_SHOES,
        ESP_MANTLE,
        ESP_A_UPPER_HELMET,
        ESP_A_DOWN_HELMET,
        ESP_A_JACKET,
        ESP_A_PANTS,
        ESP_A_ARM,
        ESP_A_NECKLET,
        ESP_A_SHOES,
        ESP_TITLE,
        ESP_WEAPON,
    }
};

KGCUserEquipBox::KGCUserEquipBox(void)
    : m_bEquipLock(false)
    , m_iScrollTop(0)
    , m_pkItemButton(NULL)
    , m_pkEmoticonButton(NULL)
    , m_pkScrollbar(NULL)
    , m_pkPetWnd(NULL)
    , m_pkPetInfoBox(NULL)
    , m_pkBtnMaskSelect(NULL)
    , m_pkZeroMaskDlg(NULL)
{
    if (GC_GLOBAL_DEFINE::bWeaponCoordi)
        m_nLookWnd = NUM_LOOK_WND_EX;
    else
        m_nLookWnd = NUM_LOOK_WND;

    for (int i = 0; i < (int)m_apItemWnd.size(); ++i)
    {
        m_apItemWnd[i] = NULL;
    }

    for (int i = 0; i < NUM_CATEGORY; i++)
    {
        m_apSkillWnd[i] = NULL;
    }
    LINK_CONTROL("skillbox_item_s1", m_apSkillWnd[3]);
    LINK_CONTROL("skillbox_item_s2", m_apSkillWnd[7]);
    LINK_CONTROL("skillbox_item_s3", m_apSkillWnd[11]);

    char strTemp[MAX_PATH] = { 0, };

    int i;
    for (i = 0; i < NUM_ITEM_WND; ++i)
    {
        m_apItemWnd[i] = NULL;
        LINK_CONTROL_STM("armorbox_item" << i, m_apItemWnd[i]);

        // 위치잡기용 윈도우
        m_apWnd[i] = NULL;
        LINK_CONTROL_STM("skillbox_wnd" << i, m_apWnd[i]);

        m_pkClosedNomalItemWnd[i] = NULL;
        LINK_CONTROL_STM("close_item" << i, m_pkClosedNomalItemWnd[i]);

        m_pkClosedapItemWnd[i] = NULL;
        LINK_CONTROL_STM("amor_close_item" << i, m_pkClosedapItemWnd[i]);
    }

    for (int i = 0; i < NUM_PET_WND; ++i)
    {
        m_apPetItemWnd[i] = NULL;
        LINK_CONTROL_STM("petbox_item" << i, m_apPetItemWnd[i]);
    }

    for (int i = 0; i < NUM_SHORTCUT_WND; ++i)
    {
        m_apShortCutWnd[i] = NULL;
        LINK_CONTROL_STM("shortcutbox" << i, m_apShortCutWnd[i]);
    }

    for (int i = 0; i < NUM_CLOSED_WND; ++i)
    {
        m_pkClosedShortCutWnd[i] = NULL;
        LINK_CONTROL_STM("closedbox" << i, m_pkClosedShortCutWnd[i]);

        m_apAccessoryWnd[i] = NULL;
        sprintf(strTemp, "accessorybox_item%d", i);
        LINK_CONTROL(strTemp, m_apAccessoryWnd[i]);

        m_apCloseAcceWnd[i] = NULL;
        LINK_CONTROL_STM("closed_acce_box" << i, m_apCloseAcceWnd[i]);
    }

    for (int i = 0; i < NUM_LOOK_WND_EX; ++i)
    {
        m_apLookWnd[i] = NULL;
    }

    for (int i = 0; i < m_nLookWnd; ++i)
    {
        LINK_CONTROL_STM("lookitem" << std::setw(2) << std::setfill('0') << i + 1, m_apLookWnd[i]);
    }


    m_pkBtnLookChang = NULL;
    m_pkBtnCoordiCompose = NULL;
    m_pkBtnCoordiUpgrade = NULL;
    LINK_CONTROL("btn_change_look", m_pkBtnLookChang);
    LINK_CONTROL("btn_coordi_compose", m_pkBtnCoordiCompose);
    LINK_CONTROL("btn_coordi_upgrade", m_pkBtnCoordiUpgrade);
    LINK_CONTROL("look_save_panel", m_pkLookSavePanel);
    LINK_CONTROL("shortcut_item_tab", m_pkItemButton);
    LINK_CONTROL("shortcut_emoticon_tab", m_pkEmoticonButton);
    LINK_CONTROL("scroll", m_pkScrollbar);
    m_pkScrollbarBack = NULL;
    LINK_CONTROL("scroll_back", m_pkScrollbarBack);
    LINK_CONTROL("pet_window", m_pkPetWnd);
    LINK_CONTROL("pet_info_box", m_pkPetInfoBox);
    LINK_CONTROL("btn_mask_select", m_pkBtnMaskSelect);
    LINK_CONTROL("zeromask_select_box", m_pkZeroMaskDlg);
}

KGCUserEquipBox::~KGCUserEquipBox(void)
{
}

void KGCUserEquipBox::OnCreate(void)
{
    std::vector<KD3DWnd*> vecSkillWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\skillbox_item0", true, NUM_1STEPSKILL);
    if (!vecSkillWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSkillWnd = vecSkillWndList.begin();
        for (int i = 0; iterSkillWnd != vecSkillWndList.end(); ++iterSkillWnd, i += 4)
        {
            m_apSkillWnd[i] = static_cast<KGCUserEquipWnd*>(*iterSkillWnd);
        }
    }
    vecSkillWndList.clear();

    vecSkillWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\skillbox_item1", true, NUM_2STEPSKILL);
    if (!vecSkillWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSkillWnd = vecSkillWndList.begin();
        for (int i = 1; iterSkillWnd != vecSkillWndList.end(); ++iterSkillWnd, i += 4)
        {
            m_apSkillWnd[i] = static_cast<KGCUserEquipWnd*>(*iterSkillWnd);
        }
    }
    vecSkillWndList.clear();

    vecSkillWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\skillbox_item2", true, NUM_3STEPSKILL);
    if (!vecSkillWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSkillWnd = vecSkillWndList.begin();
        for (int i = 2; iterSkillWnd != vecSkillWndList.end(); ++iterSkillWnd, i += 4)
        {
            m_apSkillWnd[i] = static_cast<KGCUserEquipWnd*>(*iterSkillWnd);
        }
    }
    vecSkillWndList.clear();

    std::vector<KD3DWnd*> vecCloseWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\closedbox", true, NUM_CLOSED_WND);
    if (!vecCloseWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterCloseWnd = vecCloseWndList.begin();
        for (int i = 0; iterCloseWnd != vecCloseWndList.end(); ++iterCloseWnd, i++)
        {
            m_pkClosedShortCutWnd[i] = (*iterCloseWnd);
        }
    }
    vecCloseWndList.clear();

    vecCloseWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\amore_close_box", true, NUM_ITEM_WND);
    if (!vecCloseWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterCloseWnd = vecCloseWndList.begin();
        for (int i = 0; iterCloseWnd != vecCloseWndList.end(); ++iterCloseWnd, i++)
        {
            m_pkClosedapItemWnd[i] = (*iterCloseWnd);
        }
    }
    vecCloseWndList.clear();

    vecCloseWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\acce_close_box", true, NUM_CLOSED_WND);
    if (!vecCloseWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterCloseWnd = vecCloseWndList.begin();
        for (int i = 0; iterCloseWnd != vecCloseWndList.end(); ++iterCloseWnd, i++)
        {
            m_apCloseAcceWnd[i] = (*iterCloseWnd);
        }
    }
    vecCloseWndList.clear();

    vecCloseWndList = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_my_info_scene.stg", "", "my_info_scene\\myinfo_item_box\\close_item", true, NUM_ITEM_WND);
    if (!vecCloseWndList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterCloseWnd = vecCloseWndList.begin();
        for (int i = 0; iterCloseWnd != vecCloseWndList.end(); ++iterCloseWnd, i++)
        {
            m_pkClosedNomalItemWnd[i] = (*iterCloseWnd);
        }
    }
    vecCloseWndList.clear();

    Clear();
    ResetScroll();

    for (int i = 0; i < NUM_CLOSED_WND; i++)
    {
        m_pkClosedShortCutWnd[i]->SetFixedWindowLocalPos(m_apShortCutWnd[i + 5]->GetFixedWindowLocalPos() + D3DXVECTOR2(1, 1));
    }

    for (int i = 0; i < NUM_CLOSED_WND; i++)
    {
        m_apCloseAcceWnd[i]->SetFixedWindowLocalPos(m_apAccessoryWnd[i]->GetFixedWindowLocalPos() + D3DXVECTOR2(1, 1));
    }

    for (int i = 0; i < NUM_ITEM_WND; i++)
    {
        m_pkClosedapItemWnd[i]->SetFixedWindowLocalPos(m_apItemWnd[i]->GetFixedWindowLocalPos() + D3DXVECTOR2(1, 1));
    }

    m_pkBtnLookChang->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCUserEquipBox::OnClickBtnChange );

    if ( m_pkBtnCoordiCompose )
    {
        m_pkBtnCoordiCompose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCUserEquipBox::OnClickBtnCompose );
    }

    if ( m_pkBtnCoordiUpgrade )
        m_pkBtnCoordiUpgrade->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCUserEquipBox::OnClickBtnUpgrade );

    if ( m_pkBtnMaskSelect )
        m_pkBtnMaskSelect->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCUserEquipBox::OnClickBtnMask );
}

void KGCUserEquipBox::Clear()
{
    for (int i = 0; i < NUM_ITEM_WND; ++i)
    {
        m_apItemWnd[i]->InitState(false);
        m_apItemWnd[i]->SetItem();
        m_apItemWnd[i]->SetSocketItem(NULL);
        m_pkClosedNomalItemWnd[i]->InitState(false);
    }

    for (int i = 0; i < NUM_PET_WND; ++i)
    {
        m_apPetItemWnd[i]->InitState(false);
        m_apPetItemWnd[i]->SetItem();
    }

    for (int i = 0; i < NUM_CATEGORY; i++)
    {
        m_apSkillWnd[i]->InitState(false);
        m_apSkillWnd[i]->SetItem();
    }

    for (int i = 0; i < NUM_SHORTCUT_WND; ++i)
    {
        m_apShortCutWnd[i]->InitState(false);
        m_apShortCutWnd[i]->SetItem();
    }

    for (int i = 0; i < NUM_CLOSED_WND; ++i)
    {
        m_pkClosedShortCutWnd[i]->InitState(false);
        m_apAccessoryWnd[i]->InitState(false);
        m_apAccessoryWnd[i]->SetItem();
        m_apAccessoryWnd[i]->SetSocketItem(NULL);
    }

    for (int i = 0; i < m_nLookWnd; ++i) {
        if (m_apLookWnd[i] == NULL)
            break;

        m_apLookWnd[i]->InitState(false);
        m_apLookWnd[i]->SetItem();
    }

    if (m_pkBtnLookChang)
        m_pkBtnLookChang->InitState(false, true, this);

    if (m_pkBtnCoordiCompose)
        m_pkBtnCoordiCompose->InitState(false, true, this);

    if (m_pkBtnCoordiUpgrade)
        m_pkBtnCoordiUpgrade->InitState(false, true, this);

    m_pkPetWnd->InitState(false);
    m_pkPetInfoBox->InitState(false);

    if (m_pkBtnMaskSelect)
        m_pkBtnMaskSelect->InitState(false, true, this);

    if (m_pkZeroMaskDlg)
        m_pkZeroMaskDlg->InitState(false, true, this);
}

void KGCUserEquipBox::ResetScroll()
{
    m_pkScrollbar->SetScrollRangeMax(0);
    m_pkScrollbar->SetScrollPageSize(2);
    m_pkScrollbar->SetScrollPos(0);

    m_pkScrollbar->AddActionListener(this);
    m_pkScrollbar->SetSelfInputCheck(false);

    m_iScrollTop = 0;
}

void KGCUserEquipBox::SetScrollPos(int _ScrollPos)
{
    m_pkScrollbar->SetScrollPos(_ScrollPos);
}

void KGCUserEquipBox::ActionPerformed(const KActionEvent& event)
{
    // 커서가 범위에 들어왔는지 검사
    if (event.m_dwCode == D3DWE_CURSOR_ENTER)
    {
        g_KDSound.Play("30");
        return;
    }

    //잠김 상태면 인풋을 처리하지 않겠음
    if (IsLocked())
        return;

    // 커서가 스크롤바를 건드렸는지 검사
    if (event.m_pWnd == m_pkScrollbar)
    {
        m_iScrollTop = m_pkScrollbar->GetScrollPos();
        SetItemCategory(m_ItemCategory);
        return;
    }

    // 단축슬롯인 경우 우측탭을 눌렀을 때 이벤트를 처리합니다.
    if (m_ItemCategory == UEC_SHORTCUT_SLOT)
    {
        if (event.m_dwCode == D3DWE_BUTTON_UP || event.m_dwCode == D3DWE_BUTTON_CLICK)
        {
            if (event.m_pWnd == m_pkItemButton)
                m_pkScrollbar->SetScrollPos(0);
            else if (event.m_pWnd == m_pkEmoticonButton)
                m_pkScrollbar->SetScrollPos(1);

            SetItemCategory(m_ItemCategory);
        }
    }

    // 커서가 뭔가를 클릭했는지 검사
    if (event.m_dwCode != D3DWE_BUTTON_DOWN)
        return;

    KGCUserEquipWnd* pEventItemWnd = NULL;

    int iStart = 0;
    if (m_ItemCategory == UEC_SHORTCUT_SLOT || m_ItemCategory == UEC_EQUIP2)
        iStart = m_iScrollTop * 5;
    else
        iStart = m_iScrollTop * 4;

    int iMaxWnd = NUM_ITEM_WND;
    if (m_ItemCategory == UEC_EQUIP2)
        iMaxWnd = NUM_CLOSED_WND;

    for (int i = iStart; i < iStart + iMaxWnd; ++i)
    {
        //장착중인 아이템
        if ((i < (int)m_apItemWnd.size() && m_apItemWnd[i] == event.m_pWnd) ||
            (i < (int)m_apAccessoryWnd.size() && m_apAccessoryWnd[i] == event.m_pWnd) ||
            (i < (int)m_apSkillWnd.size() && m_apSkillWnd[i] == event.m_pWnd) ||
            (i < (int)m_apPetItemWnd.size() && m_apPetItemWnd[i] == event.m_pWnd))
        {
            pEventItemWnd = (KGCUserEquipWnd*)event.m_pWnd;
            break;
        }
    }

    if (pEventItemWnd == NULL)
    {
        for (int i = 0; i < NUM_SHORTCUT_WND; ++i)
        {
            if (m_apShortCutWnd[i] == event.m_pWnd)
            {
                pEventItemWnd = (KGCUserEquipWnd*)event.m_pWnd;
                break;
            }
        }
    }

    if (pEventItemWnd == NULL)
    {
        for (int i = 0; i < m_nLookWnd; ++i)
        {
            if (m_apLookWnd[i] == event.m_pWnd)
            {
                pEventItemWnd = (KGCUserEquipWnd*)event.m_pWnd;
                break;
            }
        }
    }

    if (pEventItemWnd == NULL)
        return;


    g_KDSound.Play("31");
    ReleaseCapture();

    // 장착중인 아이템이 없을때
    if (pEventItemWnd->GetItemID() == -1)
    {

        // PetSkill Slot일 경우 자동으로 찾아서 넣는다.
        if (event.m_pWnd == m_apPetItemWnd[1])
        {
            g_MyD3D->m_TempPlayer.AutoEquipPetItem();
            g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();
            g_pkUIScene->m_pkMyInfoScene->UpdateGameData(false, g_pkUIScene->m_pkMyInfoScene->GetEquipCategory(), m_pkScrollbar->GetScrollPos());
        }

        return;
    }

    GCItem* kItem = pEventItemWnd->GetItemInfo();
    RETURN_NIL(kItem);

    bool bCoordi = false;
    KItem* pkInvenItem = g_pItemMgr->GetInventoryItem(pEventItemWnd->GetItemUID());
    if (pkInvenItem != NULL && pkInvenItem->CheckItemType(KItem::TYPE_LOOK)) {
        g_pItemMgr->UnequipItemForItemUID(&g_MyD3D->m_TempPlayer.GetCurrentChar(), pEventItemWnd->GetItemUID());
        pEventItemWnd->SetItem();
        bCoordi = true;
    }

    // 펫 : 펫 장착해제
    if (kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_PET)
    {
        SCharInfo& SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
        g_MyD3D->m_TempPlayer.UnEquipPet(SChar.iCharType);
    }

    // 펫 : 펫아이템 장착해제
    else if (kItem->eItemKind == GIK_PET && kItem->ePetItemKind == EPET_KIND_EQUIP_ITEM)
    {
        SCharInfo& SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
        KPetInfo& kPet = SChar.kPetInfo;

        //  펫 스킬 슬롯일 경우
        if (event.m_pWnd == m_apPetItemWnd[1])
        {
            SPetTemplate sPetTemplate = g_kGlobalValue.GetPetTemplateFromItemID(kPet.m_dwID, kPet.m_cPromotion);

            std::vector<GCITEMID> vecPetSkill;
            sPetTemplate.GetPetSkillInfo(vecPetSkill);

            int nFindIndex = -1;
            int nSlotIndex = -1;

            //  착용하고 있는 스킬을 찾아낸다.
            for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); ++i)
            {
                for (int j = 0; j < (int)vecPetSkill.size(); ++j)
                {
                    if (kPet.m_vecEquipItem[i].m_dwID == vecPetSkill[j])
                    {
                        nFindIndex = j;
                        nSlotIndex = i;
                        break;
                    }
                }

                if (nFindIndex != -1)
                    break;
            }

            // 찾아낸 스킬의 다음 스킬을 선택한다. 없다면 장착해제
            if (nFindIndex != -1)
            {
                bool bFind = false;


                int i = 0;
                //꼬마 루루 펫 처럼 GP와 CASH 구분 없이 같은 스킬 아이디를 쓰는 경우다. 이때는 다음 스킬도 똑같으므로 다다음으로 넘겨준다.
                if (vecPetSkill[0] == vecPetSkill[1])
                    i = nFindIndex + 2;
                else
                    i = nFindIndex + 1;

                for (; i < (int)vecPetSkill.size(); ++i)
                {
                    if (g_pItemMgr->FindInventoryForItemID(vecPetSkill[i]))
                    {
                        g_MyD3D->m_TempPlayer.EquipPetItem(vecPetSkill[i]);
                        bFind = true;
                        break;
                    }
                }

                if (!bFind)
                {
                    kPet.m_vecEquipItem.erase(kPet.m_vecEquipItem.begin() + nSlotIndex);
                    UnEquipPetItemForOtherChar();
                }
            }
        }
        else
        {

            for (int iPetEquipIndex = 0; iPetEquipIndex < (int)kPet.m_vecEquipItem.size(); iPetEquipIndex++)
            {
                GCItem* kPetItem = NULL;
                if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[iPetEquipIndex].m_dwUID, &kPetItem))
                {
                    // 펫 아이템이 아닌데 끼고 있거나..
                    // 장착해제 하려는 슬롯에 끼는 아이템이라면..
                    // 다른전직의 아이템을 끼고 있다면..
                    if (kPetItem->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_INVALID ||
                        kPetItem->dwPetEquipItemSlot == kItem->dwPetEquipItemSlot)
                    {
                        kPet.m_vecEquipItem.erase(kPet.m_vecEquipItem.begin() + iPetEquipIndex);
                        iPetEquipIndex--;
                        continue;
                    }
                }
                else
                {
                    kPet.m_vecEquipItem.erase(kPet.m_vecEquipItem.begin() + iPetEquipIndex);
                    iPetEquipIndex--;
                    continue;
                }
            }
            g_MyD3D->m_mapPetInfo[kPet.m_dwUID].m_vecEquipItem = kPet.m_vecEquipItem;

            UnEquipPetItemForOtherChar(); // 다른 캐릭터가 장착한 펫이 현재 캐릭터와 같다면 장착아이템도 동일하게 해제시킨다

        }
    }

    // 단축슬롯 아이템을 장착해제 하는 경우
    else if (kItem->eItemKind == GIK_SHORTCUT_SLOT)
    {
        DWORD dwItemID = kItem->dwGoodsID;
        g_MyD3D->GetMyShortCutSlot()->DischargeItem(dwItemID);
        g_MyD3D->GetMyShortCutSlot()->DischargeEmot(dwItemID);
    }

    // 펫이나 펫아이템이 아닌 아이템을 장착해제 하는 경우(!bCoordi의 의미 : 위에서 이미 해제된 코디와 같은 아이디를 가진 장비이라면 장착해제를 하지말자.)
    else if (!bCoordi)
    {
        g_pItemMgr->UnequipItemForItemUID(&g_MyD3D->m_TempPlayer.GetCurrentChar(), pkInvenItem->m_ItemUID);
    }

    g_MyD3D->m_TempPlayer.RemovePlayerObject();
    g_MyD3D->m_TempPlayer.UpdatePlayerResource(true);
    g_MyD3D->m_TempPlayer.UpdateGlobalUserInfo();

    g_pkUIScene->m_pkMyInfoScene->UpdateGameData(false, g_pkUIScene->m_pkMyInfoScene->GetEquipCategory(), m_pkScrollbar->GetScrollPos());

    return;

}

void KGCUserEquipBox::OnClickBtnMask()
{
    m_pkZeroMaskDlg->ToggleRender( !m_pkZeroMaskDlg->IsRenderOn() );
}


void KGCUserEquipBox::SetItemCategory(USER_EQUIP_CATEGORY eCategory)
{


    Clear();

    // 카테고리 변경이 있었다면
    if (m_ItemCategory != eCategory)
    {
        m_pkScrollbar->InitState(true, true, this);
        ResetScroll();
    }

    m_ItemCategory = eCategory;

    if (eCategory == UEC_EQUIP1)
    {
        for (int i = 0; i < NUM_ITEM_WND; ++i)
        {
            m_apItemWnd[i]->GetSocketWnd()->ToggleRender(true);
            switch (s_aItemCategory[UEC_EQUIP1][i])
            {
            case ESP_HELMET: m_apItemWnd[i]->GetSocketWnd()->SetIcon(KGCSocketWnd::HELMET); break;
            case ESP_JACKET: m_apItemWnd[i]->GetSocketWnd()->SetIcon(KGCSocketWnd::ARMOR); break;
            case ESP_WEAPON: m_apItemWnd[i]->GetSocketWnd()->SetIcon(KGCSocketWnd::WEAPON); break;
            default:
            {
                m_apItemWnd[i]->GetSocketWnd()->ToggleRender(false);
                break;
            }
            }

            m_apItemWnd[i]->GetSocketWnd()->ToggleRender(false);


#ifdef WEAPONCHANGE
            // 펫이었다... 무기채인지로 바꿀란다 ㅠ 
            if (i == 7)
            {
                SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();
                std::map<char, std::pair<bool, GCITEMUID> >::iterator mit = g_kGlobalValue.m_mapChangeWeapon.find(pCharInfo->iCharType);
                if (mit != g_kGlobalValue.m_mapChangeWeapon.end())
                {
                    m_apItemWnd[i]->SetWCBtn((*mit).second.first);
                    if ((*mit).second.first)
                    {
                        std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>>::iterator mit1 = g_MyD3D->m_TempPlayer.m_mapSwapWeapon.find(pCharInfo->iCharType);
                        if (mit1 != g_MyD3D->m_TempPlayer.m_mapSwapWeapon.end() && (*mit1).second.second.second != 0)
                        {
                            KItem* pkItem;
                            pkItem = g_MyD3D->m_kItemMgr.GetInventoryItem((*mit1).second.second.second);
                            if (pkItem != NULL)
                            {
                                m_apItemWnd[i]->SetItem(pkItem);
                            }
                        }
                    }
                }
                else
                {
                    m_apItemWnd[i]->SetWCBtn(false);
                }
            }
#endif
        }
    }

    if (eCategory == UEC_EQUIP2)
    {
        for (int i = 0; i < NUM_CLOSED_WND; ++i)
        {
            m_apAccessoryWnd[i]->GetSocketWnd()->ToggleRender(false);
        }
    }

    if (eCategory == UEC_LOOK)
    {
        RenderLookWnd(true);
    }

    // 장착아이템을 새로 세팅합니다.
    int iCharIndex = g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex;
    SCharInfo* pCharInfo = &g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar();
    if (pCharInfo == NULL)
        g_kGlobalValue.m_strNullFunc = __FUNCTION__;

    KUserItemVector* vecItemInfo = NULL;
    if (eCategory == UEC_LOOK)
    {
        SetItemLookCategory(eCategory, &pCharInfo->vecLookInfo);
    }
    else
    {
        SetItemEquipCategory(eCategory, &pCharInfo->vecItemInfo);
    }

    if (eCategory == UEC_SHORTCUT_SLOT)
    {
        SetItemShotcutCategory(eCategory);
    }

    SetItemPetCategory(eCategory, pCharInfo);

    ResetWindow();
}

void KGCUserEquipBox::ResetWindow()
{
    if (m_ItemCategory == UEC_PET)
    {
        m_pkScrollbar->SetScrollRangeMax(2);

        m_pkPetWnd->ToggleRender(true);
        m_pkPetInfoBox->ToggleRender(true);
        m_pkItemButton->InitState(false, false, this);
        m_pkEmoticonButton->InitState(false, false, this);
        for (int i = 5; i < NUM_CLOSED_WND; i++)
            m_pkClosedShortCutWnd[i]->ToggleRender(false);

        for (int i = 0; i < NUM_PET_WND; i++)
        {
            m_apPetItemWnd[i]->InitState(true, true, this);

#if defined( USE_SIMPLE_PET_BUY )
            // PetItem에 구입버튼  그리거나 지운다
            m_apPetItemWnd[i]->CheckIsItemRedraw();
#endif 

        }
    }
    else if (m_ItemCategory == UEC_SHORTCUT_SLOT)
    {
        m_pkScrollbar->SetScrollRangeMax(3);
        m_pkPetWnd->ToggleRender(false);
        m_pkPetInfoBox->ToggleRender(false);
#if defined( EMOTICON_SYSTEM )
        m_pkItemButton->InitState(true, true, this);
        m_pkEmoticonButton->InitState(true, true, this);
        // 		m_pkItemButton->Activate(true);
        // 		m_pkEmoticonButton->Activate(false);
#else
        m_pkItemButton->InitState(false, true, this);
        m_pkEmoticonButton->InitState(false, true, this);
#endif
        for (int i = 0; i < ITEM_SLOT_SIZE; i++)
            m_apShortCutWnd[i]->InitState(true, true, this);
    }
    else
    {

        switch (m_ItemCategory)
        {
        case UEC_EQUIP1:
        case UEC_EQUIP2:
            m_pkScrollbar->SetScrollRangeMax(2);
            break;
        case UEC_SKILL:
        case UEC_SKILL_WHITE:
        case UEC_SKILL_BLACK:
            m_pkScrollbar->ToggleRender(true);
            m_pkScrollbar->SetScrollRangeMax(3);
            m_pkScrollbarBack->ToggleRender(true);
            break;
        }
        m_pkPetWnd->ToggleRender(false);
        m_pkPetInfoBox->ToggleRender(false);
        m_pkItemButton->InitState(false, false, this);
        m_pkEmoticonButton->InitState(false, false, this);
        for (int i = 5; i < NUM_CLOSED_WND; i++)
            m_pkClosedShortCutWnd[i]->ToggleRender(false);

        int iStart = 0;
        if (m_ItemCategory == UEC_SHORTCUT_SLOT || m_ItemCategory == UEC_EQUIP2)
            iStart = m_iScrollTop * 5;
        else
            iStart = m_iScrollTop * 4;

        int iMaxWnd = NUM_ITEM_WND;
        if (m_ItemCategory == UEC_EQUIP2)
            iMaxWnd = NUM_CLOSED_WND;

        for (int i = iStart; i < iStart + iMaxWnd; ++i)
        {
            switch (m_ItemCategory)
            {
            case UEC_EQUIP1:
                m_apItemWnd[i]->InitState(true, true, this);
                //m_apItemWnd[i]->SetWindowPosDirect( m_apWnd[i-iStart]->GetFixedWindowLocalPos() );
                break;
            case UEC_EQUIP2:
                m_apAccessoryWnd[i]->InitState(true, true, this);
                //m_apAccessoryWnd[i]->SetWindowPosDirect( m_apWnd[i-iStart]->GetFixedWindowLocalPos() );
                break;
            case UEC_SKILL:
            case UEC_SKILL_WHITE:
            case UEC_SKILL_BLACK:
                m_apSkillWnd[i]->InitState(true, true, this);
                m_apSkillWnd[i]->SetWindowPosDirect(m_apWnd[i - iStart]->GetFixedWindowLocalPos());
                for (int j = 0; j < NUM_ITEM_WND; j++)
                {
                    m_pkClosedNomalItemWnd[j]->SetFixedWindowLocalPos(m_apSkillWnd[j]->GetFixedWindowLocalPos() + D3DXVECTOR2(1, 1));
                }
                break;
            }
        }
    }

    if ( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType == GC_CHAR_ZERO && m_ItemCategory == UEC_LOOK )
        m_pkBtnMaskSelect->InitState( true, true, this );
    else
        m_pkBtnMaskSelect->InitState( false, true, this );
}

bool KGCUserEquipBox::GetCurrentHoveredItem(GCItem*& pItem, KItem*& pkInventoryItem, D3DXVECTOR2& vecPos)
{
    if (IsLocked())
        return  false;

    if (m_ItemCategory == UEC_EQUIP1)
    {
        return InitSlotInfo(pItem, pkInventoryItem, vecPos, m_apItemWnd);
    }
    else if (m_ItemCategory == UEC_EQUIP2)
    {
        return InitSlotInfo(pItem, pkInventoryItem, vecPos, m_apAccessoryWnd);
    }
    else if (m_ItemCategory == UEC_SHORTCUT_SLOT)
    {
        return InitSlotInfo(pItem, pkInventoryItem, vecPos, m_apShortCutWnd);
    }
    else if (m_ItemCategory == UEC_PET)
    {
        return InitSlotInfo(pItem, pkInventoryItem, vecPos, m_apPetItemWnd);
    }
    else if (m_ItemCategory == UEC_LOOK)
    {
        return InitSlotInfo(pItem, pkInventoryItem, vecPos, m_apLookWnd);
    }
    else
    {
        return InitSlotInfo(pItem, pkInventoryItem, vecPos, m_apSkillWnd);
    }
}

void KGCUserEquipBox::LockEquipment(bool bLock)
{
    Lock(bLock);

    switch (m_ItemCategory)
    {
    case UEC_SHORTCUT_SLOT:
        for (int i = 0; i < NUM_ITEM_WND; ++i)
        {
            m_pkClosedNomalItemWnd[i]->ToggleRender(false);
            m_pkClosedapItemWnd[i]->ToggleRender(false);
        }
        for (int i = 0; i < NUM_CLOSED_WND; ++i)
        {
            m_apCloseAcceWnd[i]->ToggleRender(false);
            if (i >= NUM_CLOSED_WND / 2 && bLock == false && m_pkScrollbar->GetScrollPos() == 0)

            {
                m_pkClosedShortCutWnd[i]->ToggleRender(true);

            }
            else
            {
                m_pkClosedShortCutWnd[i]->ToggleRender(bLock);
            }
        }
        break;
    case UEC_EQUIP2:
        for (int i = 0; i < NUM_CLOSED_WND; ++i)
        {
            m_pkClosedShortCutWnd[i]->ToggleRender(false);
            m_apCloseAcceWnd[i]->ToggleRender(bLock);
        }
        for (int i = 0; i < NUM_ITEM_WND; ++i)
        {
            m_pkClosedNomalItemWnd[i]->ToggleRender(false);
            m_pkClosedapItemWnd[i]->ToggleRender(false);
        }
        break;
    case UEC_EQUIP1:
        for (int i = 0; i < NUM_CLOSED_WND; ++i)
        {
            m_pkClosedShortCutWnd[i]->ToggleRender(false);
            m_apCloseAcceWnd[i]->ToggleRender(false);
        }
        for (int i = 0; i < NUM_ITEM_WND; ++i)
        {
            m_pkClosedNomalItemWnd[i]->ToggleRender(false);
            m_pkClosedapItemWnd[i]->ToggleRender(bLock);
        }
        break;
    case UEC_SKILL:
    case UEC_SKILL_WHITE:
    case UEC_SKILL_BLACK:
        for (int i = 0; i < NUM_CLOSED_WND; ++i)
        {
            m_pkClosedShortCutWnd[i]->ToggleRender(false);
            m_apCloseAcceWnd[i]->ToggleRender(false);
        }
        for (int i = 0; i < NUM_ITEM_WND; ++i)
        {
            m_pkClosedapItemWnd[i]->ToggleRender(false);
            m_pkClosedNomalItemWnd[i]->ToggleRender(bLock);
        }
        break;
    case UEC_PET:
    default:
        break;
    }
}

int KGCUserEquipBox::GetCurItemSlot()
{
    return m_pkScrollbar->GetScrollPos();
}

void KGCUserEquipBox::RenderAccessoryWnd(bool bRender)
{
    if (GetCurrentCategory() == UEC_EQUIP2)
    {
        for (int i = 0; i < KGCUserEquipBox::NUM_CLOSED_WND; i++)
            m_apAccessoryWnd[i]->ToggleRender(bRender);
    }
}

void KGCUserEquipBox::RenderLookWnd(bool bRender)
{
    if (GetCurrentCategory() != UEC_LOOK)
        return;

    if (bRender)
    {
        if (m_pkBtnCoordiCompose)
            m_pkBtnCoordiCompose->InitState(true, true, this);
#if !defined(NATION_CHINA)
        if (m_pkBtnLookChang)
            m_pkBtnLookChang->InitState(true, true, this);
#endif
        if (m_pkBtnCoordiCompose && GC_GLOBAL_DEFINE::bCoordiCompose)
            m_pkBtnCoordiCompose->InitState(true, true, this);

        if (m_pkBtnCoordiUpgrade && GC_GLOBAL_DEFINE::bCoordiUpgrade)
            m_pkBtnCoordiUpgrade->InitState(true, true, this);

        for (int i = 0; i < m_nLookWnd; ++i) {
            if (m_apLookWnd[i] == NULL)
                break;

            m_apLookWnd[i]->InitState(true, true, this);
            m_apLookWnd[i]->SetSelect(false);

        }

        //  무기 슬롯 다시 처리 해준다.
        if (GC_GLOBAL_DEFINE::bWeaponCoordi)
        {
            if (!g_MyD3D->m_TempPlayer.IsOneWeaponChar())
            {
                m_apLookWnd[COORDI_PARTS_WEAPON]->InitState(false, true, this);
            }
            else
            {
                for (int i = NUM_LOOK_WND; i < NUM_LOOK_WND_EX; ++i)
                {
                    if (m_apLookWnd[i] == NULL)
                        break;

                    m_apLookWnd[i]->InitState(false, true, this);
                }
            }
        }
    }
    else
    {

        for (int i = 0; i < m_nLookWnd; ++i)
        {
            if (m_apLookWnd[i] == NULL)
                break;

            m_apLookWnd[i]->InitState(false);
        }

    }
}

void KGCUserEquipBox::SetItemLookCategory(USER_EQUIP_CATEGORY eCategory, KUserItemVector* vecLookInfo)
{
    GCItem* kGCItem = NULL;
    KItem* kInventoryItem = NULL;

    m_pkScrollbar->InitState(false, false, this);
    m_pkScrollbarBack->ToggleRender(false);

    if (!vecLookInfo)
        return;

    int nJobLevel = g_MyD3D->m_TempPlayer.GetWeaponJobLevel();

    for (int j = 0; j < (int)(*vecLookInfo).size(); ++j)
    {
        // 초기화
        kGCItem = NULL;
        kInventoryItem = NULL;
        if (!g_MyD3D->m_kItemMgr.FindInventory((*vecLookInfo)[j].dwItemUID, &kInventoryItem, &kGCItem))
            continue;

        for (int i = 0; i < NUM_LOOK_CATEGORY; ++i)
        {
            if (eCategory == UEC_LOOK)
            {
                if (kGCItem == NULL)
                    continue;

                if (kGCItem->dwSlotPosition & s_aItemCategory[eCategory][i])
                {
                    //  무기일때 전직용 무기 처리해준다.
                    if (kGCItem->dwSlotPosition & ESP_WEAPON)
                    {
                        if (!g_MyD3D->m_TempPlayer.IsOneWeaponChar())
                        {
                            int nIndex = i + kGCItem->iNeedChangeJobLevelForEquippingItem + 1;

                            if (nIndex >= m_nLookWnd)
                                continue;

                            m_apLookWnd[nIndex]->SetItem(kInventoryItem);

                            if (nJobLevel == kGCItem->iNeedChangeJobLevelForEquippingItem)
                            {
                                m_apLookWnd[nIndex]->SetSelect(true);
                            }

                        }
                        else
                        {
                            m_apLookWnd[i]->SetItem(kInventoryItem);
                        }
                    }
                    else
                    {
                        m_apLookWnd[i]->SetItem(kInventoryItem);
                    }
                }
            }
        }
    }
}

void KGCUserEquipBox::SetItemEquipCategory(USER_EQUIP_CATEGORY eCategory, KUserItemVector* vecItemInfo)
{
    m_pkScrollbar->InitState(false, false, this);
    m_pkScrollbarBack->ToggleRender(false);

    if (!vecItemInfo)
        return;

    GCItem* kGCItem = NULL;
    KItem* kInventoryItem = NULL;
    for (int j = 0; j < (int)(*vecItemInfo).size(); ++j)
    {
        // 초기화
        kGCItem = NULL;
        kInventoryItem = NULL;
        if (!g_MyD3D->m_kItemMgr.FindInventory((*vecItemInfo)[j].dwItemUID, &kInventoryItem, &kGCItem))
            continue;

        if (NULL == kGCItem) {
            continue;
        }

        for (int i = 0; i < NUM_CATEGORY; ++i)
        {
            if (eCategory < UEC_PET)
            {
                if (kGCItem->dwSlotPosition & s_aItemCategory[eCategory][i])
                {
                    if (eCategory == UEC_EQUIP1)
                    {
                        // Asirion : 2007-12-17
                        // 요놈들을 카테고리로 빼기도 뭐하고 해서 이렇게 처리 한다. Sorry!!                        
                        if (kGCItem->dwSlotPosition & ESP_HELMET_SOCKET ||
                            kGCItem->dwSlotPosition & ESP_JACKET_SOCKET ||
                            kGCItem->dwSlotPosition & ESP_WEAPON_SOCKET)
                        {
                            int iSocketIndex = 0;
                            switch (s_aItemCategory[eCategory][i])
                            {
                            case ESP_A_HELMET_SOCKET: iSocketIndex = 0; break;
                            case ESP_JACKET_SOCKET: iSocketIndex = 1; break;
                            case ESP_WEAPON_SOCKET: iSocketIndex = 6; break;
                            }
                            m_apItemWnd[iSocketIndex]->SetSocketItem(kInventoryItem);
                        }
                        else
                        {
                            m_apItemWnd[i]->SetItem(kInventoryItem);
                        }
                    }
                    else if (eCategory == UEC_EQUIP2)
                    {
                        if (kGCItem->dwSlotPosition & ESP_A_HELMET_SOCKET)
                        {
                            m_apAccessoryWnd[0]->SetSocketItem(kInventoryItem);
                        }
                        else if (kGCItem->dwSlotPosition & ESP_A_EARRING)
                        {
                            if (m_apAccessoryWnd[8]->GetItemID() != -1 && i == 8)
                                continue;

                            if (m_apAccessoryWnd[8]->GetItemID() == kInventoryItem->m_ItemID)
                                continue;

                            m_apAccessoryWnd[i]->SetItem(kInventoryItem);
                        }
                        else
                        {
                            m_apAccessoryWnd[i]->SetItem(kInventoryItem);
                        }

                    }
                }
            }
            else if (eCategory == UEC_PET)
            {
                // vecItemInfo 에서 펫 아이템 & 펫장착 아이템 을 저장하지 않으므로
                // 앞으로 여기로 들어올 일은 없을것이다
                if (kGCItem->dwPetEquipItemSlot & s_aItemCategory[eCategory][i])
                {
                    m_apPetItemWnd[i]->SetItem(kInventoryItem);
                }
            }
            else
            {
                if (kGCItem->dwSkillPosition & s_aItemCategory[eCategory][i])
                {
                    if (eCategory == UEC_SKILL || eCategory == UEC_SKILL_WHITE || eCategory == UEC_SKILL_BLACK)
                    {
                        m_apSkillWnd[i]->SetItem(kInventoryItem);
                    }
                }
            }
        }
    }
}

void KGCUserEquipBox::SetItemShotcutCategory(USER_EQUIP_CATEGORY eCategory)
{
    // 단축 슬롯 : 장착정보가 vecItemInfo에 저장되지 않으므로 밖으로 뺌
    m_pkScrollbar->InitState(false, false, this);
    m_pkScrollbarBack->ToggleRender(false);

    // 포션이면
    if (m_pkScrollbar->GetScrollPos() == 0)
    {
        for (int i = 0; i < NUM_POTION_WND; i++)
            m_apShortCutWnd[i]->InitState(true, true, this);
        for (int i = NUM_POTION_WND; i < NUM_CLOSED_WND; i++)
            m_pkClosedShortCutWnd[i]->ToggleRender(true);

        for (int index = 0; index < ITEM_SLOT_SIZE; index++)
        {
            m_apShortCutWnd[index]->SetItem(index, true);
        }

        m_pkItemButton->Activate(true);
        m_pkEmoticonButton->Activate(false);
    }
    //이모티콘이면
    if (m_pkScrollbar->GetScrollPos() == 1)
    {
        for (int i = NUM_POTION_WND; i < NUM_SHORTCUT_WND; i++)
        {
            m_apShortCutWnd[i]->InitState(true, true, this);
            m_apShortCutWnd[i]->SetItem(i, false);
        }
        for (int i = 0; i < NUM_CLOSED_WND; i++)
        {
            m_pkClosedShortCutWnd[i]->ToggleRender(false);
        }

        m_pkItemButton->Activate(false);
        m_pkEmoticonButton->Activate(true);
    }
}

void KGCUserEquipBox::SetItemPetCategory(USER_EQUIP_CATEGORY eCategory, SCharInfo* pCharInfo)
{
    m_pkScrollbar->InitState(false, false, this);
    m_pkScrollbarBack->ToggleRender(false);

    if (!pCharInfo)
        return;

    GCItem* kGCItem = NULL;
    KItem* kInventoryItem = NULL;

    // 펫을 장착 합니다
    KPetInfo& kPet = pCharInfo->kPetInfo;
    GCITEMUID dwPetUID = kPet.m_dwUID;
    if (dwPetUID != 0)
    {
#if !defined(WEAPONCHANGE)
        // 펫 정보가 존재하고
        if (g_MyD3D->IsExistPetInfo(dwPetUID))
        {
            kGCItem = NULL;
            kInventoryItem = NULL;
            // 인벤토리에서 찾고
            if (g_pItemMgr->FindInventory(dwPetUID, &kInventoryItem, &kGCItem))
            {
                // 현재 카테고리가 [장비] 일때
                if (eCategory == UEC_EQUIP1)
                {
                    // 카테고리를 돌면서 슬롯포지션이 일치하는 녀석을찾는다
                    for (int i = 0; i < NUM_CATEGORY; ++i)
                    {
                        if (kGCItem->dwSlotPosition & s_aItemCategory[eCategory][i])
                        {
                            // 아이템 윈도우 펫 위치에 펫을 세팅한다
                            m_apItemWnd[i]->SetItem(kInventoryItem);
                        }
                    }
                }
            }
            else
            {
                kPet.Init();
            }
        }
        else
        {
            kPet.Init();
        }
#endif    
        // 펫장착 아이템을 장착합니다
        for (int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++)
        {
            kGCItem = NULL;
            kInventoryItem = NULL;
            // 인벤토리에서 찾고
            if (g_pItemMgr->FindInventory(kPet.m_vecEquipItem[i].m_dwUID, &kInventoryItem, &kGCItem))
            {



                // 현재 카테고리가 [펫] 일때
                if (eCategory == UEC_PET)
                {
                    // 카테고리를 돌면서 슬롯에 맞는 녀석을 찾는다
                    for (int i = 0; i < NUM_CATEGORY; ++i)
                    {
                        if (kGCItem->dwPetEquipItemSlot & s_aItemCategory[eCategory][i])
                        {
                            // 슬롯에 맞게 세팅한다
                            m_apPetItemWnd[i]->SetItem(kInventoryItem);
                            m_pkPetInfoBox->OneFrameMove();
                        }
                    }
                }
            }
            else
            {
                // 인벤토리에 펫아이템이 없다면 삭제
                kPet.m_vecEquipItem.erase(kPet.m_vecEquipItem.begin() + i);
                --i;
            }
        }
    }
}

void KGCUserEquipBox::OnClickBtnChange()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_COORDI_CHANGE_DLG);
}

void KGCUserEquipBox::OnClickBtnCompose()
{
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_COORDI_COMPOSE_DLG);
}

void KGCUserEquipBox::OnClickBtnUpgrade()
{
    g_pItemMgr->CoordiGradeUpgradeInfoReq();
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_COORDI_UPGRADE_DLG);
}

void KGCUserEquipBox::UnEquipPetItemForOtherChar(void)			// 모든 캐릭터 펫 해제 기존 로직->함수로  장원형
{
    SCharInfo& SChar = g_MyD3D->m_TempPlayer.GetCurrentChar();
    KPetInfo& kPet = SChar.kPetInfo;

    g_MyD3D->m_mapPetInfo[kPet.m_dwUID].m_vecEquipItem = kPet.m_vecEquipItem;

    // 다른 캐릭터가 장착한 펫이 현재 캐릭터와 같다면 장착아이템도 동일하게 해제시킨다
    for (int iCharIndex = 0; iCharIndex < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); iCharIndex++)
    {
        SCharInfo& SChar_ = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[iCharIndex];
        KPetInfo& kPet_ = SChar_.kPetInfo;

        if (SChar.iCharType == SChar_.iCharType)
            continue;

        if (kPet.m_dwUID == kPet_.m_dwUID)
        {
            kPet_.m_vecEquipItem = g_MyD3D->m_mapPetInfo[kPet_.m_dwUID].m_vecEquipItem;
        }
    }

}
