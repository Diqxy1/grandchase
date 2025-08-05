#pragma once

//#include "UserPacket.h"
//#include <vector>
#include <WTypes.h>
//#include "GCEnum.h"
#include <KNCSingleton.h>
//#include <sstream>
#include <d3dx9math.h>
//#include "KLuaManager.h"

struct SNeonSignFrame
{
    //D3DXVECTOR4 TexPos;
    DWORD dwAnimateTime;
    KSafeArray<D3DXVECTOR4,9> TexUVPos;
    KSafeArray<D3DXVECTOR2,9> TexSize;
};

class KNeonSign;

class KNeonSignItem
{
public:
    enum NEON_SIGN_POSITION
    {
        NSP_LEFTTOP = 0,
        NSP_TOP,
        NSP_RIGHTTOP,
        NSP_LEFT,
        NSP_CENTER,
        NSP_RIGHT,
        NSP_LEFTBOTTOM,
        NSP_BOTTOM,
        NSP_RIGHTBOTTOM,
        NSP_MAX,
    };

	enum NEON_SIGN_SHOW_TYPE
	{
		NSST_HIDE = 0, //사용할 순 있지만 보여주지 않는다.(판매하지 않는 전광판)
		NSST_SHOW = 1, //보여준다.
	};

    KNeonSignItem(void);
    virtual ~KNeonSignItem(void);

    // getter/setter
    void SetTextureName( const std::string& strTexName );
    std::string GetTextureName( void ) const { return m_strTexName; }
    void SetGoodsID( const DWORD& dwGoodsID );
    DWORD GetGoodsID( void ) const { return m_dwGoodsID; }
    void SetTexWidth( float fSize ) { m_fTexWidth = fSize; }
    float GetTexWidth() const { return m_fTexWidth; }
    void SetTexHeight( float fSize ) { m_fTexHeight = fSize; }
    float GetTexHeight() const { return m_fTexHeight; }
    DWORD GetAnimTime() const { return m_dwAnimTime; }
    KSignBoardData::SIGN_BOARD_TYPE GetType() const { return m_eType; }
    void SetType( KSignBoardData::SIGN_BOARD_TYPE val ) { m_eType = val; }

    // functions
    void AddFrameInfo( const SNeonSignFrame& sNeonSignFrame );
    void GetFrameInfo( OUT std::vector<SNeonSignFrame>& sNeonFrame ) { sNeonFrame = m_vecNeonSignFrame; }

    bool empty() const { return m_vecNeonSignFrame.empty(); }
    const SNeonSignFrame& GetCurFrameData();
    void NextFrame();
    void IncreaseTime();
    void DoPreprocess();
    void Init();
    void GetRect( NEON_SIGN_POSITION ePos, OUT D3DXVECTOR2& vRect ) const;
    void ResetTime();

	NEON_SIGN_SHOW_TYPE GetShowType() const { return m_eShowType; }
	void SetShowType(NEON_SIGN_SHOW_TYPE showtype) { m_eShowType = showtype; }

    friend class KNeonSign;

private:
    std::string m_strTexName;
    DWORD m_dwGoodsID;
    std::vector<SNeonSignFrame> m_vecNeonSignFrame;
    int m_iCurFrame;
    DWORD m_dwAnimTime;
    float m_fTexWidth;
    float m_fTexHeight;
    KSignBoardData::SIGN_BOARD_TYPE m_eType;
	NEON_SIGN_SHOW_TYPE m_eShowType;
};

class KNeonSign
{
    DeclareSingleton( KNeonSign );

public:
    KNeonSign( void );
    virtual ~KNeonSign( void );

    void LoadNeonSign( void );
    void GetUVPos( IN KLuaManager& luaMgr, IN std::string strTableName, IN int nIdx, OUT SNeonSignFrame& kFrame );
    void GetNeonSignData( IN DWORD dwGoodsID, OUT KNeonSignItem& kData );
    const std::vector< int >& GetNeonsignItemList( KSignBoardData::SIGN_BOARD_TYPE eType ) const;
    bool IsServerNeonsign( int nItemID ) const;
	KNeonSignItem::NEON_SIGN_SHOW_TYPE GetNeonSignShowType( DWORD dwGoodsID);

    // debug function
    void Dump( std::stringstream& stm );
    
private:
    std::vector<KNeonSignItem> m_vecNeonSign;
    std::vector< int > m_vecSquareItemID;
    std::vector< int > m_vecServerItemID;
};

DefSingletonInline( KNeonSign );

