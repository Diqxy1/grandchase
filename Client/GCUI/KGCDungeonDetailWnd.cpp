#include "stdafx.h"
#include "KGCDungeonDetailWnd.h"

IMPLEMENT_CLASSNAME( KGCDungeonDetailWnd );
IMPLEMENT_WND_FACTORY( KGCDungeonDetailWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonDetailWnd, "gc_dungeon_detail_wnd" );

KGCDungeonDetailWnd::KGCDungeonDetailWnd( void )
: m_pkDesc1(NULL)
, m_pkDesc2(NULL)
, m_pkValue1(NULL)
, m_pkValue2(NULL)
{
    m_mapListIndexAutoDivideStringDesc1.clear();
    m_mapListIndexAutoDivideStringDesc2.clear();

    LINK_CONTROL("detail_desc1", m_pkDesc1 );
    LINK_CONTROL("detail_desc2", m_pkDesc2 );
    LINK_CONTROL("detail_value1", m_pkValue1 );
    LINK_CONTROL("detail_value2", m_pkValue2 );
}

KGCDungeonDetailWnd::~KGCDungeonDetailWnd( void )
{
}

void KGCDungeonDetailWnd::OnCreate()
{
    m_pkDesc1->InitState(true, false, this );
    m_pkDesc2->InitState(true, false, this);
    m_pkValue1->InitState(true, false, this);
    m_pkValue2->InitState(true, false, this);

    InitializeListBox();
    SetDescString();
    SetValueString();

    m_pkDesc1->SetCheckList(true);
    m_pkDesc2->SetCheckList(true);
    m_pkValue1->SetCheckList(true);
    m_pkValue2->SetCheckList(true);
}

void KGCDungeonDetailWnd::ActionPerformed(const KActionEvent& event )
{

}

void KGCDungeonDetailWnd::FrameMoveInEnabledState()
{

}

void KGCDungeonDetailWnd::InitializeListBox()
{
    m_pkDesc1->Clear();
    m_pkDesc1->SetFontColor(0xffffffff);
    m_pkDesc2->Clear();
    m_pkDesc2->SetFontColor(0xffffffff);
    m_pkValue1->Clear();
    m_pkValue1->SetFontColor(0xffffffff);
    m_pkValue2->Clear();
    m_pkValue2->SetFontColor(0xffffffff);

    m_pkDesc1->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkDesc1->SetShadow(true);
    m_pkDesc1->SetLineSpace(2.0f);
    m_pkDesc2->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkDesc2->SetShadow(true);
    m_pkDesc2->SetLineSpace(2.0f);
    m_pkValue1->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkValue1->SetShadow(true);
    m_pkValue1->SetLineSpace(2.0f);
    m_pkValue2->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkValue2->SetShadow(true);
    m_pkValue2->SetLineSpace(2.0f);
}

void KGCDungeonDetailWnd::SetDescString()
{
    int index = 0;
    int AutoDevideStringCnt = 0;

    m_pkDesc1->AddString( g_pkStrLoader->GetString(STD_ID_MAX_COMBO_NUM) );
    SetListAutoDevideStringCntDesc1(index, AutoDevideStringCnt);
    m_pkDesc1->AddString( g_pkStrLoader->GetString(STD_ID_MAX_COMBO_DAMAGE) );
    SetListAutoDevideStringCntDesc1(index, AutoDevideStringCnt);
    m_pkDesc1->AddString( g_pkStrLoader->GetString(STD_ID_CONTINOUS_KILL_NUM) );
    SetListAutoDevideStringCntDesc1(index, AutoDevideStringCnt);

    index = 0;
    AutoDevideStringCnt = 0;

    m_pkDesc2->AddString( g_pkStrLoader->GetString(STD_ID_BACK_AERIAL_ATTACK_NUM) );
    SetListAutoDevideStringCntDesc2(index, AutoDevideStringCnt);
    m_pkDesc2->AddString( g_pkStrLoader->GetString(STD_ID_HITTED_BY_MONSTER) );
    SetListAutoDevideStringCntDesc2(index, AutoDevideStringCnt);

    if ( g_pkQuestManager->GetQuestStartUserCnt() > 1) {
        m_pkDesc2->AddString( g_pkStrLoader->GetString(STD_ID_DUNGEON_DAMAGE_CONTRIBUTION) );
        SetListAutoDevideStringCntDesc2(index, AutoDevideStringCnt);
    }
}

void KGCDungeonDetailWnd::SetValueString()
{
    m_pkValue1->Clear();
    m_pkValue2->Clear();

    int iMaxCombo =0;
    int iMaxComboDamage = 0;
    int iContributionPercent = 0;
    int iTotalDamageFromMonster = 0;
    int iTotalBackAerial = 0 ;
    int iMaxKillCombo =0;

    g_pkQuestManager->GetDungeonDetailInfo( iMaxCombo , iMaxComboDamage, iContributionPercent, iTotalDamageFromMonster, iTotalBackAerial, iMaxKillCombo);

    std::wstring wstrMaxCombo = g_pkStrLoader->GetReplacedString( STR_ID_NUM_REPLACE, "i", iMaxCombo);
    std::wstring wstrMaxComboDamage = g_pkStrLoader->GetReplacedString( STR_ID_NUM_REPLACE, "i", iMaxComboDamage);
    std::wstring wstrContributionPercent = g_pkStrLoader->GetReplacedString( STR_ID_SOCKS_PERCENT, "i", iContributionPercent);
    std::wstring wstrTotalDamageFromMonster = g_pkStrLoader->GetReplacedString( STR_ID_NUM_REPLACE, "i", iTotalDamageFromMonster);
    std::wstring wstrTotalBackAerial = g_pkStrLoader->GetReplacedString( STR_ID_NUM_REPLACE, "i", iTotalBackAerial);
    std::wstring wstrMaxKillCombo = g_pkStrLoader->GetReplacedString( STR_ID_NUM_REPLACE, "i", iMaxKillCombo);

    m_pkValue1->AddString( wstrMaxCombo );
    SetListAutoDevideStringValue1( 0 );
    m_pkValue1->AddString( wstrMaxComboDamage );
    SetListAutoDevideStringValue1( 1 );
    m_pkValue1->AddString( wstrMaxKillCombo );
    SetListAutoDevideStringValue1( 2 );

    m_pkValue2->AddString( wstrTotalBackAerial );
    SetListAutoDevideStringValue2( 0 );
    m_pkValue2->AddString( wstrTotalDamageFromMonster );
    SetListAutoDevideStringValue2( 1 );
    if ( g_pkQuestManager->GetQuestStartUserCnt() > 1) {
        m_pkValue2->AddString( wstrContributionPercent );
        SetListAutoDevideStringValue2( 2 );
    }
}

void KGCDungeonDetailWnd::SetListAutoDevideStringCntDesc1( int &index, int &AutoDevideStringCnt)
{
    index++;
    m_mapListIndexAutoDivideStringDesc1.insert(std::pair<int,int>(index - 1, m_pkDesc1->GetCount() - (index + AutoDevideStringCnt)));
    AutoDevideStringCnt += m_pkDesc1->GetCount() - (index + AutoDevideStringCnt);
}

void KGCDungeonDetailWnd::SetListAutoDevideStringCntDesc2( int &index, int &AutoDevideStringCnt)
{
    index++;
    m_mapListIndexAutoDivideStringDesc2.insert(std::pair<int,int>(index - 1, m_pkDesc2->GetCount() - (index + AutoDevideStringCnt)));
    AutoDevideStringCnt += m_pkDesc2->GetCount() - (index + AutoDevideStringCnt);
}


void KGCDungeonDetailWnd::SetListAutoDevideStringValue1( int index)
{
    std::map<int,int>::iterator mit = m_mapListIndexAutoDivideStringDesc1.find(index);

    if(mit != m_mapListIndexAutoDivideStringDesc1.end())
    {
        for(int i = 0; i < mit->second ; i++)
        {
            m_pkValue1->AddString(L" ");
        }
    }
}

void KGCDungeonDetailWnd::SetListAutoDevideStringValue2( int index)
{
    std::map<int,int>::iterator mit = m_mapListIndexAutoDivideStringDesc2.find(index);

    if(mit != m_mapListIndexAutoDivideStringDesc2.end())
    {
        for(int i = 0; i < mit->second ; i++)
        {
            m_pkValue2->AddString(L" ");
        }
    }
}
