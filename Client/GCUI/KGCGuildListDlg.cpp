#include "StdAfx.h"
#include "KGCGuildListDlg.h"
#include "KGCGuildListInfo.h"
#include <boost/tokenizer.hpp>


IMPLEMENT_CLASSNAME(KGCGuildListDlg);
IMPLEMENT_WND_FACTORY(KGCGuildListDlg);
IMPLEMENT_WND_FACTORY_NAME(KGCGuildListDlg, "gc_guild_list_dlg");

KGCGuildListDlg::KGCGuildListDlg(void)
	:m_pkBtnClose(NULL)
	, m_pkBtnGuildList(NULL)
	, m_pkBtnGuildCreate(NULL)
	, m_pkBtnSearch(NULL)
	, m_pkBtnJoinCancel(NULL)
	, m_pkBtnPrevPage(NULL)
	, m_pkBtnNextPage(NULL)
	, m_pkBtnJoinReq(NULL)
	, m_pkEditSearch(NULL)
	, m_pkStaticGuildListPage(NULL)
	, m_pkStaticMyGuildName(NULL)
	, m_pkStaticGuildName(NULL)
	, m_pkStaticGuildGrade(NULL)
	, m_pkStaticGuildMasterName(NULL)
	, m_pkStaticGuildBithday(NULL)
	, m_pkStaticGuildURL(NULL)
	, m_pkStaticGuildIntroduce(NULL)
	, m_pkStaticGuildPoint(NULL)
	, m_pkStaticGuildMemberNum(NULL)
	, m_pkStaticGuildTotalRanking(NULL)
	, m_pkStaticGuildMonthRanking(NULL)
	, m_pkStaticGuildWeekRanking(NULL)
	, m_pkBtnSortingGuild(NULL)
	, m_pkBtnSortingGuildGrade(NULL)
	, m_pkBtnSortingGuildBirthday(NULL)
	, m_pkStaticTitleGuildIntroduce(NULL)
	, m_pkStaticTitleGuildInfo(NULL)
	, m_pkListHoverBG(NULL)
	, m_pkStaticGuildJoinstate(NULL)
	, m_pkStaticGuildJoinCondition(NULL)
	, m_pkGuildMarkIcon(NULL)
	, m_pkGuildSearchingBlind(NULL)
	, m_iCurPage(1)
	, m_iMaxPage(100)
	, m_bIsPressedBtn(false)
	, m_ucSortType(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_NAME)
	, m_iPrevIndex(-1)
	, m_fListHoverBoxPosY(0.f)
	, m_iMaxListNum(0)
	, m_iSelectGuildUID(0)
	, m_iStratSearch(-1)
{
	char str[MAX_PATH] = { 0, };
	for (int i = 0; i < MAX_GUILD_LIST_NUM; ++i)
	{
		m_pkGuildListInfo[i] = NULL;
		sprintf_s(str, "guild_list_info%d", i);
		LINK_CONTROL(str, m_pkGuildListInfo[i]);
	}

	for (int i = 0; i < MAX_SORTING_BTN; i++)
	{
		m_pkSortArrowUp[i] = NULL;
		sprintf_s(str, "btn_sort_up%d", i);
		LINK_CONTROL(str, m_pkSortArrowUp[i]);

		m_pkSortArrowDown[i] = NULL;
		sprintf_s(str, "btn_sort_down%d", i);
		LINK_CONTROL(str, m_pkSortArrowDown[i]);
	}

	LINK_CONTROL("close_btn", m_pkBtnClose);
	LINK_CONTROL("btn_guild_list", m_pkBtnGuildList);
	LINK_CONTROL("btn_create _guild", m_pkBtnGuildCreate);
	LINK_CONTROL("btn_guild_serch", m_pkBtnSearch);
	LINK_CONTROL("btn_guild_join_cancel", m_pkBtnJoinCancel);
	LINK_CONTROL("btn_prev_guild_list", m_pkBtnPrevPage);
	LINK_CONTROL("btn_next_guild_list", m_pkBtnNextPage);
	LINK_CONTROL("btn_sorting_guild", m_pkBtnSortingGuild);
	LINK_CONTROL("btn_sorting_guild_grade", m_pkBtnSortingGuildGrade);
	LINK_CONTROL("btn_sorting_guild_birthday", m_pkBtnSortingGuildBirthday);
	LINK_CONTROL("btn_guild_join_application", m_pkBtnJoinReq);

	LINK_CONTROL("edit_guild_serch", m_pkEditSearch);
	LINK_CONTROL("static_guild_list_page", m_pkStaticGuildListPage);
	LINK_CONTROL("static_my_guild_name", m_pkStaticMyGuildName);
	LINK_CONTROL("static_guild_join_state", m_pkStaticGuildJoinstate);

	LINK_CONTROL("static_guild_name", m_pkStaticGuildName);
	LINK_CONTROL("static_guild_grade", m_pkStaticGuildGrade);
	LINK_CONTROL("static_guild_master_name", m_pkStaticGuildMasterName);
	LINK_CONTROL("static_guild_birthday", m_pkStaticGuildBithday);
	LINK_CONTROL("static_guild_url", m_pkStaticGuildURL);
	LINK_CONTROL("static_guild_introduce", m_pkStaticGuildIntroduce);
	LINK_CONTROL("static_guild_point", m_pkStaticGuildPoint);
	LINK_CONTROL("static_guild_member_num", m_pkStaticGuildMemberNum);
	LINK_CONTROL("static_guild_total_ranking", m_pkStaticGuildTotalRanking);
	LINK_CONTROL("static_guild_month_ranking", m_pkStaticGuildMonthRanking);
	LINK_CONTROL("static_guild_week_ranking", m_pkStaticGuildWeekRanking);

	LINK_CONTROL("static_title_guild_introduce", m_pkStaticTitleGuildIntroduce);
	LINK_CONTROL("static_title_guild_info", m_pkStaticTitleGuildInfo);

	LINK_CONTROL("static_guild_join_condition", m_pkStaticGuildJoinCondition);

	LINK_CONTROL("bg_list_hover", m_pkListHoverBG);

	LINK_CONTROL("guild_mark_icon", m_pkGuildMarkIcon);

	LINK_CONTROL("guild_searching_blind", m_pkGuildSearchingBlind);

	m_vecDetalGuildInfo.clear();
}

KGCGuildListDlg::~KGCGuildListDlg(void)
{
}


void KGCGuildListDlg::OnCreate(void)
{
	for (int i = 0; i < MAX_GUILD_LIST_NUM; i++)
	{
		m_pkGuildListInfo[i]->InitState(true, true, this);
	}

	for (int i = 0; i < MAX_SORTING_BTN; i++)
	{
		m_pkSortArrowUp[i]->InitState(false);
		m_pkSortArrowDown[i]->InitState(false);
	}

	m_pkBtnClose->InitState(true, true, this);
	m_pkBtnGuildList->InitState(true, true, this);
	m_pkBtnGuildCreate->InitState(true, true, this);
	m_pkBtnSearch->InitState(true, true, this);
	m_pkBtnJoinCancel->InitState(false, true, this);
	m_pkBtnPrevPage->InitState(true, true, this);
	m_pkBtnNextPage->InitState(true, true, this);
	m_pkBtnSortingGuild->InitState(true, true, this);
	m_pkBtnSortingGuildGrade->InitState(true, true, this);
	m_pkBtnSortingGuildBirthday->InitState(true, true, this);
	m_pkBtnJoinReq->InitState(true, true, this);
	m_pkEditSearch->InitState(true, false, this);
	m_pkEditSearch->SetText(L"");
	m_pkEditSearch->SetFocus();
	m_pkEditSearch->SetLimitText(MAX_SEARCH_WORD_LENGTH);
	m_pkStaticGuildListPage->InitState(true);
	m_pkStaticGuildListPage->SetAlign(DT_CENTER);
	m_pkStaticMyGuildName->InitState(true);

	m_pkStaticGuildName->InitState(true);
	m_pkStaticGuildGrade->InitState(true);
	m_pkStaticGuildMasterName->InitState(true);
	m_pkStaticGuildBithday->InitState(true);
	m_pkStaticGuildURL->InitState(true);
	m_pkStaticGuildURL->SetWordWrap(true);
	m_pkStaticGuildIntroduce->InitState(true);
	m_pkStaticGuildIntroduce->SetWordWrap(true);
	m_pkStaticGuildIntroduce->SetMultiLine(true);
	m_pkStaticGuildIntroduce->SetFontColor(D3DCOLOR_RGBA(184, 134, 11, 255));
	m_pkStaticGuildPoint->InitState(true);
	m_pkStaticGuildPoint->SetShadowColor(0xff623e00);
	m_pkStaticGuildMemberNum->InitState(true);
	m_pkStaticGuildMemberNum->SetShadowColor(0xff623e00);
	m_pkStaticGuildTotalRanking->InitState(true);
	m_pkStaticGuildTotalRanking->SetShadowColor(0xff623e00);
	m_pkStaticGuildMonthRanking->InitState(true);
	m_pkStaticGuildMonthRanking->SetShadowColor(0xff623e00);
	m_pkStaticGuildWeekRanking->InitState(true);
	m_pkStaticGuildWeekRanking->SetShadowColor(0xff623e00);

	m_pkStaticTitleGuildIntroduce->InitState(true);
	m_pkStaticTitleGuildIntroduce->SetAlign(DT_CENTER);
	m_pkStaticTitleGuildIntroduce->SetFontColor(D3DCOLOR_RGBA(184, 134, 11, 255));
	m_pkStaticTitleGuildIntroduce->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_INTRODUCE_TITLE));
	m_pkStaticTitleGuildInfo->InitState(true);
	m_pkStaticTitleGuildInfo->SetAlign(DT_CENTER);
	m_pkStaticTitleGuildInfo->SetFontColor(D3DCOLOR_RGBA(184, 134, 11, 255));
	m_pkStaticTitleGuildInfo->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_INFO_TITLE));

	m_pkStaticGuildJoinstate->InitState(true);
	m_pkStaticGuildJoinstate->SetAlign(DT_RIGHT);

	UpdateWaitingJoinerState();

	m_pkListHoverBG->InitState(false);

	ToggleRenderSortArrow((int)KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE);

	m_fListHoverBoxPosY = m_pkListHoverBG->GetFixedWindowLocalPos().y;

	m_pkGuildMarkIcon->InitState(true, true, this);
	m_pkGuildSearchingBlind->InitState(false);

	m_pkBtnClose->SetHotKey(DIK_ESCAPE);
}

void KGCGuildListDlg::OnCreateComplete(void)
{
}

void KGCGuildListDlg::ActionPerformed(const KActionEvent& event)
{
	if (m_iStratSearch != -1)
		return;

	GCWND_MSG_MAP(m_pkBtnNextPage, KD3DWnd::D3DWE_BUTTON_CLICK, OnNextPage);
	GCWND_MSG_MAP(m_pkBtnPrevPage, KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevPage);
	GCWND_MSG_MAP(m_pkBtnSearch, KD3DWnd::D3DWE_BUTTON_CLICK, OnSearchKeyword);
	GCWND_MSG_MAP(m_pkEditSearch, KD3DWnd::D3DWE_EDIT_RETURN, OnSearchKeyword);
	GCWND_MSG_MAP(m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
	GCWND_MSG_MAP(m_pkBtnGuildCreate, KD3DWnd::D3DWE_BUTTON_CLICK, OnGuildCreateDlg);
	GCWND_MSG_MAP(m_pkBtnJoinReq, KD3DWnd::D3DWE_BUTTON_CLICK, OnJoinReq);
	GCWND_MSG_MAP(m_pkBtnJoinCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnJoinCancel);

	for (int i = 0; i < m_iMaxListNum; i++)
	{
		if (event.m_pWnd == m_pkGuildListInfo[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
			UpdateDetailGuildInfo(i);
	}

	if (event.m_pWnd == m_pkBtnSortingGuild && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		SortingGuildList(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_NAME);
		ToggleRenderSortArrow((int)KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_NAME);
		g_KDSound.Play("31");
	}

	if (event.m_pWnd == m_pkBtnSortingGuildGrade && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		SortingGuildList(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_GRADE);
		ToggleRenderSortArrow((int)KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_GRADE);
		g_KDSound.Play("31");
	}

	if (event.m_pWnd == m_pkBtnSortingGuildBirthday && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		SortingGuildList(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE);
		ToggleRenderSortArrow((int)KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE);
		g_KDSound.Play("31");
	}

	if (event.m_pWnd == m_pkBtnGuildList && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		g_KDSound.Play("31");
		if (m_bIsPressedBtn)
			return;

		KP2P::GetInstance()->Send_SearchGuildListReq(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE, L"");
		m_bIsPressedBtn = true;

		m_pkEditSearch->SetText(L"");
		if (m_iPrevIndex >= 0)
			m_pkGuildListInfo[m_iPrevIndex]->SetRenderListSelectBG(false);
	}
}

void KGCGuildListDlg::OnDestroyComplete(void)
{
	m_pkListHoverBG->SetWindowPosY(m_fListHoverBoxPosY);
	m_vecDetalGuildInfo.clear();
}

void KGCGuildListDlg::FrameMoveInEnabledState(void)
{
	if (m_iStratSearch != -1)
		return;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);

	if (m_pkStaticGuildURL->CheckPosInWindowBound(vMousePos))
	{
		m_pkStaticGuildURL->SetFontColor(D3DCOLOR_RGBA(0, 0, 255, 255));
		if (g_pkInput->BtnDown(KInput::MBLEFT) && m_pkStaticGuildURL->GetText() != L"")
		{
			PopUpWebPage((LPTSTR)(LPCTSTR)m_wstrGuildUrl.c_str());
		}
	}
	else
		m_pkStaticGuildURL->SetFontColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	m_pkListHoverBG->ToggleRender(false);
	for (int i = 0; i < m_iMaxListNum; i++)
	{
		float fOffsetY;

		if (m_iPrevIndex != -1 && m_iPrevIndex != i)
			m_pkGuildListInfo[i]->SetRenderListSelectBG(false);

		if (m_pkGuildListInfo[i]->CheckPosInWindowBound(vMousePos))
		{
			fOffsetY = m_fListHoverBoxPosY + i * m_pkListHoverBG->GetHeight();
			m_pkListHoverBG->SetWindowPosY(fOffsetY);
			m_pkListHoverBG->ToggleRender(true);

			if (g_pkInput->BtnDown(KInput::MBLEFT))
			{
				if (m_pkGuildListInfo[i]->IsRenderONListSelectBG())
					m_pkGuildListInfo[i]->SetRenderListSelectBG(false);
				else
					m_pkGuildListInfo[i]->SetRenderListSelectBG(true);

				m_iPrevIndex = i;
			}

		}
	}
}

void KGCGuildListDlg::PostChildDraw(void)
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		g_ParticleManager->Render(GC_LAYER_UPPER_SQUARE_UI_0);
		g_ParticleManager->Render(GC_LAYER_UPPER_SQUARE_UI_1);
	}
	g_pGCDeviceManager2->PopState();

	if (m_iStratSearch >= 0)
	{
		if (m_iStratSearch == 0)
		{
			// create loading particle!!
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Guild_Search_01", 0.0f, 0.05f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_1);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Guild_Search_02", 0.0f, 0.05f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_1);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Guild_Search_03", 0.0f, 0.05f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_1);
			g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("Guild_Search_04", 0.0f, 0.05f, 0.5f), GC_LAYER_UPPER_SQUARE_UI_0);
			this->LockAll(true);
			m_pkGuildSearchingBlind->ToggleRender(true);
		}
		m_iStratSearch++;
		if (m_iStratSearch >= GC_FPS_LIMIT * SEARCH_LIMIT_TIME)
		{
			this->LockAll(false);
			m_pkGuildSearchingBlind->ToggleRender(false);
			UpdateWaitingJoinerState();
			m_iStratSearch = -1;
		}
	}
}

void KGCGuildListDlg::OnGuildCreateDlg(void)
{
	g_KDSound.Play("31");
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_CREATE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
}

void KGCGuildListDlg::OnClose(void)
{
	g_KDSound.Play("31");
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void KGCGuildListDlg::OnJoinReq()
{
	g_KDSound.Play("31");
	g_pkUIScene->m_pkGuildJoinReqMbox->SetJoinerInfo(m_iSelectGuildUID);
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_JOIN_REQ_MBOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
}

void KGCGuildListDlg::UpdateGuildList()
{
	for (int i = 0; i < MAX_GUILD_LIST_NUM; i++)
	{
		if (i < m_iMaxListNum)
		{
			m_pkGuildListInfo[i]->SetGuildInfoData(m_vecDetalGuildInfo[i], true);
			m_pkGuildListInfo[i]->RenderGuildMark(true);
		}
		else
		{
			m_pkGuildListInfo[i]->SetGuildInfoData(m_vecDetalGuildInfo[i], false);
			m_pkGuildListInfo[i]->RenderGuildMark(false);
		}
	}

	UpdateWaitingJoinerState();
}

void KGCGuildListDlg::MovePage(bool bNext_)
{
	g_KDSound.Play("31");

	if (m_iPrevIndex >= 0) {
		m_pkGuildListInfo[m_iPrevIndex]->SetRenderListSelectBG(false);
	}

	if (m_bIsPressedBtn || m_vecDetalGuildInfo.empty()) { //중복입력을 막기위함
		return;
	}

	bool bSendReq = true;
	if ((bNext_) && (m_iCurPage < m_iMaxPage)) {
		m_iCurPage++;
	}
	else if ((!bNext_) && (m_iCurPage > 1)) {
		m_iCurPage--;
	}
	else {
		bSendReq = false;
	}

	if (bSendReq) {
		KP2P::GetInstance()->Send_GuildListReq(m_ucSortType, m_pkEditSearch->GetText(), m_iCurPage);
		m_bIsPressedBtn = true;
	}

}


void KGCGuildListDlg::OnNextPage()
{
	this->MovePage(true);
}

void KGCGuildListDlg::OnPrevPage()
{
	this->MovePage(false);
}

void KGCGuildListDlg::SetDetailGuildInfo(std::vector<KNGuildInfo> & vecDetailGuildInfo_, int iCurPage_, int iMaxPage_)
{
	for (int i = 0; i < MAX_GUILD_LIST_NUM; i++)
		m_pkGuildListInfo[i]->InitGuildInfo();

	m_vecDetalGuildInfo.clear();
	m_vecDetalGuildInfo = vecDetailGuildInfo_;
	m_iCurPage = iCurPage_;
	m_iMaxPage = iMaxPage_;
	m_iMaxListNum = m_vecDetalGuildInfo.size();

	if (g_kGlobalValue.m_kGuildInfo.m_dwUID != 0) {
		WCHAR strText[20];
		swprintf(strText, 20, L"%d / %d", m_iCurPage, m_iMaxPage);
		m_pkStaticGuildListPage->SetText(strText);
		m_pkStaticMyGuildName->SetText(g_kGlobalValue.m_kGuildInfo.m_strName);
	}
	else {
		m_pkStaticMyGuildName->SetText(L"");
	}

	//길드에 가입상태에 따라서 길드가입신청 버튼의 상태를 결정하자.
	if (g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ||
		g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_DRUMOUT)
		UpdateDetailGuildInfo(0); //길드가입상태라면 길드정보창에는 자신의 길드 정보를 보여주자
	else
		UpdateDetailGuildInfo(-1);

	UpdateWaitingJoinerState();
	UpdateGuildList();
}

//길드 탈퇴 후 길드 리스트에서 해산된 길드정보를 보여줌.(초기화 구문이 필요) 
void KGCGuildListDlg::InitStaticData()
{
	m_pkStaticGuildJoinCondition->SetText(L"");
	m_pkStaticGuildMasterName->SetText(L"");
	m_pkStaticGuildURL->SetText(L"");
	m_pkStaticGuildURL->ClearToolTip();
	m_pkStaticGuildMemberNum->SetText(L"");
	m_pkStaticGuildPoint->SetText(L"");
	m_pkStaticGuildTotalRanking->SetText(L"");
	m_pkStaticGuildMonthRanking->SetText(L"");
	m_pkStaticGuildWeekRanking->SetText(L"");
	m_pkStaticGuildName->SetText(L"");
	m_pkStaticGuildBithday->SetText(L"");
	m_pkStaticGuildIntroduce->SetText(L"");
	m_pkStaticGuildGrade->SetText(L"");
	m_pkGuildMarkIcon->SetTextureName(L"");
	m_pkStaticMyGuildName->SetText(L"");
}
void KGCGuildListDlg::UpdateDetailGuildInfo(int iIndex_)
{
	KNGuildInfo sGuildInfo;
	if (iIndex_ >= m_iMaxListNum) {
		InitStaticData();
		return;
	}
	else if (iIndex_ == -1)
		sGuildInfo = g_kGlobalValue.m_kGuildInfo;
	else
		sGuildInfo = m_vecDetalGuildInfo[iIndex_];

	if (sGuildInfo.m_ucJoinSetting == KNGuildInfo::JS_AUTO_ACCEPT)
		m_pkStaticGuildJoinCondition->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_DIRECTLY));
	else if (sGuildInfo.m_ucJoinSetting == KNGuildInfo::JS_AFTER_DECISION)
		m_pkStaticGuildJoinCondition->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_AFTER));
	else //가입중지
		m_pkStaticGuildJoinCondition->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_STOP));

	m_pkStaticGuildMasterName->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_MASTER_NAME, "l", L"#c" + sGuildInfo.m_strMasterNickColor + sGuildInfo.m_strMasterNick + L"#cX"));

	m_pkStaticGuildURL->SetForceWordWrapText(sGuildInfo.m_strURL, true);
	m_pkStaticGuildURL->SetToolTip(sGuildInfo.m_strURL);
	m_wstrGuildUrl = sGuildInfo.m_strURL;

	m_pkStaticGuildMemberNum->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_MEMBER_NUM, "i", sGuildInfo.m_nNumMembers));
	m_pkStaticGuildPoint->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_POINT, "i", sGuildInfo.m_dwPoint));
	m_pkStaticGuildTotalRanking->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_TOTAL_RANKING, "i", sGuildInfo.m_nTotalRank));
	m_pkStaticGuildMonthRanking->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_MONTH_RANKING, "i", sGuildInfo.m_nMonthlyRank));
	m_pkStaticGuildWeekRanking->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_WEEK_RANKING, "i", sGuildInfo.m_nWeeklyRank));
	m_pkStaticGuildName->SetText(sGuildInfo.m_strName);
	m_pkStaticGuildBithday->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_BIRTHDAY, "iii",
		(int)sGuildInfo.m_kFoundingDate.m_sYear, (int)sGuildInfo.m_kFoundingDate.m_cMonth, (int)sGuildInfo.m_kFoundingDate.m_cDay));

	USES_CONVERSION;
	typedef boost::tokenizer< boost::char_separator< char > > tokenizer;

	std::string strTemp = "";

	strTemp = W2A(sGuildInfo.m_strComment.c_str());
	boost::char_separator<char> sep("\r\n");
	tokenizer tokens(strTemp, sep);

	int count = 0;
	std::string strChanged = "";
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		count++;
		if (count < 4)
		{
			strChanged += (*tok_iter);
			strChanged += "\n";
		}
		else
		{
			strChanged += (*tok_iter);
		}
	}

	std::wstring strChangedComment;
	strChangedComment = A2W(strChanged.c_str());

	m_pkStaticGuildIntroduce->SetForceWordWrapText(strChangedComment, true);

	if (sGuildInfo.m_ucGrade == KNGuildInfo::GG_NEWBIE)
		m_pkStaticGuildGrade->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE) + L" " + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE_NEW));
	else
		m_pkStaticGuildGrade->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE) + L" " + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_GRADE_REGULAR));

	m_iSelectGuildUID = sGuildInfo.m_dwUID;

	//길드마크
	SiKGCIconImageManager()->LoadIconImageFromHTTP(sGuildInfo.m_strFileName);
	m_pkGuildMarkIcon->SetTextureName(sGuildInfo.m_strFileName);
}

void KGCGuildListDlg::SortingGuildList(UCHAR ucSortType_)
{
	if (m_iPrevIndex >= 0)
		m_pkGuildListInfo[m_iPrevIndex]->SetRenderListSelectBG(false);

	if (m_bIsPressedBtn || m_vecDetalGuildInfo.empty())
		return;

	m_pkEditSearch->SetText(L"");
	m_ucSortType = ucSortType_;
	KP2P::GetInstance()->Send_SearchGuildListReq(ucSortType_);
	m_bIsPressedBtn = true;
}

void KGCGuildListDlg::OnSearchKeyword()
{
	g_KDSound.Play("31");

	std::wstring strTemp = m_pkEditSearch->GetText();
	if (strTemp.length() == 0)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5),
			g_pkStrLoader->GetString(STR_ID_NEW_GUILD_INPUT_SEARCH_KEYWORD), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true);
		::SetFocus(g_hUIMainWnd);
		return;
	}
	else if (strTemp.length() == 1)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5),
			g_pkStrLoader->GetString(STR_ID_GUILD_SEARCH_ONE_TEXT_ERROR), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, true, true);
		return;
	}

	if (m_bIsPressedBtn)
		return;

	KP2P::GetInstance()->Send_SearchGuildListReq(KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE, m_pkEditSearch->GetText());
	m_bIsPressedBtn = true;
	m_iStratSearch = 0;

	if (m_iPrevIndex >= 0)
		m_pkGuildListInfo[m_iPrevIndex]->SetRenderListSelectBG(false);
}

void KGCGuildListDlg::ToggleRenderSortArrow(int iWhatSorting_)
{
	//소팅기능 제대로 안되고 있음.
	return;
	for (int i = 0; i < MAX_SORTING_BTN; i++)
	{
		if (iWhatSorting_ != i)
			m_pkSortArrowUp[i]->ToggleRender(false);
		m_pkSortArrowDown[i]->ToggleRender(false);
	}

	if (iWhatSorting_ < MAX_SORTING_BTN)
	{
		if (m_pkSortArrowUp[iWhatSorting_]->IsRenderOn() == false && m_pkSortArrowDown[iWhatSorting_]->IsRenderOn() == false)
			m_pkSortArrowUp[iWhatSorting_]->ToggleRender(true);
		else if (m_pkSortArrowUp[iWhatSorting_]->IsRenderOn())
		{
			m_pkSortArrowUp[iWhatSorting_]->ToggleRender(false);
			m_pkSortArrowDown[iWhatSorting_]->ToggleRender(true);
		}
		else
		{
			m_pkSortArrowUp[iWhatSorting_]->ToggleRender(true);
			m_pkSortArrowDown[iWhatSorting_]->ToggleRender(false);
		}
	}
}

void KGCGuildListDlg::OnJoinCancel()
{
	g_KDSound.Play("31");
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX,
		g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_CANCEL_REQ), g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_CANCEL_MSG),
		KGCUIScene::GC_MBOX_GUILD_JOIN_CANCEL_REQ, KGCGuildPublicMbox::MBR_CONFIRM_CANCEL, 0, false, true);
}

void KGCGuildListDlg::UpdateWaitingJoinerState()
{
	if (g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER)
	{
		m_pkBtnJoinCancel->ToggleRender(true);
		m_pkBtnJoinReq->Lock(true);
		m_pkBtnGuildCreate->Lock(true);
		m_pkStaticGuildJoinstate->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_WAITING_STATE));
	}
	else
	{
		m_pkBtnJoinCancel->ToggleRender(false);
		if (g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ||
			g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_DRUMOUT)
		{
			m_pkBtnJoinReq->Lock(false);
			m_pkBtnGuildCreate->Lock(false);
		}
		else
		{
			m_pkBtnJoinReq->Lock(true);
			m_pkBtnGuildCreate->Lock(true);
		}
		m_pkStaticGuildJoinstate->SetText(L"");
	}
}