// KMyListCtrl.h: interface for the KMyListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KMYLISTCTRL_H__9048D456_A080_4E47_95F3_3D8D4F40C847__INCLUDED_)
#define AFX_KMYLISTCTRL_H__9048D456_A080_4E47_95F3_3D8D4F40C847__INCLUDED_

class KMyListCtrl : public CListCtrl  
{
public:
	KMyListCtrl();
	virtual ~KMyListCtrl();
	BOOL SwapItem(int nItem1, int nItem2);
	BOOL SortUp( int nCol, bool bAscending );
};

#endif // !defined(AFX_KMYLISTCTRL_H__9048D456_A080_4E47_95F3_3D8D4F40C847__INCLUDED_)
