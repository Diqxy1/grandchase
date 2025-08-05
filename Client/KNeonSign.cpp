#include "stdafx.h"
#include ".\KNeonSign.h"
//
//

#ifdef _DEBUG
#include "GCElapsedTime.h"
#endif



//

ImplementSingleton( KNeonSign );
KNeonSign::KNeonSign( void )
{
    m_vecNeonSign.clear();
    m_vecSquareItemID.clear();
    m_vecServerItemID.clear();
}

KNeonSign::~KNeonSign( void )
{
    m_vecNeonSign.clear();
    m_vecSquareItemID.clear();
    m_vecServerItemID.clear();
}

void KNeonSign::LoadNeonSign( void )
{
    m_vecNeonSign.clear();
    m_vecSquareItemID.clear();
    m_vecServerItemID.clear();

#ifdef _DEBUG
    GCElapsedTime eTime( L"KNeonSign::LoadNeonSign" );
#endif

    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr , "NeonSign.lua" ) == false )
        return ;

    LUA_BEGIN_TABLE( "NEONSIGN_DATA", return )
    {
        KNeonSignItem kNeonSignItem;
        std::string strTexName;
        DWORD dwTmpGoodsID, dwAniTime;
        D3DXVECTOR4 vPos;
        float fTexSize;
        int nType;
		int nShowType;

        for ( int i = 1; ; ++i )
        {
            char strTable[MAX_PATH];
            sprintf( strTable, "NEONSIGN_ITEM%d", i );
            LUA_BEGIN_TABLE( strTable, break )
            {
                kNeonSignItem.Init();
                LUA_GET_VALUE( "GOODSID", dwTmpGoodsID, break );
                LUA_GET_VALUE( "TEXTURE_NAME", strTexName, break );
                LUA_GET_VALUE( "TEXTURE_WIDTH", fTexSize, break );
                kNeonSignItem.SetTexWidth( fTexSize );
                LUA_GET_VALUE( "TEXTURE_HEIGHT", fTexSize, break );
                kNeonSignItem.SetTexHeight( fTexSize );
                kNeonSignItem.SetGoodsID( dwTmpGoodsID );
                kNeonSignItem.SetTextureName( strTexName );
                LUA_GET_VALUE( "TYPE", nType, break );
                kNeonSignItem.SetType( (KSignBoardData::SIGN_BOARD_TYPE)nType );

				LUA_GET_VALUE_DEF("SHOW_TYPE", nShowType, 1 );
				kNeonSignItem.SetShowType( (KNeonSignItem::NEON_SIGN_SHOW_TYPE)nShowType );


                for ( int j = 1; ; ++j )
                {
                    sprintf( strTable, "FRAME%d", j );
                    LUA_BEGIN_TABLE( strTable, break )
                    {
                        SNeonSignFrame sNeonSignFrame;
                        LUA_GET_VALUE_DEF( "TIME", dwAniTime, 1500 );
                        sNeonSignFrame.dwAnimateTime = dwAniTime;

                        GetUVPos( luaMgr, "LEFTTOP",        0, sNeonSignFrame );
                        GetUVPos( luaMgr, "TOP",            1, sNeonSignFrame );
                        GetUVPos( luaMgr, "RIGHTTOP",       2, sNeonSignFrame );
                        GetUVPos( luaMgr, "LEFT",           3, sNeonSignFrame );
                        GetUVPos( luaMgr, "CENTER",         4, sNeonSignFrame );
                        GetUVPos( luaMgr, "RIGHT",          5, sNeonSignFrame );
                        GetUVPos( luaMgr, "LEFTBOTTOM",     6, sNeonSignFrame );
                        GetUVPos( luaMgr, "BOTTOM",         7, sNeonSignFrame );
                        GetUVPos( luaMgr, "RIGHTBOTTOM",    8, sNeonSignFrame );

                        kNeonSignItem.AddFrameInfo( sNeonSignFrame );
                    }
                    LUA_END_TABLE( break );
                }
                kNeonSignItem.DoPreprocess();
                m_vecNeonSign.push_back( kNeonSignItem );

                switch( nType )
                {
                case KSignBoardData::SBT_SQUARE:
                    m_vecSquareItemID.push_back( dwTmpGoodsID );
                    break;
                case KSignBoardData::SBT_SERVER:
                    m_vecServerItemID.push_back( dwTmpGoodsID );
                    break;
                default:
                    ASSERT( !"Invalid TYpe" );
                    break;
                }
            }
            LUA_END_TABLE( break )
        }
    }
    LUA_END_TABLE( return );

     //std::stringstream stm;
     //Dump( stm );
}

void KNeonSign::GetUVPos( IN KLuaManager& luaMgr, IN std::string strTableName, IN int nIdx, OUT SNeonSignFrame& kFrame )
{
    D3DXVECTOR4 vUV;

    LUA_BEGIN_TABLE ( strTableName.c_str(), return )
    {
        LUA_GET_VALUE_DEF( "tu1", vUV.x, 0.f );
        LUA_GET_VALUE_DEF( "tv1", vUV.y, 0.f );
        LUA_GET_VALUE_DEF( "tu2", vUV.z, 1.f );
        LUA_GET_VALUE_DEF( "tv2", vUV.w, 1.f );
    }
    LUA_END_TABLE( return );

    kFrame.TexUVPos[nIdx] = vUV;
    kFrame.TexSize[nIdx].x = vUV.z - vUV.x;
    kFrame.TexSize[nIdx].y = vUV.w - vUV.y;
}

void KNeonSign::Dump( std::stringstream& stm )
{
    std::vector<KNeonSignItem>::iterator vit = m_vecNeonSign.begin();
    for (; vit != m_vecNeonSign.end(); vit++ )
    {
        stm << "TextureName: " << vit->GetTextureName() << std::endl;
        stm << "GoodsID: " << vit->GetGoodsID() << std::endl;

        std::vector<SNeonSignFrame> vecNeonFrame;
        vit->GetFrameInfo( vecNeonFrame );
        std::vector<SNeonSignFrame>::iterator vItr = vecNeonFrame.begin();
        for (; vItr != vecNeonFrame.end(); ++vItr )
        {
            stm << "ANIMATION_TIME: " << vItr->dwAnimateTime << std::endl;
            for (int j = 0; j < 9; j++)
                stm << " UVPOS = { " << vItr->TexUVPos[j].x << ", " << vItr->TexUVPos[j].y << " }" << std::endl;
        }
    }

    START_LOG( clog, "Dump for NeonSign" << dbg::endl << stm.str().c_str() );
}

void KNeonSign::GetNeonSignData( IN DWORD dwGoodsID, OUT KNeonSignItem& kData )
{
    std::vector<KNeonSignItem>::iterator vit;
    vit = std::find_if( m_vecNeonSign.begin(), m_vecNeonSign.end(),
        boost::bind( &KNeonSignItem::m_dwGoodsID, _1 ) == dwGoodsID );
    ASSERT( vit != m_vecNeonSign.end() );
    kData = (*vit);
}

bool KNeonSign::IsServerNeonsign( int nItemID ) const
{
    for( int i = 0; i < (int)m_vecNeonSign.size(); ++i )
    {
        const KNeonSignItem& kData = m_vecNeonSign[i];
        if( nItemID == (int)kData.GetGoodsID() / 10 )
        {
            if( kData.GetType() == KSignBoardData::SBT_SERVER )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

const std::vector< int >& KNeonSign::GetNeonsignItemList( KSignBoardData::SIGN_BOARD_TYPE eType ) const
{
    switch( eType )
    {
    case KSignBoardData::SBT_SQUARE:
        return m_vecSquareItemID;
        break;
    case KSignBoardData::SBT_SERVER:
        return m_vecServerItemID;
        break;
    }
    ASSERT( !"Invalid TYpe" );
    return m_vecSquareItemID;
}

KNeonSignItem::NEON_SIGN_SHOW_TYPE KNeonSign::GetNeonSignShowType( DWORD dwGoodsID)
{
	for( int i = 0; i < (int)m_vecNeonSign.size(); ++i )
	{
		const KNeonSignItem& kData = m_vecNeonSign[i];
		if( dwGoodsID == (int)kData.GetGoodsID() )
		{
			return kData.GetShowType();
		}
	}

	return KNeonSignItem::NSST_HIDE;
}
//////////////////////////////////////////////////////////////////////////
KNeonSignItem::KNeonSignItem( void )
: m_dwGoodsID(0)
, m_iCurFrame(0)
, m_dwAnimTime(0)
, m_fTexWidth(0.0f)
, m_fTexHeight(0.0f)
{
    m_strTexName = "";
    m_vecNeonSignFrame.clear();
}

KNeonSignItem::~KNeonSignItem( void )
{
    m_vecNeonSignFrame.clear();
}

void KNeonSignItem::SetTextureName( const std::string& strTexName )
{
    m_strTexName = strTexName;
}

void KNeonSignItem::SetGoodsID( const DWORD& dwGoodsID )
{
    m_dwGoodsID = dwGoodsID;
}

void KNeonSignItem::AddFrameInfo( const SNeonSignFrame& sNeonSignFrame )
{
    m_vecNeonSignFrame.push_back( sNeonSignFrame );
}

const SNeonSignFrame& KNeonSignItem::GetCurFrameData()
{
    ASSERT( m_iCurFrame >= 0 && m_iCurFrame < (int)m_vecNeonSignFrame.size() );
    return m_vecNeonSignFrame[m_iCurFrame];
}

void KNeonSignItem::NextFrame()
{
    if( m_vecNeonSignFrame.empty() )
        return;

    m_iCurFrame = ( m_iCurFrame + 1 ) % m_vecNeonSignFrame.size();
}

void KNeonSignItem::IncreaseTime()
{
    if( m_vecNeonSignFrame.empty() )
        return;

    m_dwAnimTime++;
    const SNeonSignFrame& kCurFrame = GetCurFrameData();
    if( m_dwAnimTime > kCurFrame.dwAnimateTime )
    {
        NextFrame();
        m_dwAnimTime = 0;
    }
}

void KNeonSignItem::DoPreprocess()
{
    for( int i = 0; i < (int)m_vecNeonSignFrame.size(); ++i )
    {
        SNeonSignFrame& kFrame = m_vecNeonSignFrame[i];
        // ( x - 0.5f ) / texsize
        for( int j = 0; j < NSP_MAX; ++j )
        {
            kFrame.TexUVPos[j] -= D3DXVECTOR4( 0.5f, 0.5f, 0.5f, 0.5f );
            kFrame.TexUVPos[j].x /= m_fTexWidth;
            kFrame.TexUVPos[j].z /= m_fTexWidth;
            kFrame.TexUVPos[j].y /= m_fTexHeight;
            kFrame.TexUVPos[j].w /= m_fTexHeight;
        }
    }
}

void KNeonSignItem::Init()
{
    m_strTexName = "";
    m_dwGoodsID = 0;
    m_vecNeonSignFrame.clear();
    m_iCurFrame = 0;
    m_dwAnimTime = 0;
    m_fTexWidth = 1.0f; // ? divide by zero?
    m_fTexHeight = 1.0f; // ? divide by zero?
    m_eType = KSignBoardData::SBT_MAX;
}

void KNeonSignItem::GetRect( NEON_SIGN_POSITION ePos, D3DXVECTOR2& vRect ) const
{
    vRect.x = 0.0f;
    vRect.y = 0.0f;

    if( m_vecNeonSignFrame.empty() )
        return;

    ASSERT( ePos >= NSP_LEFTTOP && ePos < NSP_MAX );
    vRect = m_vecNeonSignFrame[m_iCurFrame].TexSize[ePos];
}

void KNeonSignItem::ResetTime()
{
    m_dwAnimTime = 0;
    m_iCurFrame = 0;
}