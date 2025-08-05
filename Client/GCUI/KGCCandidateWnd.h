#ifndef _KGCCANDIDATEWND_H_
#define _KGCCANDIDATEWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DSizingBox.h"

#define MAX_CANDIDATE_SIZE	5

class KGCCandidateWnd : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCCandidateWnd );
	/// Default constructor
	KGCCandidateWnd( void );
	/// Default destructor
	virtual ~KGCCandidateWnd( void );
	/// Copy constructor
	KGCCandidateWnd( const KGCCandidateWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCandidateWnd& operator=( const KGCCandidateWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void OnDestroy( void );

	void SetCurrCandidate(int iIndex);
	void SetCandidate(int iIndex, WCHAR* strCandidate);
	//void SetPage(int iCurrPage, int iTotalPage);
	
	void SetPosition( D3DXVECTOR2 vPos, float fTextHeight );

private:
	KSafeArray<KD3DStatic*,MAX_CANDIDATE_SIZE> m_pCandidate;
	//KD3DStatic* m_pPage;
	KD3DSizingBox*	m_pSizingBox;
	int m_iCurrCandidate;
};

DEFINE_WND_FACTORY( KGCCandidateWnd );
DECLARE_WND_FACTORY( KGCCandidateWnd );
DECLARE_WND_FACTORY_NAME( KGCCandidateWnd );

#endif // _KGCCANDIDATEWND_H_