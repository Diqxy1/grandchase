#include "stdafx.h"
#include "D3DNumber.h"

IMPLEMENT_CLASSNAME( KD3DNumber );
IMPLEMENT_WND_FACTORY( KD3DNumber );
IMPLEMENT_WND_FACTORY_NAME( KD3DNumber, "gc_number" );

KD3DNumber::KD3DNumber(void)
{
}

KD3DNumber::~KD3DNumber(void)
{
}

// [2/16/2008] breadceo. 마법의 숫자 0.5 처리
void KD3DNumber::SetNumberCoordi( UINT nNumber, const D3DXVECTOR4& vCoordi_ )
{
    if( nNumber > 9 )
    {
        ASSERT( !"Number must be between 0 to 9" );
        return;
    }

    if( m_mapTexCoordi.count( nNumber ) != 0 )
        return;

    D3DXVECTOR4 vCoordi;
    vCoordi.x = (vCoordi_.x + 0.5f ) / 512.0f;
    vCoordi.y = (vCoordi_.y + 0.5f ) / 512.0f;
    vCoordi.z = (vCoordi_.z + 0.5f ) / 512.0f;
    vCoordi.w = (vCoordi_.w + 0.5f ) / 512.0f;

    m_mapTexCoordi[ nNumber ] = vCoordi;
}

void KD3DNumber::SetNumber( UINT nNumber, ED3DWndMode eMode/* = D3DWM_DEFAULT*/ )
{
    if( nNumber > 9 )
    {
        ASSERT( !"Number must be between 0 to 9" );
        return;
    }

    std::map< UINT, D3DXVECTOR4 >::iterator mit = m_mapTexCoordi.find( nNumber );
    if( mit == m_mapTexCoordi.end() )
    {
        ASSERT( !"Invalid Number" );
        return;
    }

    SetWndTexCoords( eMode, mit->second );
}
