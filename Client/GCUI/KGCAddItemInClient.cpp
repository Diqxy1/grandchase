#include "stdafx.h"

#include "gcui/KGCAddItemInClient.h"
#include "gcui/KGCAddItemInClientItemList.h"


#include "GCItemManager.h"




IMPLEMENT_CLASSNAME( KGCAddItemInClient );
IMPLEMENT_WND_FACTORY( KGCAddItemInClient );
IMPLEMENT_WND_FACTORY_NAME( KGCAddItemInClient, "gc_additeminclient" );

#ifndef __PATH__
KGCAddItemInClient::KGCAddItemInClient( void )
{

    m_pkSearchBack = NULL;
    m_pkSearchButton = NULL;
    m_pkAddButton = NULL;
    m_pkEditSearch = NULL;
    m_pkItemList = NULL;
    for(int i = 0; i < NUM_BTN; ++i)
    {
        m_apkBtn[i] = NULL;
    }
   
    m_pkPageBack = NULL;
    m_pkStaticPage = NULL;
    m_pkCharTypeCombo = NULL;
    m_pkAllItemListCheckBox = NULL;
    m_pkAddBookMark = NULL;
    m_pkAllItemListBookMark = NULL;
    m_pkDeleteBookMark = NULL;

    m_pkBookMarkBack = NULL;
    m_pkStaticBookMark= NULL;   
    m_pkStaticCheck= NULL;
    m_pkStaticNameID= NULL;
    m_pkStaticCharType= NULL;
    m_pkStaticSearch= NULL;
    m_pkStaticAdd= NULL;

    m_pkStaticBookMarkTip = NULL;

    LINK_CONTROL( "search_bar_back", m_pkSearchBack );
    LINK_CONTROL( "search_bar_searchbutton", m_pkSearchButton );
    LINK_CONTROL( "search_bar_addbutton", m_pkAddButton ); //UI Script Tool에서 컨트롤 순서 바꾸니까 크래시가 사라짐. 연구필요
    LINK_CONTROL( "search_bar_edit", m_pkEditSearch );
    LINK_CONTROL( "additemitemlist", m_pkItemList ); //additem_itemlist를 additemitemlist로 바꾸니까 크래시가 사라짐. 연구필요
    
    LINK_CONTROL( "btnfirst",       m_apkBtn[ BTN_FIRST ] );
    LINK_CONTROL( "btnprev",        m_apkBtn[ BTN_PREV ] );
    LINK_CONTROL( "btnnext",        m_apkBtn[ BTN_NEXT ] );
    LINK_CONTROL( "btnend",         m_apkBtn[ BTN_END ] );
    
    
    LINK_CONTROL( "pageback",     m_pkPageBack );
    LINK_CONTROL( "staticpage",     m_pkStaticPage );

    LINK_CONTROL( "chartypecombobox",   m_pkCharTypeCombo );
    
    LINK_CONTROL( "allitemlistcheckbox", m_pkAllItemListCheckBox);

    
    LINK_CONTROL( "addbookmark", m_pkAddBookMark);
    
    LINK_CONTROL( "allitemlistbookmark", m_pkAllItemListBookMark);
    
    LINK_CONTROL( "deletebookmark", m_pkDeleteBookMark);

    LINK_CONTROL( "static_bookmark", m_pkStaticBookMark);
    LINK_CONTROL( "static_check", m_pkStaticCheck);
    LINK_CONTROL( "static_nameID", m_pkStaticNameID);
    LINK_CONTROL( "static_chartype", m_pkStaticCharType);
    LINK_CONTROL( "static_search", m_pkStaticSearch);
    LINK_CONTROL( "static_add", m_pkStaticAdd);
    
    LINK_CONTROL( "bookmark_back", m_pkBookMarkBack);

    LINK_CONTROL( "static_bookmarktip", m_pkStaticBookMarkTip);
    

    m_usCurrentPage = 0;
    m_usMaxPage = 0;
    m_usDisplayMinItemNum = 0;
    m_usDisplayMaxItemNum = m_usDisplayMinItemNum + ITEMLISTNUM;
    m_icount = 0;
    m_bLoaded = false;
    m_bDeleteBookMark = false;
    m_bSearch = false;
    m_iColumn = 0;
    m_iRow = 0;

}

KGCAddItemInClient::~KGCAddItemInClient( void )
{
}



void KGCAddItemInClient::OnCreate( void )
{
    if( m_pkSearchBack )
        m_pkSearchBack->InitState( false );
    
    if( m_pkSearchButton )
    m_pkSearchButton->InitState( false , true, this );
    
    if( m_pkAddButton )
    m_pkAddButton->InitState( false, true, this );

    if( m_pkSearchBack )
        m_pkSearchBack->InitState( false, true, this );
    
    if( m_pkEditSearch)
    m_pkEditSearch->InitState( false , true, this );

    if( m_pkItemList )
    m_pkItemList->InitState( false, true, this );

    for( int i=0 ; i<NUM_BTN ; ++i )
    {    
        if( m_apkBtn[i] )
        m_apkBtn[i]->InitState( false, true, this );
    }

    if( m_pkPageBack )
    m_pkPageBack->InitState( false );
    
    if( m_pkStaticPage )
    m_pkStaticPage->InitState(false);
    
    if( m_pkCharTypeCombo )
    m_pkCharTypeCombo->InitState( false, true, this);
    
    if( m_pkCharTypeCombo )
    m_pkCharTypeCombo->AddActionListener( this );
    
    if( m_pkAllItemListCheckBox )
    m_pkAllItemListCheckBox->InitState( false, true, this );
    
    if( m_pkAddBookMark )
    m_pkAddBookMark->InitState(false, true, this);
    
    if( m_pkAllItemListBookMark )
    m_pkAllItemListBookMark->InitState( false, true, this );
    
    if( m_pkDeleteBookMark )
    m_pkDeleteBookMark->InitState( false, true, this );
    

    g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_KEY_DOWN, this, &KGCAddItemInClient::OnKeyDown );

    if( m_pkCharTypeCombo)
    {
        m_pkCharTypeCombo->AddString(L"선택안함"); 
        for( int i = 0 ; i <  GC_CHAR_NUM; ++i )
            m_pkCharTypeCombo->AddString(GCFUNC::GetCharName(i));
    }

    
    if( m_pkStaticBookMark )
    {
        m_pkStaticBookMark->InitState( false, true, this);
        m_pkStaticBookMark->SetText(L"북마크");
    }
    if( m_pkStaticCheck )
    {
        m_pkStaticCheck->InitState( false, true, this);
        m_pkStaticCheck->SetText(L"체크");
    }
    if( m_pkStaticNameID )
    {
        m_pkStaticNameID->InitState( false, true, this);
        m_pkStaticNameID->SetText(L"아이템 이름/ID");
    }
    if( m_pkStaticCharType )
    {
        m_pkStaticCharType->InitState( false, true, this);
        m_pkStaticCharType->SetText(L"캐릭터타입");
    }
    if( m_pkStaticSearch )
    {
        m_pkStaticSearch->InitState( false, true, this);
        m_pkStaticSearch->SetText(L"검색");
    }
    if( m_pkStaticAdd )
    {
        m_pkStaticAdd->InitState( false, true, this);
        m_pkStaticAdd->SetText(L"추가");
    }

    if( m_pkBookMarkBack )
    {
        m_pkBookMarkBack->InitState(false);
    }
    if( m_pkStaticBookMarkTip )
    {
        m_pkStaticBookMarkTip->InitState( false, true, this );
        m_pkStaticBookMarkTip->SetTextAutoMultiline( L"북마크 사용법 : 북마크를 체크 후에 + 버튼을 누른다. Alt를 누르면 북마크 리스트가 나타난다.");
    }

}
void KGCAddItemInClient::OnKeyBookMark()
{
    OnRenderUpLine(true);
    OnRenderDownLine(true);
    m_usDisplayMinItemNum = 0;
    m_usDisplayMaxItemNum = m_usDisplayMinItemNum + ITEMLISTNUM;
    m_usCurrentPage = 1;
    LoadBookMark();
}
void KGCAddItemInClient::ClearEdit()
{
    m_pkEditSearch->SetText(L"");
}
void KGCAddItemInClient::OnKeyDown( const KActionEvent& event )
{
    if( (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO || g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
        && m_pkEditSearch->IsRenderOn() ) 
    {
        if( event.m_dwlParam == DIK_ESCAPE )
        {
            g_pkUIScene->EndMessageBox();
            AllRender(false);   
            ClearEdit();   
            m_iColumn = 0;
            m_iRow = 0;
            m_pkItemList->SetCurrentCursorPos(m_iColumn, m_iRow);
            m_pkItemList->SetUpDownFocusControl();
            m_pkItemList->SetLeftRightFocusControl();
        }
        else if( event.m_dwlParam == DIK_LMENU )
        {
            OnKeyBookMark();
        }
        else if( event.m_dwlParam == DIK_CAPSLOCK )
        {
            Refreshpage( BTN_NEXT );
        }
        else if( event.m_dwlParam == DIK_LSHIFT )
        {
            Refreshpage( BTN_PREV );
        }
        else if( event.m_dwlParam == DIK_DOWN )
        {
            m_iRow = 0;
            if( m_iColumn < 9)
                m_iColumn++;
            m_pkItemList->SetCurrentCursorPos(m_iColumn, m_iRow);
            m_pkItemList->SetUpDownFocusControl();
            m_pkItemList->SetLeftRightFocusControl();
            
        }
        else if( event.m_dwlParam == DIK_UP )
        {
            m_iRow = 0;
            if( m_iColumn >= 1)
                m_iColumn--;
            m_pkItemList->SetCurrentCursorPos(m_iColumn, m_iRow);
            m_pkItemList->SetUpDownFocusControl();
            m_pkItemList->SetLeftRightFocusControl();
        }
         else if( event.m_dwlParam == DIK_LEFT )
        {
            if( m_iRow > -2 )
                m_iRow--;
            m_pkItemList->SetCurrentCursorPos(m_iColumn, m_iRow);
            m_pkItemList->SetLeftRightFocusControl( );
        }
        else if( event.m_dwlParam == DIK_RIGHT )
        {
            if( m_iRow < 4)
                m_iRow++;
            m_pkItemList->SetCurrentCursorPos(m_iColumn, m_iRow);
            m_pkItemList->SetLeftRightFocusControl( );
        }
         else if( event.m_dwlParam == DIK_RETURN )
        {
            ActionAdd();
        }
        

    }
 /*   else if( event.m_dwlParam == DIK_RETURN )
    {
        PushSearchButton();
    }*/
}
void KGCAddItemInClient::FrameMoveInEnabledState()
{

}
void KGCAddItemInClient::AllRender(bool bRender)
{
    OnRenderUpLine(bRender);
    OnRenderDownLine(bRender);
}

void KGCAddItemInClient::OnRenderUpLine(bool bRender)
{
    m_pkSearchBack->ToggleRender( bRender );
    m_pkSearchButton->ToggleRender( bRender );
    m_pkAddButton->ToggleRender( bRender );
    m_pkEditSearch->ToggleRender( bRender );
    m_pkCharTypeCombo->ToggleRender( bRender );
    m_pkAllItemListCheckBox->ToggleRender( bRender );

    m_pkAllItemListBookMark->ToggleRender( bRender );

    m_pkStaticBookMark->ToggleRender( bRender );   
    m_pkStaticCheck->ToggleRender( bRender );
    m_pkStaticNameID->ToggleRender( bRender );
    m_pkStaticCharType->ToggleRender( bRender );
    m_pkStaticSearch->ToggleRender( bRender );
    m_pkStaticAdd->ToggleRender( bRender );
}
void KGCAddItemInClient::OnRenderDownLine(bool bRender)
{

 
    m_pkItemList->ToggleRender( bRender );
    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->ToggleRender( bRender );
  
    m_pkPageBack->ToggleRender( bRender );
    m_pkStaticPage->ToggleRender(bRender);
  
    m_pkBookMarkBack->ToggleRender( bRender );
    m_pkAddBookMark->ToggleRender( bRender );
 
    m_pkDeleteBookMark->ToggleRender( bRender );
    m_pkStaticBookMarkTip->ToggleRender( bRender );

}
void KGCAddItemInClient::LoadBookMark()
{
    FILE* fp = NULL;
    WCHAR strLine[MAX_PATH] = {0,};
    WCHAR seps[] = L"\n";
    WCHAR* strTemp = NULL;
    std::vector<std::wstring> vecString;
    
    
    m_bLoaded = true;
    
    if( (fp = _wfopen( L"BookMarkItemInfo.txt",L"r")) != NULL )
    {
        m_vecBookMarkItemDataList.clear();    
        m_vecBookMarkItemInfo.clear();
        while( fgetws(strLine, MAX_PATH, fp) != NULL)
        {
            if(strLine[0] == L'\n')
                continue;

            strTemp = wcstok( strLine , seps);
            
            if(strTemp == NULL)
                continue;


            GCUTIL_STR::Tokenizer( strTemp, L" ", vecString );
            
            m_vecBookMarkItemDataList.push_back(g_pItemMgr->GetItemData(boost::lexical_cast<DWORD>( vecString[0])));
             //여기에 차곡차곡 쌓아 놓고 밑에 RefreshItemList처럼 표시할걸 나누면 된다.
            
            m_vecBookMarkItemInfo.push_back(boost::lexical_cast<int>(vecString[1]));
            m_vecBookMarkItemInfo.push_back(boost::lexical_cast<int>(vecString[2]));
            m_vecBookMarkItemInfo.push_back(boost::lexical_cast<int>(vecString[3]));
            m_vecBookMarkItemInfo.push_back(boost::lexical_cast<int>(vecString[4]));
            
            vecString.clear();
        }

        if(m_vecBookMarkItemInfo.empty())
            return;
        
        RefreshItemList();
        fclose(fp); 
    }
    
    
    

}
void KGCAddItemInClient::RefreshItemList()
{
   std::vector<GCItem*> vecItemList;
    
   if(m_bLoaded == true)
   {
       
       std::vector<int> veciitemlist;
       std::vector<int>::iterator itor = m_vecBookMarkItemInfo.begin();
       
       
       for(unsigned int iLoop =  m_usDisplayMinItemNum * 4 ; iLoop < m_usDisplayMaxItemNum * 4; ++iLoop)
       {
           if( iLoop > m_vecBookMarkItemInfo.size() -1 )
               break;
           veciitemlist.push_back(m_vecBookMarkItemInfo.at(iLoop));
       }
           
       m_pkItemList->SetItemData(veciitemlist);  
       vecItemList = m_vecBookMarkItemDataList;
   }
   else
   {
       vecItemList = m_vecItemDataList;
       m_pkItemList->SetItemDataText(L"-1");
   }


   m_usMaxPage = vecItemList.size() / 10;
   
   if( (vecItemList.size() % 10) > 0 )
       m_usMaxPage++;
   if( m_usCurrentPage <= 0 || m_usCurrentPage > m_usMaxPage)
       m_usCurrentPage  = 1;
   if( m_usMaxPage <= 0)
       m_usMaxPage = 1;          

   if( m_bSearch )
   {
       m_usCurrentPage = 1;
       m_usDisplayMinItemNum = 0;
       m_usDisplayMaxItemNum = 10;
       m_bSearch = false;
   }
   std::wostringstream stm;

   stm << m_usCurrentPage << L" / " << m_usMaxPage; 
   m_pkStaticPage->SetText( stm.str() );


   m_vectoritemlistStorage.clear();
   unsigned int itercount = 0;
    for(unsigned int i = m_usDisplayMinItemNum ; i < m_usDisplayMaxItemNum; ++i)
    {
        if((vecItemList.size() > itercount) && vecItemList.size() > i)
            m_vectoritemlistStorage.push_back(vecItemList.at(i));
        itercount++;
    }

    m_pkItemList->SetText(m_vectoritemlistStorage);
     for(int i = 0; i < ITEMLISTNUM; ++i)
     {
         m_pkItemList->SetCheckBoxState(i, false);
         m_pkItemList->SetBookMarkState(i, false);
     }

}
void KGCAddItemInClient::ActionSearch()
{
    m_bSearch = true;
    OnRenderDownLine(true);

    m_bLoaded = false;

    m_vecItemDataList.clear();

    int iCurrentText = _wtoi(m_pkEditSearch->GetText());

    if( iCurrentText == 0)
        m_vecItemDataList = g_pItemMgr->GetItemIDFromName( m_pkEditSearch->GetText(), m_pkCharTypeCombo->GetCurSel());
    else
    {   
        GCItem* item = g_pItemMgr->GetItemData( iCurrentText / 10 ); 
        if( NULL != item)
            m_vecItemDataList.push_back( item );
    }

    if(!m_vecItemDataList.empty())
        RefreshItemList();
}
void KGCAddItemInClient::ActionAdd()
{
    std::map<int, std::vector<const WCHAR*>> mapitemdata;
    mapitemdata = m_pkItemList->GetItemData();            
    std::map<int, std::vector<const WCHAR*>>::iterator itor = mapitemdata.begin();

    std::vector<const WCHAR*> vecitemdata;
    std::vector<GCItem*> vectoritemlist;
    //체크박스 활성화 체크

    for(unsigned int i = 0; i < ITEMLISTNUM; ++i, ++itor  )
    {
        if( m_pkItemList->GetCheckBoxState(i) == true )
        {
            if(m_vectoritemlistStorage.size() <= i)
                break;
            vectoritemlist.push_back(m_vectoritemlistStorage.at(i));

            for(int j = 0; j < 4; j++)
                vecitemdata.push_back(itor->second.at(j));
        }
    }



    g_pItemMgr->AddItemInInventory(vectoritemlist,vecitemdata); 
}
void KGCAddItemInClient::ActionPerformed( const KActionEvent& event )
{
    if(g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO && g_MyD3D->m_pStateMachine->GetState() != GS_MY_INFO_FROM_ROOM)
        return;
    
   
    if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        //검색 버튼을 눌렸을때
        if( event.m_pWnd == m_pkSearchButton )
        {
            ActionSearch();
        }
        //확인(추가) 버튼을 눌렸을때
        if( event.m_pWnd == m_pkAddButton )
        {
            ActionAdd();
        }
        //전부 체크, 체크해제 체크박스를 눌렀을때
        if( event.m_pWnd == m_pkAllItemListCheckBox )
        {
            m_pkAllItemListCheckBox->ToggleCheck();
             for(int i = 0; i < ITEMLISTNUM; ++i)
             {
                if(m_pkAllItemListCheckBox->GetCheck() == true)
                    m_pkItemList->SetCheckBoxState(i, true);
                else    
                    m_pkItemList->SetCheckBoxState(i, false);
             }
        }
        //전부 북마크, 북마크해제 체크박스를 눌렀을때
        if( event.m_pWnd == m_pkAllItemListBookMark )
        {
            m_pkAllItemListBookMark->ToggleCheck();
            for(int i = 0; i < ITEMLISTNUM; ++i)
            {
                if(m_pkAllItemListBookMark->GetCheck() == true)
                    m_pkItemList->SetBookMarkState(i, true);
                else    
                    m_pkItemList->SetBookMarkState(i, false);
            }
        }    
        
        //북마크 추가 버튼을 눌렀을 때
        if( event.m_pWnd == m_pkAddBookMark )
        {
            m_bDeleteBookMark = false;
            std::map<int, std::vector<const WCHAR*>> mapitemdata;
            mapitemdata = m_pkItemList->GetItemData();            
            std::map<int, std::vector<const WCHAR*>>::iterator itor = mapitemdata.begin();
            m_vecBookMarkItemList.clear();
            m_vecBookMarkItemData.clear();                 
            
            for(unsigned int i = 0; i < ITEMLISTNUM; ++i, ++itor  )
            {
                if( m_pkItemList->GetBookMarkState(i) == true )
                {
                  
                    if(m_vectoritemlistStorage.size() <= i)
                        break;
                    m_vecBookMarkItemList.push_back(m_vectoritemlistStorage.at(i));    
                    

                    for(int j = 0; j < 4; j++)
                        m_vecBookMarkItemData.push_back(itor->second.at(j));                                   
                }
            }

            OnPrintBookMarkItemInfo();

        }
        //북마크 제거 버튼을 눌렸을 때 
        if( event.m_pWnd == m_pkDeleteBookMark ) 
        {
            m_bDeleteBookMark = true;
            m_vecBookMarkItemList.clear();

            for(unsigned int i = 0; i < ITEMLISTNUM; ++i)
            {
                if( m_pkItemList->GetBookMarkState(i) == true )
                {
                    if(m_vectoritemlistStorage.size() <= i)
                        break;
                    m_vecBookMarkItemList.push_back(m_vectoritemlistStorage.at(i));    
                }
            }
            OnPrintBookMarkItemInfo();
        }

    }


    // 엔터를 눌렀을 때
    if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
    {
        std::wstring strKeyword = m_pkEditSearch->GetText();
        if( strKeyword.size() > 0 )
        {
            ActionSearch();
        }
        else
        {
            SetFocus( false );
        }
    }

    //	페이지 버튼 처리
    if ( event.m_pWnd == m_apkBtn[BTN_FIRST] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
    {
        Refreshpage( BTN_FIRST );
    }
    else if ( event.m_pWnd == m_apkBtn[BTN_PREV] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
    {
        Refreshpage( BTN_PREV );
    }
    else if ( event.m_pWnd == m_apkBtn[BTN_END] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
    {
        Refreshpage( BTN_END );
    }
    else if ( event.m_pWnd == m_apkBtn[BTN_NEXT] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
    {
        Refreshpage( BTN_NEXT );
    }
}

void KGCAddItemInClient::OnPrintBookMarkItemInfo()
{
    FILE *fo;
    WCHAR strFileName[MAX_PATH];   

    swprintf(strFileName,L"BookMarkItemInfo.txt");
    if(m_bDeleteBookMark == true)
    {
        fo = _wfopen( strFileName,L"w"); 
        WCHAR strItemInfo[MAX_PATH];   
        std::vector<int> itemdata;
        std::vector<GCItem*> vectemsotrage;
        vectemsotrage = m_vecBookMarkItemDataList;
        std::vector<GCItem*>::iterator itor = vectemsotrage.begin();

         for(unsigned int i = 0; i < vectemsotrage.size(); ++i, ++itor  )
         {
             for(unsigned int j = 0; j < m_vecBookMarkItemList.size(); ++j  )
             {
                 if(vectemsotrage.at(i) == m_vecBookMarkItemList.at(j))
                     vectemsotrage.erase(itor);

             }
         }
    
        int iLoop = 0;
        fwprintf(fo,L"\n");
        for(unsigned int i = 0; i < vectemsotrage.size(); ++i)
        {
            itemdata.clear();
            for(int j = 0; j < 4; j++) 
            {
                itemdata.push_back(m_vecBookMarkItemInfo.at(iLoop));               
                iLoop++;
            }

            wsprintf(strItemInfo, L"%d     %4ld     %4ld     %4ld     %4ld\n",vectemsotrage.at(i)->dwGoodsID, 
                itemdata.at(0), itemdata.at(1), itemdata.at(2), itemdata.at(3));

            fwprintf(fo,L"%s",strItemInfo);

        }

    }
    else
    {
        fo = _wfopen( strFileName,L"a");
        
        WCHAR strItemInfo[MAX_PATH];   
        std::vector<const WCHAR*> itemdata;
        
        int iLoop = 0;
        fwprintf(fo,L"\n");
        for(unsigned int i = 0; i < m_vecBookMarkItemList.size(); ++i)
        {
            itemdata.clear();
            for(int j = 0; j < 4; j++) 
            {
                itemdata.push_back(m_vecBookMarkItemData.at(iLoop));               
                iLoop++;
            }
            
            wsprintf(strItemInfo, L"%d     %4ld     %4ld     %4ld     %4ld\n",m_vecBookMarkItemList.at(i)->dwGoodsID, 
                 _wtoi(itemdata.at(0)), _wtoi(itemdata.at(1)), _wtoi(itemdata.at(2)), _wtoi(itemdata.at(3)));
        
            fwprintf(fo,L"%s",strItemInfo);
            
        }
    }   
    fclose(fo); 
}

void KGCAddItemInClient::Refreshpage( BTN_TYPE emBtnType)
{
 if ( emBtnType < BTN_FIRST || emBtnType > BTN_END ) 
        return;

    switch ( emBtnType ) 
    {
   case BTN_FIRST:
        {
            if ( m_usCurrentPage <= 1 ) 
                return;
            m_usCurrentPage = 1;
            m_usDisplayMinItemNum = 0;
            m_usDisplayMaxItemNum = 10;
            
        }
        break;
    case BTN_PREV:
        {
            if(m_bLoaded == false)
            {
                if ( m_usCurrentPage <= 1 || m_vecItemDataList.empty() ) 
                   return;
            }
            else
            {
                if ( m_usCurrentPage <= 1 || m_vecBookMarkItemDataList.empty() ) 
                    return;
            }
            m_usCurrentPage--;
            m_usDisplayMinItemNum -= ITEMLISTNUM;
            m_usDisplayMaxItemNum -= ITEMLISTNUM;
            
        }
        break;
    case BTN_END:
        {
            if ( m_usCurrentPage >= m_usMaxPage ) 
                return;
            m_usCurrentPage = m_usMaxPage;

            m_usDisplayMinItemNum = ((m_usMaxPage -1) * 10);
            m_usDisplayMaxItemNum = ((m_usMaxPage -1) * 10) + 10;
            
        }
        break;
    case BTN_NEXT:
        {
            if(m_bLoaded == false)
            {
                if ( m_usCurrentPage >= m_usMaxPage || m_vecItemDataList.empty() ) 
                    return;
            }
            else
            {
                if ( m_usCurrentPage >= m_usMaxPage || m_vecBookMarkItemDataList.empty() ) 
                    return;
            }
            m_usCurrentPage++;
            m_usDisplayMinItemNum += ITEMLISTNUM;
            m_usDisplayMaxItemNum += ITEMLISTNUM;
            
        }
        break;
    }
    RefreshItemList();
}
void KGCAddItemInClient::Init()
{
    m_pkEditSearch->SetFocus();
    m_pkEditSearch->SetText( L"" );
}

void KGCAddItemInClient::SetFocus( bool bFocus )
{
    if( bFocus )
        m_pkEditSearch->SetFocus();
    else
        m_pkEditSearch->Activate( bFocus );
}
#else

KGCAddItemInClient::KGCAddItemInClient( void ){}
KGCAddItemInClient::~KGCAddItemInClient( void ){}
void KGCAddItemInClient::ActionPerformed( const KActionEvent& event ){}
void KGCAddItemInClient::OnCreate( void ){}
void KGCAddItemInClient::FrameMoveInEnabledState(){}

#endif // __PATH__