#include "stdafx.h"
//#include <assert.h>
//#include <algorithm>
#include ".\timelineeditor.h"
#include "DlgEditValue.h"
#include "Tool_Particle.h"
#include "ParticleEditor.h"

CTimeLineEditor::CTimeLineEditor()
{
	InitTimeLine();	
	m_iClipBoardRow = -1;
}

CTimeLineEditor::~CTimeLineEditor(void)
{
	RemoveAllKeyFrame();
}

void CTimeLineEditor::InitTimeLine()
{
	BOOL m_bEditable = TRUE;
	BOOL m_bListMode = FALSE;
	int m_nRows = CKTDGParticleSystem::EVENT_TYPE_NUM + 1;
	int m_nCols = 1;
	int m_nFixRows = 1;
	int m_nFixCols = 1;

	SetEditable(m_bEditable);
	SetListMode(m_bListMode);
	EnableDragAndDrop(TRUE);
	SetTextBkColor(RGB(255, 255, 192));

	SetRowCount(m_nRows);
	SetColumnCount(m_nCols);
	SetFixedRowCount(m_nFixRows);
	SetRowResize( FALSE );
	SetColumnResize( FALSE );
	SetFixedColumnCount(m_nFixCols);


	DWORD dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;

	// fill rows/cols with text
	for (int row = 0; row < GetRowCount(); row++) 
	{		
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;			
		Item.nFormat = DT_LEFT|DT_WORDBREAK;
		if( row == 0 )		
			Item.szText = "Time(0.1s)";
		else
		{
			Item.szText = strCParticleEventName[row-1];
		}
		
		
		SetItem(&Item);	
		SetRowHeight(row,20);
		SetColumnWidth(0,100);
		SetItemState(row,0, GetItemState(row,0) | GVIS_READONLY);
		SetItemBkColour(row, 0, RGB(150,150,150) );
	}

	SetMaxTime();
	/*
	if (rand() % 10 == 1) 
	{
	COLORREF clr = RGB(rand() % 128+128, 
	rand() % 128+128, 
	rand() % 128+128);
	SetItemBkColour(row, col, clr);
	SetItemFgColour(row, col, RGB(255,0,0));
	}		
	*/
	//	
	
}

void CTimeLineEditor::RemoveAllKeyFrame(int iLimitCol)
{
	for( int row = 1; row < GetRowCount(); row++ )
	{
		for( int col = iLimitCol; col < GetColumnCount(); col++ )
		{
			if( col == 0 )
				continue;
			SKeyFrame* pFrame = (SKeyFrame*)GetItemData(row,col);
			SAFE_DELETE( pFrame );
			SetItemData(row,col,0);

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR;
			Item.row = row;
			Item.col = col;
			Item.nFormat = DT_LEFT|DT_WORDBREAK;			
			Item.crBkClr = RGB(255, 255, 192);				
			Item.szText = "";

			SetItem(&Item);
			
		}
	}
}

void CTimeLineEditor::SetMaxTime(DWORD dwMaxTime)
{	
	m_dwMaxTime = dwMaxTime;

	RemoveAllKeyFrame( dwMaxTime+2 );

	SetColumnCount(dwMaxTime+2);

	for( int col = 1; col < GetColumnCount(); col++ )
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;			
		Item.nFormat = DT_LEFT|DT_WORDBREAK;
		Item.crBkClr = RGB(185,186,247);
		
		if( col % 10 == 1 )
		{
			Item.szText.Format("%d", (col - 1) / 10 );
			Item.mask |= GVIF_BKCLR;
			Item.crBkClr = RGB(185,186,247);
		}

		SetColumnWidth(col, 20);
		SetItem(&Item);	
		//SetItemState(0,col, GetItemState(0,col) | GVIS_READONLY);
	}
}

bool CTimeLineEditor::AddEvent(CKTDGParticleSystem::CParticleEvent* pEvent)
{
	SKeyFrame KeyFrame1;
	SKeyFrame KeyFrame2;

	KeyFrame1.m_bFade = false;
	KeyFrame2.m_bFade = pEvent->IsFade();
	
	switch( pEvent->GetEventType() )
	{
	case CKTDGParticleSystem::EVENT_SIZE:
		{		
			CKTDGParticleSystem::CParticleEvent_Size* pEventSize = (CKTDGParticleSystem::CParticleEvent_Size*)pEvent;			
			KeyFrame2.m_strFrameData = pEventSize->GetSize().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_ROTATE:
		{
			CKTDGParticleSystem::CParticleEvent_Rotate* pEventRot = (CKTDGParticleSystem::CParticleEvent_Rotate*)pEvent;
			KeyFrame2.m_strFrameData = pEventRot->GetVelocity().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_COLOR:
		{
			CKTDGParticleSystem::CParticleEvent_Color* pEventColor = (CKTDGParticleSystem::CParticleEvent_Color*)pEvent;
			KeyFrame2.m_strFrameData = pEventColor->GetColor().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_VELOCITY:
		{
			CKTDGParticleSystem::CParticleEvent_Velocity* pEventVel = (CKTDGParticleSystem::CParticleEvent_Velocity*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetVelocity().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_BLACKHOLE:
		{
			CKTDGParticleSystem::CParticleEvent_BlackHole* pEventBlackHole = (CKTDGParticleSystem::CParticleEvent_BlackHole*)pEvent;
			KeyFrame2.m_strFrameData = pEventBlackHole->GetEventTimer().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_VELOCITYX:
		{
			CKTDGParticleSystem::CParticleEvent_VelocityX* pEventVel = (CKTDGParticleSystem::CParticleEvent_VelocityX*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetValue().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_VELOCITYY:
		{
			CKTDGParticleSystem::CParticleEvent_VelocityY* pEventVel = (CKTDGParticleSystem::CParticleEvent_VelocityY*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetValue().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_VELOCITYZ:
		{
			CKTDGParticleSystem::CParticleEvent_VelocityZ* pEventVel = (CKTDGParticleSystem::CParticleEvent_VelocityZ*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetValue().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_ROTATEX:
		{
			CKTDGParticleSystem::CParticleEvent_RotateX* pEventVel = (CKTDGParticleSystem::CParticleEvent_RotateX*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetValue().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_ROTATEY:
		{
			CKTDGParticleSystem::CParticleEvent_RotateY* pEventVel = (CKTDGParticleSystem::CParticleEvent_RotateY*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetValue().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_ROTATEZ:
		{
			CKTDGParticleSystem::CParticleEvent_RotateZ* pEventVel = (CKTDGParticleSystem::CParticleEvent_RotateZ*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetValue().toString().c_str();
		}
		break;
	case CKTDGParticleSystem::EVENT_TIMER:
		{
			CKTDGParticleSystem::CParticleEvent_EventTimer* pEventVel = (CKTDGParticleSystem::CParticleEvent_EventTimer*)pEvent;
			KeyFrame2.m_strFrameData = pEventVel->GetEventTimer().toString().c_str();
		}
		break;
	default:
		return false;
	}
	
	if( pEvent->IsFade() )
	{
		AddKeyFrame( KeyFrame1, (DWORD)(pEvent->GetActualTime().m_Min*10), pEvent->GetEventType() );
	}
	AddKeyFrame( KeyFrame2, (DWORD)(pEvent->GetActualTime().m_Max*10), pEvent->GetEventType() );

	
	//AddKeyFrame( pEvent->GetEventType() );

	return true;
}

bool CTimeLineEditor::AddKeyFrame(SKeyFrame& KeyFrame, DWORD dwTime, CKTDGParticleSystem::EventType eEventType)
{	
	int row = eEventType+1;
	int col = dwTime + 1;
	SKeyFrame* pKeyFrame = (SKeyFrame*)GetItemData( row, col );
	
	if( pKeyFrame != NULL )
	{
		if(!KeyFrame.m_bFade)
			return true;		// 이미 들어있음

		SAFE_DELETE( pKeyFrame );
	}
	
	SetItemData( row, col, (LPARAM)(new SKeyFrame(KeyFrame)) );
	return true;
}

void CTimeLineEditor::Display()
{		
	SKeyFrame* pKeyFrame = NULL;

	for( int iEventType = 0; iEventType < CKTDGParticleSystem::EVENT_TYPE_NUM; iEventType++ )
	{	
		std::map<DWORD,SKeyFrame*>::iterator itor;
		bool bFillContinueFrame = false;
		for( int icol = GetColumnCount() - 1; icol > 0; icol-- )
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR;
			Item.row = iEventType+1;
			Item.col = icol;
			Item.nFormat = DT_LEFT|DT_WORDBREAK;

			pKeyFrame = (SKeyFrame*)GetItemData( Item.row, Item.col );
			
			if( pKeyFrame == NULL )
			{
				Item.crBkClr = RGB(255, 255, 192);
				if( bFillContinueFrame )		
					Item.szText = "→";
				else
					Item.szText = "";
			}
			else
			{				
				if( !pKeyFrame->m_bFade )
				{
					Item.crBkClr = RGB(255,132,225);
					Item.szText = "□";
					bFillContinueFrame = false;
				}
				else
				{
					Item.crBkClr = RGB(255,132,225);
					Item.szText = "■";
					bFillContinueFrame = true;
				}
			}		
			SetItem(&Item);		
		}
	}
	Invalidate(TRUE);
}

bool CTimeLineEditor::EditCell(int nRow, int nCol)
{
	CDlgEditValue dlgEditValue;
	SKeyFrame* pFrame = (SKeyFrame*)GetItemData( nRow, nCol );

	dlgEditValue.SetValue( (CKTDGParticleSystem::EventType)(nRow - 1), pFrame->m_strFrameData, pFrame->m_bFade );
	
	if( dlgEditValue.DoModal() != IDOK )
		return false;

	pFrame->m_strFrameData = dlgEditValue.m_strValue;
	pFrame->m_bFade = (dlgEditValue.m_bFade == TRUE);
	if( nCol == 1 )
		pFrame->m_bFade = FALSE;
	g_pParticleEditor->UpdateParticleProperty();
	return true;
}

void CTimeLineEditor::OnEditCell(int nRow, int nCol, UINT nChar)
{
	if( nRow < 1 || nCol < 1 )
		return;

	SKeyFrame* pFrame = (SKeyFrame*)GetItemData( nRow, nCol );
	bool bMakeMode = false;

	if( pFrame == NULL)
	{
		pFrame = new SKeyFrame();
		pFrame->m_strFrameData = "";
		pFrame->m_bFade = false;
		SetItemData( nRow, nCol, (LPARAM)pFrame );		
		bMakeMode = true;

		//이전의 데이터를 찾아서 비슷하게 만들어 주자..
		for(int ifind = nCol-1; ifind > 0; ifind-- )
		{
			SKeyFrame* pKeyFrame = (SKeyFrame*)GetItemData( nRow, ifind );
			if( pKeyFrame == NULL )
				continue;
			*pFrame = *pKeyFrame;
			break;
		}
	}

	if( EditCell(nRow,nCol) )
	{
		//적용시킵니다.
		Display();
		g_AppFormView->Invalidate();
	}
	else
	{
		if(bMakeMode)
		{
			SAFE_DELETE(pFrame);
			SetItemData(nRow, nCol, 0);
		}
	}	
}

int CTimeLineEditor::FindFadeFrame(CKTDGParticleSystem::EventType eEventType, int iStartCol )
{
	for(int icol = iStartCol; icol < GetColumnCount(); icol++ )
	{
		SKeyFrame* pKeyFrame = (SKeyFrame*)GetItemData( (int)eEventType + 1, icol );
		if( pKeyFrame == NULL )
			continue;
		
		if( pKeyFrame->m_bFade )
		{
			return icol;
		}
		return -1;		
	}
	return -1;
}

void CTimeLineEditor::KeyFrameToParticleEvent(CKTDGParticleSystem::CParticleEventSequence* pWorkParticle)
{
	/*
	EVENT_SIZE,			
	EVENT_COLOR,
	EVENT_ROTATE,
	EVENT_VELOCITY,
	EVENT_TYPE_NUM,
	*/	
	for( int i = 0; i < (int)pWorkParticle->m_Events.size(); i++ )
	{
		SAFE_DELETE( pWorkParticle->m_Events[i] );
	}
	pWorkParticle->m_Events.clear();

	for( int iEventType = 0; iEventType < CKTDGParticleSystem::EVENT_TYPE_NUM; iEventType++ )
	{
		for( int icol = 1; icol < GetColumnCount(); icol++ )
		{
			SKeyFrame* pKeyFrame = (SKeyFrame*)GetItemData( iEventType+1,icol );
			if( pKeyFrame == NULL )
			{				
				continue;
			}
			else
			{	
				//===============================Script 생성====================================				
				CString strScript;
				int iFadeFrame = FindFadeFrame((CKTDGParticleSystem::EventType)iEventType,icol+1);

				if( icol == 1 )	// 0 Frame일 경우
				{
					CString strValue = pKeyFrame->m_strFrameData;
					if( strValue == "" )
						strValue = EDIT_VALUE_TYPE_DEFAULT[iEventType];

					strScript.Format("at %f %s = %s",
						((float)(icol-1)) / 10.0f, 
						strCParticleEventName[iEventType], 
						(LPCTSTR) strValue
						);
					pWorkParticle->m_Events.push_back(ScriptToEvent(strScript));
					if( iFadeFrame == -1 )
						continue;
				}
				

				if( iFadeFrame == -1 )
				{			
					if( pKeyFrame->m_bFade )
						continue;
					strScript.Format("at %f %s = %s",
									((float)(icol-1)) / 10.0f, 
									strCParticleEventName[iEventType], 
									(LPCTSTR)pKeyFrame->m_strFrameData 
									);						
				}
				else
				{
					// "fade so"
					pKeyFrame = (SKeyFrame*)GetItemData( iEventType+1, iFadeFrame );
					strScript.Format("fade so from %f to %f %s = %s",
									((float)(icol-1)) / 10.0f,
									((float)(iFadeFrame-1)) / 10.0f,
									strCParticleEventName[iEventType], 
									(LPCTSTR)pKeyFrame->m_strFrameData
						);
				}			
				//===============================Script 생성 끝==================================				
				pWorkParticle->m_Events.push_back(ScriptToEvent(strScript));				
			}			
		}
	}
}

CKTDGParticleSystem::CParticleEvent* CTimeLineEditor::ScriptToEvent(CString strScript)
{
	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	bool IsFade = false;
	CMinMax<float> TimeRange;
	CKTDGParticleSystem::CParticleEvent* pEvent = NULL;
	try
	{
		tokenizer.Tokenize( (LPCTSTR)strScript );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();

		CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessTime(TimeRange, IsFade, 0.0f, 10.0f, IterCur, IterEnd);
		pEvent = CKTDGParticleSystem::EventFactory(IterCur->m_strValue); // parse the time range section of the event line
		pEvent->ProcessTokenStream(IterCur, IterEnd);
		pEvent->SetActualTime(TimeRange);
		pEvent->SetFade(IsFade);
	}
	catch(...)
	{
	}
	return pEvent;
}

BEGIN_MESSAGE_MAP(CTimeLineEditor, CGridCtrl)
ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CTimeLineEditor::DeleteKeyFrame(int nRow, int nCol, bool bUpdateNDisplay)
{
	SKeyFrame* pFrame = (SKeyFrame*)GetItemData(nRow, nCol);
	if( pFrame == NULL)
		return;

	SAFE_DELETE(pFrame);
	SetItemData(nRow,nCol,0);
	if( bUpdateNDisplay )
	{
		g_pParticleEditor->UpdateParticleProperty();
		Display();
	}	
}

void CTimeLineEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if( nChar == VK_DELETE )
	{
		if( GetSelectedCount() == 1 )
			DeleteKeyFrame(GetSelectedCellRange().GetMinRow(), GetSelectedCellRange().GetMinCol());
	}

	if ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )
	{
		if(nChar == 'X')
			OnEditCut();
		if(nChar == 'V')
			OnEditPaste();
		if(nChar == 'C')
			OnEditCopy();
	}

	CGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTimeLineEditor::OnEditCut()
{
	if( GetSelectedCount() == 0 )
		return;
	if( GetSelectedCount() > 1 )
	{
		MessageBox("한개씩만 잘라낼 수 있습니다");
		return;
	}
	CCellRange range = GetSelectedCellRange();	
	SKeyFrame* pFrame = (SKeyFrame*)GetItemData(range.GetMinRow(),range.GetMinCol());
	if( pFrame == NULL )
	{
//		MessageBox("데이터가 없습니다");
		return;
	}		
	//복사..
	m_iClipBoardRow = range.GetMinRow();
	m_keyClipboard = *pFrame;

	DeleteKeyFrame( range.GetMinRow(), range.GetMaxCol());
}
void CTimeLineEditor::OnEditCopy()
{
	if( GetSelectedCount() == 0 )
		return;
	if( GetSelectedCount() > 1 )
	{
		MessageBox("한개씩만 복사할 수 있습니다");
		return;
	}
	CCellRange range = GetSelectedCellRange();		
	SKeyFrame* pFrame = (SKeyFrame*)GetItemData(range.GetMinRow(),range.GetMinCol());
	if( pFrame == NULL )
	{			
		return;
	}
	m_iClipBoardRow = range.GetMinRow();
	m_keyClipboard = *pFrame;
}

bool CTimeLineEditor::EditPaste( int iRow, int iCol )
{
	if( m_iClipBoardRow != iRow )
		return false;

	if( GetItemData(iRow,iCol) == 0 )
	{
		SKeyFrame* pFrame = new SKeyFrame(m_keyClipboard);
		SetItemData( iRow, iCol, (LPARAM)pFrame);
	}
	else
	{
		SKeyFrame* pFrame =(SKeyFrame*)GetItemData(iRow,iCol);
		*pFrame = m_keyClipboard;
	}	

	g_pParticleEditor->UpdateParticleProperty();
	Display();
	return true;
}

void CTimeLineEditor::OnEditPaste()
{
	CCellID cell = GetFocusCell();
	if (!IsValid(cell)) 
	{
		if( GetSelectedCount() == 1 )
			EditPaste( GetSelectedCellRange().GetMinRow(), GetSelectedCellRange().GetMinCol() );
		return;
	}
	EditPaste(cell.row,cell.col);	
}
/*
void CTimeLineEditor::OnBeginDrag()
{
	CCellID cell = GetFocusCell();
	if( GetItemData( cell.row, cell.col ) == NULL )
		return;

	m_cellDrag = cell;
	m_MouseMode = MOUSE_DRAGGING;
}

BOOL CTimeLineEditor::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	BOOL bResult = FALSE;
	CCellID cell = GetCellFromPt(point, FALSE);

	if (!IsValid(cell))	
		return FALSE;

	if( cell.col == 0 )
		return FALSE;

	if( cell.row != m_cellDrag.row )
		return FALSE;

	// 복사하자!!
	CKTDGParticleSystem::CParticleEventSequence* pData = (CKTDGParticleSystem::CParticleEventSequence*)GetItemData(cell.row,cell.col);
	SAFE_DELETE( pData );
	SetItemData(cell.row,cell.col,GetItemData(m_cellDrag.row,m_cellDrag.col));
	SetItemData(m_cellDrag.row,m_cellDrag.col,0);

	m_cellDrag.row = -1;
	m_cellDrag.col = -1;

	return TRUE;
}
*/
void CTimeLineEditor::CutSelectedText()
{
	OnEditCut();
}

COleDataSource* CTimeLineEditor::CopyTextFromGrid()
{
	OnEditCopy();
	COleDataSource* pSource = new COleDataSource();
	return pSource;
}

BOOL CTimeLineEditor::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
	if( m_FirstDragOverCell == m_LastDragOverCell )
		return FALSE;
	return EditPaste(cell.row,cell.col);	
}