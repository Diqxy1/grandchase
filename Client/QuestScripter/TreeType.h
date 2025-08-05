// TreeType.h: interface for the CTreeType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREETYPE_H__B3378694_9AD2_4136_9766_EF9EC54988E6__INCLUDED_)
#define AFX_TREETYPE_H__B3378694_9AD2_4136_9766_EF9EC54988E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct NODE_TYPE
{
	WORD wNbItems;
    WORD wBufSize;
    HTREEITEM hti;
	UINT uState;
	char *buf;
}; 

enum TREE_TYPE_ERROR
{
    TTE_MEMORY,
    TTE_INVALID_FILE,
	TTE_INVALID_CSV_FILE,
    TTE_OPEN,
    TTE_READ,
    TTE_WRITE,
    TTE_CLOSE,
	TTE_EMPTY_FILE,
    TTE_INVALID_DIRECTORY,
	TTE_MAX_LEVEL,
};

class CTreeType  
{
public:
    CTreeType();
	virtual ~CTreeType();
	BOOL AllocateTree(size_t uCount);
    void DeAllocateTree();

	WORD m_wTotalNbItems;
    NODE_TYPE *m_pnt;
};

#endif // !defined(AFX_TREETYPE_H__B3378694_9AD2_4136_9766_EF9EC54988E6__INCLUDED_)

