#include "stdafx.h"
#include "gcui/KGCAddItemInClientItemList.h"



IMPLEMENT_CLASSNAME( KGCAddItemInClientItemListElement );
IMPLEMENT_WND_FACTORY( KGCAddItemInClientItemListElement );
IMPLEMENT_WND_FACTORY_NAME( KGCAddItemInClientItemListElement, "gc_additem_itemlist_element" );

#ifndef __PATH__

KGCAddItemInClientItemListElement::KGCAddItemInClientItemListElement( void )
{

    m_pkName = NULL;
    m_pkItemId = NULL;
    m_pkGrade = NULL;
    m_pkPeriod = NULL;
    m_pkDuration = NULL;
    m_pkLevel = NULL;
    m_apkBookMarkCheckBox = NULL;
    m_apkCheckBox = NULL;

    LINK_CONTROL( "name", m_pkName );
    LINK_CONTROL( "itemid", m_pkItemId );
    
    LINK_CONTROL( "grade", m_pkGrade );
    LINK_CONTROL( "period", m_pkPeriod );
    LINK_CONTROL( "duration", m_pkDuration );
    LINK_CONTROL( "level", m_pkLevel );

    LINK_CONTROL( "itemlistcheckbox", m_apkCheckBox );
    LINK_CONTROL( "itemlistbookmark", m_apkBookMarkCheckBox );

}

KGCAddItemInClientItemListElement::~KGCAddItemInClientItemListElement( void )
{
}


void KGCAddItemInClientItemListElement::OnCreate( void )
{
    if( m_pkName )
        m_pkName->InitState( true, true, this );
   
    if( m_pkItemId )
        m_pkItemId->InitState( true, true, this );


    if( m_pkGrade )
    {
    m_pkGrade->InitState( true, true, this );
        m_pkGrade->SetText( L"-1");
    }
    
    if( m_pkPeriod )
    {
    m_pkPeriod->InitState( true, true, this );
        m_pkPeriod->SetText( L"-1");
    }
    
    if( m_pkDuration )
    {
    m_pkDuration->InitState( true, true, this );
        m_pkDuration->SetText( L"-1");
    }
    
    if( m_pkLevel )
    {
    m_pkLevel->InitState( true, true, this );
        m_pkLevel->SetText( L"-1");
    }

    if( m_apkCheckBox )
    m_apkCheckBox->InitState( true, true, this );
    
    if( m_apkBookMarkCheckBox )
    m_apkBookMarkCheckBox->InitState( true, true, this );
    
}

void KGCAddItemInClientItemListElement::SetItemDataText(const WCHAR* text)
{
    m_pkGrade->SetText( text);
    m_pkPeriod->SetText(text);
    m_pkDuration->SetText( text);
    m_pkLevel->SetText(text);

}

void KGCAddItemInClientItemListElement::SetText(const std::wstring& text)
{
    m_pkName->SetText( text );
}

void KGCAddItemInClientItemListElement::SetItemIdText(DWORD dwItemId)
{
    if( m_pkItemId )
    {
        //ItemID瑜??듭㎏濡?蹂댁ъ???
        m_pkItemId->SetText( dwItemId * 10 );
    }
}


std::vector<const WCHAR*> KGCAddItemInClientItemListElement::GetItemData()
{
    std::vector<const WCHAR*> vecItemData;
    
    vecItemData.push_back(m_pkGrade->GetText());
    vecItemData.push_back(m_pkPeriod->GetText());
    vecItemData.push_back(m_pkDuration->GetText());
    vecItemData.push_back(m_pkLevel->GetText());

    return vecItemData;
}

void KGCAddItemInClientItemListElement::SetItemData(  std::vector<int> vecItemData)
{
    WCHAR strTemp[ MAX_PATH ] = L"";
 
    _itow_s(vecItemData.at(0), strTemp, MAX_PATH, 10 );
    m_pkGrade->SetText(strTemp);
    _itow_s(vecItemData.at(1), strTemp, MAX_PATH, 10 );
    m_pkPeriod->SetText(strTemp);
    _itow_s(vecItemData.at(2), strTemp, MAX_PATH, 10 );
    m_pkDuration->SetText(strTemp);
    _itow_s(vecItemData.at(3), strTemp, MAX_PATH, 10 );
    m_pkLevel->SetText(strTemp);
    /*m_pkPeriod->SetText(boost::lexical_cast<WCHAR*>(vecItemData[1]));
    m_pkDuration->SetText(boost::lexical_cast<WCHAR*>(vecItemData[2]));
    m_pkLevel->SetText(boost::lexical_cast<WCHAR*>(vecItemData[3]));*/
    
}

void KGCAddItemInClientItemListElement::FrameMoveInEnabledState()
{}

void KGCAddItemInClientItemListElement::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
       if( event.m_pWnd == m_apkCheckBox )
            m_apkCheckBox->ToggleCheck();
    
       if( event.m_pWnd == m_apkBookMarkCheckBox )
           m_apkBookMarkCheckBox->ToggleCheck();
    }

}

bool KGCAddItemInClientItemListElement::GetCheckBoxState()
{
    return m_apkCheckBox->GetCheck();
}
bool KGCAddItemInClientItemListElement::GetBookMarkState()
{
    return m_apkBookMarkCheckBox->GetCheck();
}
void KGCAddItemInClientItemListElement::SetCheckBoxState(bool bcheck)
{
    m_apkCheckBox->SetCheck( bcheck );
}
void KGCAddItemInClientItemListElement::SetBookMarkState(bool bcheck)
{
    m_apkBookMarkCheckBox->SetCheck( bcheck );
}
#else

KGCAddItemInClientItemListElement::KGCAddItemInClientItemListElement( void ){}
KGCAddItemInClientItemListElement::~KGCAddItemInClientItemListElement( void ){}
void KGCAddItemInClientItemListElement::OnCreate( void ){}
void KGCAddItemInClientItemListElement::FrameMoveInEnabledState(){}
void KGCAddItemInClientItemListElement::ActionPerformed( const KActionEvent& event ){}

#endif
////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_CLASSNAME( KGCAddItemInClientItemList );
IMPLEMENT_WND_FACTORY( KGCAddItemInClientItemList );
IMPLEMENT_WND_FACTORY_NAME( KGCAddItemInClientItemList, "gc_additem_itemlist" );

#ifndef __PATH__

KGCAddItemInClientItemList::KGCAddItemInClientItemList( void )
{


    m_pkStaticGrade = NULL;
    m_pkStaticPeriod= NULL;
    m_pkStaticDuration= NULL;
    m_pkStaticLevel= NULL;
    m_pkStaticItemId = NULL;
    m_iColumn = 0;
    m_iRow = 0;
    
  char str[MAX_PATH] = {0,};

  for(int i = 0; i < ITEMLISTNUM; ++i)
  {
      m_pkItemListElement[i] = NULL;
  }
  
  for(int iLoop = 1; iLoop < ITEMLISTNUM + 1; ++iLoop)
  {
      sprintf(str, "itemlist%d", iLoop);
      LINK_CONTROL( str, m_pkItemListElement[iLoop - 1]);
  }
    
  
  
  LINK_CONTROL( "staticgrade", m_pkStaticGrade);
  LINK_CONTROL( "staticperiod", m_pkStaticPeriod);
  LINK_CONTROL( "staticduration", m_pkStaticDuration);
  LINK_CONTROL( "staticlevel", m_pkStaticLevel);
  LINK_CONTROL( "staticitemid", m_pkStaticItemId);

  

}

KGCAddItemInClientItemList::~KGCAddItemInClientItemList( void )
{
}

void KGCAddItemInClientItemList::OnCreate( void )
{
    
    for(int iLoop = 0; iLoop < ITEMLISTNUM; ++iLoop)
    {
        if( m_pkItemListElement[iLoop] )
            m_pkItemListElement[iLoop]->InitState( true );
      //  m_apkCheckBox[iLoop]->InitState( true, true, this );
      //  m_apkBookMarkCheckBox[iLoop]->InitState( true, true, this );
    }

    if( m_pkStaticGrade )
    {
        m_pkStaticGrade->InitState(true, true, this);
        m_pkStaticGrade->SetText(L"등급");
    }
    if( m_pkStaticPeriod )
    {
        m_pkStaticPeriod->InitState(true, true, this);
        m_pkStaticPeriod->SetText(L"기간");
    }
    if( m_pkStaticDuration )
    {
        m_pkStaticDuration->InitState(true, true, this);
        m_pkStaticDuration->SetText(L"수량");
    }
    if( m_pkStaticLevel )
    {
        m_pkStaticLevel->InitState(true, true, this);
        m_pkStaticLevel->SetText(L"레벨");
    }

    if( m_pkStaticItemId )
    {
        m_pkStaticItemId->InitState(true, true, this);
        m_pkStaticItemId->SetText(L"아이템ID");
    }
    
    
    g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_KEY_DOWN, this, &KGCAddItemInClientItemList::OnKeyDown );

}
std::map<int, std::vector<const WCHAR*>> KGCAddItemInClientItemList::GetItemData()
{
    std::map<int, std::vector<const WCHAR*>> mapitemdata;
    typedef pair <int, std::vector<const WCHAR*>> pairvecitem;
    int iLoop = 0;
    for(; iLoop < ITEMLISTNUM ; ++iLoop)
    {
        
        mapitemdata.insert(pairvecitem(iLoop, m_pkItemListElement[iLoop]->GetItemData()));
    }
    return mapitemdata;

}
void KGCAddItemInClientItemList::SetItemData(  std::vector<int> vecItemData)
{
    unsigned int iLoop = 0;
    int j = 0;
    std::vector<int> ivecItemData;

    for(; iLoop < ITEMLISTNUM ; ++iLoop)
    {
        if(vecItemData.size()/4 <= iLoop)
            break;
        ivecItemData.clear();
        for(int i = 0; i < 4; i ++)       
        {
            ivecItemData.push_back(vecItemData.at(j));
            j++;
        }
        m_pkItemListElement[iLoop]->SetItemData(ivecItemData);
    }
    if( iLoop < 10 )
    {
        ivecItemData.clear();
        ivecItemData.push_back(-1);ivecItemData.push_back(-1);ivecItemData.push_back(-1);ivecItemData.push_back(-1);
        for(; iLoop < ITEMLISTNUM; ++iLoop)
        {
            m_pkItemListElement[iLoop]->SetItemData(ivecItemData);
        }
    }
}
void KGCAddItemInClientItemList::SetItemDataText(const WCHAR* text)
{
    int iLoop = 0;
    for(; iLoop < ITEMLISTNUM ; ++iLoop)
    {
        m_pkItemListElement[iLoop]->SetItemDataText(text);
    }
}
void KGCAddItemInClientItemList::SetText( std::vector<GCItem*> vecwstring)
{
    int iLoop = 0;
    unsigned int itercount = 0;
    for(; iLoop < ITEMLISTNUM ; ++iLoop)
    {
       if(vecwstring.size() > itercount )
       {
           m_pkItemListElement[iLoop]->SetText(vecwstring.at(iLoop)->strItemName);
           m_pkItemListElement[iLoop]->SetItemIdText(vecwstring.at(iLoop)->dwGoodsID);
       }
       else
       {
           m_pkItemListElement[iLoop]->SetText(L"");    
           m_pkItemListElement[iLoop]->SetItemIdText(0);    
       }
       itercount++;
    }

}
bool KGCAddItemInClientItemList::GetCheckBoxState(int icheck)
{
    return m_pkItemListElement[icheck]->GetCheckBoxState();
}

bool KGCAddItemInClientItemList::GetBookMarkState(int icheck)
{
    return m_pkItemListElement[icheck]->GetBookMarkState();
}
void KGCAddItemInClientItemList::SetCheckBoxState(int icheck, bool bcheck)
{
    for( int iLoop = 0; iLoop < ITEMLISTNUM; ++iLoop )
    {
        if( icheck == iLoop)
            m_pkItemListElement[iLoop]->SetCheckBoxState( bcheck );
    }
}
void KGCAddItemInClientItemList::SetBookMarkState(int icheck, bool bcheck)
{
    for( int iLoop = 0; iLoop < ITEMLISTNUM; ++iLoop )
    {
        if( icheck == iLoop)
            m_pkItemListElement[iLoop]->SetBookMarkState( bcheck );
    }
  //  m_apkBookMarkCheckBox[icheck]->SetCheck(bcheck);
}
void KGCAddItemInClientItemList::SetUpDownFocusControl()
{
    for( int iLoop = 0; iLoop < ITEMLISTNUM; ++iLoop)
    {
        if( iLoop == m_iColumn)
            m_pkItemListElement[iLoop]->m_pkName->SetFontColor(D3DCOLOR_ARGB(255, 255, 0, 0));
        else
            m_pkItemListElement[iLoop]->m_pkName->SetFontColor(D3DCOLOR_ARGB(255, 255, 255, 255));
    }
}
void KGCAddItemInClientItemList::OnKeyDown( const KActionEvent& event )
{
    if( (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO || g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM )&& m_pkItemListElement[0]->IsRenderOn() ) 
    {
        if( event.m_dwlParam == DIK_SPACE )
        {
             if( m_iRow == -2 )
                m_pkItemListElement[m_iColumn]->m_apkBookMarkCheckBox->ToggleCheck();
             else if( m_iRow == -1 )
                  m_pkItemListElement[m_iColumn]->m_apkCheckBox->ToggleCheck();
        }
    }
}   
void KGCAddItemInClientItemList::SetCurrentCursorPos(int iColumn, int iRow)
{
    m_iColumn = iColumn;
    m_iRow = iRow;
}
void KGCAddItemInClientItemList::SetLeftRightFocusControl()
{
    for( int iLoop = 0; iLoop < ITEMLISTNUM; ++iLoop)
    {
        m_pkItemListElement[iLoop]->m_apkBookMarkCheckBox->SetAllWindowColor(D3DCOLOR_ARGB(255, 255, 255, 255));
        m_pkItemListElement[iLoop]->m_apkCheckBox->SetAllWindowColor(D3DCOLOR_ARGB(255, 255, 255, 255));
    }
    for( int iLoop = 0; iLoop < ITEMLISTNUM; ++iLoop)
    {
        if( iLoop == m_iColumn )
        {
            if( m_iRow == -2 )
            {
                /*::SetFocus(NULL);
                m_pkItemListElement[iLoop]->m_apkBookMarkCheckBox->Activate(true);*/
                m_pkItemListElement[iLoop]->m_apkBookMarkCheckBox->SetAllWindowColor(D3DCOLOR_ARGB(255, 255, 0, 0));
                /*if( g_pkInput->IsDown( DIK_SPACE ))
                {
                     m_pkItemListElement[iLoop]->m_apkBookMarkCheckBox->ToggleCheck();
                }*/
            }
            else if( m_iRow == -1 )
            {
                /*::SetFocus(NULL);
                m_pkItemListElement[iLoop]->m_apkCheckBox->Activate(true);*/
                m_pkItemListElement[iLoop]->m_apkCheckBox->SetAllWindowColor(D3DCOLOR_ARGB(255, 255, 0, 0));
                /*if( g_pkInput->IsDown( DIK_SPACE ))
                {
                    m_pkItemListElement[iLoop]->m_apkCheckBox->ToggleCheck();
                }*/
            }
            else if( m_iRow == 0 )
            {
                //m_pkItemListElement[iLoop]->m_pkGrade->SetFocus();
            }
            else if( m_iRow == 1 )
            {
                m_pkItemListElement[iLoop]->m_pkGrade->SetFocus();
            }
            else if( m_iRow == 2 )
            {
                m_pkItemListElement[iLoop]->m_pkPeriod->SetFocus();
            }
            else if( m_iRow == 3 )
            {
                m_pkItemListElement[iLoop]->m_pkDuration->SetFocus();
            }
            else if( m_iRow == 4 )
            {
                m_pkItemListElement[iLoop]->m_pkLevel->SetFocus();
            }
        }
            
            //m_pkItemListElement[iLoop]->m_apkCheckBox->SetFontColor(D3DCOLOR_ARGB(255, 255, 255, 255));
    }
}
void KGCAddItemInClientItemList::FrameMoveInEnabledState()
{
}

void KGCAddItemInClientItemList::ActionPerformed( const KActionEvent& event )
{}              

#else

KGCAddItemInClientItemList::KGCAddItemInClientItemList( void ){}
KGCAddItemInClientItemList::~KGCAddItemInClientItemList( void ){}
void KGCAddItemInClientItemList::OnCreate( void ){}
void KGCAddItemInClientItemList::FrameMoveInEnabledState(){}
void KGCAddItemInClientItemList::ActionPerformed( const KActionEvent& event ){}

#endif // __PATH__