// KChannel.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "gcservermanager.h"
#include "KChannel.h"
#include "KRoomInfoDlg.h"
#include "KUserInfoDlg.h"
#include "KDialogIDDlg.h"
#include "KChangeDBInfoDlg.h"
#include "GCServerManagerView.h"
#include ".\kchannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KChannel dialog


KChannel::KChannel(CWnd* pParent /*=NULL*/)
	: CDialog(KChannel::IDD, pParent)
{
	//{{AFX_DATA_INIT(KChannel)
	//m_cstrAdminChat = _T("");
	m_cstrNormalChat = _T("");
	//}}AFX_DATA_INIT
    m_pChildView            = NULL;
    m_bStop                 = false;
    m_iRoomPage             = 0;
    m_iRoomPageSize         = 0;
    m_iGameMode             = 0;
    m_iUserListPage         = 0;
    m_iUserListPageSize     = 0;//080703. kurrung님의 프로세서 디버깅과 단서. 초기화가 안된 문제로 무한로딩.

    m_iChannelType          = 0;
    m_iSquareUserListPage   = 0;
    m_iSquareUserListPageSize =0;

    m_pMainFrm = static_cast<CMainFrame*>( ::AfxGetMainWnd() );
    m_mapGameMode.clear();
    eArea = AC_ALL;
}


void KChannel::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(KChannel)
    DDX_Control(pDX, IDC_BUTTON_CHAT_STOP, m_btnStop);
    DDX_Control(pDX, IDC_LIST_USER, m_ctrlUserList);
    DDX_Control(pDX, IDC_LIST_ROOM, m_ctrlRoomList);
    DDX_Control(pDX, IDC_LIST_CHAT, m_ctrlChatList);
    DDX_Control(pDX, IDC_LIST_SQUARE, m_ctrlSquareList);
    DDX_Control(pDX, IDC_LIST_SQUARE_USER, m_ctrlSquareUserList);
    DDX_Control(pDX, IDC_EDIT_NORMAL_CHAT, m_ctrlEditNormalChat);
    //DDX_Control(pDX, IDC_EDIT_ADMIN_CHAT, m_ctrlEditAdminChat);
    //DDX_Text(pDX, IDC_EDIT_ADMIN_CHAT, m_cstrAdminChat);
    DDX_Text(pDX, IDC_EDIT_NORMAL_CHAT, m_cstrNormalChat);

    DDX_Control(pDX, IDC_COMBO_PAGE, m_comboRoomPage);
    DDX_Control(pDX, IDC_COMBO_GAMEMODE, m_comboGameMode);
    DDX_Control(pDX, IDC_COMBO_USER_PAGE, m_comboUserListPage);
    DDX_Control(pDX, IDC_COMBO_SQUARE_USER_PAGE, m_comboSquareUserListPage);

    DDX_Control(pDX, IDC_RADIO_ADMIN_CHAT, m_raNotice_Message[SBCM_ADMIN_CHAT]);
    DDX_Control(pDX, IDC_RADIO_SB_NOTICE, m_raNotice_Message[SBCM_NOTICE]);
    DDX_Control(pDX, IDC_RADIO_SB_SERVER, m_raNotice_Message[SBCM_SERVER]);
    DDX_Control(pDX, IDC_RADIO_NORMAL_CHAT, m_raNotice_Message[SBCM_NORMAL_CHAT]);
    //}}AFX_DATA_MAP

    DDX_Control(pDX, IDC_COMBO_AREA, m_comboArea);
}


BEGIN_MESSAGE_MAP(KChannel, CDialog)
	//{{AFX_MSG_MAP(KChannel)
	ON_BN_CLICKED(IDC_DEF_BTN_ADMIN, OnDefBtnAdmin)
	ON_LBN_DBLCLK(IDC_LIST_USER, OnDblclkListUser)
	ON_LBN_DBLCLK(IDC_LIST_ROOM, OnDblclkListRoom)
	ON_LBN_DBLCLK(IDC_LIST_CHAT, OnDblclkListChat)
	ON_BN_CLICKED(IDC_BUTTON_ROOM_LIST_UPDATE, OnButtonRoomListUpdate)
	ON_BN_CLICKED(IDC_BUTTON_USER_LIST_UPDATE, OnButtonUserListUpdate)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_BUTTON_CHAT_STOP, OnButtonChatStop)
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_COMBO_PAGE, OnCbnSelchangeComboPage)
    ON_CBN_SELCHANGE(IDC_COMBO_GAMEMODE, OnCbnSelchangeComboGamemode)
    ON_CBN_SELCHANGE(IDC_COMBO_USER_PAGE, OnCbnSelchangeComboUserPage)
    ON_LBN_DBLCLK(IDC_LIST_SQUARE, OnLbnDblclkListSquare)
    ON_BN_CLICKED(IDC_BUTTON_SQUARE_UPDATE, OnBnClickedButtonSquareUpdate)
    ON_CBN_SELCHANGE(IDC_COMBO_SQUARE_USER_PAGE, OnCbnSelchangeComboSquareUersInfo)
    ON_LBN_DBLCLK(IDC_LIST_SQUARE_USER, OnLbnDblclkListSquareUser)
    ON_CBN_SELCHANGE(IDC_COMBO_AREA, &KChannel::OnCbnSelchangeComboArea)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KChannel message handlers

void KChannel::OnDefBtnAdmin( void )
{
    UpdateData( TRUE );

    //if ( &m_ctrlEditAdminChat == GetFocus() )
    //{
    //    ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM(IDC_EDIT_ADMIN_CHAT, 0), 0 );
    //}

    if ( &m_ctrlEditNormalChat == GetFocus() )
    {
        ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM(IDC_EDIT_NORMAL_CHAT, 0), 0 );
    }
}

void KChannel::OnCancel( void )
{
    ::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM(ID_BTN_CHANNEL_EXIT, 0), 0 );
    eArea = AC_ALL;
}

void KChannel::OnDblclkListUser( void )
{
    KUserInfoDlg dlg( GetParent() );

    dlg.m_iIdxInfo = m_ctrlUserList.GetCurSel();

    dlg.DoModal();
}

void KChannel::OnDblclkListRoom( void )
{
    KRoomInfoDlg dlg( GetParent() );

    dlg.m_iIdxInfo = m_ctrlRoomList.GetCurSel();

    dlg.DoModal();
}

void KChannel::OnOK( void )
{
    if( g_nLevel > 0 )
    {
        AfxMessageBox( _T("you don't have authority") );
        return;
    }
    KDialogIDDlg dlg( GetParent() );

    if ( IDOK == dlg.DoModal() )
    {
        //microcat. 아이디와 닉네임 둘다 들어올 수 있음.
        m_pChildView->m_server.Send_UserDBInfoReq( m_pChildView->m_strWideServerName.c_str(),
                                                   std::wstring( dlg.m_cstrUserID ),
                                                   dlg.m_iNickname == 1 );
        m_pChildView->m_pMainFrm->m_logFile << L"[request] " << (const wchar_t*)dlg.m_cstrUserID
                                            << L" It queries user's DB information." << std::endl;
    }
}

BOOL KChannel::OnInitDialog( void )
{
    CDialog::OnInitDialog();

    m_pChildView = static_cast<CGCServerManagerView*>( GetParent() );

    InitArea();
    InitGameMode();

    m_comboRoomPage.SetCurSel( 0 );
    m_comboUserListPage.SetCurSel( 0 );
    m_comboSquareUserListPage.SetCurSel( 0 );

    if ( GetChannelType() == CT_DUNGEON )
        m_comboArea.SetCurSel( 0 );
    else 
        m_comboArea.EnableWindow( FALSE ); // 대전일 경우 지역 정보 비활성화

    if( GetChannelType() == CT_DUNGEON )
        SetGameMode( m_comboGameMode.GetCurSel() + GC_GM_QUEST0 );
    else
        SetGameMode( m_comboGameMode.GetCurSel() );

    MakeRoomPage();
    MakeUserListPage();
    MakeSquarePage();

    m_raNotice_Message[SBCM_ADMIN_CHAT].SetCheck( 1 );
    m_ctrlChatList.SetHorizontalExtent( 3000 );
    m_ctrlRoomList.SetHorizontalExtent( 400 );

    std::map<CString, KGAME_MODE>::iterator mit;
    std::map<CString, KGAME_MODE>::iterator mWhere;
    int nIndex = 0;

    mWhere = m_mapGameMode.find("시련의 숲");
    if ( m_mapGameMode.end() != mWhere ) {
    for ( mit = m_mapGameMode.begin(); mit != m_mapGameMode.end(); mit++ ) {
        if ( mit->first.Compare(mWhere->first) == 0 )
            break;

        nIndex++;
    }
    }
    
    m_comboGameMode.SetCurSel( nIndex ); // 초기화 시 시련의 숲 으로 초기화

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void KChannel::MakeRoomPage()
{
    CString strPage;
    int i = 0;

    m_comboRoomPage.ResetContent();

    for( i = 1; i <= m_iRoomPageSize ; ++i ) // m_iRoomPageSize 바로 쓰기-_-
    {
        strPage.Format( L"%d", i );
        m_comboRoomPage.AddString( strPage );
    }

    m_comboRoomPage.SetCurSel( GetRoomListPage() );
}

void KChannel::MakeUserListPage()
{
    CString strPage;
    int i = 0;

    m_comboUserListPage.ResetContent();

    for( i = 1; i <= m_iUserListPageSize ; ++i )
    {
        strPage.Format( L"%d", i );

        m_comboUserListPage.AddString( strPage );
    }
    m_comboUserListPage.SetCurSel( GetUserListPage() );
}

void KChannel::InitGameMode()
{
    // 기존에는 바로 콤보박스에 스트링을 넣는 형태였으나
    // 사용자 편의성을 고려하여 사용자가 알 필요 없는 것들을 제거
    // getcursel로 해당 던전의 번호를 받았었기 때문에
    // 던전 별 enum값의 수치를 맵으로 저장해서 해결
    m_mapGameMode.clear();

    // m_bKorean 메인 프레임에 저장 된 한국어 출력 여부 변수
    if ( m_pMainFrm->m_bKorean ) {
        if( GetChannelType() == CT_DUNGEON )
        {
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("시련의 숲",            KGAME_MODE(GC_GM_QUEST0, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("시련의 탑",            KGAME_MODE(GC_GM_QUEST1, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("세르딘 외곽",          KGAME_MODE(GC_GM_QUEST2, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("캐리비치",             KGAME_MODE(GC_GM_QUEST3, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("오크 사원",            KGAME_MODE(GC_GM_QUEST4, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("고르고스 던전",        KGAME_MODE(GC_GM_QUEST5, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("엘프의 숲",            KGAME_MODE(GC_GM_QUEST6, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("맹세의 계곡",          KGAME_MODE(GC_GM_QUEST7, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("망각의 늪",            KGAME_MODE(GC_GM_QUEST8, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("망자의 묘지",          KGAME_MODE(GC_GM_QUEST9, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("잊혀진 도시",          KGAME_MODE(GC_GM_QUEST10, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("가이코즈의 성",        KGAME_MODE(GC_GM_QUEST11, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("파투세이의 바다",      KGAME_MODE(GC_GM_QUEST12, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카미키 성채",          KGAME_MODE(GC_GM_QUEST13, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("불의 사원",            KGAME_MODE(GC_GM_QUEST14, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("헬 브릿지",            KGAME_MODE(GC_GM_QUEST15, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카제아제의 성",        KGAME_MODE(GC_GM_QUEST16, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카스툴 유적지 하층",   KGAME_MODE(GC_GM_QUEST17, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카스툴 유적지 상층",   KGAME_MODE(GC_GM_QUEST18, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("베르메시아의 최후",    KGAME_MODE(GC_GM_QUEST19, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("제니아 접경지",        KGAME_MODE(GC_GM_QUEST20, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("순환의 신전",          KGAME_MODE(GC_GM_QUEST21, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("소실의 협곡",          KGAME_MODE(GC_GM_QUEST22, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("성난 엔트의 숲",       KGAME_MODE(GC_GM_QUEST23, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("실버나이츠 폐허",      KGAME_MODE(GC_GM_QUEST24, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("미명의 호수",          KGAME_MODE(GC_GM_QUEST25, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("이그니스의 산맥",      KGAME_MODE(GC_GM_QUEST26, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("수룡의 늪",            KGAME_MODE(GC_GM_QUEST27, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("원시의 섬",            KGAME_MODE(GC_GM_QUEST28, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("드레이크 웜의 영지",   KGAME_MODE(GC_GM_QUEST29, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("빅터의 요새",          KGAME_MODE(GC_GM_QUEST30, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("조율의 제단",          KGAME_MODE(GC_GM_QUEST31, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("파괴의 신전",          KGAME_MODE(GC_GM_QUEST32, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("생명의 숲",            KGAME_MODE(GC_GM_QUEST33, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("지배의 성",            KGAME_MODE(GC_GM_QUEST34, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("베르메시아 보스 던전", KGAME_MODE(GC_GM_QUEST35, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("실버랜드 보스 던전",   KGAME_MODE(GC_GM_QUEST36, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("엘리아 보스 던전",     KGAME_MODE(GC_GM_QUEST37, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("제니아 보스 던전",     KGAME_MODE(GC_GM_QUEST38, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("고블린 노역소",        KGAME_MODE(GC_GM_QUEST39, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("지하암흑 터널",        KGAME_MODE(GC_GM_QUEST40, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("쿵지쿵지",             KGAME_MODE(GC_GM_QUEST41, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("드워프 전초기지",      KGAME_MODE(GC_GM_QUEST42, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("신기루 사막",          KGAME_MODE(GC_GM_QUEST43, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("드워프 보급로",        KGAME_MODE(GC_GM_QUEST44, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("썬더 해머",            KGAME_MODE(GC_GM_QUEST45, AC_ACHAEMEDIA)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("[영웅]파멸의 성소",    KGAME_MODE(GC_GM_QUEST46, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("[영웅]지옥의 용광로",  KGAME_MODE(GC_GM_QUEST47, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("고대 왕국의 흔적",     KGAME_MODE(GC_GM_QUEST48, AC_ACHAEMEDIA)) );
            // 여기부터 이벤트 던전
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카니발",               KGAME_MODE(GC_GM_QUEST49, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("릴레이 던전",          KGAME_MODE(GC_GM_QUEST50, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("[영웅]서커스",         KGAME_MODE(GC_GM_QUEST51, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("환영의 미궁",          KGAME_MODE(GC_GM_QUEST52, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("명계 열차",            KGAME_MODE(GC_GM_QUEST53, AC_HERO_AND_EVENT)) );

            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("배고픈 펫들의 반란",   KGAME_MODE(GC_GM_QUEST54, AC_ALL)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카루엘 접경지",        KGAME_MODE(GC_GM_QUEST55, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("카루엘",               KGAME_MODE(GC_GM_QUEST56, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("달빛 요괴마을",        KGAME_MODE(GC_GM_QUEST57, AC_ALL)) );
        } // if( GetChannelType() == CT_DUNGEON )
        else
        {
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("대전", KGAME_MODE(GC_GMC_MATCH, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("길드전", KGAME_MODE(GC_GMC_GUILD_BATTLE, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("태그 매치", KGAME_MODE(GC_GMC_TAG_MATCH, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("데스매치", KGAME_MODE(GC_GMC_DEATHMATCH, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("천사의 알", KGAME_MODE(GC_GMC_ANGELS_EGG, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("대장전", KGAME_MODE(GC_GMC_CAPTAIN, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("점령전", KGAME_MODE(GC_GMC_DOTA, AC_MATCH)) );
        }
    } // if ( m_pMainFrm->m_bKorean )
    else {
        if( GetChannelType() == CT_DUNGEON )
        {
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Trial Forest",                KGAME_MODE(GC_GM_QUEST0, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Trial Tower",                 KGAME_MODE(GC_GM_QUEST1, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Outer wall of Serdin",        KGAME_MODE(GC_GM_QUEST2, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Kerrie Beach",                KGAME_MODE(GC_GM_QUEST3, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Orc Temple",                  KGAME_MODE(GC_GM_QUEST4, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Gorgos'' Dungeon",            KGAME_MODE(GC_GM_QUEST5, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Elven Forest",                KGAME_MODE(GC_GM_QUEST6, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Gorge of Oath",               KGAME_MODE(GC_GM_QUEST7, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Marsh of Oblivion",           KGAME_MODE(GC_GM_QUEST8, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Forsaken Barrows",            KGAME_MODE(GC_GM_QUEST9, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Forgotten City",              KGAME_MODE(GC_GM_QUEST10, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Gaikoz''s Castle",            KGAME_MODE(GC_GM_QUEST11, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Partusay''s Sea",             KGAME_MODE(GC_GM_QUEST12, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Kamiki Castle",               KGAME_MODE(GC_GM_QUEST13, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("The Temple of Fire",          KGAME_MODE(GC_GM_QUEST14, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Hell Bridge",                 KGAME_MODE(GC_GM_QUEST15, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Kaze''aze''s Castle",         KGAME_MODE(GC_GM_QUEST16, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Kastulle Ruins Bottom Floor", KGAME_MODE(GC_GM_QUEST17, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Kastulle Ruins Upper Floor",  KGAME_MODE(GC_GM_QUEST18, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Battle for Bermesiah",        KGAME_MODE(GC_GM_QUEST19, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Xenia Border",                KGAME_MODE(GC_GM_QUEST20, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Temple of Cuatal",            KGAME_MODE(GC_GM_QUEST21, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Valstrath",                   KGAME_MODE(GC_GM_QUEST22, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("The Whispering Woods",        KGAME_MODE(GC_GM_QUEST23, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Ruins of silver Knights",     KGAME_MODE(GC_GM_QUEST24, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Lake Aurora",                 KGAME_MODE(GC_GM_QUEST25, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("The Wyrm''s Maw",             KGAME_MODE(GC_GM_QUEST26, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("King Guang''s Fen",           KGAME_MODE(GC_GM_QUEST27, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Primeval Island",             KGAME_MODE(GC_GM_QUEST28, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Drake Worm",                  KGAME_MODE(GC_GM_QUEST29, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Fortress of victor",          KGAME_MODE(GC_GM_QUEST30, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Altar of judgement",          KGAME_MODE(GC_GM_QUEST31, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Temple of Destruction",       KGAME_MODE(GC_GM_QUEST32, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Forest of Life",              KGAME_MODE(GC_GM_QUEST33, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Castle of Domination",        KGAME_MODE(GC_GM_QUEST34, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Bermesiah Boss Dungeon",      KGAME_MODE(GC_GM_QUEST35, AC_BERMESIAH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Silver Land Boss Dungeon",    KGAME_MODE(GC_GM_QUEST36, AC_SILVERLAND)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Ellia Boss Dungeon",          KGAME_MODE(GC_GM_QUEST37, AC_ELIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Xenia Boss Dungeon",          KGAME_MODE(GC_GM_QUEST38, AC_XENIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Goblin Research",             KGAME_MODE(GC_GM_QUEST39, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Underground Tunnel",          KGAME_MODE(GC_GM_QUEST40, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Kungji Village",              KGAME_MODE(GC_GM_QUEST41, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Dwarf Base",                  KGAME_MODE(GC_GM_QUEST42, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Mirage Desert",               KGAME_MODE(GC_GM_QUEST43, AC_ATUM)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Dwarf Supply line",           KGAME_MODE(GC_GM_QUEST44, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Thunder Hammer",              KGAME_MODE(GC_GM_QUEST45, AC_ACHAEMEDIA)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Hero Dungeon Ellia",          KGAME_MODE(GC_GM_QUEST46, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Hero Dungeon Bermesiah",      KGAME_MODE(GC_GM_QUEST47, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Wreck of Ancient Kingdom",    KGAME_MODE(GC_GM_QUEST48, AC_ACHAEMEDIA)) );
            // 여기부터 이벤트 던전
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Event Carnival",              KGAME_MODE(GC_GM_QUEST49, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Event RelayDungeon",          KGAME_MODE(GC_GM_QUEST50, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Event HeroDungeon",           KGAME_MODE(GC_GM_QUEST51, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Relay HeroDungeon",           KGAME_MODE(GC_GM_QUEST52, AC_HERO_AND_EVENT)) );
			m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Monster Train",               KGAME_MODE(GC_GM_QUEST53, AC_HERO_AND_EVENT)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Hungry PetDungeon",           KGAME_MODE(GC_GM_QUEST54, AC_ALL)) );

            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("KaruelBorder",                KGAME_MODE(GC_GM_QUEST55, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Karuel",                      KGAME_MODE(GC_GM_QUEST56, AC_ACHAEMEDIA)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("Moonlight villige",           KGAME_MODE(GC_GM_QUEST57, AC_ALL)) );

        } // if( GetChannelType() == CT_DUNGEON )
        else
        {
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("MATCH", KGAME_MODE(GC_GMC_MATCH, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("GUILD_BATTLE", KGAME_MODE(GC_GMC_GUILD_BATTLE, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("TAG_MATCH", KGAME_MODE(GC_GMC_TAG_MATCH, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("DEATHMATCH", KGAME_MODE(GC_GMC_DEATHMATCH, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("ANGELS_EGG", KGAME_MODE(GC_GMC_ANGELS_EGG, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("CAPTAIN", KGAME_MODE(GC_GMC_CAPTAIN, AC_MATCH)) );
            m_mapGameMode.insert( std::map<CString, KGAME_MODE>::value_type("DOTA", KGAME_MODE(GC_GMC_DOTA, AC_MATCH)) );
        }
    } // if ( m_pMainFrm->m_bKorean ) else

    // 저장 된 맵의 정보를 콤보박스에 갱신
    UpdateGameMode();
}

void KChannel::OnDblclkListChat( void )
{
    CString cstrMsg;

    m_ctrlChatList.GetText( m_ctrlChatList.GetCurSel(), cstrMsg );

    if ( TRUE == cstrMsg.IsEmpty() ) return;

    int iColon = cstrMsg.Find( _T(':') );
    int iSpace = cstrMsg.Find( _T(' ') );
    int iEnd = ( iSpace < iColon ? iSpace : iColon );

    std::wstring strLogin;
    for ( int i = 0; i < iEnd; i++ )
    {
        strLogin += cstrMsg[i];
    }

    //microcat. 채팅창에서 클릭한 것은 모두 닉네임이다.
    m_pChildView->m_server.Send_UserDBInfoReq( m_pChildView->m_strWideServerName.c_str(), strLogin, true );
    m_pChildView->m_pMainFrm->m_logFile << L"[request] " << strLogin
                                        << L" It queries user's DB information." << std::endl;
}

void KChannel::OnButtonRoomListUpdate( void )
{
    m_pChildView->ChangeRoomPage();

    //m_pChildView->UpdateRoomInfoList(); // 갱신은 룸리스트 ack 받은 후에 처리
}

void KChannel::OnButtonUserListUpdate( void )
{
    m_pChildView->ChangeUserPage();
    //m_pChildView->UpdateUserInfoList();
}

void KChannel::OnButtonChatStop( void )
{
    if ( true == ( m_bStop = !m_bStop ) )
    {
        m_btnStop.SetWindowText( L"Stop" );
    }
    else
    {
        m_btnStop.SetWindowText( L"Go" );
        std::vector<std::wstring>::iterator vit;
        for( vit = m_pChildView->m_vecChatBuf.begin(); vit != m_pChildView->m_vecChatBuf.end(); vit++ )
        {
            m_ctrlChatList.InsertString( -1, vit->c_str() );
            m_ctrlChatList.SetCurSel( m_ctrlChatList.GetCount() - 1 );
            m_ctrlChatList.SetCurSel( -1 );
        }
        m_pChildView->m_vecChatBuf.clear();
    }
}



void KChannel::OnCbnSelchangeComboPage()
{
    //CString strPage;
    //m_comboRoomPage.GetLBText( m_comboRoomPage.GetCurSel(), strPage );

    ////strPage.
    //int iPage = 0;

    //char* buff = LPSTR(LPCTSTR(strPage)); 
    //
    //iPage = atoi( buff );

    //SetRoomListPage( iPage );
    SetRoomListPage( m_comboRoomPage.GetCurSel() );

    // 방 정보 리스트를 요청한다.
    if ( false == m_pChildView->m_server.Send_RoomListReq( NULL, GetGameMode(), GetRoomListPage() ) )
    {
        ::AfxMessageBox( _T("failed to Send_RoomListReq.") );
    }
}

// m_comboRoomPage 서버에서 보내온 최대 페이지 수만큼  콤보박스 컨트롤의 크기-페이지-를 설정하고  페이지를 세팅하고 싶다 , AddString?

void KChannel::OnCbnSelchangeComboGamemode()
{
    // AC_NONE 이면 eGCCategory 정보 사용하자
    CString strMode;
    int nModeIndex=0;
    std::map<CString, KGAME_MODE>::iterator mit;

    m_comboGameMode.GetLBText(m_comboGameMode.GetCurSel(), strMode);
    mit = m_mapGameMode.find(strMode);

    if (mit != m_mapGameMode.end()) {
        if ( GetChannelType() == CT_DUNGEON )
            nModeIndex = mit->second.GetGameMode();
        else
            nModeIndex = mit->second.GetGameModeCategory();
    }
    else {
        AfxMessageBox(_T("해당 문자열 키 값 없음"));
    }

    strMode=_T("");
    strMode.Format(_T("index = %d"), nModeIndex);

    // 기존에는 선택 한 콤보박스 위치에 GC_GM_QUEST0를 더해서 인덱스를 계산했는데
    // 구조변경으로 그냥 맵에 저장 된 게임모드 받아오면 된다
    SetGameMode( nModeIndex );

    SetRoomListPage( 0 ); // 게임 모드 바뀌면 1페이지로 초기화
    //m_comboRoomPage.SetCurSel( 0 );

    // 방 정보 리스트를 요청한다.
    if ( false == m_pChildView->m_server.Send_RoomListReq( NULL, GetGameMode(), 0 ) )
    {
        ::AfxMessageBox( _T("failed to Send_RoomListReq.") );
    }
}

void KChannel::OnCbnSelchangeComboUserPage()
{
    SetUserListPage( m_comboUserListPage.GetCurSel() );

    // 로비 유저 리스트 요청
    if ( false == m_pChildView->m_server.Send_UserListReq( NULL, GetUserListPage() ) ) 
    {
        ::AfxMessageBox( _T("failed to Send_UserListReq.") );
    }
}

int KChannel::GetSBChatMessageType(void)
{
    if( false == m_raNotice_Message[0].GetSafeHwnd() )
        return -1;

    for( int i = 0; i < NUM_OF_SBCM; ++i )
    {
        if( 1 == m_raNotice_Message[i].GetCheck() )
        {
            return i;
        }
    }

    return -1;
}

void KChannel::OnLbnDblclkListSquare()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // 더블 클릭하면 광장 유저목록을 요청하자.
    m_pChildView->UpdateSquareUserList();
}

void KChannel::OnBnClickedButtonSquareUpdate()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // 버튼 클릭하면 광장 리스트 정보를 요청하자.
    m_pChildView->ChangeSquareInfoUpdate();
}

void KChannel::OnCbnSelchangeComboSquareUersInfo()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // 광장 유저 정보를 페이지 별로 볼 수 있도록.
    SetSquareUserListPage( m_comboSquareUserListPage.GetCurSel() );

    if ( false == m_pChildView->m_server.Send_SquareUserListReq( m_ctrlSquareList.GetAnchorIndex()+1, GetSquareUserListPage() ) )
    {
        ::AfxMessageBox( _T("failed to Send_SquareUserListReq fail.") );
    }
}

void KChannel::MakeSquarePage()
{
    CString strPage;
    int i = 0;

    m_comboSquareUserListPage.ResetContent();

    for( i = 1; i <= m_iSquareUserListPageSize; ++i )
    {
        strPage.Format( L"%d", i );
        m_comboSquareUserListPage.AddString( strPage );
    }

    m_comboSquareUserListPage.SetCurSel( GetSquareUserListPage() );
}

void KChannel::OnLbnDblclkListSquareUser()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString cstrNickName;

    m_ctrlSquareUserList.GetText( m_ctrlSquareUserList.GetCurSel(), cstrNickName );

    if ( TRUE == cstrNickName.IsEmpty() ) return;

    std::wstring strLogin;
    strLogin.clear();

    for ( int i = 0; i < cstrNickName.GetLength(); i++ )
    {
        strLogin += cstrNickName[i];
    }

    // 모두 닉네임
    m_pChildView->m_server.Send_UserDBInfoReq( m_pChildView->m_strWideServerName.c_str(), strLogin, true );
    m_pChildView->m_pMainFrm->m_logFile << L"[request] " << strLogin << L" It queries user's DB information." << std::endl;
}

void KChannel::OnCbnSelchangeComboArea()
{
    int nIndex = m_comboArea.GetCurSel();

    // Area의 enum 값 추가 혹은 변경 시 여기도 변경해야 한다
    if ( nIndex == AC_ALL ) {
        eArea = AC_ALL;
    }
    else if ( nIndex == AC_BERMESIAH ) {
        eArea = AC_BERMESIAH;
    }
    else if ( nIndex == AC_SILVERLAND ) {
        eArea = AC_SILVERLAND;
    }
    else if ( nIndex == AC_ELIA ) {
        eArea = AC_ELIA;
    }
    else if ( nIndex == AC_XENIA ) {
        eArea = AC_XENIA;
    }
    else if ( nIndex == AC_ACHAEMEDIA ) {
        eArea = AC_ACHAEMEDIA;
    }
    else if ( nIndex == AC_ATUM ) {
        eArea = AC_ATUM;
    }

    UpdateGameMode();
}

void KChannel::InitArea() // 지역 정보를 초기화 한다
{
    m_mapAreaCode.clear();

    if ( m_pMainFrm->m_bKorean ) {
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ALL, "전체지역"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_MATCH, "대전"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_BERMESIAH, "베르메시아"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_SILVERLAND, "실버랜드"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ELIA, "엘리아"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_XENIA, "제니아"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ACHAEMEDIA, "아케메디아"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ATUM, "아툼"));
		m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_HERO_AND_EVENT, "영웅&이벤트"));
    }
    else {
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ALL, "All Area"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_MATCH, "MATCH"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_BERMESIAH, "BERMESIAH"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_SILVERLAND, "SILVERLAND"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ELIA, "ELIA"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_XENIA, "XENIA"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ACHAEMEDIA, "ACHAEMEDIA"));
        m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_ATUM, "ATUM"));
		m_mapAreaCode.insert( std::map<int, CString>::value_type(AC_HERO_AND_EVENT, "Hero&Event"));
    }

    if ( !m_mapAreaCode.empty() ) {
        std::map<int, CString>::iterator mit;
        for ( mit = m_mapAreaCode.begin(); mit != m_mapAreaCode.end(); mit++ ) {
            if ( mit->first != AC_MATCH ) // 대전은 콤보박스에 출력 안함...
                m_comboArea.AddString(mit->second);
        }
    }
}

void KChannel::UpdateGameMode()
{
    m_comboGameMode.ResetContent();

    std::map<CString, KGAME_MODE>::iterator mit;
    for ( mit = m_mapGameMode.begin(); mit != m_mapGameMode.end(); mit++ ) {
        if ( eArea == mit->second.GetArea() || eArea == AC_ALL ) // 지역 정보를 받아 해당 지역만 추가. eArea는 현재 선택 지역 ( AC_ALL 이면 전부 출력 )
            m_comboGameMode.AddString(mit->first);
    }

    m_comboGameMode.SetCurSel( 0 );
    OnCbnSelchangeComboGamemode(); // 지역 정보 변경 시 해당 지역의 처음 셀에 있는 게임 모드 방 정보 서버에 요청
}