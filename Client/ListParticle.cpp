// ListParticle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool_Particle.h"
#include "ListParticle.h"
#include ".\listparticle.h"
#include "ParticleEditor.h"


// CListParticle

IMPLEMENT_DYNAMIC(CListParticle, CListCtrl)
CListParticle::CListParticle()
{
	m_bLock = false;
}

CListParticle::~CListParticle()
{	
}

void CListParticle::InitList()
{
	InsertColumn( 0, "Particle Name", LVCFMT_LEFT,300);		
	SetExtendedStyle( LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
}

// CListParticle 메시지 처리기입니다.
BEGIN_MESSAGE_MAP(CListParticle, CListCtrl)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
END_MESSAGE_MAP()




void CListParticle::OnDelete()
{
	//선택된게 빠지니깐.. 그 다음거 선택되도록 해야함
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nItem = GetNextSelectedItem(pos);
	if( g_pParticleEditor->DeleteParticle((CKTDGParticleSystem::CParticleEventSequence*)GetItemData(nItem)) )
	{
		DeleteItem(nItem);
		
		if( nItem >= GetItemCount() )	//맨 마지막걸 지우면..안된다.
			nItem--;

		if( nItem >= 0 )
			SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
		else
			g_pParticleEditor->SetWorkParticle(NULL);
	}
	
}

void CListParticle::OnCopy()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nItem = GetNextSelectedItem(pos);
	CKTDGParticleSystem::CParticleEventSequence* pSequence = (CKTDGParticleSystem::CParticleEventSequence*)GetItemData(nItem);
	if( pSequence == NULL )
		return;

	m_strClipboard = g_pParticleEditor->SequenceToScript(pSequence,"@NAME");
	m_strClipboardParticleName = pSequence->GetName().c_str();
}

void CListParticle::OnPaste()
{
	if( m_strClipboard.IsEmpty() )
		return;

	CString newName = m_strClipboardParticleName;
	while(true)
	{	
		newName.Format("copy %s",newName);
		if( g_pParticleEditor->FindSequence(newName) == NULL )
			break;
	}
	CString script = m_strClipboard;
	script.Replace("@NAME",newName);

	g_pParticleEditor->OnCreateParticle(script);
}

void CListParticle::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if( nChar == VK_DELETE )
	{
		OnDelete();
	}

	if( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )
	{
		if( nChar == 'C' )	//COPY
			OnCopy();
		if( nChar == 'V' )  //PASTE
			OnPaste();
	}

	// Copy & Paste & Delete 구현해야지
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CListParticle::CreateSampleParticle( std::vector<CString>& vecList, CKTDGParticleSystem::CParticleEventSequence* pWorkParticle )
{
	vecList.clear();
	vecList.push_back( pWorkParticle->GetName().c_str() );

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
	for(int i = 0; i < GetItemCount(); i++ )
	{
		pSeq = (CKTDGParticleSystem::CParticleEventSequence*)GetItemData(i);
		if( pSeq == pWorkParticle )
			continue;
		if( !GetCheck(i) )
			continue;
		vecList.push_back( pSeq->GetName().c_str() );
	}	
}

void CListParticle::SetSelectItemText(CString strName)
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nItem = GetNextSelectedItem(pos);
	SetItemText(nItem,0,strName);	
}

void CListParticle::SetSelectItemData(DWORD dwValue)
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nItem = GetNextSelectedItem(pos);
	SetItemData(nItem,dwValue);	
}

void CListParticle::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( !m_bLock )
	{	
		if( pNMLV->uNewState & LVIS_SELECTED )
		{	
			g_pParticleEditor->SetWorkParticle((CKTDGParticleSystem::CParticleEventSequence*) pNMLV->lParam );	
		}
		else
		{		
			if( ((BOOL)(pNMLV->uNewState >> 12) -1) != ((BOOL)(pNMLV->uOldState >> 12) -1 )	)
				g_pParticleEditor->CreateSampleParticle();		
		}
	}
	*pResult = 0;
	this->SetFocus();
}
