#include "stdafx.h"


#pragma warning(disable:4786)

//==========================For CMyD3DApplication
//
//

#include "DamageManager.h"


#include "QuestGameManager.h"
#include "Headup Display.h"
#include "GCO2Manager.h"
#include "Item.h"
#include "GCCameraOrtho.h"

#include "Monster.h"
#include "GCStateMachine.h"
//

#include "EveryTriggerManager.h"

#include "KGCRoomManager.h"
#include "KGCAnnounceMgr.h"

#include "buff/KGCBuffManager.h"
//
#include "TriggerManager.h"
#include "GCSKT.h"

#include "../BuddySystem/KFriendShip.h"
#include "KGCModeTutorial.h"
#include "TGDialogue.h"
#include "KncP2PLib/KncP2P.h"

#include "KGCPostLetterManager.h"
#include "KGCQALogManager.h"
#include "KGCEmbarkManager.h"
#include "GCChallengeErrandManager.h"

#include "GCUpdatePlanManager.h"
#include "./MapSystem/GCWallManager.h"
#include "KGCSubjectManager.h"
#include "KGCJumpingCharManager.h"
#include "KGCVirtualDepotManager.h"

//==============================================================================================
//                      KStats는 사내 테스트에서만 작동 하도록 한다.
//==============================================================================================
#ifndef __PATH__
#include ".\kstats.h"
//



//==============================================================================================
//                                       KStatsSegment
//==============================================================================================
KStatsSegment::KStatsSegment( UINT uiType_, std::wstring strName_, void* pPtr_ )
{
    m_uiType = uiType_;
    m_strName = strName_;
    memset( m_strBuffer, 0, sizeof(WCHAR)*KSTATS_VALUE_LENGTH );
    memset( m_strInitValue, 0, sizeof(WCHAR)*KSTATS_VALUE_LENGTH );
    m_pPtr = pPtr_;
    switch( m_uiType )
    {
        case STATS_TYPE_INT:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%d", *((int*)m_pPtr) );
            break;
        }
        case STATS_TYPE_FLOAT:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%f", *((float*)m_pPtr) );
            break;
        }
        case STATS_TYPE_UINT:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%u", *((UINT*)m_pPtr) );
            break;
        }
        case STATS_TYPE_DWORD:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%u", *((DWORD*)m_pPtr) );
            break;
        }
        case STATS_TYPE_PT_INT:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%d", (int)(*((KProtectedType<int>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_PT_FLOAT:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%f", (float)(*((KProtectedType<float>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_PT_UINT:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%d", (UINT)(*((KProtectedType<UINT>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_PT_CHAR:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%d", (int)(*((KProtectedType<float>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_BOOL:
        {
            swprintf( m_strInitValue, KSTATS_VALUE_LENGTH, L"%s", *((bool*)m_pPtr) == true ? L"true" : L"false" );
            break;
        }
    }
}
KStatsSegment::~KStatsSegment()
{
    m_pPtr = NULL;
}
WCHAR* KStatsSegment::ToString()
{
    memset( m_strBuffer, 0, sizeof(WCHAR)*KSTATS_VALUE_LENGTH );
    switch( m_uiType )
    {
        case STATS_TYPE_INT:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %d", m_strName.c_str(), *((int*)m_pPtr) );
            break;
        }
        case STATS_TYPE_FLOAT:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %f", (char*)m_strName.c_str(), *((float*)m_pPtr) );
            break;
        }
        case STATS_TYPE_UINT:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %u", m_strName.c_str(), *((UINT*)m_pPtr) );
            break;
        }
        case STATS_TYPE_DWORD:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %u", m_strName.c_str(), *((DWORD*)m_pPtr) );
            break;
        }
        case STATS_TYPE_PT_INT:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %d", m_strName.c_str(), (int)(*((KProtectedType<int>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_PT_FLOAT:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %f", m_strName.c_str(), (float)(*((KProtectedType<float>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_PT_UINT:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %d", m_strName.c_str(), (UINT)(*((KProtectedType<UINT>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_PT_CHAR:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %d", m_strName.c_str(), (int)(*((KProtectedType<float>*)m_pPtr)) );
            break;
        }
        case STATS_TYPE_BOOL:
        {
            swprintf( m_strBuffer, KSTATS_VALUE_LENGTH, L"[%s] : %s", m_strName.c_str(), *((bool*)m_pPtr) == true ? L"true" : L"false" );
            break;
        }
        case STATS_TYPE_STATIC_TEXT:
        {
            return (WCHAR*)m_strName.c_str();
        }
    }
    return m_strBuffer;
}
void KStatsSegment::RestoreValue()
{
    SetValue( m_strInitValue );
}
void KStatsSegment::SetValue( WCHAR* pValue )
{
    switch( m_uiType )
    {
        case STATS_TYPE_INT:
        {
            *((int*)m_pPtr) = _wtoi( pValue );
            break;
        }
        case STATS_TYPE_FLOAT:
        {
            *((float*)m_pPtr) = (float)_wtof( pValue );
            break;
        }
        case STATS_TYPE_UINT:
        {
            *((UINT*)m_pPtr) = (UINT)_wtoi( pValue );
            break;
        }
        case STATS_TYPE_DWORD:
        {
            *((DWORD*)m_pPtr) = (DWORD)_wtoi( pValue );
            break;
        }
        case STATS_TYPE_PT_INT:
        {
            *((KProtectedType<int>*)m_pPtr) = _wtoi(pValue);
            break;
        }
        case STATS_TYPE_PT_FLOAT:
        {
            *((KProtectedType<float>*)m_pPtr) = (float)_wtof(pValue);
            break;
        }
        case STATS_TYPE_PT_UINT:
        {
            *((KProtectedType<UINT>*)m_pPtr) = (UINT)_wtof(pValue);
            break;
        }
        case STATS_TYPE_PT_CHAR:
        {
            *((KProtectedType<char>*)m_pPtr) = (char)_wtoi(pValue);
            break;
        }
        case STATS_TYPE_BOOL:
        {
            wcslwr( pValue );
            if ( wcscmp( pValue, L"true" ) == 0 )
                *((bool*)m_pPtr) = true;
            else
                *((bool*)m_pPtr) = false;
            break;
        }
    }
}

//==============================================================================================
//                                       KStatsPage
//==============================================================================================

KStatsPage::KStatsPage( std::wstring strName_, D3DCOLOR dwPageNameColor_, D3DCOLOR dwTextColor_, D3DCOLOR dwSelectedTextColor_ )
{
    m_strName = strName_;
    m_pkCurSegment = NULL;
    m_dwPageNameColor = dwPageNameColor_;
    m_dwTextColor = dwTextColor_;
    m_dwSelectedTextColor = dwSelectedTextColor_;
    m_uiCurIndex = 0;
    m_uiIndexOnScreen = 0;
}

KStatsPage::~KStatsPage()
{
    CleanUp();
}

void KStatsPage::CleanUp()
{
    KStatsLocker kLock( m_kPageCS.GetCS() );
    int iCount = (int)m_vecSegment.size();
    for ( int i = 0; i < iCount; i++ )
    {
        SAFE_DELETE( m_vecSegment[i] );
    }
    m_vecSegment.clear();
    m_pkCurSegment = NULL;
}

void KStatsPage::Print()
{
    KStatsLocker kLock( m_kPageCS.GetCS() );
    char strFileName[KSTATS_VALUE_LENGTH] = {0, };
    sprintf( strFileName, "./%s.txt", GCUTIL_STR::GCStrWideToChar( m_strName.c_str() ));
    FILE* pFIle = fopen( strFileName, "w" );
    for ( int i = 0; i < (int)m_vecSegment.size(); i++ )
    {
        fwrite( GCUTIL_STR::GCStrWideToChar( m_vecSegment[i]->ToString() ), sizeof(char),  
                strlen( GCUTIL_STR::GCStrWideToChar( m_vecSegment[i]->ToString() )), pFIle );
        fwrite( "\n", sizeof(char), strlen( "\n" ), pFIle );
    }
    fclose( pFIle );
}

bool KStatsPage::operator == ( KStatsPage* pPage_ )
{
    if ( pPage_->GetName() == GetName() )
        return true;
    return false;
}

std::wstring KStatsPage::GetName()
{
    return m_strName;
}
void KStatsPage::InsertValue( UINT uiType_, std::wstring strName_, void* pPtr_, bool bExceptSameSegment_ )
{
    KStatsLocker kLock( m_kPageCS.GetCS() );
    if ( bExceptSameSegment_ == false )
    {
        for ( int i = 0; i < (int)m_vecSegment.size(); i++ )
        {
            if ( strName_ == m_vecSegment[i]->GetName() )
                return;
        }
    }
    KStatsSegment* pSegment = new KStatsSegment( uiType_, strName_, pPtr_ );
    m_vecSegment.push_back( pSegment );
    m_pkCurSegment = m_vecSegment[0];
    m_uiCurIndex = 0;
    m_uiIndexOnScreen = 0;
}
void KStatsPage::SetValue( WCHAR* pStrValue_ ) 
{
    if ( m_pkCurSegment == NULL )
        return;

    m_pkCurSegment->SetValue( pStrValue_ );
}
KStatsSegment* KStatsPage::GetSegment( int iIndex_ )
{
    if ( (int)m_vecSegment.size() < iIndex_ )
        return NULL;
    return m_vecSegment[iIndex_];
}

void KStatsPage::RestorePage()
{
    KStatsLocker kLock( m_kPageCS.GetCS() );
    for( int i = 0; i < (int)m_vecSegment.size(); i++ )
    {
        m_vecSegment[i]->RestoreValue();
    }
}
void KStatsPage::RestoreCurSeg()
{
    if ( m_pkCurSegment != NULL )
        m_pkCurSegment->RestoreValue();
}

void KStatsPage::SetStartPos( POINT kStartPos_ )
{
    m_kStartPos = kStartPos_;
    m_kCurPos = kStartPos_;
}
void KStatsPage::Next()
{
    if( m_vecSegment.size() == 0 || m_uiCurIndex == m_vecSegment.size() - 1 )
        return;
    m_uiCurIndex++;
    m_uiIndexOnScreen++;
    if ( KSTATS_VIEW_SEGMENT_NUM - 1 <= m_uiIndexOnScreen)
        m_uiIndexOnScreen = KSTATS_VIEW_SEGMENT_NUM - 1;
    m_pkCurSegment = m_vecSegment[m_uiCurIndex];
}
void KStatsPage::Previous()
{
    if ( m_vecSegment.size() == 0 || m_uiCurIndex == 0 )
        return;
    m_uiCurIndex--;
    if ( 0 < m_uiIndexOnScreen )
        m_uiIndexOnScreen--;
    m_pkCurSegment = m_vecSegment[m_uiCurIndex];
}

void KStatsPage::Render( UINT uiCurIndex_, UINT uiSize_ )
{
//    g_pkFontMgr->SetFontSize( KSTATS_LINE_HEIGHT );
    memset( m_strPageName, 0, sizeof(WCHAR)*(KSTATS_VALUE_LENGTH + 32) );
    memset( m_strCurSegInfo, 0, sizeof(WCHAR)*(KSTATS_VALUE_LENGTH + 32) );
    _snwprintf( m_strPageName, KSTATS_VALUE_LENGTH + 32, L"[[  %s : %u / %u  ]]", m_strName.c_str(), uiCurIndex_, uiSize_ );
    // 개인적으로 ? <<< 요놈 싫어 하지만...if 넣기가 좀 뭐해서-_-;;
    _snwprintf( m_strCurSegInfo, KSTATS_VALUE_LENGTH + 32, L"[[ %u / %u ]]", m_vecSegment.size() == 0 ? 0 : m_uiCurIndex + 1, m_vecSegment.size() );
    DrawText( m_strPageName, m_dwPageNameColor );
    DrawText( L"", m_dwPageNameColor );
    DrawText( m_strCurSegInfo, m_dwPageNameColor );

    UINT uiSkipCount = m_uiCurIndex - m_uiIndexOnScreen;
    for ( UINT i = uiSkipCount; i < uiSkipCount + KSTATS_VIEW_SEGMENT_NUM; i++ )
    {
        if ( m_vecSegment.size() <= i )
            break;
        if ( m_vecSegment[i] == m_pkCurSegment )
        {
            DrawText( m_vecSegment[i]->ToString(), m_dwSelectedTextColor );
        }
        else
        {
            DrawText( m_vecSegment[i]->ToString(), m_dwTextColor  );
        }
    }
}
void KStatsPage::DrawText( WCHAR* str_, D3DCOLOR dwColor_ )
{
    //DWORD dwPrevColor = g_pkFontMgr->GetFontColor();
    //g_pkFontMgr->SetFontColor( dwColor_ );
    g_pkFontMgr->OutTextXY( m_kCurPos.x, m_kCurPos.y, str_, dwColor_ );
    //g_pkFontMgr->SetFontColor( dwPrevColor );
    m_kCurPos.y += KSTATS_LINE_HEIGHT;
}

//==============================================================================================
//                                       KStatsManager
//==============================================================================================
KStatsManager g_kStatsManager;

KStatsManager::KStatsManager(void)
{
    m_pkCurPage = NULL;
    m_dwTextColor = D3DCOLOR_RGBA( 255, 255, 255, 255 );
    m_dwSelectedTextColor = D3DCOLOR_RGBA( 255, 0, 0, 255 );
    m_dwPageNameColor = D3DCOLOR_RGBA( 255, 255, 0, 255 );
    m_bModeHUD = false;
    m_bEaglesite = false;
}

KStatsManager::~KStatsManager(void)
{
    int iCount = (int)m_vecPage.size();
    for ( int i = 0; i < iCount; i++ )
    {
        SAFE_DELETE( m_vecPage[i].m_pPage );
    }
    m_vecPage.clear();
    m_pkCurPage = NULL;
}
void KStatsManager::CleanUp()
{
    int iCount = (int)m_vecPage.size();
    for ( int i = 0; i < iCount; i++ )
    {
        SAFE_DELETE( m_vecPage[i].m_pPage );
    }
    m_vecPage.clear();
    m_pkCurPage = NULL;
}

void KStatsManager::InsertPage( std::wstring strName_ )
{
    std::wstring strNameTemp = strName_;
    wcslwr( (WCHAR*)strNameTemp.c_str() );
    if ( Find( strNameTemp ) )
    {
        InsertLog( L"Exist Page, Delete Old Page And Insert Page" );
        DeletePage( strNameTemp );
    }
    KStatsPage* pPage = new KStatsPage( strName_, m_dwPageNameColor, m_dwTextColor, m_dwSelectedTextColor );
    KElement kElement( strNameTemp, pPage );
    m_vecPage.push_back( kElement );
    m_pkCurPage = pPage;
    m_uiCurIndex = (UINT)m_vecPage.size() - 1;
}

void KStatsManager::DeletePage( std::wstring strName_ )
{
    KStatsPage* pPage = GetPage( (WCHAR*)strName_.c_str() );
    if ( pPage == NULL )
    {
        std::wstring strError = L"Delete Page Error : " + strName_;
        InsertLog( (WCHAR*)strError.c_str() );
        return;
    }

    Previous();

    for ( int i = 0; i < (int)m_vecPage.size(); i++ )
    {
        std::wstring strName = pPage->GetName();
        wcslwr( (WCHAR*)strName.c_str() );
        if ( strName == m_vecPage[i].m_strName )
        {
            m_vecPage.erase( m_vecPage.begin() + i );
            SAFE_DELETE( pPage );
            break;
        }
    }

    if( m_vecPage.empty() )
    {
        m_pkCurPage = NULL;
        m_uiCurIndex = -1;
    }
}

bool KStatsManager::Find( std::wstring strName_ )
{
    std::wstring strNameTemp = strName_;
    wcslwr( (WCHAR*)strNameTemp.c_str() );
    for ( int i = 0; i < (int)m_vecPage.size(); i++ )
    {
        if ( m_vecPage[i].m_strName == strNameTemp )
            return true;
    }
    return false;
}

void KStatsManager::Next()
{
    if( m_vecPage.size() == 0 || m_uiCurIndex == m_vecPage.size() - 1)
        return;
    m_uiCurIndex++;
    m_pkCurPage = m_vecPage[m_uiCurIndex].m_pPage;
}
void KStatsManager::Previous()
{
    if ( m_vecPage.size() == 0 || m_uiCurIndex == 0 )
        return;
    m_uiCurIndex--;
    m_pkCurPage = m_vecPage[m_uiCurIndex].m_pPage;
}

KStatsPage* KStatsManager::GetPage( WCHAR* pStrName_ )
{
    if ( pStrName_ == NULL )
        return m_pkCurPage;
    std::wstring strNameTemp = pStrName_;
    wcslwr( (WCHAR*)strNameTemp.c_str() );
    for ( int i = 0; i < (int)m_vecPage.size(); i++ )
    {
        if ( m_vecPage[i].m_strName == strNameTemp )
            return m_vecPage[i].m_pPage;
    }
    return NULL;
}

void KStatsManager::InsertLog( WCHAR* str_ )
{
    std::wstring str = str_;
    if ( m_vecLog.size() > KSTATS_LOG_NUM )
    {
        m_vecLog.erase( m_vecLog.begin() );
    }
    m_vecLog.push_back( str );
}

void KStatsManager::InsertStaticText( WCHAR* pStr_ )
{
    GetPage( L"STATIC_TEXT" )->InsertValue( STATS_TYPE_STATIC_TEXT, pStr_, NULL );
}
void KStatsManager::InsertDebugValue( WCHAR* pStr_ )
{
    GetPage( L"DEBUG_VALUE" )->InsertValue( STATS_TYPE_STATIC_TEXT, pStr_, NULL, true );
}
void KStatsManager::ClearDebugValue()
{
    GetPage( L"DEBUG_VALUE" )->CleanUp();
}

void KStatsManager::InsertValue( UINT uiType_, std::wstring strName_, void* pPtr_, KStatsPage* pPage_ )
{
    if ( pPage_ == GetPage( L"DEBUG_VALUE" ) || pPage_ == GetPage( L"STATIC_TEXT" ) )
        return;
    pPage_->InsertValue( uiType_, strName_, pPtr_ );
}
void KStatsManager::InsertValue( std::wstring strName_, int* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_INT, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, UINT* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_UINT, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, FLOAT* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_FLOAT, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, KProtectedType<int>* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_PT_INT, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, KProtectedType<float>* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_PT_FLOAT, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, KProtectedType<UINT>* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_PT_UINT, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, KProtectedType<char>* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_PT_CHAR, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, KProtectedType<UCHAR>* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_PT_UCHAR, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, bool* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_BOOL, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}
void KStatsManager::InsertValue( std::wstring strName_, DWORD* pPtr_, WCHAR* pPageName_/* = NULL */)
{
    InsertValue( STATS_TYPE_DWORD, strName_, (void*)pPtr_, GetPage( pPageName_ ) );
}

void KStatsManager::SetValue( WCHAR* pStrValue_ )
{
    if ( m_bEnable == false )
        return;

    m_pkCurPage->SetValue( pStrValue_ );
}

void KStatsManager::Init( LONG lStartX_, LONG lStartY_, DWORD dwColor_, bool bEnable_ )
{   
    POINT kStartPos;
    kStartPos.x = lStartX_;
    kStartPos.y = lStartY_;
    SetStartPos( kStartPos );
    m_dwTextColor = dwColor_;
    m_dwSelectedTextColor = D3DCOLOR_RGBA( 255, 0, 0, 255 );
    m_dwPageNameColor = D3DCOLOR_RGBA( 255, 255, 0, 255 );
    InsertPage( L"STATIC_TEXT" );
    {
        InsertHandler();
        InsertHelperHandler();
        InsertStaticText( L"// : Make Full MP, HP" );
        InsertStaticText( L"/item itemname : Insert Item To Player Inventory" );
        InsertStaticText( L"/next, /next stagenumber : Go To Next Stage or StageNumber" );
        InsertStaticText( L"/sv ID value : Change ID's To value" );
        InsertStaticText( L"/reload enum : Reload Script" );
        InsertStaticText( L"/fireinfo num : Modify Damage Script_FireInfo Script" );
        InsertStaticText( L"/rfi : ReloadFireInfo" );
        InsertStaticText( L"/print_page : Print Current Page" );
        InsertStaticText( L"/drop_item value : ItemDrop" );
        InsertStaticText( L"/reloadai filename ainame : ReloadAI" );
        InsertStaticText( L"/device : Print Device List" );
        InsertStaticText( L"/aienable value : NewAI Operator" );
        InsertStaticText( L"/player_action motion, frame : Player action perform" );
        InsertStaticText( L"eaglesite : EagleSight Effect" );
        InsertStaticText( L"reloadaction : Action Script Reload" );
        InsertStaticText( L"reloadpet : Pet Script Reload" );
        InsertStaticText( L"reloadmission : Mission Script Reload" );
        InsertStaticText( L"addusermission missionid: missionid add" );
        InsertStaticText( L"reloadmonster : reload MonsterTemplate.stg" );
        InsertStaticText( L"masterarcher : Set ArcherContest Score" );
        InsertStaticText( L"checkitemimage : Check All Items' existence of Shop Image, Mesh and Texture" );
        InsertStaticText( L"/announce type msg : test for announce" );
		InsertStaticText( L"makegif : gif recording mode." );
        InsertStaticText( L"/aki or /aki PlayerIdx : ALLKill And GetItem" );
        InsertStaticText( L"/console : Open Console Window" );
    }
    InsertPage( L"DEBUG_VALUE" );
    SetEnable( bEnable_ );
}

void KStatsManager::InsertHandler()
{
    //무조건 소문자로.
    m_mapHandler[L"/"] = &KStatsManager::OnFullHPMP;
    m_mapHandler[L"next"] = &KStatsManager::OnNextStage;
    m_mapHandler[L"wsize"] = &KStatsManager::OnReSizeWindow;
    m_mapHandler[L"crash"] = &KStatsManager::MakeCrash;
    m_mapHandler[L"sv"] = &KStatsManager::OnSetValue;
    m_mapHandler[L"reload"] = &KStatsManager::OnReloadScript;
    m_mapHandler[L"print_page"] = &KStatsManager::OnPrintPage;
    m_mapHandler[L"gi"] = &KStatsManager::OnDropGameItem;
    m_mapHandler[L"reloadai"] = &KStatsManager::OnReload_AI;
    m_mapHandler[L"device"] = &KStatsManager::OnDeviceList;
    m_mapHandler[L"aienable"] = &KStatsManager::OnAIEnable;
    m_mapHandler[L"rendermonster"] = &KStatsManager::OnRenderMonster;
    m_mapHandler[L"player_action"] = &KStatsManager::OnPlayerAction;
    m_mapHandler[L"eaglesite"] = &KStatsManager::OnEagleSite;
    m_mapHandler[L"ra"] = &KStatsManager::OnActionScript;
    m_mapHandler[L"rfi"] = &KStatsManager::OnReloadFireInfo;
    m_mapHandler[L"reloadpet"] = &KStatsManager::OnReloadPet;
    m_mapHandler[L"rpt"] = &KStatsManager::OnReloadPlayerTemplate;
    m_mapHandler[L"outputchat"] = &KStatsManager::OnOutputChat;
    m_mapHandler[L"dummyreplay"] = &KStatsManager::OnDummyReplay;
    m_mapHandler[L"1"] = &KStatsManager::OnReloadMonster; // 치기 쉽게 하기 위해서 1번으로 설정해뒀음..=ㅅ= 몬스터 스크립트 리로드..
    m_mapHandler[L"checkitemimage"] = &KStatsManager::OnCheckSbta;
    m_mapHandler[L"setlevel"] = &KStatsManager::OnSetPlayerLevel;
    m_mapHandler[L"announce"] = &KStatsManager::OnAnnounce;
    m_mapHandler[L"announcelist"] = &KStatsManager::OnAnnounceList;
    m_mapHandler[L"makegif"] = &KStatsManager::OnMakeGif;
    m_mapHandler[L"luamode"] = &KStatsManager::OnLuaMode;
    m_mapHandler[L"log"] = &KStatsManager::OnLogLevel;
    m_mapHandler[L"buff"] = &KStatsManager::OnBuff;
    m_mapHandler[L"adddamage"] = &KStatsManager::OnAddDamage;
    m_mapHandler[L"trainskill"] = &KStatsManager::OnSkillTrain;
    m_mapHandler[L"untrainskill"] = &KStatsManager::OnSkillUnTrain;
    m_mapHandler[L"dumpskillsetting"] = &KStatsManager::OnDumpSkillSetting;
    m_mapHandler[L"dumpskill"] = &KStatsManager::OnDumpSkill;
    m_mapHandler[L"setskillsetting"] = &KStatsManager::SetSkillSetting;
    m_mapHandler[L"applyskill"] = &KStatsManager::ApplySetting;
    m_mapHandler[L"equipskill"] = &KStatsManager::OnEquipSkill;
    m_mapHandler[L"unequipskill"] = &KStatsManager::OnUnEquipSkill;
    m_mapHandler[L"unlockgroup"] = &KStatsManager::OnUnlockGroup;
    m_mapHandler[L"ak"] = &KStatsManager::OnAllKill;
    m_mapHandler[L"aki"] =  &KStatsManager::OnAllKillItem;
    m_mapHandler[L"lo"] = &KStatsManager::OnLogOff;
    m_mapHandler[L"sp75"] = &KStatsManager::OnReloadSpecial75Texture;
    m_mapHandler[L"stlcrash"] =  &KStatsManager::OnSTLCrashTest;
    m_mapHandler[L"accessviolation"] =  &KStatsManager::OnAccessViolation;
    m_mapHandler[L"cppexception"] =  &KStatsManager::OnCppException;
    m_mapHandler[L"god"] =  &KStatsManager::OnGodMode;
    m_mapHandler[L"perfect"] =  &KStatsManager::OnPerfect;
    m_mapHandler[L"cl"] =  &KStatsManager::OnClear;
    m_mapHandler[L"print_itemdramainfo"] =  &KStatsManager::OnPrintDungeonItemDramaInfo;
    m_mapHandler[L"addmon"] = &KStatsManager::OnAddMonsterInQuest;
    m_mapHandler[L"hud"] =  &KStatsManager::OnToggleHUD;
    m_mapHandler[L"ma"] =  &KStatsManager::OnMonsterAction;
    m_mapHandler[L"tq"] =  &KStatsManager::OnTQ;
    m_mapHandler[L"공격궤적"] =  &KStatsManager::OnToggleAttackSlash;
    m_mapHandler[L"rhi"] =  &KStatsManager::OnReloadHeroItemInfo;
    m_mapHandler[L"box"] =  &KStatsManager::OnItemBoxAlphaToggle;
    m_mapHandler[L"replayopen"] =  &KStatsManager::OnReplayOpen;
    m_mapHandler[L"cri"] =  &KStatsManager::OnCriticalToggle;
    m_mapHandler[L"font"] =  &KStatsManager::OnChangeFont;
    m_mapHandler[L"wnd"] =  &KStatsManager::OnWndInfo;
    m_mapHandler[L"console"] =  &KStatsManager::OnOpenConsole;
    m_mapHandler[L"motion"] =  &KStatsManager::OnMotion;
    m_mapHandler[L"m"] =  &KStatsManager::OnMotion;
    m_mapHandler[L"petmotion"] =  &KStatsManager::OnPetMotion;

    m_mapHandler[L"dmg"] = &KStatsManager::OnSetDamageRaito;
    m_mapHandler[L"pdamage"] = &KStatsManager::OnCheckParentDamage;

    m_mapHandler[L"crioff"] = &KStatsManager::OnCriticalOffToggle;

    m_mapHandler[L"changesc"] = &KStatsManager::OnSwitchScreenShot;

    // 테스트
    m_mapHandler[L"exit"] = &KStatsManager::OnGoExit;

    m_mapHandler[L"writeframe"] = &KStatsManager::OnWriteFrame;
    m_mapHandler[L"pid"] = &KStatsManager::OnProcessID;


    m_mapHandler[L"drama"] = &KStatsManager::OnTestDrama;
    m_mapHandler[L"dlg"] = &KStatsManager::OnDialog;

    m_mapHandler[L"tcptest"] = &KStatsManager::OnTcpTest;
    m_mapHandler[L"host"] = &KStatsManager::OnHost;
    

    // Qa용 로그파일 쓰기 
    m_mapHandler[L"logfile"] = &KStatsManager::OnWriteQALog;
    m_mapHandler[L"sf"] = &KStatsManager::OnShowFootHold;
    m_mapHandler[L"st"] = &KStatsManager::OnSendTimeForP2PPacket;
    m_mapHandler[L"drop"] = &KStatsManager::OnDropItem;

    m_mapHandler[L"combo"] =  &KStatsManager::OnComboEffectShow;
    m_mapHandler[L"face"] = &KStatsManager::OnSpecialFaceShow;

    m_mapHandler[L"hs"] = &KStatsManager::OnRecoverHP;
    m_mapHandler[L"ㅗㄴ"] = &KStatsManager::OnRecoverHP;
    m_mapHandler[L"ms"] = &KStatsManager::OnRecoverMP;
    m_mapHandler[L"ㅡㄴ"] = &KStatsManager::OnRecoverMP;
    m_mapHandler[L"hp"] = &KStatsManager::OnPlusHP;
    m_mapHandler[L"ㅗㅔ"] = &KStatsManager::OnPlusHP;
    m_mapHandler[L"mp"] = &KStatsManager::OnPlusMP;
    m_mapHandler[L"ㅡㅔ"] = &KStatsManager::OnPlusMP;
    m_mapHandler[L"p"] = &KStatsManager::OnPlusPetMP;
    m_mapHandler[L"ㅔ"] = &KStatsManager::OnPlusPetMP;
    m_mapHandler[L"e"] = &KStatsManager::OnEagleSite;
    m_mapHandler[L"ㄷ"] = &KStatsManager::OnEagleSite;
    m_mapHandler[L"f"] = &KStatsManager::OnFrameLock;
    m_mapHandler[L"ㄹ"] = &KStatsManager::OnFrameLock;
    m_mapHandler[L"c"] =  &KStatsManager::OnCriticalToggle;
    m_mapHandler[L"ㅊ"] =  &KStatsManager::OnCriticalToggle;
    m_mapHandler[L"em"] = &KStatsManager::OnSummonEmbark;

    m_mapHandler[L"help"] =  &KStatsManager::OnHelp;
    m_mapHandler[L"profile"] = &KStatsManager::OnProfileToggle;
    m_mapHandler[L"inven"] = &KStatsManager::OnInvenDump;

    // 스킬창 열어주심
    m_mapHandler[L"s"] = &KStatsManager::OnOpenSkillTreeUI;
    m_mapHandler[L"ㄴ"] = &KStatsManager::OnOpenSkillTreeUI;
    m_mapHandler[L"skill"] = &KStatsManager::OnOpenSkillTreeUI;
    m_mapHandler[L"나ㅑㅣㅣ"] = &KStatsManager::OnOpenSkillTreeUI;

    m_mapHandler[L"img"] = &KStatsManager::OnShowTestImg;
    m_mapHandler[L"wa"] = &KStatsManager::OnAddWall;	
    m_mapHandler[L"sj"] = &KStatsManager::OnAddSubject;
    m_mapHandler[L"tg"] = &KStatsManager::OnTGPrint;

    // 테스트용 Static 컨트롤 
    m_mapHandler[L"ts"] = &KStatsManager::OnToggleTestStatic;
}

void KStatsManager::InsertHelperHandler()
{
    m_mapHelperHandler[L"/"] = &KStatsManager::OnFullHPMP_Helper;
    m_mapHelperHandler[L"next"] = &KStatsManager::OnNextStage_Helper;
    m_mapHelperHandler[L"wsize"] = &KStatsManager::OnReSizeWindow_Helper;
    m_mapHelperHandler[L"crash"] = &KStatsManager::MakeCrash_Helper;
    m_mapHelperHandler[L"sv"] = &KStatsManager::OnSetValue_Helper;
    m_mapHelperHandler[L"reload"] = &KStatsManager::OnReloadScript_Helper;
    m_mapHelperHandler[L"print_page"] = &KStatsManager::OnPrintPage_Helper;
    m_mapHelperHandler[L"gi"] = &KStatsManager::OnDropGameItem_Helper;
    m_mapHelperHandler[L"device"] = &KStatsManager::OnDeviceList_Helper;
    m_mapHelperHandler[L"aienable"] = &KStatsManager::OnAIEnable_Helper;
    m_mapHelperHandler[L"rendermonster"] = &KStatsManager::OnRenderMonster_Helper;
    m_mapHelperHandler[L"player_action"] = &KStatsManager::OnPlayerAction_Helper;
    m_mapHelperHandler[L"eaglesite"] = &KStatsManager::OnEagleSite_Helper;
    m_mapHelperHandler[L"ra"] = &KStatsManager::OnActionScript_Helper;
    m_mapHelperHandler[L"rfi"] = &KStatsManager::OnReloadFireInfo_Helper;
    m_mapHelperHandler[L"reloadpet"] = &KStatsManager::OnReloadPet_Helper;
    m_mapHelperHandler[L"dummyreplay"] = &KStatsManager::OnDummyReplay_Helper;
    m_mapHelperHandler[L"1"] = &KStatsManager::OnReloadMonster_Helper;
    m_mapHelperHandler[L"checkitemimage"] = &KStatsManager::OnCheckSbta_Helper;
    m_mapHelperHandler[L"setlevel"] = &KStatsManager::OnSetPlayerLevel_Helper;
    m_mapHelperHandler[L"makegif"] = &KStatsManager::OnMakeGif_Helper;
    m_mapHelperHandler[L"luamode"] = &KStatsManager::OnLuaMode_Helper;
    m_mapHelperHandler[L"log"] = &KStatsManager::OnLogLevel_Helper;
    m_mapHelperHandler[L"buff"] = &KStatsManager::OnBuff_Helper;
    m_mapHelperHandler[L"adddamage"] = &KStatsManager::OnAddDamage_Helper;
    m_mapHelperHandler[L"trainskill"] = &KStatsManager::OnSkillTrain_Helper;
    m_mapHelperHandler[L"untrainskill"] = &KStatsManager::OnSkillUnTrain_Helper;
    m_mapHelperHandler[L"dumpskillsetting"] = &KStatsManager::OnDumpSkillSetting_Helper;
    m_mapHelperHandler[L"dumpskill"] = &KStatsManager::OnDumpSkill_Helper;
    m_mapHelperHandler[L"applyskill"] = &KStatsManager::ApplySetting_Helper;
    m_mapHelperHandler[L"equipskill"] = &KStatsManager::OnEquipSkill_Helper;
    m_mapHelperHandler[L"unequipskill"] = &KStatsManager::OnUnEquipSkill_Helper;
    m_mapHelperHandler[L"unlockgroup"] = &KStatsManager::OnUnlockGroup_Helper;
    m_mapHelperHandler[L"ak"] = &KStatsManager::OnAllKill_Helper;
    m_mapHelperHandler[L"aki"] =  &KStatsManager::OnAllKillItem_Helper;
    m_mapHelperHandler[L"lo"] = &KStatsManager::OnLogOff_Helper;
    m_mapHelperHandler[L"sp75"] = &KStatsManager::OnReloadSpecial75Texture_Helper;
    m_mapHelperHandler[L"stlcrash"] =  &KStatsManager::OnSTLCrashTest_Helper;
    m_mapHelperHandler[L"accessviolation"] =  &KStatsManager::OnAccessViolation_Helper;
    m_mapHelperHandler[L"god"] =  &KStatsManager::OnGodMode_Helper;
    m_mapHelperHandler[L"perfect"] =  &KStatsManager::OnPerfect_Helper;
    m_mapHelperHandler[L"cl"] =  &KStatsManager::OnClear_Helper;
    m_mapHelperHandler[L"print_itemdramainfo"] =  &KStatsManager::OnPrintDungeonItemDramaInfo_Helper;
    m_mapHelperHandler[L"addmon"] = &KStatsManager::OnAddMonsterInQuest_Helper;
    m_mapHelperHandler[L"hud"] =  &KStatsManager::OnToggleHUD_Helper;
    m_mapHelperHandler[L"ma"] =  &KStatsManager::OnMonsterAction_Helper;
    m_mapHelperHandler[L"tq"] =  &KStatsManager::OnTQ_Helper;
    m_mapHelperHandler[L"공격궤적"] =  &KStatsManager::OnToggleAttackSlash_Helper;
    m_mapHelperHandler[L"rhi"] =  &KStatsManager::OnReloadHeroItemInfo_Helper;
    m_mapHelperHandler[L"box"] =  &KStatsManager::OnItemBoxAlphaToggle_Helper;
    m_mapHelperHandler[L"replayopen"] =  &KStatsManager::OnReplayOpen_Helper;
    m_mapHelperHandler[L"cri"] =  &KStatsManager::OnCriticalToggle_Helper;
    m_mapHelperHandler[L"font"] =  &KStatsManager::OnChangeFont_Helper;
    m_mapHelperHandler[L"wnd"] =  &KStatsManager::OnWndInfo_Helper;
    m_mapHelperHandler[L"console"] =  &KStatsManager::OnOpenConsole_Helper;
    m_mapHelperHandler[L"motion"] =  &KStatsManager::OnMotion_Helper;
    m_mapHelperHandler[L"m"] =  &KStatsManager::OnMotion_Helper;
    m_mapHelperHandler[L"petmotion"] =  &KStatsManager::OnPetMotion_Helper;
    m_mapHelperHandler[L"dmg"] = &KStatsManager::OnSetDamageRaito_Helper;
    m_mapHelperHandler[L"pdamage"] = &KStatsManager::OnCheckParentDamage_Helper;
    m_mapHelperHandler[L"crioff"] = &KStatsManager::OnCriticalOffToggle_Helper;
    m_mapHelperHandler[L"changesc"] = &KStatsManager::OnSwitchScreenShot_Helper;
    m_mapHelperHandler[L"exit"] = &KStatsManager::OnGoExit_Helper;
    m_mapHelperHandler[L"writeframe"] = &KStatsManager::OnWriteFrame_Helper;
    m_mapHelperHandler[L"pid"] = &KStatsManager::OnProcessID_Helper;
    m_mapHelperHandler[L"logfile"] = &KStatsManager::OnWriteQALog_Helper;
    m_mapHelperHandler[L"sf"] = &KStatsManager::OnShowFootHold_Helper;
    m_mapHelperHandler[L"st"] = &KStatsManager::OnSendTimeForP2PPacket_Helper;
    m_mapHelperHandler[L"combo"] =  &KStatsManager::OnComboEffectShow_Helper;
    m_mapHelperHandler[L"face"] = &KStatsManager::OnSpecialFaceShow_Helper;
    m_mapHelperHandler[L"hs"] = &KStatsManager::OnRecoverHP_Helper;
    m_mapHelperHandler[L"ㅗㄴ"] = &KStatsManager::OnRecoverHP_Helper;
    m_mapHelperHandler[L"ms"] = &KStatsManager::OnRecoverMP_Helper;
    m_mapHelperHandler[L"ㅡㄴ"] = &KStatsManager::OnRecoverMP_Helper;
    m_mapHelperHandler[L"hp"] = &KStatsManager::OnPlusHP_Helper;
    m_mapHelperHandler[L"ㅗㅔ"] = &KStatsManager::OnPlusHP_Helper;
    m_mapHelperHandler[L"mp"] = &KStatsManager::OnPlusMP_Helper;
    m_mapHelperHandler[L"ㅡㅔ"] = &KStatsManager::OnPlusMP_Helper;
    m_mapHelperHandler[L"p"] = &KStatsManager::OnPlusPetMP_Helper;
    m_mapHelperHandler[L"ㅔ"] = &KStatsManager::OnPlusPetMP_Helper;
    m_mapHelperHandler[L"e"] = &KStatsManager::OnEagleSite_Helper;
    m_mapHelperHandler[L"ㄷ"] = &KStatsManager::OnEagleSite_Helper;
    m_mapHelperHandler[L"f"] = &KStatsManager::OnFrameLock_Helper;
    m_mapHelperHandler[L"ㄹ"] = &KStatsManager::OnFrameLock_Helper;
    m_mapHelperHandler[L"c"] =  &KStatsManager::OnCriticalToggle_Helper;
    m_mapHelperHandler[L"ㅊ"] =  &KStatsManager::OnCriticalToggle_Helper;
    m_mapHelperHandler[L"em"] = &KStatsManager::OnSummonEmbark_Helper;
    m_mapHelperHandler[L"inven"] = &KStatsManager::OnInvenDump_Helper;
    m_mapHelperHandler[L"s"] = &KStatsManager::OnOpenSkillTreeUI_Helper;
    m_mapHelperHandler[L"ㄴ"] = &KStatsManager::OnOpenSkillTreeUI_Helper;
    m_mapHelperHandler[L"skill"] = &KStatsManager::OnOpenSkillTreeUI_Helper;
    m_mapHelperHandler[L"나ㅑㅣㅣ"] = &KStatsManager::OnOpenSkillTreeUI_Helper;
    m_mapHelperHandler[L"dlg"] = &KStatsManager::OnDialog_Helper;
    m_mapHelperHandler[L"tg"] = &KStatsManager::OnTGPrint_Helper;
}

void KStatsManager::SetEnable( bool bEnable_ )
{
    m_bEnable = bEnable_;
}
void KStatsManager::SetStartPos( POINT kPos_ )
{
    m_kStartPos = kPos_;
}
bool KStatsManager::Process( WCHAR* pMsg_ )
{
    WCHAR pMsg[MAX_PATH];
    wcscpy( pMsg, pMsg_ );
    WCHAR seps[] = L" ";
    WCHAR* token;
    token = wcstok( &pMsg[1], seps );

    if( token == NULL )
        return false;

    wcslwr( token );

    if( m_mapHandler.find(token) == m_mapHandler.end() )
    {
        if ( m_bEnable == true )
        {
            SetValue( token );
        }
        else
        {
            InsertLog( L"Not Exist Command" );
        }
		return false;
    }
    (this->*m_mapHandler[token])();
    return true;
}
void KStatsManager::Render()
{
    if ( m_bEnable == false )
        return;
    m_pkCurPage->SetStartPos( m_kStartPos );
    m_pkCurPage->Render( m_uiCurIndex + 1, (UINT)m_vecPage.size() );
    int iStartLogPos = m_kStartPos.y + KSTATS_LINE_HEIGHT*( KSTATS_VIEW_SEGMENT_NUM + 5 );
    for( int i = 0; i < (int)m_vecLog.size(); i++ )
    {
        g_pkFontMgr->OutTextXY( m_kStartPos.x, iStartLogPos, (WCHAR*)m_vecLog[i].c_str() );
        iStartLogPos += KSTATS_LINE_HEIGHT;
    }
}

void KStatsManager::EventProc( UINT uiEvent_ )
{
    if ( m_bEnable == false )
        return;
    switch( uiEvent_ ) 
    {
        case VK_NUMPAD8:
        {
            m_pkCurPage->Previous();
            break;
        }
        case VK_NUMPAD5:
        {
            m_pkCurPage->Next();
            break;
        }
        case VK_NUMPAD6:
        {
            Next();
            break;
        }
        case VK_NUMPAD4:
        {
            Previous();
            break;
        }
        case VK_NUMPAD0:
        {
            ClearDebugValue();
            break;
        }
        case VK_NUMPAD1:
        {
            m_pkCurPage->GetCurSegment()->RestoreValue();
        }
    }
}

void KStatsManager::OnReloadPet()
{
    KPet::BindLua();
}

void KStatsManager::OnOutputChat()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if( token != NULL )
    {
        std::wstring strParam1, strParam2;
        strParam1 = token;
        token = wcstok( NULL, seps );
        if( token != NULL )
            strParam2 = token;

        std::vector< std::wstring > vecTemp;
        vecTemp.push_back( strParam2 );
        SiGCPairPlaySystem()->OutputChatLog( strParam1, vecTemp );
    }
}

void KStatsManager::OnDummyReplay()
{
    SiGCPairPlaySystem()->MakeDummyReplay();
}

//void KStatsManager::OnTutorialMotion()
//{
//    if( SiKGCRoomManager()->GetGameMode() != GC_GM_TUTORIAL )
//        return;
//
//    WCHAR seps[] = L" ";
//    WCHAR* token = wcstok(NULL,seps);
//    int iMotion;
//    if ( token == NULL )
//        return;
//    iMotion = _wtoi(token);
//
//    g_MyD3D->MyPlayer[1]->m_iTutorialPlayerAction = iMotion;
//
//    ((KGCModeTutorial*)(g_pStateGame->GetCurGameModePtr()))->m_uiPractice_Motion = iMotion;
//}

void KStatsManager::MakeCrash()
{
	//크러쉬 생성 치트키
	int* i=NULL;
	*i=123;	
}
void KStatsManager::OnReSizeWindow()
{
	WCHAR seps[] = L" ";
	WCHAR* token = wcstok(NULL,seps);
	DWORD dwDevice = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].dwCurrentDevice;
	DWORD dwNumModes = g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwNumModes;
	D3DDeviceInfo* pDevice = &g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice];
	float fWSize;
	bool bOk = true;
	if (token == NULL)
	{
		bOk=false;
	}
	else
	{
		fWSize = static_cast<float>( _wtof(token) );
	}

	if (fWSize == 99)
	{
		::PostMessage(g_MyD3D->Get_hWnd(),WM_COMMAND,IDM_CHANGEDEVICE,TRUE);
	}
	

	if (fWSize <= 0 || fWSize > dwNumModes)
	{
		bOk=false;
	}

	if (bOk==true)
	{	
		fWSize-=1;
        g_MyD3D->m_KGCOption.SetResolution( (int)fWSize );
	}
	else
	{
		dwDevice=-1;// *표시 없얘기 위해서 지워줌
	}
	
	char strTemp[256];
	for (unsigned int i=0;i<dwNumModes;i++)
	{
		sprintf_s(strTemp,"%s Mode - %2d. Resolution Size = %7d x %7d "
			,i == g_MyD3D->m_Adapters[g_MyD3D->m_dwAdapter].devices[dwDevice].dwCurrentMode ? "*" : " "
			,i+1
			,(unsigned short)pDevice->modes[i].Width
			,(unsigned short)pDevice->modes[i].Height);
		g_MyD3D->m_kChatManager.AddChatMsg( GCUTIL_STR::GCStrCharToWide(strTemp) );
	}
}

void KStatsManager::OnNextStage()
{
	bool bBoss = false;
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok(NULL,seps);
    int iNextStage;

    CTriggerManager* pTriggerMgr = g_kEveryTriggerManager.GetCurrentStageTriggerManager();
    if( pTriggerMgr == NULL )
        return;

    pTriggerMgr->SetActive( false );
    if ( token == NULL )
    {
        iNextStage = g_kEveryTriggerManager.GetActiveStage();
        iNextStage++;
    }
    else
    {
		wcslwr( token );
		if( wcscmp( token, L"boss" ) == 0 )
		{
			bBoss = true;
		}
		else
		{
			iNextStage = _wtoi(token);
		}
    }
    pTriggerMgr = g_kEveryTriggerManager.GetNextStageTriggerManager();
    if( pTriggerMgr == NULL )
        return;
    pTriggerMgr->SetActive( true );

	if ( bBoss )
		g_kEveryTriggerManager.SetNextStage( g_pkQuestManager->m_Quest.FindBossStage() );
	else
		g_kEveryTriggerManager.SetNextStage( g_pkQuestManager->m_Quest.FindStage(iNextStage) );
}

void KStatsManager::OnFullHPMP()
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if(pPlayer == NULL)
        return;
    pPlayer->SetHPFull();
    pPlayer->InitQuantityPoint();
    pPlayer->m_fO2 = MAX_PLAYER_O2;
    g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
    //pPlayer->ProcessMetamorphosisSyncEnd();

    KPet* kPet = g_MyD3D->m_akPet[ g_MyD3D->Get_MyPlayer() ];
    kPet->m_fCurrentMP = 1.0f;
    pPlayer->m_fBurnningPoint = 3.0f;

	pPlayer->ClearSkillCoolTime();
	
    for( int iLoop=0; iLoop<int(g_MyD3D->MyCtrl->m_vecKeyCnt.size()); ++iLoop )
    {
        g_MyD3D->MyCtrl->m_vecKeyCnt[iLoop] = 0;
    }

    g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->m_pkSavingGauge->SetGauge( -1.f );
	
}
void KStatsManager::OnSetValue()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    wcslwr( token );
    KStatsPage* pPage = NULL;
    for ( int i = 0; i < (int)m_vecPage.size(); i++ )
    {
        pPage = m_vecPage[i].m_pPage;
        KStatsSegment* pSegment = NULL;
        for ( int j = 0; j < pPage->GetSize(); j++ )
        {
            pSegment = pPage->GetSegment( j );
            std::wstring strName = pSegment->GetName();
            wcslwr( (WCHAR*)strName.c_str() );            
            if ( wcscmp( token, strName.c_str() ) == 0 )
            {
                WCHAR* token = wcstok( NULL, seps );
                if ( token == NULL )
                {
                    InsertLog( L"OnSetValue : token is NULL" );
                    return;
                }
                pSegment->SetValue( token );
            }
        }
    }

}
// _P는 Particle, _A는 Anim
enum EScriptType
{
    EST_MAP_P = 0,
    EST_MAP_A,
};

void KStatsManager::OnReloadScript()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;

    UINT uiID = (UINT)_wtoi( token );

    switch( uiID )
    {
        case EST_MAP_A:
        {
            std::string strScript = "GCMapAnim.stg";
            g_MyD3D->m_pMapAnim->DeleteDeviceObjects();
            g_MyD3D->m_pMapAnim->Compile( (char*)strScript.c_str() );

            g_AnimManager.DeleteDeviceObjects();
            g_AnimManager.InitDeviceObjects();
            std::string strAnimScript;
            strAnimScript = "AnimEff.stg";
            g_AnimManager.LoadFromLuaScript( "etc.stg", (char*)strAnimScript.c_str() );
            break;
        }
        case EST_MAP_P:
        {
            std::string strParticle = g_strCurrentPath + "Stage\\GCNewMapParticleScript.stg";
            std::string strItemLua = "ParticleEff.stg";
            g_ParticleManager->Clear();
            g_ParticleManager->Load( (char*)strParticle.c_str(), (char*)strItemLua.c_str() );
            break;
        }
    }
}
void KStatsManager::OnPrintPage()
{
    if ( m_pkCurPage != NULL )
        m_pkCurPage->Print();
}

void KStatsManager::OnDropGameItem()
{
    if(!g_kGlobalValue.m_kUserInfo.bHost)
        return;

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if( pPlayer == NULL )
        return;

    int iWhat;

    WCHAR seps[] = L" ";
    WCHAR* token = wcstok(NULL, seps);
    if ( token == NULL )
        iWhat = rand() % GC_GAME_ITEM_NUM;
    else
        iWhat = _wtoi(token);

    D3DXVECTOR2 Vec;
    if( pPlayer->bIsRight )
        Vec.x = pPlayer->vPos.x - 0.05f + 0.135f;
    else 
        Vec.x = pPlayer->vPos.x - 0.05f - 0.135f; 

    Vec.y = pPlayer->vPos.y + 0.2f;
    g_MyD3D->MyItms->Add( iWhat, Vec, false, 3000 );
}
void KStatsManager::OnReload_AI()
{
    if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
        return;

    WCHAR* strFileName;
    WCHAR* strAIName;
    WCHAR seps[] = L" ";
    strFileName = wcstok(NULL, seps);
    strAIName = wcstok(NULL, seps);
    //g_MyD3D->m_AITemplet.LoadTemplateFormLua( "AITemplate.lua" );
}

void KStatsManager::OnDeviceList()
{    
    CTime t = CTime::GetCurrentTime();
    CString s = t.Format( L"DeviceList%D_%H_%M_%S.txt" );

    g_pGCDeviceManager2->MakeDeviceList(GCUTIL_STR::GCStrWideToChar((LPCTSTR)s));
}

void KStatsManager::OnAIEnable()
{
    WCHAR* strValue;
    WCHAR seps[] = L"";

    strValue = wcstok( NULL, seps );

    if( strValue == NULL )
        return;

    wcslwr( strValue );

    if( wcscmp( strValue, L"true" ) == 0 )
        g_kGlobalValue.m_bAIEnable = true;
    else if( wcscmp( strValue, L"false" ) == 0 )
        g_kGlobalValue.m_bAIEnable = false;

    //if( wcscmp( strValue, L"true" ) == 0 )
    //    g_MyD3D->m_AITemplet.AIEnable( true );
    //else if( wcscmp( strValue, L"false" ) == 0 )
    //    g_MyD3D->m_AITemplet.AIEnable( false );
}

void KStatsManager::OnRenderMonster()
{
    WCHAR* strValue;
    WCHAR seps[] = L"";

    strValue = wcstok( NULL, seps );

    if( strValue == NULL )
        strValue = L"true";

    wcslwr( strValue );

    if( wcscmp( strValue, L"true" ) == 0 )
        g_kGlobalValue.m_bRenderMonster = true;
    else if( wcscmp( strValue, L"false" ) == 0 )
        g_kGlobalValue.m_bRenderMonster = false;
}

void KStatsManager::OnPlayerAction()
{
    if( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
        return;

    WCHAR* strValue;
    WCHAR seps[] = L" ";

    int iMotion;
    int iFrame;

    strValue = wcstok( NULL, seps );
    if( strValue == NULL )
        return;
    iMotion = _wtoi(strValue);

    strValue = wcstok( NULL, seps );
    if( strValue == NULL )
        return;
    iFrame = _wtoi(strValue);

    char strFunc[MAX_PATH];
    sprintf( strFunc, "Player_Action%d_%dFrm(%d)", iMotion, iFrame, g_MyD3D->Get_MyPlayer() );

    if( FAILED( KGCLuabinder::getInstance().DoString( strFunc, strlen(strFunc) ) ) )
    {
        ASSERT( false && "Player Action Failed!" );
    }
}

void KStatsManager::OnEagleSite()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok(NULL,seps);
    float fZoom;

    //원거리에서 보도록 초기화한다.
    g_kCamera.InitCamera();
    if ( token == NULL )
    {
        g_kCamera.m_fWideMode = 9.9f;
        m_bEaglesite = false;
    }
    else
    {
        fZoom = static_cast<float>( _wtof(token) );
        g_kCamera.m_fWideMode = fZoom;
        m_bEaglesite = true;
    }
    // 나 자신을 타겟으로 카메라를 잡는다.
    g_kCamera.SetTargetPlayer(g_MyD3D->Get_MyPlayer()); 
    //카메라 포지션을 잡는다.
    g_kCamera.InitCameraPosition();
}

void KStatsManager::OnReloadSpecial75Texture()
{
	WCHAR seps[] = L" ";
	WCHAR* token = wcstok(NULL,seps);

	if ( token == NULL )
	{
		g_MyD3D->m_pDamageManager->ReloadSpecial75(false);
	}
	else
	{
		g_MyD3D->m_pDamageManager->ReloadSpecial75(true);
	}
}

void KStatsManager::OnReloadFireInfo()
{
    g_MyD3D->m_pDamageManager->ReloadFireInfo();
}

void KStatsManager::OnActionScript()
{
//     GCMemoryFile memfile(g_pGCDeviceManager2->GetMassFileManager(), "ActionScript.stg" );
// 
//     if( !memfile.IsLoaded())
//         return;
// 
//     if( FAILED( KGCLuabinder::getInstance().DoString( (const char*)memfile.GetDataPointer() )))
//         return;

	if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance() , "LoadMotion.stg" ) == false )
    {
        MessageBoxA( NULL, "ActionScript.stg Load Failed!", NULL, MB_OK );
        exit(0);
    }
    
    {
        KLuaManager luaMgr;
        if( GCFUNC::LoadLuaScript( luaMgr, "Actionfiles.stg" ) == false )
        {
            MessageBoxA( NULL, "ActionFiles.stg Load Failed!", NULL, MB_OK );
            exit(0);
        }
        else
        {
            if( SUCCEEDED( luaMgr.BeginTable("ActionFiles") ) )
            {
                std::string strFileName;
                for(int i = 1;SUCCEEDED(luaMgr.GetValue(i,strFileName)); i++)
                {
                    if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance() , strFileName ) == false )
                        return;
                }
                luaMgr.EndTable();
            }            
        }
    }
        
    g_pGraphicsHelper->OnReloadActionScript();
}

void KStatsManager::OnReloadMonster()
{
    KGCGlobal::LoadMonsterStatInfo();
    g_kMonsterManager.ReLoadScript();

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( pMonster->m_bLive )
		{
			int iMonsterType            = pMonster->m_iMonsterType;
			int iLevel                  = pMonster->m_iLevel;
			DWORD dwChampionProperty    = pMonster->GetChampionProperty();

			pMonster->Release();
			pMonster->StartMonster( (EGCMonster)iMonsterType, iLevel, dwChampionProperty );
			pMonster->SetState( "WAIT" );
		}
	}

    g_kGlobalValue.m_bRenderMonsterStatus = true;
}

void KStatsManager::OnReloadPlayerTemplate()
{
    int iMyIndex = g_MyD3D->Get_MyPlayer();
    int iMyExtraNum = g_MyD3D->MyPlayer[iMyIndex]->Extra_Char_Num;

    g_MyD3D->MyPlayer[iMyIndex]->SetSkillType(iMyExtraNum);
}

void KStatsManager::OnCheckSbta()
{
    if ( !g_MyD3D->m_kItemMgr.CheckItemImageFull() )
    {
        for ( int i = 0; i < (int)g_MyD3D->m_kItemMgr.m_vecStrNullItemImage.size(); i++ )
        {
            g_MyD3D->m_kChatManager.AddChatMsg( KncUtil::toWideString( g_MyD3D->m_kItemMgr.m_vecStrNullItemImage[i] ) );
        }
        g_MyD3D->m_kChatManager.AddChatMsg( L"some files are missing." );
    }
    else
        g_MyD3D->m_kChatManager.AddChatMsg( L"good to go ^-^" );
}

int KStatsManager::m_aiAbility[][3] =
{
    { 100, 100, 100 }, // 0
    { 100, 100, 100 }, // 1
    { 102, 101, 100 }, // 2
    { 112, 108, 100 }, // 3
    { 122, 115, 100 }, // 4
    { 132, 122, 103 }, // 5
    { 142, 129, 108 }, // 6
    { 152, 136, 113 }, // 7
    { 162, 143, 118 }, // 8
    { 172, 150, 123 }, // 9
    { 182, 157, 128 }, // 10
    { 192, 164, 133 }, // 11
    { 202, 171, 138 }, // 12
    { 212, 178, 143 }, // 13
    { 222, 185, 148 }, // 14
    { 232, 192, 153 }, // 15
    { 242, 199, 158 }, // 16
    { 252, 206, 163 }, // 17
    { 262, 213, 168 }, // 18
    { 272, 220, 173 }, // 19
    { 282, 227, 178 }, // 20
    { 292, 234, 183 }, // 21
    { 302, 241, 188 }, // 22
    { 312, 248, 193 }, // 23
    { 322, 255, 198 }, // 24
    { 332, 262, 203 }, // 25
    { 342, 269, 208 }, // 26
    { 352, 276, 213 }, // 27
    { 362, 283, 218 }, // 28
    { 372, 290, 223 }, // 29
    { 382, 297, 228 }, // 30
    { 392, 304, 233 }, // 31
    { 402, 311, 238 }, // 32
    { 412, 318, 243 }, // 33
    { 422, 325, 248 }, // 34
    { 432, 332, 253 }, // 35
    { 442, 339, 258 }, // 36
    { 452, 346, 263 }, // 37
    { 462, 353, 268 }, // 38
    { 472, 360, 273 }, // 39
    { 482, 367, 278 }, // 40
    { 492, 374, 283 }, // 41
    { 502, 381, 288 }, // 42
    { 512, 388, 293 }, // 43
    { 522, 395, 298 }, // 44
    { 532, 403, 303 }, // 45
    { 542, 411, 308 }, // 46
    { 552, 419, 313 }, // 47
    { 562, 427, 318 }, // 48
    { 572, 435, 323 }, // 49
    { 582, 443, 328 }, // 50
    { 592, 451, 333 }, // 51
    { 602, 459, 338 }, // 52
    { 612, 467, 343 }, // 53
    { 622, 475, 348 }, // 54
    { 632, 483, 353 }, // 55
    { 642, 491, 358 }, // 56
    { 652, 499, 363 }, // 57
    { 662, 507, 368 }, // 58
    { 672, 515, 373 }, // 59
    { 682, 523, 378 }, // 60
    { 692, 531, 383 }, // 61
    { 702, 539, 388 }, // 62
    { 712, 547, 393 }, // 63
    { 722, 555, 398 }, // 64
    { 732, 563, 404 }, // 65
    { 742, 571, 410 }, // 66
    { 752, 579, 416 }, // 67
    { 762, 587, 422 }, // 68
    { 772, 595, 428 }, // 69
    { 782, 603, 434 }, // 70
    { 792, 611, 440 }, // 71
    { 802, 619, 446 }, // 72
    { 812, 627, 452 }, // 73
    { 822, 635, 458 }, // 74
    { 832, 643, 464 }, // 75
    { 1000, 643, 900000000 }, // 무적!
};

void KStatsManager::OnSetPlayerLevel()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );

    int iLevel = 76;
    if( token != NULL )
        iLevel = _wtoi( token );

    if( iLevel < 0 )
        iLevel = 0;
    else if( iLevel > 75 + 1 )
        iLevel = 75 + 1;

    // 입력받은 iLevel로 능력치 세팅-ㅅ -
    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( pPlayer->m_kUserInfo.bLive )
        {
            pPlayer->m_fAbility[ABILITY_ATK] = (float)m_aiAbility[iLevel][0];
            pPlayer->m_fAbility[ABILITY_DEF] = (float)m_aiAbility[iLevel][1];
            pPlayer->m_fAbility[ABILITY_HP]  = (float)m_aiAbility[iLevel][2];
            pPlayer->SetHPFull();
        }
    }
}

void KStatsManager::OnAnnounce()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok(NULL,seps);
    if( token == NULL )
        return;

    KSignBoardData kData;
    kData.m_ItemID = 888; // 일단 젬!
    kData.m_dwSenderUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kData.m_strSenderNick = g_kGlobalValue.m_kUserInfo.strNickName;
    kData.m_eType = KSignBoardData::SBT_SQUARE;

    DWORD dwColor = (DWORD)wcstol( token, NULL, 16 );
    kData.m_dwColor = dwColor;

    token = wcstok(NULL,seps);
    if( token == NULL )
        return;

    // 두번째 토큰은 메세지
    kData.m_strMsg = token;

    SiKGCAnnounceMgr()->ReserveforDebug( kData );
}

void KStatsManager::OnAnnounceList()
{
    SiKGCAnnounceMgr()->RefreshAnnounceList();
}

void KStatsManager::OnMakeGif()
{
	WCHAR seps[] = L" ";
	WCHAR* token = wcstok(NULL, seps);

	int iSecond = 5;
	if ( token != NULL )
		iSecond = _wtoi(token);


	WCHAR* token2 = wcstok(NULL, seps);

	int iFrame = 5;
	if ( token2 != NULL )
		iFrame = _wtoi(token2);


	WCHAR* token3 = wcstok(NULL, seps);

	BYTE r= 0xff;
	if ( token3 != NULL )
		r = _wtoi(token3);

	WCHAR* token4 = wcstok(NULL, seps);

	BYTE g= 0xff;
	if ( token4 != NULL )
		g = _wtoi(token4);

	WCHAR* token5 = wcstok(NULL, seps);

	BYTE b= 0xff;
	if ( token5 != NULL )
		b = _wtoi(token5);

	g_MyD3D->SetColorARGB( 0xff, r, g, b );
	g_MyD3D->StartGifRecored( iSecond , iFrame );
}

void KStatsManager::OnLuaMode()
{
	g_MyD3D->m_bLuaInputMode = !g_MyD3D->m_bLuaInputMode;
}

void KStatsManager::OnLogLevel()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iLogLv = _wtoi( token );
    if( iLogLv < 3 )
    {
        dbg::logfile::SetLogLevel( iLogLv );
    }
}

void KStatsManager::OnBuff()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iBuffType = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    float fBuffTime = (float)_wtof( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iBuffLevel = _wtoi( token );

    g_pMagicEffect->SetMagicEffect( g_MyD3D->Get_MyPlayer(), iBuffType, fBuffTime, iBuffLevel );

}

void KStatsManager::OnAddDamage()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;

    int iIndex = g_MyD3D->Get_MyPlayer();
    if( 0 <= iIndex && iIndex < MAX_PLAYER_NUM )
    {
        int iDamageEnum = _wtoi( token );
        g_MyD3D->MyPlayer[iIndex]->AddDamage( iDamageEnum, 0 );

        KGC_PID_BROAD_ADD_DAMAGE kPacket;
        kPacket.dwUID = g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.dwUID;
        kPacket.iPlayerIndex = iIndex;
        kPacket.iDamageEnum = iDamageEnum;
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_ADD_DAMAGE" );
    }
}

void KStatsManager::OnSkillTrain()
{
    
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nSkill = _wtoi( token );
    SiGCSKT()->_D_TrainSkillReqtoServer( nSkill );
}

void KStatsManager::OnSkillUnTrain()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nSkill = _wtoi( token );
    SiGCSKT()->_D_UnTrainSkillReqtoServer( nSkill );
}

void KStatsManager::OnDumpSkillSetting()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iCharType = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nPromotion = _wtoi( token );

    SiGCSKT()->_D_Dump_Setting( iCharType, nPromotion );
}

void KStatsManager::OnDumpSkill()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iCharType = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nPromotion = _wtoi( token );

    SiGCSKT()->_D_Dump_Skill( iCharType, nPromotion );
}

void KStatsManager::SetSkillSetting()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iCharType = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nPromotion = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nSetting = _wtoi( token );

    SiGCSKT()->_D_Set_Setting( iCharType, nPromotion, nSetting );
}

void KStatsManager::ApplySetting()
{
    SiGCSKT()->_D_Apply_SettingChangetoServer();
}

void KStatsManager::OnEquipSkill()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iCharType = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nPromotion = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nSetting = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nID = _wtoi( token );

    SiGCSKT()->_D_Equip_Skill( iCharType, nPromotion, nSetting, nID );
};

void KStatsManager::OnUnEquipSkill()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int iCharType = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nPromotion = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nSetting = _wtoi( token );

    token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nID = _wtoi( token );

    SiGCSKT()->_D_UnEquip_Skill( iCharType, nPromotion, nSetting, nID );
};

void KStatsManager::OnUnlockGroup()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok( NULL, seps );
    if ( token == NULL )
        return;
    int nGroupID = _wtoi( token );
    SiGCSKT()->Process_UnlockGroupItem( nGroupID, false );
}

void KStatsManager::OnAllKill()
{
    g_kMonsterManager.AllKillMonster();
}

void KStatsManager::OnAllKillItem()
{
    // 스테이지에 살아 있는 몬스터가 있는지 확인
    //if( 0 < g_MyD3D->m_pkQuestGameMgr->m_iCurMonsterNum )
    {
        int iCharacterIdx=-1;

        // 토큰처리
        WCHAR seps[] = L" ";
        WCHAR* token = wcstok( NULL, seps );
        if( token != NULL )
        {
            iCharacterIdx = _wtoi(token);

            // 인덱스 예외처리
            if( (0 > iCharacterIdx) || (iCharacterIdx >= MAX_PLAYER_NUM) )
            {  goto END_FUNC;  }
            if( !g_MyD3D->MyPlayer[iCharacterIdx]->IsLive() )
            {  goto END_FUNC;  }
        }

        // 토큰이 없을경우 냠냠시켜줄 캐릭터 인덱스 처리
        if( iCharacterIdx < 0 )
        {
            for(iCharacterIdx = 0; iCharacterIdx<MAX_PLAYER_NUM; ++iCharacterIdx)
            {
                if( g_MyD3D->MyPlayer[ iCharacterIdx ]->IsLocalPlayer() )
                {  break; }
            }
            if( iCharacterIdx == MAX_PLAYER_NUM )
            {  goto END_FUNC;  }
        }

        // 몬스터 Kill처리 : Kill처리 후 DropItemUID 누적
        vector<DWORD> vecUIDList;

		for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			MONSTER *pMonster = mit->second;
			if( 0 < pMonster->GetHP() )
				SiKGCDropItemManager()->SetItemUIDToVector( pMonster->GetTriggerID(), vecUIDList );
			pMonster->SetHP( 0.0f );

		}

        // DropItem State 변경(IS_NONE -> IS_DROP)
        SiKGCDropItemManager()->DropItem( vecUIDList, D3DXVECTOR2(g_MyD3D->MyPlayer[iCharacterIdx]->GetPosition().x, g_MyD3D->MyPlayer[iCharacterIdx]->GetPosition().y) );

        // 드랍된 아이템을 선택된 캐릭터에게 냠냠시켜주기
        SiKGCDropItemManager()->GetStageDropItem( iCharacterIdx );
    }
END_FUNC:
    return;
}

void KStatsManager::OnDropItem()
{
    vector<DWORD> vecUIDList;

    int iMyPlayer = g_MyD3D->Get_MyPlayer();
    if( false == g_MyD3D->IsPlayerIndex(iMyPlayer) )
        return;

    for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
        MONSTER *pMonster = mit->second;
        if( pMonster )
            SiKGCDropItemManager()->SetItemUIDToVector( pMonster->GetTriggerID(), vecUIDList );
    }

    // DropItem State 변경(IS_NONE -> IS_DROP)
    SiKGCDropItemManager()->DropItem( vecUIDList, D3DXVECTOR2(g_MyD3D->MyPlayer[iMyPlayer]->GetPosition().x+0.5f, g_MyD3D->MyPlayer[iMyPlayer]->GetPosition().y) );
}

void KStatsManager::OnLogOff()
{
    SiKFriendShip()->ClearFriendShipData();	
    SiKGCVirtualDepotManager()->ClearDepotData();
    g_kGlobalValue.m_kHeroDungeonInfo.Release();

    //여기가 문제임.제길
    g_kGlobalValue.m_kUserInfo.vecCharInfo.clear();
    g_kGlobalValue.SendSavedCharType();
    g_kGlobalValue.SetSavedCharType(-1);

#if defined( USE_POST_LETTER_SYSTEM )
    SiKGCPostLetterManager()->ClearPostInfo();
#endif

    g_MyD3D->m_pStateMachine->GoState( GS_LOGIN );
    SiKGCJumpingCharManager()->SetEnableEvent( false );
	g_kGlobalValue.m_bSendServerInfoNot = false;
}

void KStatsManager::OnSTLCrashTest()
{
	std::vector<int> vecInt;
	vecInt[0] = 3;
}

void KStatsManager::OnAccessViolation()
{
	int* ptr = 0;
	*ptr = 0;
}

void KStatsManager::OnCppException()
{
// 	CArray<int> array;
// 	array.SetSize(1000000000);	
}

void KStatsManager::OnGodMode()
{
    if( 0 < g_MyD3D->GetMyPlayer()->Item_Remain_Time[GC_GAME_ITEM_SUPER] )
        g_MyD3D->GetMyPlayer()->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 0;
    else
        g_MyD3D->GetMyPlayer()->Item_Remain_Time[GC_GAME_ITEM_SUPER] = INT_MAX;
}

void KStatsManager::OnPerfect()
{
    g_pkQuestManager->m_bPerfectMode = true;
}

void KStatsManager::OnClear()
{
    if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON ) return;

    g_pkQuestManager->m_bWin = true;

    // 아이템 다 먹게 해준다
    SiKGCDropItemManager()->ClearDungeonGetItem();
    // 몹 다 죽인것 처럼 해준다.
    g_pkQuestManager->ClearDungeonKillMon();
    // 게임을 끝내준다.
    KGCGameModeInterface* pQuest = g_pStateGame->GetCurGameModePtr();
    pQuest->GameEndProcess();
    pQuest->GameEnd();

    g_pkQuestManager->m_bEndGame = true;
}

void KStatsManager::OnPrintDungeonItemDramaInfo()
{
    if ( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON ) return;

    std::set<KGCDropItem*> setAllDropItem = SiKGCDropItemManager()->GetAllDropItem();

    FILE *fo;
    WCHAR strFileName[100];   
    swprintf(strFileName,L"DungeonItemDramaInfo_%d_%d.txt", SiKGCRoomManager()->GetGameMode(), SiKGCRoomManager()->GetDungeonLevel() );

    fo = _wfopen( strFileName,L"w");

    WCHAR defaultDungeonInfo[200];   
    wsprintf( defaultDungeonInfo, L"[ Dungeon DropItem GoodsID Info || DungeonID : %d, DungeonLevel : %d ]\n\n", 
        SiKGCRoomManager()->GetGameMode(), SiKGCRoomManager()->GetDungeonLevel() );
    fwprintf(fo,L"%s", defaultDungeonInfo);

    std::set<KGCDropItem*>::iterator sit  = setAllDropItem.begin();
    for( sit; sit != setAllDropItem.end() ; ++sit )
    {
        WCHAR tempString[200];
        wsprintf(tempString, L"GoodsID : %8ld     DropType : %d\n", (*sit)->GetGoodsID()*10, (*sit)->GetDropType() );
        fwprintf(fo,L"%s", tempString);
    }  

    fwprintf(fo,L"\n\n===============================================================================\n\n");
    fwprintf(fo,L"[ Dungeon Drama NameID, StringID ]\n\n");

    std::vector<QuestSign> vecQuestSign = g_MyD3D->m_pkQuestGameMgr->m_vtQuestSign;
    std::map< std::string, std::vector<SDialogue> > mapDialogueList = g_MyD3D->m_pkQuestGameMgr->m_mapDialogueList;
    QuestSign sCurDungeonQuestSign;

    for( int i = 0; i < (int)vecQuestSign.size(); i++ )
    {
        if( vecQuestSign[i].iQuestID == SiKGCRoomManager()->GetGameMode() )
        {
            sCurDungeonQuestSign = vecQuestSign[i];
        }
    }

    std::string strTGFileName = g_MyD3D->m_pkQuestGameMgr->CreateQuestFileName( sCurDungeonQuestSign.GetQuestDifficultInfo(SiKGCRoomManager()->GetDungeonLevel()) );

    std::vector<CTriggerCondition*> vecTGCondition;

    for( int i = 0; i < g_MyD3D->m_pkQuestGameMgr->GetAllLevelMaxStage(); i++ )
        g_kEveryTriggerManager.GetTriggerConditionByType( i, CTriggerCondition::TG_DIALOGUE, vecTGCondition);   

    for( int i = 0; i < (int)vecTGCondition.size(); i++ )
    {
        if( vecTGCondition[i] )
        {
            CTGDialogue *pDialogue = static_cast<CTGDialogue*>(vecTGCondition[i]);

            if( !g_pkQuestManager->IsEndDialogList( pDialogue->m_szDialgue ) )
            {
                std::map<std::string, std::vector<SDialogue>>::iterator mit = mapDialogueList.find(pDialogue->m_szDialgue);
                
                if( mit != mapDialogueList.end() )
                {
                    for( int j = 0; j < (int)mit->second.size(); j++ )
                    {
                        WCHAR dungeonDramaInfo[200];   
                        wsprintf( dungeonDramaInfo, L"NameID : %6ld     StringID : %6ld\n", mit->second[j].iSpeakerID, mit->second[j].iSpeechID );
                        fwprintf(fo,L"%s", dungeonDramaInfo);
                    }
                    fwprintf(fo,L"\n");
                }    
            }
        }        
    }    

    fclose(fo);
}

void KStatsManager::OnToggleHUD()
{
	m_bModeHUD = (!m_bModeHUD);
}

void KStatsManager::OnAddMonsterInQuest()
{
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        int iMonType;

        WCHAR* token = wcstok( NULL, L" " );
        if( token != NULL )
            iMonType = _wtoi(token);

        if( 0 > iMonType || iMonType >= MONSTER_MODEL_NUM )
           iMonType = 0;

        int iMonIndex = g_MyD3D->CheckDunMonsterSummonSlot( );

        if ( iMonIndex == -1 ) 
            return;

        int iPlayerNum = g_MyD3D->Get_MyPlayer();
        if( false == g_MyD3D->IsPlayerIndex(iPlayerNum) ) 
            return;

        KSummonInfo kSummonInfo;
        kSummonInfo.Init();
        kSummonInfo.SetState( "WAIT" );
        kSummonInfo.iMonType = iMonType;
        kSummonInfo.fX = g_MyD3D->MyPlayer[iPlayerNum]->vPos.x;
        kSummonInfo.fY = g_MyD3D->MyPlayer[iPlayerNum]->vPos.y + 0.02f;

		MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin();

		if ( mit != g_kMonsterManager.m_mapMonster.end() )
	        mit->second->SummonMonsterStaticSlot( &kSummonInfo, true, true, g_MyD3D->MyPlayer[iPlayerNum]->GetCurrentChar().iLevel, iMonIndex );
        else
        {
            g_MyD3D->AddMonster( kSummonInfo.iMonType, kSummonInfo.fX, kSummonInfo.fY, true, 0, 0, g_MyD3D->MyPlayer[iPlayerNum]->GetCurrentChar().iLevel, 0, true, false, 0 );
            MONSTER* pMon = g_kMonsterManager.GetMonster( 0 );
            if( NULL == pMon ) return;
            pMon->m_bLive = true;
            pMon->m_pObject->SetRender( true );
            pMon->SetCountValue( 0 );
            pMon->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
        }
    }
}

void KStatsManager::OnMonsterAction()
{

	g_kGlobalValue.m_bMonsterAction = !g_kGlobalValue.m_bMonsterAction;

    //WCHAR* strValue;
    //WCHAR seps[] = L"";

    //strValue = wcstok( NULL, seps );

    //if( strValue == NULL )
    //    return;

    //wcslwr( strValue );

	
    //if( wcscmp( strValue, L"true" ) == 0 )
    //    g_kGlobalValue.m_bMonsterAction = true;
    //else if( wcscmp( strValue, L"false" ) == 0 )
    //    g_kGlobalValue.m_bMonsterAction = false;
}

void KStatsManager::OnTQ()
{
    g_MyD3D->GetMyPlayer()->m_cLife = 0;
    g_MyD3D->GetMyPlayer()->SetHP( 0.0f );
    g_MyD3D->GetMyPlayer()->Change_Motion( MID_COMMON_DOWN_AND_STANDUP );
}
void KStatsManager::OnToggleAttackSlash()
{
	PLAYER::m_bAttackSwoosh = !PLAYER::m_bAttackSwoosh;

}

void KStatsManager::OnReloadHeroItemInfo()
{
    g_pItemMgr->LoadHeroItemSpecialAbility();

    SCharInfo pCharInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();    

    //  히어로 아이템 버프 리셋
    g_pItemMgr->InitHeroItemSpecialAbility( g_MyD3D->Get_MyPlayer() );

    for( int i = 0; i < static_cast<int>(pCharInfo.vecItemInfo.size()); i++ )
    {
        g_pItemMgr->SettingHeroItemSpecialAbility(pCharInfo.vecItemInfo[i].iItemID, g_MyD3D->Get_MyPlayer());
    }
}

void KStatsManager::OnItemBoxAlphaToggle()
{
    g_MyD3D->m_KGCOption.SetAlphaAnimation( !g_MyD3D->m_KGCOption.GetAlphaAnimation() );
}

void KStatsManager::OnReplayOpen()
{
    if( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
        g_kGlobalValue.m_bUseOldReplay = !g_kGlobalValue.m_bUseOldReplay;
}

void KStatsManager::OnCriticalToggle()
{
    g_kGlobalValue.m_bCriticalToggle = !g_kGlobalValue.m_bCriticalToggle;
    if( g_kGlobalValue.m_bCriticalToggle )
        g_pkChatManager->AddSystemMsg( 0, L"100% 크리티컬 데미지 ON" );
    else
        g_pkChatManager->AddSystemMsg( 0, L"100% 크리티컬 데미지 OFF" );
}

void KStatsManager::OnChangeFont()
{
#if defined(FONT_INSTALL_FUNC)
    int iIndex;
    WCHAR wszTemp[1024];
    WCHAR* token = wcstok( NULL, L" " );

    if( token != NULL )
    {
        iIndex = _wtoi(token);
        if( (-1 > iIndex) || (iIndex >= int(g_kGlobalValue.m_vecFontList.size()) ) )
        {
            wsprintf(wszTemp, L"인덱스 범위오류입니다..\n입력가능한 폰트 인덱스 범위는 %d~%d입니다.", 0, int(g_kGlobalValue.m_vecFontList.size())-1);
            g_pkChatManager->AddSystemMsg( 0, wszTemp );
        }
        else
        {
            g_kGlobalValue.m_bFontInstallSuccess = false;
            g_kGlobalValue.m_strFontName = "";
            g_kGlobalValue.m_iSelectFontIndex = iIndex;
            g_MyD3D->m_pkUIMgr->CreateFonts();
            if( -1 == iIndex )
            {
                g_pkChatManager->AddChatMsg( L"기본 글꼴이 적용되었습니다.", 3, 0xffffff00 );
            }
            else if( g_kGlobalValue.m_bFontInstallSuccess )
            {
                std::wstring strFontFileName = GCUTIL_STR::GCStrCharToWide(g_kGlobalValue.m_vecFontList[iIndex].c_str());
                std::wstring strFontName = GCUTIL_STR::GCStrCharToWide(g_kGlobalValue.m_strFontName.c_str());
                wsprintf(wszTemp, L"Success FontInstall : 폰트파일명 %s, 글꼴명 %s", strFontFileName.c_str(), strFontName.c_str() );

                g_pkChatManager->AddChatMsg( wszTemp, 3, 0xffffff00 );
            }
            else
            {
                std::wstring strFontFileName = GCUTIL_STR::GCStrCharToWide(g_kGlobalValue.m_vecFontList[iIndex].c_str());
                std::wstring strFontName = GCUTIL_STR::GCStrCharToWide(g_kGlobalValue.m_strFontName.c_str());
                wsprintf(wszTemp, L"Fail FontInstall : 폰트파일명 %s, 글꼴명 %s", strFontFileName.c_str(), strFontName.c_str() );

                g_pkChatManager->AddChatMsg( wszTemp, 3, 0xffffff00 );
            }
        }
    }
    else
    {
        g_kGlobalValue.LoadFontList();
        if( 0 == g_kGlobalValue.m_vecFontList.size() )
        {
            DWORD dwSize = 1024;
            char szCurPath[1024];
            GetCurrentDirectoryA(dwSize, szCurPath);
            wsprintf(wszTemp, L"%s\\Fonts폴더에 폰트파일이 없습니다.(확장자 : ttf, ttc, fon, fot)", GCUTIL_STR::GCStrCharToWide( szCurPath ));
        }
        else
            wsprintf(wszTemp, L"입력가능한 폰트 인덱스 범위는 %d~%d입니다.", 0, int(g_kGlobalValue.m_vecFontList.size())-1);
        g_pkChatManager->AddSystemMsg( 0, wszTemp );
    }
#endif
}

void KStatsManager::OnWndInfo()
{
    g_kGlobalValue.m_bUIInfo = !g_kGlobalValue.m_bUIInfo;
}

void KStatsManager::OnOpenConsole()
{
    KSingleton<KncP2P>::GetInstance()->CreateConsoleWnd();
}

void KStatsManager::OnMotion()
{
    WCHAR* token = wcstok( NULL, L" " );

    if( token != NULL )
    {
        int iPlayer = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex( iPlayer ) )
        {
            if( wcscmp( token, L"all" ) == 0 ) {
                if( g_kGlobalValue.m_bStartCollectDamageList == false ) {
                    g_kGlobalValue.m_bStartCollectDamageList = true;
                }                
            } else if( wcscmp( token, L"stop" ) == 0 ) {
                if( g_kGlobalValue.m_bStartCollectDamageList ) {
                    g_kGlobalValue.m_bStartCollectDamageList = false;
                }
            } else {
                int iMotionID = _wtoi(token);
                g_MyD3D->MyPlayer[iPlayer]->SetPlayerState( iMotionID );
                g_MyD3D->MyPlayer[iPlayer]->SetFrameIndex( 0 );
            }
        }
    }
}

void KStatsManager::OnPetMotion()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( token != NULL )
    {
        int iPlayer = g_MyD3D->Get_MyPlayer();
        if( g_MyD3D->IsPlayerIndex( iPlayer ) )
        {
            int iMotionID = _wtoi(token);
            g_MyD3D->m_akPet[iPlayer]->SetPetMotion( iMotionID );
        }
    }
}

void KStatsManager::OnSetDamageRaito()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( token != NULL ) { 
        float fRaito = static_cast<float>( _wtof( token ) );
        if( fRaito < 0.0f ) { 
            fRaito = 0.0f;
        }
        g_kGlobalValue.SetDamageRaito( fRaito );
    }
}
void KStatsManager::OnGoExit()
{
   g_MyD3D->m_pStateMachine->GoExitGame(KGCGlobal::EXIT_GAME_STATE_DISCONNECT, 0 );
}

void KStatsManager::OnCriticalOffToggle()
{
    g_kGlobalValue.m_bCriticalOffToggle = !g_kGlobalValue.m_bCriticalOffToggle;
    if( g_kGlobalValue.m_bCriticalOffToggle ) {
        if( g_kGlobalValue.m_bCriticalToggle ) { 
            OnCriticalToggle();
        }
        g_pkChatManager->AddSystemMsg( 0, L"크리티컬 확률 0% ON" );
    }
    else
        g_pkChatManager->AddSystemMsg( 0, L"크리티컬 확률 0% OFF" );
}

void KStatsManager::OnCheckParentDamage()
{
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_GAME ) {
        g_kGlobalValue.CheckParentDamage();
    }
}

void KStatsManager::OnWriteFrame()
{
    g_kGlobalValue.m_bStartFrameCheck = true;
    g_kGlobalValue.WriteCollectedFrameData();
}

void KStatsManager::OnSwitchScreenShot()
{
    g_kGlobalValue.m_bJPGScreenShot = !g_kGlobalValue.m_bJPGScreenShot;
}

void KStatsManager::OnProcessID()
{
    DWORD dwPID;
    GetWindowThreadProcessId( g_MyD3D->Get_hWnd(), &dwPID );
    
    char szTemp[1024];
    sprintf(szTemp, "PID : %d", dwPID);
    MessageBoxA( NULL, szTemp, "ProcessInfo", MB_OK );
}

void KStatsManager::OnWriteQALog()
{
    SiKGCQALogManager()->SetQALogState( !SiKGCQALogManager()->IsQALog() );
}
void KStatsManager::OnComboEffectShow()
{
    g_kGlobalValue.m_bComboEffectShow = !g_kGlobalValue.m_bComboEffectShow;
    if( g_kGlobalValue.m_bComboEffectShow )
        g_pkChatManager->AddSystemMsg( 0, L"Combo Effect ON" );
    else
        g_pkChatManager->AddSystemMsg( 0, L"Combo Effect OFF" );
}

void KStatsManager::OnSpecialFaceShow()
{
    g_kGlobalValue.m_bSpecialFaceShow = !g_kGlobalValue.m_bSpecialFaceShow;
    if( g_kGlobalValue.m_bSpecialFaceShow )
        g_pkChatManager->AddSystemMsg( 0, L"Special Face ON" );
    else
        g_pkChatManager->AddSystemMsg( 0, L"Special Face OFF" );
}

void KStatsManager::OnSendTimeForP2PPacket()
{
    WCHAR* wcToken = wcstok( NULL, L" " );
    if( NULL == wcToken )
        return;

    g_kGlobalValue.m_iSendTimeForPacket = _wtoi( wcToken );
    if( -1 < g_kGlobalValue.m_iSendTimeForPacket )
    {
        g_kGlobalValue.m_kSendTimeForPacket.SetStopWatch( g_kGlobalValue.m_iSendTimeForPacket );
    }

    std::vector<KAddSendPacket>::iterator vecIter = g_kGlobalValue.m_vecSendPacket.begin();
    for(; vecIter != g_kGlobalValue.m_vecSendPacket.end(); )
    {
        RealSendP2PPacket( vecIter->pData, vecIter->uiPacketSize, vecIter->dwUID, vecIter->uiSendOption );
        vecIter = g_kGlobalValue.m_vecSendPacket.erase( vecIter );
    }
    g_kGlobalValue.m_vecSendPacket.clear();
}

void KStatsManager::OnShowFootHold()
{
    g_kGlobalValue.m_bRenderFootHold = !g_kGlobalValue.m_bRenderFootHold;
}

void KStatsManager::OnRecoverHP()
{
    g_kGlobalValue.m_bToggleRecoverHP = !g_kGlobalValue.m_bToggleRecoverHP;
    if( g_kGlobalValue.m_bToggleRecoverHP )
        g_pkChatManager->AddSystemMsg( 0, L"OFF HP Recovery" );
    else
        g_pkChatManager->AddSystemMsg( 0, L"ON HP Recovery" );
}

void KStatsManager::OnRecoverMP()
{
    g_kGlobalValue.m_bToggleRecoverMP = !g_kGlobalValue.m_bToggleRecoverMP;
    if( g_kGlobalValue.m_bToggleRecoverMP )
        g_pkChatManager->AddSystemMsg( 0, L"OFF MP Recovery" );
    else
        g_pkChatManager->AddSystemMsg( 0, L"ON MP Recovery" );
}

void KStatsManager::OnPlusHP()
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if(pPlayer == NULL)
        return;

    WCHAR* token = wcstok( NULL, L" " );

    if( NULL == token )
    {
        pPlayer->SetHPFull();
    }
    else
    {
        float fHp = static_cast<float>(_wtof(token));
        pPlayer->SetHP( fHp );
    }
}

void KStatsManager::OnPlusMP()
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if(pPlayer == NULL)
        return;

    WCHAR* token = wcstok( NULL, L" " );

    if( NULL == token )
    {
        g_MyD3D->MyHead->Level = g_kGlobalValue.GetMaxMP();
    }
    else
    {
        float fMp = static_cast<float>(_wtof(token));
        g_MyD3D->MyHead->Level = fMp;
    }
}

void KStatsManager::OnPlusPetMP()
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if(pPlayer == NULL)
        return;

    KPet* kPet = g_MyD3D->m_akPet[ pPlayer->m_iPlayerIndex ];
    if(kPet == NULL)
        return;

    WCHAR* token = wcstok( NULL, L" " );
    if( NULL == token )
    {
        kPet->m_fCurrentMP = 1.0f;
    }
    else
    {
        float fMp = static_cast<float>(_wtof(token));
        kPet->m_fCurrentMP += fMp;
    }
}

void KStatsManager::OnFrameLock()
{
    g_kGlobalValue.m_bFrameLock = !g_kGlobalValue.m_bFrameLock;
    if( g_kGlobalValue.m_bFrameLock )
        g_pkChatManager->AddSystemMsg( 0, L"ON Frame Lock" );
    else
        g_pkChatManager->AddSystemMsg( 0, L"OFF Frame Lock" );
}

void KStatsManager::OnProfileToggle()
{
    WCHAR seps[] = L" ";
    WCHAR* token = wcstok(NULL,seps);

    if ( token == NULL )
    {
        g_bProfile = true;
        PROFILE_INIT();
        g_pkChatManager->AddSystemMsg( 0, L"Profile ON" );
    }
    else
    {
        g_bProfile = false;
        g_pkChatManager->AddSystemMsg( 0, L"Profile OFF" );
    }
}



void KStatsManager::OnHelp()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( NULL == token )
    {
        FILE *pFile = fopen("CheatList.txt", "wt");
        std::map<std::wstring, HANDLER>::iterator mapIter = m_mapHandler.begin();
        for(; mapIter != m_mapHandler.end(); ++mapIter)
        {
            fprintf(pFile, "%s\n", GCUTIL_STR::GCStrWideToChar( mapIter->first.c_str() ));
            std::wstring strMsg = L"/";
            strMsg += mapIter->first.c_str();
            g_pkChatManager->AddSystemMsg( 0, strMsg.c_str() );
        }
        fclose( pFile );

        g_pkChatManager->AddSystemMsg( 0, L"치트 리스트를 CheatList.txt파일로 생성했습니다." );
    }
    else
    {
        std::map<std::wstring, HANDLER>::iterator mapIter = m_mapHelperHandler.find( token );
        if( mapIter != m_mapHelperHandler.end() )
        {
            (this->*m_mapHelperHandler[token])();
        }
        else
        {
            g_pkChatManager->AddSystemMsg( 0, L"Helper에 등록되지 않는 치트키 입니다..!!" );
        }
    }
}

void KStatsManager::OnSummonEmbark()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( NULL == token )
    {
        SiKGCEmbarkManager()->ClearInstance( UINT_MAX );
    }
    else
    {
        PLAYER *pPlayer = g_MyD3D->GetMyPlayer();
        if( NULL == pPlayer )
            return;

        int iSummonType = _wtoi( token );
        SiKGCEmbarkManager()->AddEmbark( iSummonType, pPlayer->GetPosition().x, pPlayer->GetPosition().y, pPlayer->GetIsRight() );
    }
}

void KStatsManager::MakeCrash_Helper()
{
    std::wstring wstrDesc = L"설명 : 크래시를 강제로 발생시킵니다.\n사용법 : /crash";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReSizeWindow_Helper()
{
    std::wstring wstrDesc = L"설명 : 해상도를 옵션메뉴에서 보이는 해상도 순서대로 0번부터 1씩증가(크래시 발생함ㅋㅋ 걍 옵션에서 하세요..)\n사용법 : /wsize 1";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnNextStage_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 스테이지로 이동합니다.\n사용법 : /next or /next 1 or /next boss";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnFullHPMP_Helper()
{
    std::wstring wstrDesc = L"설명 : HP,MP,BUNNING,PetMP를 가득 채웁니다.\n사용법 : //";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSetValue_Helper()
{
    std::wstring wstrDesc = L"설명 : Numpad7키를 눌렀을때 확인할 수 있는 여러가지 캐릭터 상태를 변경합니다.\n사용법 : /sv 상태명 값(상태명은 Numpad7로 확인바람)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReloadScript_Helper()
{
    std::wstring wstrDesc = L"설명 : 파티클과 2D 애니메이션 관련 스크립트 리로드합니다.\n사용법 : /reload 0 or /reload 1";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPrintPage_Helper()
{
    std::wstring wstrDesc = L"설명 : Numpad7로 확인할 수 있는 캐릭터 상태를 파일(PLAYER STATUS.txt)로 남깁니다.\n사용법 : /print_page";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnDropGameItem_Helper()
{
    std::wstring wstrDesc = L"설명 : 게임 아이템(장비아님)을 드랍합니다.\n사용법 : /drop 게임아이템ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnDeviceList_Helper()
{
    std::wstring wstrDesc = L"설명 : PC사양정보를 파일(DeviceList현재시간정보.txt)로 남깁니다.\n사용법 : /device";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnAIEnable_Helper()
{
    std::wstring wstrDesc = L"설명 : AI 스크립트를 실행하거나 하지 않습니다.\n사용법 : /aienable true or /aienable false";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnRenderMonster_Helper()
{
    std::wstring wstrDesc = L"설명 : 몬스터 모델링을 렌더링 하거나 하지 않습니다.\n사용법 : /rendermonster true or /rendermonster false";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnEagleSite_Helper()
{
    std::wstring wstrDesc = L"설명 : 카메라 WideMode(줌)를 조정합니다.\n사용법 :/eaglesite Wide크기 or /e Wide크기 or /ㄷ Wide크기";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPlayerAction_Helper()
{
    std::wstring wstrDesc = L"설명 : 카메라 WideMode(줌)를 조정합니다.\n사용법 :/eaglesite Wide크기 or /e Wide크기 or /ㄷ Wide크기";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnActionScript_Helper()
{
    std::wstring wstrDesc = L"설명 : 캐릭터 ActionFiles.stg에 등록된 StateFunction파일을 모두 리로드합니다.\n사용법 : /ra";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReloadMonster_Helper()
{
    std::wstring wstrDesc = L"설명 : 몬스터에 관련된 스크립트(AI, Template, MonsterStat)를 모두 리로드합니다.\n사용법 : /1";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReloadPet_Helper()
{
    std::wstring wstrDesc = L"설명 : 펫에 관련된 스크립트(Action, Template, PetSkill)를 모두 리로드합니다.\n사용법 : /reloadpet";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReloadFireInfo_Helper()
{
    std::wstring wstrDesc = L"설명 : FireInfo.stg에 등록된 데미지 스크립트를 리로드합니다.\n사용법 : /rfi";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnDummyReplay_Helper()
{
    std::wstring wstrDesc = L"설명 : 리플레이 파일을 만듭니다.\n사용법 : /dummyreplay";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnCheckSbta_Helper()
{
    std::wstring wstrDesc = L"설명 : 모든 아이템 리소스를 검사하고, 채팅메시지로 결과를 알려줍니다.\n사용법 : /checkitemimage";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSetPlayerLevel_Helper()
{
    std::wstring wstrDesc = L"설명 : 플레이어 능력치를 특정 레벨 변경( 레벨별 능력치는 현재 치트클래스에 임의로 정의되어있음,,, 실제 능력치와 다름,,, 사용하지 말자)\n사용법 : /setlevel 5(Number가 없으면 기본 76으로 셋팅)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnMakeGif_Helper()
{
    std::wstring wstrDesc = L"설명 : 스냅샷 이미지를 만듭니다.\n사용법 : /makegif 5 5 255 255 255(5초동안 5프레임에 한번씩 배경색상은 흰색(255 255 255)으로 스크린샷을 찍는다.)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnLuaMode_Helper()
{
    std::wstring wstrDesc = L"설명 : 루아스크립팅 모드로 전환합니다.(루아스크립팅이 채팅창에서 가능해진)\n사용법 : /luamode";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnLogLevel_Helper()
{
    std::wstring wstrDesc = L"설명 : 로그레벨을 변경합니다.\n사용법 : /log 1 or /log 2";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnBuff_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 버프를 캐릭터에게 겁니다.\n사용법 : /buff 1 5 0(버프ID 1번을 5초동안 버프레벨 0으로 건다)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnAddDamage_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 데미지를 생성합니다.\n사용법 : /adddamage 데미지ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSkillTrain_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 스킬을 배웁니다.(장착아님, 사용불가능상태, 스킬ID는 ENUM파일에서 SID_로 명명된 ID)\n사용법 : /trainskill 스킬ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSkillUnTrain_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 스킬을 제거합니다.(스킬ID는 ENUM파일에서 SID_로 명명된 ID)\n사용법 : /untrainskill 스킬ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnDumpSkillSetting_Helper()
{
    std::wstring wstrDesc = L"설명 : 배운 스킬을 파일로 남깁니다.\n사용법 : /dumpskillsetting 1 0(1번캐릭(리르)의 0번전직(궁수)의 스킬)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnDumpSkill_Helper()
{
    std::wstring wstrDesc = L"설명 : 캐릭터의 모든 스킬정보를 파일로 남깁니다.\n사용법 : /dumpskill 1 0(1번캐릭(리르)의 0번전직(궁수)의 스킬)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::ApplySetting_Helper()
{
    std::wstring wstrDesc = L"설명 : 서버와 스킬정보를 동기화 합니다.\n사용법 : /applyskill";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnEquipSkill_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 캐릭터에 스킬을 장착합니다.(사용가능상태)\n사용법 : /equipskill 1 0 0 1(1번캐릭(리르)의 0번전직(궁수)의 0번스킬셋트 1번스킬을 장착)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnUnEquipSkill_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 캐릭터에 스킬을 장착해제합니다.\n사용법 : /unequipskill 1 0 0 1(1번캐릭(리르)의 0번전직(궁수)의 0번스킬셋트 1번스킬을 장착해제)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnUnlockGroup_Helper()
{
    std::wstring wstrDesc = L"설명 : Lock( 자물쇠로 잠긴 )된 스킬그룹을 해제합니다.(그룹ID는 Enum.stg에서 SG_로 명명된 ID)\n사용법 : /unlockgroup 스킬그룹ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnAllKill_Helper()
{
    std::wstring wstrDesc = L"설명 : 모든 몬스터의 HP를 0으로 만듭니다.\n사용법 : /ak";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnAllKillItem_Helper()
{
    std::wstring wstrDesc = L"설명 : 모든 몬스터의 HP를 0으로 만들고 드랍된 아이템을 모두 냠냠 먹습니다.\n사용법 : /aki";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnLogOff_Helper()
{
    std::wstring wstrDesc = L"설명 : 로그아웃 합니다.\n사용법 : /lo";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReloadSpecial75Texture_Helper()
{
    std::wstring wstrDesc = L"설명 : 데미지 박스를 봅니다.\n사용법 : /sp75 아무글자 or /sp75";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSTLCrashTest_Helper()
{
    std::wstring wstrDesc = L"설명 : STL를 이용해서 크래시를 발생시킵니다.\n사용법 : /stlcrash";
        g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnAccessViolation_Helper()
{
    std::wstring wstrDesc = L"설명 : NULL포인터에 접근해서 크래시를 발생시킵니다.\n사용법 : /accessviolation";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnGodMode_Helper()
{
    std::wstring wstrDesc = L"설명 : 캐릭터를 무적(시간 : 2147483647)으로 만들거나 풉니다.\n사용법 : /god";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPerfect_Helper()
{
    std::wstring wstrDesc = L"설명 : 랭크에 TotalScore를 90으로 변경합니다.\n사용법 : /perfect";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnClear_Helper()
{
    std::wstring wstrDesc = L"설명 : 랭크에 TotalScore를 90으로 변경합니다.\n사용법 : /perfect";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPrintDungeonItemDramaInfo_Helper()
{
    std::wstring wstrDesc = L"설명 : 던전 정보(드랍, 속성, 트리거, 드라마 등..)를 파일(DungeonItemDramaInfo_던전ID_ 난이도.txt)로 남깁니다.\n사용법 : /print_itemdramainfo";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnAddMonsterInQuest_Helper()
{
    std::wstring wstrDesc = L"설명 : 지정한 몬스터를 소환합니다.(몬스터ID는 Enum.stg에 MON_로 명명된 ID)\n사용법 : /addmon 몬스터ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnToggleHUD_Helper()
{
    std::wstring wstrDesc = L"설명 : 게임 내 UI를 끄거나 겹니다.\n사용법 : /hud";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnMonsterAction_Helper()
{
    std::wstring wstrDesc = L"설명 : 몬스터 AI의 Trigger함수를 정지시키거나 실행시킵니다.\n사용법 : /ma\nma이후에 [키나 ]키로 상태를 변경할 수도 있고, \n키로 현재 상태를 반복시킬 수도 있다.";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnTQ_Helper()
{
    std::wstring wstrDesc = L"설명 : 캐릭터가 자결합니다.(목숨도 0으로 만듬)\n사용법 : /tq";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnToggleAttackSlash_Helper()
{
    std::wstring wstrDesc = L"설명 : 공격궤적(Slash)을 출력하거나 출력하지 않습니다.\n사용법 : /공격궤적";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReloadHeroItemInfo_Helper()
{
    std::wstring wstrDesc = L"설명 : 영웅아이템들로 받은 속성(버프, 능력치)을 리셋합니다.(버프, 능력치)\n사용법 : /rhi";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnItemBoxAlphaToggle_Helper()
{
    std::wstring wstrDesc = L"설명 : 아이템 위에 마우스를 롤오버 했을때 알파애니메이션을 할것인지 말것인지 선택합니다.\n사용법 : /box";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnReplayOpen_Helper()
{
    std::wstring wstrDesc = L"설명 : 리플레이 녹화방식을 예전처럼할지 동영상방식으로 할지 토글합니다.\n사용법 : /replayopen";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnCriticalToggle_Helper()
{
    std::wstring wstrDesc = L"설명 : 100% 크리티컬 데미지를 사용할지 말지 선택합니다.\n사용법 : /cri";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnCriticalOffToggle_Helper()
{
    std::wstring wstrDesc = L"설명 : 0% 크리티컬 데미지를 사용할지 말지 선택합니다.\n사용법 : /crioff";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnChangeFont_Helper()
{
    std::wstring wstrDesc = L"설명 : 폰트를 윈도우에 설치하거나 게임에 적용합니다.\n사용법 : /font(Fonts폴더내에 있는 폰트를 윈도우에 설치), /font 1(1번 폰트 게임에 적용)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnWndInfo_Helper()
{
    std::wstring wstrDesc = L"설명 : UI에 전달되는 정보(마우스이벤트)를 채팅창에 출력합니다.\n사용법 : /wnd";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnOpenConsole_Helper()
{
    std::wstring wstrDesc = L"설명 : 콘솔 윈도우를 생성합니다.\n사용법 : /console";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnMotion_Helper()
{
    std::wstring wstrDesc = L"설명 : 캐릭터 모션을 변경합니다.\n사용법 : /motion 모션ID or /m 모션ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPetMotion_Helper()
{
    std::wstring wstrDesc = L"설명 : 펫 모션을 변경합니다.\n사용법 : /petmotion 모션ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSetDamageRaito_Helper()
{
    std::wstring wstrDesc = L"설명 : 몬스터에게 입히는 데미지 비율을 변경합니다.\n사용법 : /dmg 2(DamageToMonster에 셋팅된 값을 2배 뻥튀기한다.)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnGoExit_Helper()
{
    std::wstring wstrDesc = L"설명 : 클라이언트를 걍 끕니다.\n사용법 : /exit";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnCheckParentDamage_Helper()
{
    std::wstring wstrDesc = L"설명 : 플레이어 템플릿에 정의된 데미지들의 부모 데미지 정보를 파일(CharParentDamageList_XXXX.txt)로 남깁니다.\n사용법 : /pdamage";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnWriteFrame_Helper()
{
    std::wstring wstrDesc = L"설명 : FPS정보를 파일로 남깁니다.\n사용법 : /writeframe";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnProcessID_Helper()
{
    std::wstring wstrDesc = L"설명 : Main프로세스 PID를 메시지 박스로 출력해줍니다.\n사용법 : /pid";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSwitchScreenShot_Helper()
{
    std::wstring wstrDesc = L"설명 : 스크린샷의 확장자를 BMP로 할지 JPG로 할지 선택합니다.\n사용법 : /changesc";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnWriteQALog_Helper()
{
    std::wstring wstrDesc = L"설명 : QALog를 파일로 남깁니다.\n사용법 : /logfile";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnComboEffectShow_Helper()
{
    std::wstring wstrDesc = L"설명 : 콤보 UI이펙트를 출력하거나 출력하지 않습니다.\n사용법 : /combo";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSpecialFaceShow_Helper()
{
    std::wstring wstrDesc = L"설명 : 필살기 연출 이미지를 출력하거나 출력하지 않습니다.\n사용법 : /face";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnShowFootHold_Helper()
{
    std::wstring wstrDesc = L"설명 : 발판 블럭을 출력하거나 출력하지 않습니다.\n사용법 : /sf";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSendTimeForP2PPacket_Helper()
{
    std::wstring wstrDesc = L"설명 : P2P패킷의 전송 시간에 텀을 줍니다.(네트워크환경이 안좋을때 테스트용) \n사용법 : /st 1000(1초의 텀을 줌)";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnRecoverHP_Helper()
{
    std::wstring wstrDesc = L"설명 : HP 자동회복을 켜거나 끕니다.\n사용법 : /hs or /ㅗㄴ";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnRecoverMP_Helper()
{
    std::wstring wstrDesc = L"설명 : MP 자동회복을 켜거나 끕니다.\n사용법 : /ms or /ㅡㄴ";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPlusHP_Helper()
{
    std::wstring wstrDesc = L"설명 : HP 양을 설정합니다.\n사용법 : /hp 100 or /ㅗㅔ 100";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPlusMP_Helper()
{
    std::wstring wstrDesc = L"설명 : MP 양을 설정합니다.\n사용법 : /mp 1.5 or /ㅡㅔ 1.5";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnPlusPetMP_Helper()
{
    std::wstring wstrDesc = L"설명 : Pet MP 양을 설정합니다.\n사용법 : /p 1.0 or /ㅔ 1.0";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnFrameLock_Helper()
{
    std::wstring wstrDesc = L"설명 : FrameLock을 켜거나 끕니다.\n사용법 : /f";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
void KStatsManager::OnSummonEmbark_Helper()
{
    std::wstring wstrDesc = L"설명 : 탑승물을 소환합니다.\n사용법 : /em 탑승물ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}

void KStatsManager::OnInvenDump()
{
    g_pItemMgr->LogInventory();
}

void KStatsManager::OnInvenDump_Helper()
{
    std::wstring wstrDesc = L"설명 : 인벤토리 보유 아이템을 파일로 출력합니다.(inventoryList.txt) \n사용법 : /inven";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}

void KStatsManager::OnAddWall()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( NULL == token )
    {
        SiKGCWallManager()->DestoryAllInstance();
    }
    else
    {
        PLAYER *pPlayer = g_MyD3D->GetMyPlayer();
        if( NULL == pPlayer )
            return;

        int iSummonType = _wtoi( token );
        SiKGCWallManager()->CreateWall(iSummonType, 0, pPlayer->GetX());
    }
}

void KStatsManager::OnAddWall_Helper()
{
    std::wstring wstrDesc = L"설명 : 벽을 생성합니다. \n사용법 : /wa 벽ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}

void KStatsManager::OnAddSubject()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( NULL == token )
    {
        SiKGCSubjectManager()->DestoryAllInstance();
    }
    else
    {
        PLAYER *pPlayer = g_MyD3D->GetMyPlayer();
        if( NULL == pPlayer )
            return;

        int iType = _wtoi( token );
        SiKGCSubjectManager()->CreateSubject( iType, pPlayer->m_iPlayerIndex );
    }
}

void KStatsManager::OnAddSubject_Helper()
{
    std::wstring wstrDesc = L"설명 : 과제를 생성합니다. \n사용법 : /sj 과제ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}

void KStatsManager::OnTestDrama()
{
    WCHAR* token = wcstok( NULL, L" " );

    if( NULL != token )
    {
        int nDrama = _wtoi( token );

        if ( g_pkUIScene->m_pkGameOverlayUI )
        {
            g_pkUIScene->m_pkGameOverlayUI->SetDrama( nDrama );
        }
    }
}

void KStatsManager::OnTcpTest()
{
    SiKP2P()->m_spTRUserProxy->ReserveDestroy();
}

void KStatsManager::OnHost()
{
    bool bHost = g_kGlobalValue.m_kUserInfo.bHost;
    g_kGlobalValue.m_kUserInfo.bHost = !bHost;
    g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost = !bHost;
}

void KStatsManager::OnOpenSkillTreeUI()
{
    g_MyD3D->OnOpenSkillTreeByCheat();
}

void KStatsManager::OnOpenSkillTreeUI_Helper()
{
    std::wstring wstrDesc = L"설명 : 스킬트리창을 오픈하고 게임내에서 스킬을 바꿔 착용할 수 있습니다. \n사용법 : /skill, /s";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}

void KStatsManager::OnShowTestImg()
{
    WCHAR* strValue;
    WCHAR seps[] = L" ";

    SAFE_RELEASE(g_kGlobalValue.m_pTempTex);
    g_kGlobalValue.m_pTempTex = g_pGCDeviceManager2->CreateTexture( "TempTex.DDS" );
    if( NULL == g_kGlobalValue.m_pTempTex )
        return;

    strValue = wcstok( NULL, seps );
    if( strValue == NULL )
    {
        g_kGlobalValue.bRenderTempTexture = !g_kGlobalValue.bRenderTempTexture;
        g_kGlobalValue.vTempTexRenderPos = D3DXVECTOR2(0.0f, 0.0f);
    }
    else
    {
        float fPosX = static_cast<float>(_wtof(strValue));
        g_kGlobalValue.bRenderTempTexture = true;
        g_kGlobalValue.vTempTexRenderPos.x = fPosX;
    }
    
    strValue = wcstok( NULL, seps );
    if( strValue )
    {
        float fPosY = static_cast<float>(_wtof(strValue));
        g_kGlobalValue.bRenderTempTexture = true;
        g_kGlobalValue.vTempTexRenderPos.y = fPosY;
    }
}

void KStatsManager::OnShowTestImg_Helper()
{
    std::wstring wstrDesc = L"설명 : 임시 이미지(TestImg.dds)를 화면에 출력합니다. \n사용법 : /img PosX PosY";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}

void KStatsManager::OnDialog()
{
    WCHAR* token = wcstok( NULL, L" " );
    if( NULL != token )
    {
        int iDlg_ID = _wtoi( token );

        KLuaManager luaMgr;
        GCFUNC::LoadLuaScript( luaMgr, "CheatDlgInfo.lua" );

        int iGetID = 0;
        std::string strDialogueName;
        bool bLoadScript = false;
        if( SUCCEEDED(luaMgr.BeginTable( "CheatDlgInfo" )) )
        {
            strDialogueName.clear();
            for(int iLoop=1; SUCCEEDED(luaMgr.BeginTable(iLoop)); ++iLoop)
            {
                LUA_GET_VALUE_DEF( "DiaID", iGetID, -1 );
                LUA_GET_VALUE_DEF( "Dialogue", strDialogueName, "" );
                luaMgr.EndTable();

                if( iDlg_ID == iGetID )
                {
                    bLoadScript = true;
                    break;
                }
            }
            luaMgr.EndTable();
        }

        if( false == bLoadScript )
        {
            MessageBoxA( NULL, "CheatDlgInfo.lua에서 해당 ID를 찾지 못했습니다!!", "ErrorCleatKey", MB_OK );
            return;
        }

        g_pkQuestManager->SetDialogueList(strDialogueName);
        g_pkQuestManager->SetShowDialogue( true );
        g_pkQuestManager->m_iDialogueTG_ID = 0;

        KGC_PID_DIALOGUE_PROGRESS packet;
        packet.m_sTG_ID     = 0;
        packet.m_sIndex     = 0;
        packet.bShow        = true;
        g_kTGPacketPacker.Pack(&packet, sizeof(KGC_PID_DIALOGUE_PROGRESS));
    }
}

void KStatsManager::OnDialog_Helper()
{
    std::wstring wstrDesc = L"설명 : 드라마를 실행 합니다.(DLG_ID는 CheatDlgInfo.lua에 정의된 ID입니다.) \n사용법 : /dlg DLG_ID";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}


void KStatsManager::OnTGPrint()
{
    g_kGlobalValue.m_bPrintTG = !g_kGlobalValue.m_bPrintTG;
}

void KStatsManager::OnTGPrint_Helper()
{
    std::wstring wstrDesc = L"설명 : 현재 진행중인 트리거 이름을 콘솔창에 출력해줍니다. \n사용법 : /tg";
    g_pkChatManager->AddSystemMsg( 0, wstrDesc.c_str() );
}
bool KStatsManager::GetToogleHUD()
{
    return m_bModeHUD;
}

bool KStatsManager::GetEagleSiteCheat()
{
    return m_bEaglesite;
}

void KStatsManager::OnToggleTestStatic()
{
    g_pkGameOverlayUI->ToggleStaticInternalTest();
}

#endif