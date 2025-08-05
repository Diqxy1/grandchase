#include "stdafx.h"
#include "GCGawibawiboGameLose.h"

IMPLEMENT_CLASSNAME( KGCGawibawiboGameLose );
IMPLEMENT_WND_FACTORY( KGCGawibawiboGameLose );
IMPLEMENT_WND_FACTORY_NAME( KGCGawibawiboGameLose, "gc_gawibawibo_game_lose_info_box" );

KGCGawibawiboGameLose::KGCGawibawiboGameLose( void )
:	m_pkBtnOK(NULL)
,	m_pkBtnRetry(NULL)
,	m_pkStaticTitle1(NULL)
,	m_pkStaticTitle2(NULL)
,	m_pkStaticClear(NULL)
,	m_pkStaticRetry(NULL)
,	m_iRetryCoin(0)
,	m_pkBtnCheck(NULL)
,	m_iResult(0)
{
	LINK_CONTROL("btn_ok",m_pkBtnOK);
	LINK_CONTROL("btn_retry",m_pkBtnRetry);
	LINK_CONTROL("btn_check",m_pkBtnCheck);
	LINK_CONTROL("static_title1",m_pkStaticTitle1);
	LINK_CONTROL("static_title2",m_pkStaticTitle2);
	LINK_CONTROL("static_clear",m_pkStaticClear);
	LINK_CONTROL("static_rechallenge",m_pkStaticRetry);
}

KGCGawibawiboGameLose::~KGCGawibawiboGameLose( void )
{
}

void KGCGawibawiboGameLose::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkBtnRetry )
		{
			if(m_iRetryCoin > 0){
				// �絵�� ��Ŷ ������. �׸��� ȭ�� ��ȯ ����.
				KP2P::GetInstance()->Send_GetUserGwibawiboGiveUpReq(1);
                Result_GawibawiboGameGiveUp = INT_MAX;
                g_MyD3D->WaitForServerAck( Result_GawibawiboGameGiveUp, INT_MAX, 5000, TIME_OUT_VALUE );
				m_ActionClickFunc();
			}
			else
			{
				//�絵�� ����Ʈ�� ���� �˷���� �ȴ�.
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_ERR_9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);

			}
		}
		else if( event.m_pWnd == m_pkBtnOK )
		{
			// ���� ��Ŷ ������. �׸��� ȭ�� ��ȯ ����.
			KP2P::GetInstance()->Send_GetUserGwibawiboGiveUpReq(2);
            Result_GawibawiboGameGiveUp = INT_MAX;
            g_MyD3D->WaitForServerAck( Result_GawibawiboGameGiveUp, INT_MAX, 5000, TIME_OUT_VALUE );
			m_ActionClickFunc();
		}
		else if(event.m_pWnd == m_pkBtnCheck)
		{
			// �̰������� ������� Ȯ�ο뵵�θ� ���� �༮��.!!!
			m_ActionClickFunc();
		}
	}


}

void KGCGawibawiboGameLose::OnCreate( void )
{
	m_pkBtnOK->InitState(true,true,this);
	m_pkBtnRetry->InitState(true,true,this);
	m_pkBtnCheck->InitState(true,true,this);

	m_pkStaticTitle1->InitState(true,false,this);
	m_pkStaticTitle2->InitState(true,false,this);
	m_pkStaticClear->InitState(true,false,this);
	m_pkStaticRetry->InitState(true,false,this);
	
	SetResultInfo();
}

void KGCGawibawiboGameLose::SetResultInfo()
{
	// �� ��Ȳ�� �´� �ؽ�Ʈ�� ��� �ݽô�.
	// ���̵� �߱��� 4(������), 4(�������), 4(�̰�����) �� �����Ƿ� ���̵� ���� ���ðų� �Ͻø� �̻��� ��Ʈ����
	// ��� �� �� �ֽ��ϴ�.

	int strid = 4;
	strid *= m_iResult;

	//if(m_iResult == GR_LOSE)
	//	strid *= 0;
	//if(m_iResult == GR_WIN)
	//	strid *= 1;
	//if(m_iResult == GR_DRAW)
	//	strid *= 2;

	m_pkStaticTitle1->SetAlign(DT_CENTER);
	m_pkStaticTitle2->SetAlign(DT_CENTER);
    m_pkStaticClear->SetAlign(DT_CENTER);
    m_pkStaticRetry->SetAlign(DT_CENTER);

	m_pkStaticTitle1->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_1+strid ));
	m_pkStaticTitle2->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_2+strid ));
	m_pkStaticClear->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_3+strid ));
	m_pkStaticRetry->SetText(g_pkStrLoader->GetString( STR_ID_GAWIBAWIBO_LOSE_4+strid ));

	// �� ��Ȳ�� �´� ��ư�� ���� �ݴϴ�~!

	if(m_iResult == GR_LOSE)
	{
		m_pkBtnOK->ToggleRender(true);
		m_pkBtnRetry->ToggleRender(true);
		m_pkBtnCheck->ToggleRender(false);
	}
	else if(m_iResult == GR_WIN)
	{
		m_pkBtnOK->ToggleRender(false);
		m_pkBtnRetry->ToggleRender(false);
		m_pkBtnCheck->ToggleRender(true);

	}
	else
	{
		m_pkBtnOK->ToggleRender(false);
		m_pkBtnRetry->ToggleRender(false);
		m_pkBtnCheck->ToggleRender(true);
	}
	
}

void KGCGawibawiboGameLose::SetRetryCoin(int iCoin_)
{
	m_iRetryCoin = iCoin_;	
}



