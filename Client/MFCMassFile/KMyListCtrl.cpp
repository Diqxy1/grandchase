// KMyListCtrl.cpp: implementation of the KMyListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KMyListCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KMyListCtrl::KMyListCtrl()
{

}

KMyListCtrl::~KMyListCtrl()
{

}
BOOL KMyListCtrl::SortUp( int nCol, bool bAscending )
{
	bool bFlag = false;
	bool bSorted = false;
	int nItem = GetItemCount();
	
	//while(bFlag == false)
	for(int j = 0; j < nItem; j++)
	{
		bFlag = true;
		for (int i = 0 ; i < nItem; i++)
		{
			int cmp;
			if(bAscending)
				cmp = lstrcmp(GetItemText(i,nCol),GetItemText(i+1,nCol));
			else
				cmp = lstrcmp(GetItemText(i+1,nCol),GetItemText(i,nCol));
			
			if (cmp < 0)
			{
				SwapItem(i,i+1);
				bFlag = false;
				bSorted = true;
			}
		
		}
	}

	/*if( bAscending )
	{
		while(bFlag == false)
		{
			bFlag = true;
			for (int i = 0 ; i < nItem; i++)
			{
				int cmp = lstrcmp(GetItemText(i,nCol),GetItemText(i+1,nCol));
				if (cmp < 0)
				{
					SwapItem(i,i+1);
					bFlag = false;
					bSorted = true;
				}
			
			}
		}
	}

	else
	{
		while(bFlag == false)
		{
			bFlag = true;
			for (int i = nItem ; i > 0; i--)
			{
				int cmp = lstrcmp(GetItemText(i,nCol),GetItemText(i-1,nCol));
				if (cmp > 0)
				{
					SwapItem(i,i-1);
					bFlag = false;
					bSorted = true;
				}
				
				
			}
		}

	}*/

	return bSorted;
	
}
BOOL KMyListCtrl::SwapItem(int nItem1, int nItem2)
{
    if (nItem1 < 0 || nItem2 < 0 ||
        nItem1 >= GetItemCount() || nItem2 >= GetItemCount()) return FALSE;
    int nCount = GetHeaderCtrl()->GetItemCount();
    if (nCount <= 0) return FALSE;
    LVITEM lvItem1, lvItem2;
    lvItem1.stateMask = lvItem2.stateMask = 0xFFFF;
    char szBuff1[_MAX_PATH], szBuff2[_MAX_PATH];
    lvItem1.pszText = szBuff1;
    lvItem1.cchTextMax = sizeof(szBuff1);
    lvItem2.pszText = szBuff2;
    lvItem2.cchTextMax = sizeof(szBuff2);
    for (int i=0; i<nCount; i++) {
        if (i==0) 
            lvItem1.mask = lvItem2.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_STATE;
        else lvItem1.mask = lvItem2.mask = LVIF_TEXT;
        //Get Item
        lvItem1.iItem = nItem1;
        lvItem1.iSubItem = i;
        GetItem(&lvItem1);
        lvItem2.iItem = nItem2;
        lvItem2.iSubItem = i;
        GetItem(&lvItem2);
        //Set Item
        lvItem1.iItem = nItem2;
        lvItem1.iSubItem = i;
        SetItem(&lvItem1);
        lvItem2.iItem = nItem1;
        lvItem2.iSubItem = i;
        SetItem(&lvItem2);
    }
    //Set User Data
    DWORD w1, w2;
    w1 = (DWORD)GetItemData(nItem1);
    w2 = (DWORD)GetItemData(nItem2);
    SetItemData(nItem1, w2);
    SetItemData(nItem2, w1);
    EnsureVisible(nItem2, FALSE);
    return TRUE;
}
