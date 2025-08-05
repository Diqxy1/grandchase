#include "stdafx.h"
#include ".\particleeditor.h"
#include "Tool_Particle.h"
//#include <sstream>
#include "DlgEditValue.h"
#include "DlgScriptEdit.h"
#include "GCDeviceManager.h"

CParticleEditor* g_pParticleEditor;

CParticleEditor::CParticleEditor()
{
	g_pParticleEditor	= this;
	m_pParticleSystem	= new CKTDGParticleSystem();
	m_pParticleSystem->InitDevice();
	m_pWorkParticle		= NULL;
	m_fTimeTotal		= 0.0f;	
}

CParticleEditor::~CParticleEditor(void)
{
	SAFE_DELETE(m_pParticleSystem);
	g_pParticleEditor = NULL;
}

CMinMax<float>& CParticleEditor::StrToMinMax( CMinMax<float>& MinMax, CString str )
{
	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	try
	{

		tokenizer.Tokenize( (LPCTSTR)str );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();

		tokenizer.ProcessNumber( MinMax, IterCur, IterEnd );
	}
	catch(...)
	{
	}
	return MinMax;
}

CMinMax<D3DXVECTOR2>& CParticleEditor::StrToMinMax( CMinMax<D3DXVECTOR2>& MinMax, CString str )
{
	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	try
	{

		tokenizer.Tokenize( (LPCTSTR)str );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();

		tokenizer.ProcessVector2( MinMax, IterCur, IterEnd );
	}
	catch(...)
	{
	}
	return MinMax;
}

CMinMax<D3DXVECTOR3>& CParticleEditor::StrToMinMax( CMinMax<D3DXVECTOR3>& MinMax, CString str )
{
	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	try
	{

		tokenizer.Tokenize( (LPCTSTR)str );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();

		tokenizer.ProcessVector( MinMax, IterCur, IterEnd );
	}
	catch(...)
	{
	}
	return MinMax;
}

CMinMax<D3DXCOLOR>& CParticleEditor::StrToMinMax( CMinMax<D3DXCOLOR>& MinMax, CString str )
{
	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	try
	{

		tokenizer.Tokenize( (LPCTSTR)str );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();

		tokenizer.ProcessColor( MinMax, IterCur, IterEnd );
	}
	catch(...)
	{
	}
	return MinMax;
}

bool CParticleEditor::LoadScript( const TCHAR* strFileName )
{	
	RemoveAllParticle();

	m_strLastError = "0 error(s), you're good to go!";
	FILE* fp = fopen( strFileName, "rb" );
	long lFileSize = 0;
	char* script = NULL;
	bool rt = true;

	// Calc FileSize & Make Buffer
	fseek(fp, 0, SEEK_END );
	lFileSize = ftell(fp) + 1;
	script = new char[lFileSize];
	memset(script,0,lFileSize);

	// Read
	fseek(fp, 0, SEEK_SET );
	fread( script,1, lFileSize, fp );

	rt = m_pParticleSystem->Compile( script );

	SAFE_DELETE_ARRAY( script );

	

	if( rt )
	{
		ParticleToWindow();
	}

	return rt;	
}

bool CParticleEditor::SaveScript( const TCHAR* strFileName )
{
	FILE* fp = fopen( strFileName, "wb" );

	fprintf( fp, "particlesystem \"KTDGParticleScript\" 2.00\r\n" );
	fprintf( fp, "{\r\n" );
	{
		// EventSequence
		std::map<std::string, CKTDGParticleSystem::CParticleEventSequence*>::iterator itor;
		itor = m_pParticleSystem->m_mapSequencePool.begin();
		CString script;
		while( itor != m_pParticleSystem->m_mapSequencePool.end() )
		{
			script = SequenceToScript(itor->second);
			fprintf(fp,(LPCTSTR)script);
			itor++;
		}
	}
	fprintf( fp, "}" );
	fclose( fp );
	
	return true;
}

CKTDGParticleSystem::CParticleEventSequence* CParticleEditor::FindSequence(CString strName)
{
	std::map<std::string, CKTDGParticleSystem::CParticleEventSequence*>::iterator itor = m_pParticleSystem->m_mapSequencePool.find( (LPCTSTR)strName );

	if( itor == m_pParticleSystem->m_mapSequencePool.end() )
        return NULL;

	return itor->second;
}

CString CParticleEditor::SequenceToScript(CKTDGParticleSystem::CParticleEventSequence* pSequence,CString strName)
{
	CString strOut;
	CString strTemp;
	if( strName == "" )
		strOut.Format("\teventsequence \"%s\" \r\n", pSequence->GetName().c_str() );
	else
		strOut.Format("\teventsequence \"%s\" \r\n", strName );

	strOut += "\t{\r\n";
	strOut += "\t\t";

	strTemp.Format( "sourceblendmode = %s \r\n\t\t", CKTDGParticleSystem::CParticleEmitterTokenizer::m_BlendingModes[pSequence->GetSrcBlendMode()-1].name );
	strOut += strTemp;
	strTemp.Format( "destblendmode = %s \r\n\t\t", CKTDGParticleSystem::CParticleEmitterTokenizer::m_BlendingModes[pSequence->GetDestBlendMode()-1].name );
	strOut += strTemp;
	strTemp.Format( "numparticles = %d \r\n\t\t", pSequence->GetMaxParticleNum() );
	strOut += strTemp;	
	strTemp.Format( "gravity = %s \r\n\t\t", pSequence->GetGravity().toString().c_str() );
	strOut += strTemp;
	strTemp.Format( "emitradius = %s \r\n\t\t", pSequence->GetEmitRadius().toString().c_str() );		
	strOut += strTemp;
	strTemp.Format( "emitrate = %s \r\n\t\t", pSequence->GetEmitRate().toString().c_str() );
	strOut += strTemp;
	strTemp.Format( "lifetime = %s \r\n\t\t", pSequence->GetLifeTime().toString().c_str() );
	strOut += strTemp;
	strTemp.Format( "killtime = %.1f \r\n\t\t", pSequence->GetKillTime() );
	strOut += strTemp;
	strTemp.Format( "killnum = %d \r\n\t\t", pSequence->GetKillNum() );
	strOut += strTemp;
	strTemp.Format( "layer = %d \r\n\t\t", pSequence->GetLayer() );
	strOut += strTemp;
	strTemp.Format( "texture = \"%s\" \r\n", pSequence->GetTexName().c_str() );
	strOut += strTemp;

	// Events
	std::string strEvent;
	for(int i = 0; i < (int)pSequence->m_Events.size(); i++)
	{
		pSequence->m_Events[i]->toString(strEvent);
		strTemp.Format( "\t\t%s\r\n", strEvent.c_str() );		
		strOut += strTemp;
	}
	
	strTemp.Format( "\t}\r\n" );
	strOut += strTemp;
	return strOut;
}

void CParticleEditor::Render()
{
	// 모든 레이어를 렌더링합니다.
	for( int i=0 ; i<g_AppFormView->m_ComboLayer.GetCount() ; ++i )
		m_pParticleSystem->Render(i);
}

void CParticleEditor::FrameMove(float fElapsedTime)
{
	m_fTimeTotal += fElapsedTime;
	
	m_pParticleSystem->FrameMove( m_fTimeTotal, fElapsedTime );
}

void CParticleEditor::RemoveAllParticle()
{
	SAFE_DELETE( m_pParticleSystem );
	m_pParticleSystem = new CKTDGParticleSystem();
	m_pParticleSystem->InitDevice();
	g_AppFormView->m_ListParticle.DeleteAllItems();
}

void CParticleEditor::ParticleToWindow()
{
	std::map<std::string, CKTDGParticleSystem::CParticleEventSequence*>::iterator i;
	i = m_pParticleSystem->m_mapSequencePool.begin();
	while( i != m_pParticleSystem->m_mapSequencePool.end() )
	{
		AddParticleSequence(i->second);
		i++;
	}
}

CKTDGParticleSystem::CParticleEventSequence* CParticleEditor::OnCreateParticle(CString strScript, bool bAddToSystem )
{
	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	try
	{
		tokenizer.Tokenize( (LPCTSTR)strScript );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();

		CKTDGParticleSystem::CParticleEventSequence *newseq = new CKTDGParticleSystem::CParticleEventSequence();

		m_pParticleSystem->ProcessEventSequenceBlock( *newseq, IterCur, IterEnd );
		if( bAddToSystem )
		{
			newseq->SetTexture(g_pGCDeviceManager->CreateTexture( newseq->GetTexName() ));
			m_pParticleSystem->m_mapSequencePool[newseq->GetName()] = newseq;
			AddParticleSequence( newseq );
		}
		return newseq;
	}
	catch(...)
	{
	}
	return NULL;
}

void CParticleEditor::AddParticleSequence(CKTDGParticleSystem::CParticleEventSequence* pSequence)
{	
	int nItem = g_AppFormView->m_ListParticle.GetItemCount();
	g_AppFormView->m_ListParticle.InsertItem( nItem, pSequence->GetName().c_str());
	g_AppFormView->m_ListParticle.SetItemData( nItem, (DWORD)pSequence );
}

void CParticleEditor::SetWorkParticle( CKTDGParticleSystem::CParticleEventSequence* pSequence )
{	
	m_pWorkParticle = pSequence;
	if( pSequence == NULL )
		return;

	g_AppFormView->m_strName = pSequence->GetName().c_str();
	g_AppFormView->m_uiMaxParticle = pSequence->GetMaxParticleNum();
	g_AppFormView->m_strLifeTime = pSequence->GetLifeTime().toString().c_str();
	g_AppFormView->m_BlendSrc.SetCurSel( pSequence->GetSrcBlendMode() -1);
	g_AppFormView->m_BlendDest.SetCurSel( pSequence->GetDestBlendMode() -1);
	g_AppFormView->m_strGravity = pSequence->GetGravity().toString().c_str();
	g_AppFormView->m_strEmitRadius = pSequence->GetEmitRadius().toString().c_str();
	g_AppFormView->m_strEmitRate = pSequence->GetEmitRate().toString().c_str();
	g_AppFormView->m_fKillTime = pSequence->GetKillTime();
	g_AppFormView->m_uiKillNum = pSequence->GetKillNum();
	g_AppFormView->m_ComboLayer.SetCurSel( pSequence->GetLayer() );

	g_AppFormView->m_strTexture = pSequence->GetTexName().c_str();

	// TimeLine Editor==============================================================	
	g_AppFormView->m_GridTimeLine.RemoveAllKeyFrame();
	g_AppFormView->m_GridTimeLine.SetMaxTime( (DWORD)(pSequence->GetLifeTime().m_Min * 10.0f) );

	for(int i = 0; i < (int)pSequence->m_Events.size(); i++ )
	{	
		g_AppFormView->m_GridTimeLine.AddEvent(pSequence->m_Events[i]);
	}
	g_AppFormView->m_GridTimeLine.Display();
	// =============================================================================
	CreateSampleParticle();	

	g_AppFormView->UpdateData(FALSE);	
}

void CParticleEditor::CreateSampleParticle()
{
	if( m_pWorkParticle == NULL )
		return;
	std::vector<CString> vecList;
	g_AppFormView->m_ListParticle.CreateSampleParticle(vecList, m_pWorkParticle);
	m_pParticleSystem->DestroyAllInstance();

	// ===================================CREATE PARTICLE==================================
	CKTDGParticleSystem::CParticleEventSequence* pRenderParticle;
	CKTDGParticleSystem::CParticleEventSequence* pParticleTemplate;	
	for(int i = 0; i < (int)vecList.size(); i++ )
	{		
		pRenderParticle = m_pParticleSystem->CreateSequence( (LPCTSTR)vecList[i], 0.0f,0.0f,0.0f );		
		pParticleTemplate = m_pParticleSystem->m_mapSequencePool[(LPCTSTR)vecList[i]];

		if( pParticleTemplate->GetEmitRate().m_Max == 0 )
		{
			float fEmitRate = pParticleTemplate->GetMaxParticleNum() / pParticleTemplate->GetLifeTime().m_Min;
			CMinMax<float> fRate;
			fRate.m_Min = fRate.m_Max = fEmitRate;
			pRenderParticle->SetEmitRate( fRate );
		}
		//else
		//{
		//	pRenderParticle->SetEmitRate( pParticleTemplate->GetEmitRate() );
		//}
	}
}

bool CParticleEditor::CheckParticleProperty()
{	
	g_AppFormView->UpdateData();

	//이름이 바꼈으니.. 다른 파티클이 그런 이름을 하고 있는지 알아봐야함..
	if( g_AppFormView->m_strName != m_pWorkParticle->GetName().c_str() )
	{
		if( FindSequence( g_AppFormView->m_strName ) != NULL )
		{
			if( FindSequence( g_AppFormView->m_strName ) != m_pWorkParticle )
			{
				AfxGetMainWnd()->MessageBox("이미 같은 이름을 가진 파티클이 존재합니다");
				g_AppFormView->m_strName = m_pWorkParticle->GetName().c_str();
				g_AppFormView->UpdateData(FALSE);
				return false;
			}
		}
	}

	if( g_AppFormView->m_fKillTime < 0.0f )
	{
		g_AppFormView->m_fKillTime = 0.0f;
	}

	g_AppFormView->m_strTexture.Trim();
	if( g_AppFormView->m_strTexture.IsEmpty() )
	{
		g_AppFormView->m_strTexture = "텍스쳐 이름을 입력하세요";
		g_AppFormView->UpdateData(FALSE);
		AfxGetMainWnd()->MessageBox("텍스쳐 이름을 입력하세요");
	}

	return true;
}

void CParticleEditor::UpdateParticleProperty()
{
	if( m_pWorkParticle == NULL )
		return;

	if( !CheckParticleProperty() )
		return;

	// ========================Name========================
	g_AppFormView->m_ListParticle.m_bLock = true;
	if( g_AppFormView->m_strName != m_pWorkParticle->GetName().c_str() )
	{
		//List의 이름도 바꿔줘야해..		
		g_AppFormView->m_ListParticle.SetSelectItemText( g_AppFormView->m_strName);
		//ParticleSystem의 map도 업데이트 해줘야함
		m_pParticleSystem->m_mapSequencePool.erase(m_pParticleSystem->m_mapSequencePool.find(m_pWorkParticle->GetName()));
		m_pParticleSystem->m_mapSequencePool[(LPCTSTR)g_AppFormView->m_strName] = m_pWorkParticle;

		//m_pParticleSystem->m_mapSequencePool.find();
	}
	g_AppFormView->m_ListParticle.m_bLock = false;
	m_pWorkParticle->SetName((LPCTSTR)g_AppFormView->m_strName);	

	// ========================Particle Num========================	
	m_pWorkParticle->SetMaxParticleNum(g_AppFormView->m_uiMaxParticle);
	
	// ========================LifeTime========================
	CMinMax<float> mmFloat;	
	float fOldLifeTime = m_pWorkParticle->GetLifeTime().m_Min;
	m_pWorkParticle->SetLifetime( StrToMinMax( mmFloat, g_AppFormView->m_strLifeTime) );
	if( mmFloat.m_Min != fOldLifeTime )
	{		
		g_AppFormView->m_GridTimeLine.SetMaxTime( (DWORD)(mmFloat.m_Min*10) );
	}

	// ========================Gravity&Radius========================
	CMinMax<D3DXVECTOR3> mmV3;
	m_pWorkParticle->SetGravity( StrToMinMax( mmV3, g_AppFormView->m_strGravity ) );
	m_pWorkParticle->SetEmitRadius(StrToMinMax( mmV3, g_AppFormView->m_strEmitRadius ) );
	m_pWorkParticle->SetEmitRate(StrToMinMax( mmFloat, g_AppFormView->m_strEmitRate ) );
	m_pWorkParticle->SetKillTime( g_AppFormView->m_fKillTime );
	m_pWorkParticle->SetKillNum( g_AppFormView->m_uiKillNum );
	
	// ========================Blend========================
	m_pWorkParticle->SetSrcBlendMode( g_AppFormView->m_BlendSrc.GetCurSel() + 1 );
	m_pWorkParticle->SetDestBlendMode( g_AppFormView->m_BlendDest.GetCurSel() + 1 );
	
	// ========================Texture========================
	m_pWorkParticle->SetTextureName( (LPCTSTR)g_AppFormView->m_strTexture);
	GCDeviceTexture* pOldTex = m_pWorkParticle->GetTexture();	// 1
	m_pWorkParticle->SetTexture(g_pGCDeviceManager->CreateTexture( (LPCTSTR)g_AppFormView->m_strTexture ));
	SAFE_RELEASE( pOldTex );									// 2.. 요렇게 하면 같은 파일 로딩하는 부하가 없음.

	// ========================Layer=========================
	m_pWorkParticle->SetLayer( g_AppFormView->m_ComboLayer.GetCurSel() );

	// ========================Events========================
	g_AppFormView->m_GridTimeLine.KeyFrameToParticleEvent(m_pWorkParticle);

	CreateSampleParticle();
}

void CParticleEditor::OnNewParticle()
{
	// Create Unique Name
	CString newName;
	int ino = 0;
	while( true )
	{	
		newName.Format("NewParticle%d",ino++);
		if( FindSequence(newName) == NULL )
			break;
	}

	CKTDGParticleSystem::CParticleEventSequence* pNewSequence = new CKTDGParticleSystem::CParticleEventSequence();
	pNewSequence->SetName( (LPCTSTR)newName );
	m_pParticleSystem->m_mapSequencePool[(LPCTSTR)newName] = pNewSequence;
	AddParticleSequence(pNewSequence);	
}

bool CParticleEditor::DeleteParticle(CKTDGParticleSystem::CParticleEventSequence* pSequence)
{
	std::map<std::string, CKTDGParticleSystem::CParticleEventSequence*>::iterator itor;
	itor = m_pParticleSystem->m_mapSequencePool.find(pSequence->GetName());
	if( itor == m_pParticleSystem->m_mapSequencePool.end() )
		return false;
	m_pParticleSystem->m_mapSequencePool.erase(itor);
	return true;
}

void CParticleEditor::SetGravity()
{
	if( m_pWorkParticle == NULL )
		return;
	CDlgEditValue dlg;
	dlg.SetValue( CKTDGParticleSystem::EVENT_VELOCITY, g_AppFormView->m_strGravity, FALSE );
	if( dlg.DoModal() != IDOK )
		return;

	g_AppFormView->m_strGravity = dlg.m_strValue;
	g_AppFormView->UpdateData(FALSE);
	UpdateParticleProperty();
}

void CParticleEditor::SetEmitRadius()
{
	if( m_pWorkParticle == NULL )
		return;
	CDlgEditValue dlg;
	dlg.SetValue( CKTDGParticleSystem::EVENT_VELOCITY, g_AppFormView->m_strEmitRadius, FALSE );
	if( dlg.DoModal() != IDOK )
		return;

	g_AppFormView->m_strEmitRadius = dlg.m_strValue;
	g_AppFormView->UpdateData(FALSE);
	UpdateParticleProperty();
}

void CParticleEditor::SetEmitRate()
{
	if( m_pWorkParticle == NULL )
		return;
	CDlgEditValue dlg;
	dlg.SetValue( CKTDGParticleSystem::EVENT_TYPE_NUM, g_AppFormView->m_strEmitRate, FALSE );
	if( dlg.DoModal() != IDOK )
		return;

	g_AppFormView->m_strEmitRate = dlg.m_strValue;
	g_AppFormView->UpdateData(FALSE);
	UpdateParticleProperty();
}

void CParticleEditor::OnScriptEdit()
{
	if ( m_pWorkParticle == NULL )
	{
		AfxGetMainWnd()->MessageBox("파티클을 먼저 선택하세요");
		return;
	}
	CDlgScriptEdit dlg;
	dlg.m_strScript = SequenceToScript(m_pWorkParticle);

domodal:
	if( dlg.DoModal() != IDOK )
		return;

	CKTDGParticleSystem::CParticleEventSequence *newseq = OnCreateParticle(dlg.m_strScript, false );
	std::map<std::string, CKTDGParticleSystem::CParticleEventSequence*>::iterator itor;
	itor = m_pParticleSystem->m_mapSequencePool.find(newseq->GetName());
	if( itor != m_pParticleSystem->m_mapSequencePool.end() && itor->second != m_pWorkParticle)
	{
		AfxGetMainWnd()->MessageBox("이름이 다른 파티클과 중복됩니다.");
		SAFE_DELETE(newseq);
		goto domodal;	
	}
	
	
	//List의 이름도 바꿔줘야해..		
	g_AppFormView->m_ListParticle.SetSelectItemText( newseq->GetName().c_str() );
	g_AppFormView->m_ListParticle.SetSelectItemData( (DWORD)newseq );
	
	//ParticleSystem의 map도 업데이트 해줘야함	
	m_pParticleSystem->m_mapSequencePool.erase(m_pParticleSystem->m_mapSequencePool.find(m_pWorkParticle->GetName()));
	SAFE_DELETE(m_pWorkParticle);
	newseq->SetTexture(g_pGCDeviceManager->CreateTexture( newseq->GetTexName() ));
	m_pParticleSystem->m_mapSequencePool[newseq->GetName()] = newseq;
	SetWorkParticle(newseq);
}