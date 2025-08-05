#include "stdafx.h"
#include "GCVirtualKeyManager.h"

IMPLEMENT_CLASSNAME( KGCVirtualKeyManager );
IMPLEMENT_WND_FACTORY( KGCVirtualKeyManager );
IMPLEMENT_WND_FACTORY_NAME( KGCVirtualKeyManager, "gc_virtualkeymanager" );

KGCVirtualKeyManager::KGCVirtualKeyManager( void ) :
m_bUpperCase( false )
{
    m_vecNumberKeyData.clear();
    m_vecAlphabetKeyData.clear();
    m_vecSymbolMarkKeyData.clear();
    m_strClickKeyData.clear();

    for( int i = 0 ; i < MAX_VIRTUALKEY_SIZE ; ++i )
    {
        m_apkVirtualKey[i] = NULL;

        char szTemp[MAX_PATH];
        sprintf_s(szTemp, MAX_PATH - 1, "virtualkey_%d", i);
        LINK_CONTROL( szTemp, m_apkVirtualKey[i] );
    }
    LINK_CONTROL( "virtualkey_enter", m_apkVirtualKey_Enter );
    LINK_CONTROL( "virtualkey_backspace", m_apkVirtualKey_BackSpace );
    LINK_CONTROL( "virtualkey_upperlower", m_apkVirtualKey_UpperLower );
}

KGCVirtualKeyManager::~KGCVirtualKeyManager( void )
{
}

void KGCVirtualKeyManager::OnCreate( void )
{
    for( int i = 0 ; i < MAX_VIRTUALKEY_SIZE ; ++i )
    {
        m_apkVirtualKey[i]->InitState(true, true, this);
    }

    m_apkVirtualKey_Enter->InitState(true, true, this);
    m_apkVirtualKey_BackSpace->InitState(true, true, this);
    m_apkVirtualKey_UpperLower->InitState(true, true, this);

    for ( int i = 33; i < 127; ++i )
    {
        char cTemp = i;
        std::string strTemp;

        if ( isdigit(cTemp) )
        {
            strTemp = cTemp;
            m_vecNumberKeyData.push_back(KncUtil::toWideString(strTemp));
        }
        else if ( isalpha(cTemp) )
        {
            if ( 97 <= i && i <= 122 )
            {
                strTemp = cTemp;
                m_vecAlphabetKeyData.push_back(KncUtil::toWideString(strTemp));
            }
        }
        else
        {
            strTemp = cTemp;
            m_vecSymbolMarkKeyData.push_back(KncUtil::toWideString(strTemp));
        }
    }
}

void KGCVirtualKeyManager::OnInitialize()
{
    m_apkVirtualKey_BackSpace->SetKeyData( g_pkStrLoader->GetString( STR_ID_VIRTUALKEY_BACKSPACE ) );
    m_apkVirtualKey_UpperLower->SetKeyData( g_pkStrLoader->GetString( STR_ID_VIRTUALKEY_UPPERLOWER ) );

    ShuffleKeyData();
}

void KGCVirtualKeyManager::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            return;
        }
    case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            return;
        }
    }

    if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        for( int i = 0 ; i < MAX_VIRTUALKEY_SIZE ; ++i )
        {
            if ( event.m_pWnd == m_apkVirtualKey[i] )
            {
                m_strClickKeyData = m_apkVirtualKey[i]->GetKeyData();
                SpeakToActionListener( KActionEvent(this, D3DWE_BUTTON_CLICK, EVK_TEXT) );
                return;
            }
        }

        if ( event.m_pWnd == m_apkVirtualKey_BackSpace )
        {
            SpeakToActionListener( KActionEvent(this, D3DWE_BUTTON_CLICK, EVK_BACKSPACE) );
            return;
        }
        else if ( event.m_pWnd == m_apkVirtualKey_UpperLower )
        {
            UpperLowerChange();
            ShuffleKeyData();
            return;
        }
        else if ( event.m_pWnd == m_apkVirtualKey_Enter )
        {
            SpeakToActionListener( KActionEvent(this, D3DWE_BUTTON_CLICK, EVK_ENTER) );
            return;
        }
    }
}

void KGCVirtualKeyManager::FrameMoveInEnabledState( void )
{
    if ( m_apkVirtualKey[55]->GetWndMode() == D3DWM_HOVER )
    {
        m_apkVirtualKey_Enter->SetWndMode(D3DWM_DEFAULT);
    }
}

void KGCVirtualKeyManager::AddKeyData()
{
    int index = 0;

    std::vector< std::wstring >::iterator itor;
    for ( itor = m_vecNumberKeyData.begin(); itor != m_vecNumberKeyData.end(); ++itor )
    {
        if ( index >= MAX_VIRTUALKEY_SIZE )
            return;

        m_apkVirtualKey[index]->SetKeyData( *itor );
        index++;
    }

    for ( itor = m_vecAlphabetKeyData.begin(); itor != m_vecAlphabetKeyData.end(); ++itor )
    {
        if ( index >= MAX_VIRTUALKEY_SIZE )
            return;

        m_apkVirtualKey[index]->SetKeyData( *itor );
        index++;
    }

    for ( itor = m_vecSymbolMarkKeyData.begin(); itor != m_vecSymbolMarkKeyData.end(); ++itor )
    {
        if ( index >= MAX_VIRTUALKEY_SIZE )
            return;

        m_apkVirtualKey[index]->SetKeyData( *itor );
        index++;
    }
}

void KGCVirtualKeyManager::ShuffleKeyData()
{
	
    random_shuffle( m_vecNumberKeyData.begin(), m_vecNumberKeyData.end() );
    random_shuffle( m_vecAlphabetKeyData.begin(), m_vecAlphabetKeyData.end() );
    random_shuffle( m_vecSymbolMarkKeyData.begin(), m_vecSymbolMarkKeyData.end() );

    AddKeyData();
}

void KGCVirtualKeyManager::UpperLowerChange()
{
    std::vector< std::wstring > vecTemp;
    vecTemp.clear();

    std::vector< std::wstring >::iterator itor;
    for ( itor = m_vecAlphabetKeyData.begin(); itor != m_vecAlphabetKeyData.end(); ++itor )
    {
        std::wstring strTemp;
        strTemp = *itor;
        if ( m_bUpperCase )
        {
            GCUTIL_STR::ToLower(strTemp);
        }
        else
        {
            GCUTIL_STR::ToUpper(strTemp);
        }

        vecTemp.push_back(strTemp);
    }

    m_bUpperCase = ( m_bUpperCase ? false : true );

    m_vecAlphabetKeyData.clear();
    m_vecAlphabetKeyData = vecTemp;
}
