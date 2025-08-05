#include "StdAfx.h"
#include "MyD3D.h"
#include "GCObjectAndMotion.h"

KGCObjectAndMotion::KGCObjectAndMotion(void)
: GCObject()
, m_iFrame(2)
, m_iCurrentMotion( 0 )
, m_iReservedMotion( -1 )
, m_iLoopMode(OAM_ALL_MOTION_LOOP)
, m_vOriginalPos( 0.0f, 0.0f, 0.5f )
, m_vPos( 0.0f, 0.0f, 0.5f )
, m_vScale( 1.0f, 1.0f, 1.0f )
, m_vRotation( 0.0f, 0.0f, 0.0f )
, m_vOffset( 0.0f, 0.0f, 0.0f )
, m_bFirstFrameForPosition( false )
, m_iFirstFrame( 0 ) 
, dwGoodsID( 0 )
{
}

KGCObjectAndMotion::~KGCObjectAndMotion(void)
{
	ClearMotion();
}

void KGCObjectAndMotion::FrameMove()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

	SetWorldMatrix();

	if( m_vecMotions.empty() )
		return;

	SetFrameWithCheck();
	++m_iFrame;
	// 모션이 끝났으면 다음 모션을 선택합니다.
	if( m_vecMotions[m_iCurrentMotion].first->GetNum_Frame() <= m_iFrame )
	{
		m_iFrame = m_iFirstFrame;
		if( m_vecMotions[m_iCurrentMotion].second == true )
		{
			m_mapTrash[m_vecMotions[m_iCurrentMotion].first->GetDeviceID()] = m_vecMotions[m_iCurrentMotion].first;
			m_vecMotions.erase( m_vecMotions.begin() + m_iCurrentMotion );
			if( m_iReservedMotion > 0 )
				--m_iReservedMotion;
		}
		else
		{
			switch( m_iLoopMode )
			{
			case OAM_ALL_MOTION_LOOP:
				++m_iCurrentMotion;
				break;
			case OAM_SINGLE_MOTION_LOOP:
				break;
			}
		}

		if( m_iReservedMotion != -1 )
		{
			m_iCurrentMotion = m_iReservedMotion;
			m_iReservedMotion = -1;
		}

		if( m_iCurrentMotion < 0 )
		{
			ASSERT( !"이런 조건에 걸릴리가 없다. 이건 음수가 절대로 되면 안된다. 메모리를 씹었던지 삽질을 한거다." );
			m_iCurrentMotion = 0;
		}

		if( (int)m_vecMotions.size() <= m_iCurrentMotion )
			m_iCurrentMotion = 0;

		if( m_iFrame < m_iFirstFrame )
			m_iFrame = m_iFirstFrame;
	}
}

void KGCObjectAndMotion::FrameMoveExtra()
{
	if (g_kGlobalValue.m_bFrameLock && o3 != 0)
		return;

	SetWorldMatrix();

	if (m_vecMotions.empty())
		return;

	SetFrameWithCheck();
	++m_iFrame;

	if (m_iFrame >= m_vecMotions[m_iCurrentMotion].first->GetNum_Frame())
		m_iFrame = m_vecMotions[m_iCurrentMotion].first->GetNum_Frame() - 1;

	if (m_vecMotions[m_iCurrentMotion].first->GetNum_Frame() <= m_iFrame)
	{
		m_iFrame = m_iFirstFrame;
		if (m_vecMotions[m_iCurrentMotion].second == true)
		{
			m_mapTrash[m_vecMotions[m_iCurrentMotion].first->GetDeviceID()] = m_vecMotions[m_iCurrentMotion].first;
			m_vecMotions.erase(m_vecMotions.begin() + m_iCurrentMotion);
			if (m_iReservedMotion > 0)
				--m_iReservedMotion;
		}
		else
		{
			switch (m_iLoopMode)
			{
			case OAM_ALL_MOTION_LOOP:
				++m_iCurrentMotion;
				break;
			case OAM_SINGLE_MOTION_LOOP:
				break;
			}
		}

		if (m_iReservedMotion != -1)
		{
			m_iCurrentMotion = m_iReservedMotion;
			m_iReservedMotion = -1;
		}

		if (m_iCurrentMotion < 0)
		{
			ASSERT(!"이런 조건에 걸릴리가 없다. 이건 음수가 절대로 되면 안된다. 메모리를 씹었던지 삽질을 한거다.");
			m_iCurrentMotion = 0;
		}

		if ((int)m_vecMotions.size() <= m_iCurrentMotion)
			m_iCurrentMotion = (int)m_vecMotions.size() - 1;

		if (m_iFrame < m_iFirstFrame)
			m_iFrame = m_iFirstFrame;
	}
}

void KGCObjectAndMotion::AddMotion( std::string strMotion, bool bOneTimeMotion /*= false*/, bool bReserveNext /*= false*/ )
{
	std::map< std::string, GCDeviceMotionFRM* >::iterator mit;
	mit = m_mapTrash.find( strMotion );
	GCDeviceMotionFRM* pMotion = NULL;
	if( mit == m_mapTrash.end() )
		pMotion = g_pGCDeviceManager->CreateMotionFRM( strMotion );
	else
		pMotion = mit->second;
	
	if( pMotion == NULL )
	{
		ASSERT( !"Motion File is NULL" );
		return;
	}
#ifndef __PATH__
    if( pMotion->GetNum_Frame() == 0 )
    {
        std::string strText;
        strText = strMotion + " 모션 파일이 없음";
        MessageBoxA(NULL,strText.c_str() ,"", MB_OK );        
        return;
    }
#endif

	m_vecMotions.push_back( std::make_pair( pMotion, bOneTimeMotion ));

	if(bReserveNext)
		m_iReservedMotion=m_vecMotions.size()-1;
}

void KGCObjectAndMotion::AddMotion_Front( std::string strMotion, bool bOneTimeMotion /*= false*/ )
{
    std::map< std::string, GCDeviceMotionFRM* >::iterator mit;
    mit = m_mapTrash.find( strMotion );
    GCDeviceMotionFRM* pMotion = NULL;
    if( mit == m_mapTrash.end() )
        pMotion = g_pGCDeviceManager->CreateMotionFRM( strMotion );
    else
        pMotion = mit->second;

    if( pMotion == NULL )
    {
        ASSERT( !"Motion File is NULL" );
        return;
    }
#ifndef __PATH__
    if( pMotion->GetNum_Frame() == 0 )
    {
        std::string strText;
        strText = strMotion + " 모션 파일이 없음";
        MessageBoxA(NULL,strText.c_str() ,"", MB_OK );        
        return;
    }
#endif

    // 이전 모션 모두 제거( 기본모션 제외 )
    if( 0 < m_iCurrentMotion )
    {
        std::vector< std::pair<GCDeviceMotionFRM*, bool> >::iterator vecIter = m_vecMotions.begin() + 1;
        while( vecIter != m_vecMotions.end() )
        {
            m_mapTrash[vecIter->first->GetDeviceID()] = vecIter->first;
            vecIter = m_vecMotions.erase( vecIter );
            if( m_iReservedMotion > 0 )
                --m_iReservedMotion;
        }
    }

    // 현재 모션 삽입 및 갱신
    m_vecMotions.push_back( std::make_pair( pMotion, bOneTimeMotion ));
    m_iCurrentMotion = m_vecMotions.size()-1;
    m_iFrame = m_iFirstFrame;
}

bool KGCObjectAndMotion::SetFrameWithCheck()
{
	if( m_vecMotions.empty() )
		return false;

	SetFrame( &m_vecMotions[m_iCurrentMotion].first->GetFrameData( m_iFrame ) );

	return true;
}

void KGCObjectAndMotion::ClearMotion()
{
	m_iCurrentMotion = 0;
	m_iReservedMotion = -1;
	m_iFrame = m_iFirstFrame;
	m_vPos = m_vOriginalPos;
    std::vector< std::pair<GCDeviceMotionFRM*, bool> >::iterator vecIter = m_vecMotions.begin();
	for( ; vecIter != m_vecMotions.end(); ++vecIter )
	{
		SAFE_RELEASE( vecIter->first );
	}
	m_vecMotions.clear();
	std::map< std::string, GCDeviceMotionFRM* >::iterator mit;
	for( mit = m_mapTrash.begin() ; mit != m_mapTrash.end() ; ++mit )
	{
		SAFE_RELEASE( mit->second );
	}
	m_mapTrash.clear();
}

void KGCObjectAndMotion::ClearOneTimeMotion()
{
    m_iCurrentMotion = 0;
    m_iReservedMotion = -1;
    m_iFrame = m_iFirstFrame;
    std::vector< std::pair<GCDeviceMotionFRM*, bool> >::iterator vit;
    for( vit = m_vecMotions.begin() ; vit != m_vecMotions.end() ; )
    {
        if( vit->second )
        {
			m_mapTrash[vit->first->GetDeviceID()] = vit->first;
			vit = m_vecMotions.erase( vit );
            continue;
        }
        vit++;
    }

#ifndef __PATH__
	if( m_vecMotions.empty() )
		MessageBox(NULL,L"여기 어케 온거임?",L"여기 어케 온거임?", 0);        
#endif
}

void KGCObjectAndMotion::SetPosition( D3DXVECTOR3 vPos )
{
	m_vOriginalPos = vPos;
	m_vPos = vPos;
}

void KGCObjectAndMotion::SetScale( D3DXVECTOR3 vScale )
{
	m_vScale = vScale;
}

void KGCObjectAndMotion::SetOffset( D3DXVECTOR3 vOffset )
{
	m_vOffset = vOffset;
}

bool KGCObjectAndMotion::SetWorldMatrix()
{

    union SHORT_TO_FLOAT
    {
        short short_[2];
        float float_;
    } shot; 

    shot.float_ = 0.0f;
	float fMotionY = 0.0f, fMotionX = 0.0f;    
	if( !m_vecMotions.empty() && m_iCurrentMotion >= 0 && m_iCurrentMotion < (int)m_vecMotions.size() )
	{
		fMotionY = m_vecMotions[m_iCurrentMotion].first->GetFrameData( m_iFrame ).Pos_y * m_vScale.y;
        if( m_bFirstFrameForPosition )
		    fMotionX = (m_vecMotions[m_iCurrentMotion].first->GetFrameData(0).Plus_x + m_vecMotions[m_iCurrentMotion].first->GetFrameData(1).Plus_x)*m_vScale.x;
		m_vPos.x += m_vecMotions[m_iCurrentMotion].first->GetFrameData(m_iFrame).Plus_x * m_vScale.x;

        shot.short_[0] = m_vecMotions[m_iCurrentMotion].first->GetFrameData(m_iFrame).Reserved1;
        shot.short_[1] = m_vecMotions[m_iCurrentMotion].first->GetFrameData(m_iFrame).Reserved2;
	}
    

	D3DXMatrixScaling( &g_TempMatScale, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixTranslation( &g_TempMatTranslate, m_vPos.x + fMotionX + m_vOffset.x , m_vPos.y + fMotionY + m_vOffset.y , ( m_vPos.z + m_vOffset.z + shot.float_)  );
	D3DXMatrixRotationYawPitchRoll( &g_TempMatRotate, m_vRotation.y, m_vRotation.x, m_vRotation.z );
	D3DXMatrixIdentity( &g_TempMatWorld );

	g_TempMatWorld *= g_TempMatScale * g_TempMatRotate * g_TempMatTranslate;
	SetWorldMat( &g_TempMatWorld );

	return true;
}

void KGCObjectAndMotion::SetRotation( D3DXVECTOR3 vRot )
{
	m_vRotation = vRot;
}

void KGCObjectAndMotion::NextMotion()
{
	if( (int)m_vecMotions.size() > m_iCurrentMotion )
		m_iFrame = m_vecMotions[m_iCurrentMotion].first->GetNum_Frame() - 1;
}

void KGCObjectAndMotion::ReSetMotion()
{
    if( (int)m_vecMotions.size() > m_iCurrentMotion )
    {
        m_iFrame = m_iFirstFrame;
        SetFrame( &m_vecMotions[m_iCurrentMotion].first->GetFrameData( m_iFrame ) );
    }
}

void KGCObjectAndMotion::SetRandomlyFrame( std::string strMotion, int iFrame )
{
    if( NULL == strstr(strMotion.c_str(), ".frm") )
        strMotion = strMotion + ".frm";

    std::transform(strMotion.begin(), strMotion.end(), strMotion.begin(), tolower );

    int iLoop;
    for(iLoop=0; iLoop<(int)m_vecMotions.size(); ++iLoop)
    {
        if( m_vecMotions[iLoop].first->GetDeviceID() == strMotion )
        {
            m_iCurrentMotion = iLoop;
            break;
        }
    }

    if( iLoop >= (int)m_vecMotions.size() )
        return;

    SetFrame( &m_vecMotions[m_iCurrentMotion].first->GetFrameData( iFrame ) );
    m_iFrame = iFrame;
}

bool KGCObjectAndMotion::IsQueueMotion( std::string strMotion )
{
	
	for ( unsigned int i = 0 ; i < m_vecMotions.size(); i++ )
	{
		if ( m_vecMotions[i].first->GetDeviceID() == strMotion )
		{
			return true;
		}
	}

	return false;

}