// GCServerManagerDoc.cpp : implementation of the CGCServerManagerDoc class
//

#include "stdafx.h"
#include "GCServerManager.h"
#include "GCServerManagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerDoc

IMPLEMENT_DYNCREATE(CGCServerManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGCServerManagerDoc, CDocument)
	//{{AFX_MSG_MAP(CGCServerManagerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerDoc construction/destruction

CGCServerManagerDoc::CGCServerManagerDoc()
{
	// TODO: add one-time construction code here
}

CGCServerManagerDoc::~CGCServerManagerDoc()
{
}

BOOL CGCServerManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerDoc serialization

void CGCServerManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerDoc diagnostics

#ifdef _DEBUG
void CGCServerManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGCServerManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerDoc commands
