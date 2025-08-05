#include "stdafx.h"
#include ".\gcsquarenpc.h"
#include "GCUI/KGCSquareOverlayUI.h"

DWORD KGCSquareNPC::g_dwSquareNPC_UID = 0;

KGCSquareNPC::KGCSquareNPC(void)
{
	if( g_dwSquareNPC_UID == INT_MAX )
        g_dwSquareNPC_UID = 0;

	m_dwUID = g_dwSquareNPC_UID++;

	m_dwLastSpeech = 0;
	m_iSpeechSize = 0;
}

KGCSquareNPC::~KGCSquareNPC(void)
{
}

void KGCSquareNPC::FrameMove()
{
	KGCSquare3DObj::FrameMove();
	FrameMove_NPCSpeech();
}

void KGCSquareNPC::AddNPCSpeechList( std::wstring strSpeech )
{
	if ( strSpeech != L"" )
	{
		std::vector<std::wstring>::iterator vit = m_vecSpeechStr.begin();
		for (; vit != m_vecSpeechStr.end(); ++vit )
		{
			if ( *vit == strSpeech )
				return;
		}

		m_vecSpeechStr.push_back( strSpeech );
	}
}

void KGCSquareNPC::FrameMove_NPCSpeech( void )
{
 	m_iSpeechSize = m_vecSpeechStr.size();
// 
// 	if(m_iSpeechSize <= 1)
// 		return;

	// 마지막 입력시간에서 일정 시간이상 지나면?
	if( timeGetTime() - m_dwLastSpeech > 1000 * m_fSpeechTimter/*초*/ && m_iSpeechSize > 0 )
	{
		int i = rand() % m_iSpeechSize;
		m_dwLastSpeech = timeGetTime();
		g_pkSquareOverlayUI->SetBalloonMsg( m_vecSpeechStr[i], INT_MAX * (DWORD)2 - m_dwUID, KChatData::MSG_COMMON, 0xffffffff );
		//NPC는 UID가 마이너스로 채팅을 합니다.
	}

	D3DXVECTOR3 vPos;
	D3DXMATRIX matBasic;

	Get_BasicMatrix( &matBasic );
	D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3(0,0,0), &matBasic );

	// offsetS
	vPos.x -= 0.50f;
	vPos.y += 1.00f;

	//vPos.y += (m_vRect.y - (m_vRect.y * m_sMeshInfo.m_fScale));

	vPos.x += GetObjectLocalPos().x;

	// world coordinates -> screen coordinates
	float fScreenX =( vPos.x + 1.0f ) * 400.0f ;
	float fScreenY = ( -vPos.y + 0.75f ) * 400.0f ;

	g_pkSquareOverlayUI->ReLocateBalloon( D3DXVECTOR2(fScreenX + m_fSpeechOffsetX , fScreenY + m_fSpeechOffsetY ), INT_MAX * (DWORD)2 - m_dwUID, m_iSpeechDistance );
}
