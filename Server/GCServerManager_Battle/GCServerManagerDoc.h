// GCServerManagerDoc.h : interface of the CGCServerManagerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GCSERVERMANAGERDOC_H__2EC2C8EA_CEEF_4DC9_BF15_699C9A597DF6__INCLUDED_)
#define AFX_GCSERVERMANAGERDOC_H__2EC2C8EA_CEEF_4DC9_BF15_699C9A597DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGCServerManagerDoc : public CDocument
{
protected: // create from serialization only
	CGCServerManagerDoc();
	DECLARE_DYNCREATE(CGCServerManagerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCServerManagerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGCServerManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGCServerManagerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCSERVERMANAGERDOC_H__2EC2C8EA_CEEF_4DC9_BF15_699C9A597DF6__INCLUDED_)
